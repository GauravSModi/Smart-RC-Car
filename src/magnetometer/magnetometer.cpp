#include "magnetometer.h"
#include <iostream>
#include <fcntl.h>
#include <fstream>
#include <common/utils.h>
#include <unistd.h>

#define I2C_LIS3MDL_DEVICE 0x1E
#define REG_COMPASS_X_L 0x28
#define REG_COMPASS_X_H 0x29
#define REG_COMPASS_Y_L 0x2A
#define REG_COMPASS_Y_H 0x2B
#define REG_COMPASS_Z_L 0x2C
#define REG_COMPASS_Z_H 0x2D

#define I2C_BUS_2 "/dev/i2c-2"

static Magnetometer* instance = NULL;

Magnetometer* Magnetometer::getInstance(){
  if(instance == NULL){
    instance = new Magnetometer();
  }
  return instance;
}

Magnetometer::Magnetometer(){
  this->configSensor();
  this->sensorFD = initI2cBus(I2C_BUS_2,I2C_LIS3MDL_DEVICE);
}

Magnetometer::~Magnetometer(){
  close(this->sensorFD);
}

Vec3<int> Magnetometer::getSensorValues(){
  int BUFF_SIZE = 6;
  char values[BUFF_SIZE];

  // jump to offset, by "writing"
  unsigned char offset = REG_COMPASS_X_L;
  if(write(this->sensorFD,&offset,sizeof(offset)) != sizeof(offset)){
    std::cout << "Magnetometer not enough written.\n";
  }

  if(read(this->sensorFD,values,sizeof(char) * BUFF_SIZE) != BUFF_SIZE){
    std::cout << "Magnetometer failed to read.\n";
  } 

  Vec3<int> readings;

  readings.x = ((values[0] << 8) | (values[1]));
  readings.y = ((values[2] << 8) | (values[3]));
  readings.z = ((values[4] << 8) | (values[5]));

  return readings;
}

void Magnetometer::configSensor(){
  // configure pins to use I2C
  if(system("config-pin p9_19 i2c") != -1){
    //printf("configure pin p9_18 sucess.\n");
  }
  if(system("config-pin p9_20 i2c") != -1){
    //printf("configure pin p9_17 sucess.\n");    
  }
}
