#pragma once
// referenced VL53L0X guide by
// Irene Abrea, Heidi Tong and Joanna Niemczyk
#include "distance_sensor.h"

class TOFDistanceSensor : public DistanceSensor {
  int sensorFD;
  bool filterExtreme;

public:
  static TOFDistanceSensor* getInstance();
  unsigned int getSensorValues();
  
  void setFilterExtremeValues(bool enable);
  void setContinousSensing(bool enable);

private:
  ~TOFDistanceSensor();
  TOFDistanceSensor();
  void configSensor();

};
