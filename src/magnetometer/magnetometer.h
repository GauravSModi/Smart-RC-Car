#pragma once
#include <common/utils.h>

// Interface for LIS3MDL 
// A Digital output magnetic sensor ultra-low-power, high-performance 3-axis magnetometer
// datasheet found on https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&cad=rja&uact=8&ved=2ahUKEwi7n6-6g4P3AhUDHzQIHfVDBLUQFnoECAgQAQ&url=https%3A%2F%2Fwww.st.com%2Fresource%2Fen%2Fdatasheet%2Flis3mdl.pdf&usg=AOvVaw0er2_yXRSETSyj15KzGnCx
// another https://github.com/DavidEGrayson/minimu9-ahrs/blob/master/lis3mdl.cpp
class Magnetometer {
  int sensorFD;
  bool filterExtreme;

public:
  static Magnetometer* getInstance();
  Vec3<int> getSensorValues();

private:
  ~Magnetometer();
  Magnetometer();
  void configSensor();

};
