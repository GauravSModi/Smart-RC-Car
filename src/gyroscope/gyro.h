#include <iostream>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <fstream>
#include <unistd.h>
#include <string.h>
#include <thread>
#include <ctime>

#include <cerrno>
#include <cstring>
#include <system_error>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>

using namespace std;

#define deviceADDR 0x68
#define PWR_MGMT_1 0x6B

static std::thread* gyro_readingThread;

static int xGyro = 0;
static int yGyro = 0;
static int zGyro = 0 ;

//static int xGyro_past = 0;
//static int yGyro_past = 0;
//static int zGyro_past = 0;

static float gyroAngleX = 0;
static float gyroAngleY =0;

static float roll=0;
static float pitch=0;
static float yaw=0;

static float error_x = 0;
static float error_y = 0;
static float error_z = 0;

//static int current_time = 0;
//static int prev_time = 0;

float get_xGyro();
float get_yGyro();
float get_zGyro();

void gyro_init();
void gyro_cleanup();

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
 