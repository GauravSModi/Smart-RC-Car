#pragma once
#include <thread>
#include "distance_sensor/adafruit_tof.h"

class DistanceSensor {
public:
  virtual unsigned int  getSensorValues() = 0;
 
};
