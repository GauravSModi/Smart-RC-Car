#pragma once
// referenced VL53L0X guide by
// Irene Abrea, Heidi Tong and Joanna Niemczyk
#include "distance_sensor.h"
#include "rover/rover.h"
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
  //int askHysteresis(int reading); //change
  void decideTurn(int count);


  ~TOFDistanceSensor();
  TOFDistanceSensor(Rover* rover);

private:

  std::thread* distance_readingThread;
  Rover* rover;
  bool disableReading;
  
  int current_reading; //change
  int prev_reading;
  
  void configSensor();

};
