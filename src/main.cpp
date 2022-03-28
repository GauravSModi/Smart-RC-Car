#include <stdlib.h>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>
//#include <common/utils.h>
//#include <joystick/joystick.h>
//#include <network/network.h>
//#include <rover/rover.h>
//#include <led/led.h>
#include <distance_sensor/sharpIR_ds.h>



int main(){
  // dummy functions to test project build status
  //utilsDummy();
  //joystickDummy();
  //ledDummy();
  //networkDummy();
  //roverDummy();
  SHARPDistanceSensor* dis=SHARPDistanceSensor::getInstance(); while(1){dis->getVoltageValues();}
  
  // intialize modules

  // wait on condition var

  // call cleanup?

  // exit
  return 0;
}