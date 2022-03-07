#include <distance_sensor/distance_sensor.h>
#include <distance_sensor/adafruit_tof.h>
#include <iostream>
#include <common/utils.h>

#define PRINT_NOTHING_DETECTED_ONCE true

void testConstructor(){
  DistanceSensor* sensor = TOFDistanceSensor::getInstance();
  bool wasNothing = false;
  int counter = 0;
  while(true){
    int value = sensor->getSensorValues();
    if(value == 0){
      if(PRINT_NOTHING_DETECTED_ONCE){
        if(!wasNothing){
          std::cout << "Nothing detected\n";
          wasNothing = true;
        }
      } else {
        std::cout << "Nothing detected["<< counter++ <<"]\n";
      }
    } else {
      wasNothing = false;
      counter = 0;
      std::cout << "Sensed value: " << value << "\n";
    }
    msleep(50);
  }
}

int main(int argc, char const *argv[]){
  testConstructor();

  return 0;
}