#include "joystick.h"

#define DIRECTION_STRENGTH 0.1 // from 0.0 to 1.0, how much should the strength change per sample
#define DIRECTION_DECAY_PERSECOND 0.1 // from 0.0 to 1.0, how much strenge decay over 1 second
#define JOYSTICK_POLL_INTERVAL 50 // in miliseconds

Joystick* Joystick::instance;

void joystickDummy(){
  printf("joystick module Include success\n");
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

  this->strengthVector = {0.0, 0.0};
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
  while(isRunning){
    // poll for next event
    JoystickDirection event = this->getEpollInput();

    // lock for editing direction strength
    strengthMutex.lock();
    switch (event){
    case UP:
      this->strengthVector.z = clamp(this->strengthVector.z + DIRECTION_STRENGTH,1.0,-1.0);
      break;
    case DOWN:
      this->strengthVector.z = clamp(this->strengthVector.z - DIRECTION_STRENGTH,1.0,-1.0);
      break;
    case LEFT:
      this->strengthVector.x = clamp(this->strengthVector.x - DIRECTION_STRENGTH,1.0,-1.0);
      break;
    case RIGHT:
      this->strengthVector.x = clamp(this->strengthVector.x + DIRECTION_STRENGTH,1.0,-1.0);
    default:
      break;
    }
    strengthMutex.unlock();

    // wait for interval until next poll
    usleep(1000 * JOYSTICK_POLL_INTERVAL);
  }
}

// helper function to Joystick:getEpollInput
void addEdgeGPIOToEpoll(int epollFd, int watchFd, struct epoll_event* epollForEvent){
  epollForEvent->events = EPOLLPRI | EPOLLET | EPOLLIN;
  epollForEvent->data.fd = watchFd;
  if(epoll_ctl(epollFd, EPOLL_CTL_ADD, watchFd, epollForEvent) == -1){
    printf("ERROR ADDING INTEREST TO EPOLL fd[%d]\n",watchFd);
    exit(1);
  }
}

JoystickDirection Joystick::getEpollInput(){
  // create an epoll instance for polling joystick value changes
  int epollInstanceFd = epoll_create(4);
  if(epollInstanceFd == -1){
    printf("ERROR OPENING EPOLL\n");
    exit(1);
  }

  struct epoll_event upEvent;
  addEdgeGPIOToEpoll(epollInstanceFd,this->upValueFD,&upEvent);
  struct epoll_event downEvent;
  addEdgeGPIOToEpoll(epollInstanceFd,this->downValueFD,&downEvent);
  struct epoll_event leftEvent;
  addEdgeGPIOToEpoll(epollInstanceFd,this->leftValueFD,&leftEvent);
  struct epoll_event rightEvent;
  addEdgeGPIOToEpoll(epollInstanceFd,this->rightValueFD,&rightEvent);

  // create buffer to hold next detected event
  struct epoll_event currEvent;

  // account for weird initial 4 events
  int event_count = 2;
  while(event_count > 1){
    event_count = epoll_wait(epollInstanceFd,&currEvent,5,-1);
  }

  JoystickDirection result = INVALID;
  if(event_count == 1){
    if(currEvent.data.fd == this->upValueFD){
      result = UP;
    } else if(currEvent.data.fd == this->downValueFD){
      result = DOWN;
    } else if(currEvent.data.fd == this->leftValueFD){
      result = LEFT;
    } else if(currEvent.data.fd == this->rightValueFD){
      result = RIGHT;
    }
  }  

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
    Vec2 strength = this->getStrength();
    
    if(strength.x != 0.0){
      if(strength.x > 0.0){
        // had postive value
        strength.x = max(strength.x - DIRECTION_DECAY_PERSECOND, 0.0);
      } else {
        // had negative value
        strength.x = min(strength.x + DIRECTION_DECAY_PERSECOND, 0.0);
      }
    }

    if(strength.z != 0.0){
      if(strength.z > 0.0){
        // had postive value
        strength.z = max(strength.x - DIRECTION_DECAY_PERSECOND,0.0);
      } else {
        // had negative value
        strength.z = min(strength.x + DIRECTION_DECAY_PERSECOND,0.0);
      }
    }

    this->setStrength(strength.x,strength.z);

    this->unlockStrength();
    // finished decaying
    threadDecaying--;
  };
  std::thread* latestDecayThread = new std::thread(decay);
  // decay is called once per second
  while(this->isRunning){
    sleep(1);
    threadDecaying++;
    latestDecayThread = new std::thread(decay);
  }
  latestDecayThread->join();

  while(threadDecaying != 0){
    // wait until all decay thread finish before returning
    // to prevent early deletion of Joystick object
    sleep(1);
  }
}