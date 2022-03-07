#include "adafruit_tof.h"
#include <iostream>
#include <fcntl.h>
#include <fstream>
#include <common/utils.h>
#include <unistd.h>

#define I2C_TOF_SENSOR_DEVICE 0x29
#define REG_TOF_MSB 0x1E
#define REG_TOF_LSB 0x1F

#define REG_TOF_CONTROL 0x00
#define REG_TOF_SINGLE_MODE 0x00
#define REG_TOF_CONTINOUS_MODE 0x02

#define I2C_TOF_SENSOR_BUS "/dev/i2c-2"

static TOFDistanceSensor* instance = NULL;

TOFDistanceSensor* TOFDistanceSensor::getInstance(){
  if(instance == NULL){
    instance = new TOFDistanceSensor();
  }
  return instance;
}

TOFDistanceSensor::TOFDistanceSensor(){
  this->configSensor();
  this->sensorFD = initI2cBus(I2C_TOF_SENSOR_BUS,I2C_TOF_SENSOR_DEVICE);

  this->setContinousSensing(true);
  this->setFilterExtremeValues(true);
}

unsigned int TOFDistanceSensor::getSensorValues(){
  int BUFF_SIZE = 2;
  char values[BUFF_SIZE];

  // jump to offset, by "writing"
  unsigned char offset = REG_TOF_MSB;
  if(write(this->sensorFD,&offset,sizeof(offset)) != sizeof(offset)){
    std::cout << "Time of flight sensor not enough written.\n";
  }

  if(read(this->sensorFD,values,sizeof(char) * BUFF_SIZE) != BUFF_SIZE){
    std::cout << "Time of flight sensor failed to read.\n";
  } 

  uint16_t distance = ((values[0] << 8) | (values[1]));

  if(filterExtreme){
    if(distance == 20 || distance == 8190){
      return 0;
    }
  }
  return distance;
}

void TOFDistanceSensor::configSensor(){
  // configure pins to use I2C
  if(system("config-pin p9_19 i2c") != -1){
    //printf("configure pin p9_18 sucess.\n");
  }
  if(system("config-pin p9_20 i2c") != -1){
    //printf("configure pin p9_17 sucess.\n");    
  }
}

void TOFDistanceSensor::setFilterExtremeValues(bool enable){
  this->filterExtreme = enable;
}

void TOFDistanceSensor::setContinousSensing(bool enable){
  if(enable){
    writeI2cReg(this->sensorFD,REG_TOF_CONTROL,REG_TOF_CONTINOUS_MODE);
  } else {
    writeI2cReg(this->sensorFD,REG_TOF_CONTROL,REG_TOF_SINGLE_MODE);
  }
}