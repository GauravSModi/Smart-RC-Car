#include "orientation.h"
#include <common/utils.h>


void Orientation::initialize(){
  isRunning = true;
  this->readingThread = new std::thread(&Orientation::readingRun,this);
}

void Orientation::shutdown(){
  isRunning = false;
  if(this->readingThread){
    this->readingThread->join();
  }
}

void Orientation::readingRun(){  
  clockid_t clock = CLOCK_MONOTONIC;
  timespec_t nextRequest;
  clock_gettime(clock,&nextRequest);
  
  while(isRunning){
    // sleep until next interval
    nextRequest = timespec_add(nextRequest,0,this->sample_interval_ms * 1000000);
    // use clock_nanosleep to get a more persistant reading cycle
    // since nanosleep did not take account of the time used to compute values before calling sleep
    clock_nanosleep(clock,TIMER_ABSTIME,&nextRequest,NULL);

    // read values from gyro sensor
    // TODO: implementation:
    
    // read rotation per second:
    //Rotation rotationValue = readGyro(); 

    // calculate delta-rotation (rotational speed * time)
    //Rotation delta = rotationValue * this->sample_interval_ms;

    // apply delta-rotation to current rotation 
    //Rotation newRotation = oldRotation + delta;
  }
}

