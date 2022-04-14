#pragma once
// referenced VL53L0X guide by
// Irene Abrea, Heidi Tong and Joanna Niemczyk

#include "rover/rover.h"
#include <thread>

void init_frontDS(Rover* _myRover);
void clean_frontDS();
bool frontDS_isActive();


class TOFDistanceSensor {
  int sensorFD;
  bool filterExtreme;

public:
  unsigned int getSensorValues();
  
  void setFilterExtremeValues(bool enable);
  void setContinousSensing(bool enable);

  void distanceReading_routine();
  bool objectedDetected(int distance);
  
  void decideTurn(int count);

  bool checkDistanceSensorEnabled();


  ~TOFDistanceSensor();
  TOFDistanceSensor();
  TOFDistanceSensor(Rover* rover);

private:

  std::thread* distance_readingThread;
  Rover* rover;
  bool shutdown;
  
  int current_reading; 
  int prev_reading;
  
  void configSensor();
  friend bool frontDS_isActive();
};
