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

static int16_t xGyro = 0;
static int16_t yGyro = 0;
static int16_t zGyro = 0 ;

static float yaw=0;

//static float prev_yaw = 0;

static float error_x = 0;
static float error_y = 0;
static float error_z = 0;


float get_xGyro();
float get_yGyro();
float get_zGyro();

float getYaw();
//float getAbsYaw();


void calculateAngle();

bool is90();

void gyro_init();
void gyro_cleanup();

// reset yaw to 0
void resetYaw();
 