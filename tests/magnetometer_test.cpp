#include <magnetometer/magnetometer.h>
#include <iostream>
#include <common/utils.h>

#define PRINT_NOTHING_DETECTED_ONCE true

void testConstructor(){
  Magnetometer* sensor = Magnetometer::getInstance();
 
  
  while(true){
    auto readings = sensor->getSensorValues();

    std::cout << "Compass: (X,Y,Z) " << readings.x << ", " << readings.y << ", " << readings.z << "\n";

    msleep(1000);
  }
}

int main(int argc, char const *argv[]){
  testConstructor();

  return 0;
}