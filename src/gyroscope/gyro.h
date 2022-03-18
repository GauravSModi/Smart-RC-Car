#include <iostream>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <fstream>
#include <unistd.h>
#include <string.h>
#include <thread>
using namespace std;

#define deviceADDR 0x68
#define PWR_MGMT_1 0x6B

static int xGyro;
static int yGyro;
static int zGyro;

int get_xGyro();
int get_yGyro();
int get_zGyro();

/*#define ACCEL_X_OUT_H "0x3B"
#define ACCEL_X_OUT_L "0x3C"
#define ACCEL_Y_OUT_H "0x3D"
#define ACCEL_Y_OUT_L "0x3E"
#define ACCEL_Z_OUT_H "0x3F"
#define ACCEL_Z_OUT_L "0x40"
#define GYRO_X_OUT_H "0x43"
#define GYRO_X_OUT_L "0x44"
#define GYRO_Y_OUT_H "0x45"
#define GYRO_Y_OUT_L "0x46"
#define GYRO_Z_OUT_H "0x47"
#define GYRO_Z_OUT_L "0x48"*/
 