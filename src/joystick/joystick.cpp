#include "joystick.h"

#include <cstdio>
#include <fcntl.h>
#include <sys/epoll.h>
#include <unistd.h>

#include <atomic>
#include <common/utils.h>


#define DIRECTION_STRENGTH 1.0 // from 0.0 to 1.0, how much should the strength change per sample
#define DIRECTION_DECAY_PERSECOND 0.05 // from 0.0 to 1.0, how much strenge decay over 1 second
#define JOYSTICK_POLL_INTERVAL 50 // in miliseconds

Joystick* Joystick::instance;

void joystickDummy(){
  printf("joystick module Include success\n");
}

static void printVec2(Vec2<int> vectors){
  printf("Vector[x:%f,z:%f]\n",vectors.x,vectors.y);
}

Joystick::Joystick(){
  // Open file discriptors to joystick value paths
  this->upValueFD = open(JOYSTICK_UP_PATH "value", O_RDWR | O_NONBLOCK);
  if(this->upValueFD < 0){
    exit(1);
  }
  this->downValueFD = open(JOYSTICK_DOWN_PATH "value", O_RDWR | O_NONBLOCK);
  if(this->downValueFD < 0){
    exit(1);
  }
  this->leftValueFD = open(JOYSTICK_LEFT_PATH "value", O_RDWR | O_NONBLOCK);
  if(this->downValueFD < 0){
    exit(1);
  }
  this->rightValueFD = open(JOYSTICK_RIGHT_PATH "value", O_RDWR | O_NONBLOCK);
  if(this->rightValueFD < 0){
    exit(1);
  }

  this->strengthVector.x = 0;
  this->strengthVector.y = 0;
  this->isRunning = false;
}

Joystick::~Joystick(){
  // close file descriptors
  if(close(this->upValueFD)){
    printf("ERROR CLOSING UP JOYSTICK_VALUE\n");
    exit(1);
  };

  if(close(this->downValueFD)){
    printf("ERROR CLOSING DOWN JOYSTICK_VALUE\n");
    exit(1);
  };

  if(close(this->leftValueFD)){
    printf("ERROR CLOSING LEFT JOYSTICK_VALUE\n");
    exit(1);
  };

  if(close(this->rightValueFD)){
    printf("ERROR CLOSING RIGHT JOYSTICK_VALUE\n");
    exit(1);
  };

  delete this->joystickThread;
}

void Joystick::initialize(){
  enableJoyStickEdgeOnRising(JOYSTICK_UP_PATH);
  enableJoyStickEdgeOnRising(JOYSTICK_DOWN_PATH);
  enableJoyStickEdgeOnRising(JOYSTICK_LEFT_PATH);
  enableJoyStickEdgeOnRising(JOYSTICK_RIGHT_PATH);

  Joystick::instance = new Joystick();
  Joystick::instance->isRunning = true;
  Joystick::instance->joystickThread = new std::thread(&Joystick::run,Joystick::instance);
  Joystick::instance->decayerThread = new std::thread(&Joystick::strengthDecayer,Joystick::instance);
}

void Joystick::shutdown(){
  // end thread loop
  Joystick::instance->isRunning = false;
  // join thread
  Joystick::instance->joystickThread->join();
  delete Joystick::instance;
  Joystick::instance = nullptr;
}

void Joystick::run(){
  // event loop:
  printf("Joystick start running\n");
  while(isRunning){
    // poll for next event
    JoystickDirection event = this->getEpollInput();
  
    // lock for editing direction strength
    this->strengthMutex.lock();

    //printf("Num[%d]\n",event);

    switch (event){
    case UP:
      this->strengthVector.y = clamp(this->strengthVector.y + DIRECTION_STRENGTH,1.0,-1.0);
      break;
    case DOWN:
      this->strengthVector.y = clamp(this->strengthVector.y - DIRECTION_STRENGTH,1.0,-1.0);
      break;
    case LEFT:
      this->strengthVector.x = clamp(this->strengthVector.x - DIRECTION_STRENGTH,1.0,-1.0);
      break;
    case RIGHT:
      this->strengthVector.x = clamp(this->strengthVector.x + DIRECTION_STRENGTH,1.0,-1.0);
    default:
      break;
    }
    this->strengthMutex.unlock();

    // wait for interval until next poll
    usleep(1000 * JOYSTICK_POLL_INTERVAL);
  }
  printVec2(this->getStrength());
}

// helper function to Joystick:getEpollInput
static void addEdgeGPIOToEpoll(int epollFd, int watchFd, struct epoll_event* epollForEvent){
  epollForEvent->events = EPOLLET | EPOLLIN ;//| EPOLLPRI ;
  epollForEvent->data.fd = watchFd;
  //epollForEvent->data.ptr = epollForEvent;
  
  if(epoll_ctl(epollFd, EPOLL_CTL_ADD, watchFd, epollForEvent) == -1){
    printf("ERROR ADDING INTEREST TO EPOLL fd[%d]\n",watchFd);
    exit(1);
  }
}

void removeEdgeGPIOFromEpoll(int epollFd, int watchFd, struct epoll_event* epollForEvent){
    if(epoll_ctl(epollFd, EPOLL_CTL_DEL, watchFd, epollForEvent) == -1){
        printf("ERROR DELETING INTEREST TO EPOLL fd[%d]\n",watchFd);
        exit(1);
    }
}

JoystickDirection Joystick::getEpollInput(){
  // create an epoll instance for polling joystick value changes
  int epollInstanceFd = epoll_create1(0);
  if(epollInstanceFd == -1){
    printf("ERROR OPENING EPOLL\n");
  }

  struct epoll_event upEvent = {0};
  addEdgeGPIOToEpoll(epollInstanceFd,this->upValueFD,&upEvent);
  struct epoll_event downEvent = {0};
  addEdgeGPIOToEpoll(epollInstanceFd,this->downValueFD,&downEvent);
  struct epoll_event leftEvent = {0};
  addEdgeGPIOToEpoll(epollInstanceFd,this->leftValueFD,&leftEvent);
  struct epoll_event rightEvent = {0};
  addEdgeGPIOToEpoll(epollInstanceFd,this->rightValueFD,&rightEvent);

  // create buffer to hold next detected event
  struct epoll_event currEvents[5];

  // account for weird initial 4 events
  int event_count = 2;
  while(event_count > 1){
    event_count = epoll_wait(epollInstanceFd,currEvents,5,-1);
    printf("event waited\n");
  }

  JoystickDirection result = INVALID;

  if(event_count == 1){
    if(currEvents[0].data.fd == this->upValueFD){
      result = UP;
    } else if(currEvents[0].data.fd == this->downValueFD){
      result = DOWN;
    } else if(currEvents[0].data.fd == this->leftValueFD){
      result = LEFT;
    } else if(currEvents[0].data.fd == this->rightValueFD){
      result = RIGHT;
    }
  }  

  /*
  for(int i = 0; i < 4; i++){
    
    if(currEvents[i].events & EPOLLIN){
      //result = JoystickDirection(i);
      printf("event[%d]\n",i);
    }
  }*/

  // close epoll instance
  if(close(epollInstanceFd)){
    printf("ERROR CLOSING EPOLL\n");
    exit(1);
  }

  return result;
}

void Joystick::strengthDecayer(){

  std::atomic<int> threadDecaying(0);
  auto decay = [&](){
    this->lockStrength();
    auto strengthCopied = Vec2<int>(this->getStrength());
    auto strength = Vec2<int>(this->getStrength());
    
    if(strength.x != 0.0){
      if(strength.x > 0.0){
        // had postive value
        strength.x = max(strengthCopied.x - DIRECTION_DECAY_PERSECOND, 0.0);
      } else {
        // had negative value
        strength.x = min(strengthCopied.x + DIRECTION_DECAY_PERSECOND, 0.0);
      }
    }

    if(strength.y != 0.0){
      if(strength.y > 0.0){
        // had postive value
        strength.y = max(strengthCopied.y - DIRECTION_DECAY_PERSECOND,0.0);
      } else {
        // had negative value
        strength.y = min(strengthCopied.y + DIRECTION_DECAY_PERSECOND,0.0);
      }
    }
    printf("Setting strength from to [%f,%f] -> [%f,%f]\n\n",strengthCopied.x,strengthCopied.y,strength.x,strength.y);
    this->setStrength(strength.x,strength.y);

    this->unlockStrength();
    // finished decaying
    threadDecaying--;
  };
  std::thread* latestDecayThread = new std::thread(decay);
  // decay is called once per second
  while(this->isRunning){
    msleep(1000);
    threadDecaying++;
    latestDecayThread = new std::thread(decay);
    //printVec2(this->getStrength());
  }
  latestDecayThread->join();

  while(threadDecaying != 0){
    // wait until all decay thread finish before returning
    // to prevent early deletion of Joystick object
    sleep(1);
  }
}