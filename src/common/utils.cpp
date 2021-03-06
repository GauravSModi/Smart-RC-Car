#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

// fileIO
bool truncateToFile(std::string filePath, std::string charsToWrite){
  FILE *file = fopen(filePath.c_str(),"w");
  if (file == NULL) {
      printf("ERROR OPENING %s.\n", filePath.c_str());
      return false;
  }
  int charWritten = fprintf(file, "%s", charsToWrite.c_str());
  if (charWritten <= 0) {
      printf("ERROR WRITING DATA for %s.\n", filePath.c_str());
      return false;
  }
  fclose(file);
  return true;
}

// void writeGPIOValue(char* filePath, char* value){
void writeGPIOValue(std::string filePath, std::string value){
  int file = open(filePath.c_str(), O_WRONLY);
  if (file == -1){
    printf("Unable to open %s\n", filePath.c_str());
    return;
  }
  if (write(file, value.c_str(), value.length()) != value.length()){
    printf("Error writing to %s\n", filePath.c_str());
    return;
  }
  close(file);
}

char readGPIOValue(std::string filePath){
  FILE *file = fopen(filePath.c_str(),"r");
  if (file == NULL) {
      printf("ERROR OPENING %s.\n", filePath.c_str());
      return false;
  }
  
  const int MAX_LENGTH = 64;
  char buff[MAX_LENGTH];
  fgets(buff, MAX_LENGTH, file);

  printf("buffer[%c]",buff[0]);

  fclose(file);
  return buff[0];
}

void enableJoyStickEdgeOnRising(std::string joyStickPath){
  // mostly referenced Example in LED Guide provide by Brian Fraser
  if(!truncateToFile((joyStickPath + "edge"),"falling")){
      exit(1);
  }
}

// ========= I2C IOs ===========

// from I2CGuide.pdf by Brian Fraser, only changing var-names
int initI2cBus(std::string busPath, int address){
  int i2cFD = open(busPath.c_str(), O_RDWR);

  int result = ioctl(i2cFD, I2C_SLAVE ,address);
  if(result < 0){
    perror("I2C: Unable to set I2C device to slave address.");
    exit(1);
  }

  return i2cFD;
}

// from I2CGuide.pdf by Brian Fraser, only changing var-names
void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value){
	unsigned char buff[2];
	buff[0] = regAddr;
	buff[1] = value;
	int res = write(i2cFileDesc, buff, 2);
	if (res != 2) {
		perror("I2C: Unable to write i2c register.");
		//exit(1);
	}
}
/*{
  unsigned char buff[2];
  buff[0] = regAddr;
  buff[1] = value;
  int res = write(i2cFD, buff, 2);
  if(res != 2){
    perror("I2C: Unable to write i2c register.");
    //exit(1);
  }
}*/

unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr)
{
	// To read a register, must first write the address
	int res = write(i2cFileDesc, &regAddr, sizeof(regAddr));
	if (res != sizeof(regAddr)) {
		perror("Unable to write i2c register.");
		//exit(-1);
	}

	// Now read the value and return it
	char value = 0;
	res = read(i2cFileDesc, &value, sizeof(value));
	if (res != sizeof(value)) {
		perror("Unable to read i2c register");
		//exit(-1);
	}
	return value;
}

// ============= MATH ==============

double max(double left, double right){
  return left > right ? left : right;
}
double min(double left, double right){
  return left < right ? left : right;
}

double clamp(double value, double upper, double lower){
  return max( min(value,upper), lower);
}

// from https://stackoverflow.com/questions/16782746/what-is-faster-than-stdpow
double fastPow(double a, double b) {
  union {
    double d;
    int x[2];
  } u = { a };
  u.x[1] = (int)(b * (u.x[1] - 1072632447) + 1072632447);
  u.x[0] = 0;
  return u.d;
}

void utilsDummy(){
  printf("utils Include success\n");
}

// sleep
// referenced https://stackoverflow.com/questions/1157209/is-there-an-alternative-sleep-function-in-c-to-milliseconds
void msleep(unsigned int forMiliseconds){
  struct timespec tspec;

  tspec.tv_sec = forMiliseconds / 1000;
  tspec.tv_nsec = (forMiliseconds % 1000) * 1000000;

  nanosleep(&tspec,&tspec);
}

// timespec
timespec_t timespec_add(timespec_t t, int seconds, long long nanoseconds){
  int _seconds;
  long long _nanoseconds;
  if(nanoseconds >= NANOSECONDS_IN_SECOND){
    _seconds = seconds + (int) (nanoseconds / NANOSECONDS_IN_SECOND);
    _nanoseconds = nanoseconds % NANOSECONDS_IN_SECOND;
  } else {
    _seconds = seconds;
    _nanoseconds = nanoseconds;
  }

  t.tv_sec += _seconds;
  t.tv_nsec += _nanoseconds;
  
  if(t.tv_nsec >= NANOSECONDS_IN_SECOND){
    t.tv_sec += (int) (t.tv_nsec / NANOSECONDS_IN_SECOND);
    t.tv_nsec = t.tv_nsec % NANOSECONDS_IN_SECOND;
  }

  return t;
}