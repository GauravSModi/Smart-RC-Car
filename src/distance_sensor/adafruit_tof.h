#pragma once
// referenced VL53L0X guide by
// Irene Abrea, Heidi Tong and Joanna Niemczyk
#include "distance_sensor.h"
#include <thread>

class TOFDistanceSensor : public DistanceSensor {
  int sensorFD;
  bool filterExtreme;

public:
  //static TOFDistanceSensor* getInstance();
  unsigned int getSensorValues();
  
  void setFilterExtremeValues(bool enable);
  void setContinousSensing(bool enable);

  void distanceReading_routine();
  bool objectedDetected(int distance);

  ~TOFDistanceSensor();
  TOFDistanceSensor();

private:

  std::thread* distance_readingThread;
  
  void configSensor();

};
