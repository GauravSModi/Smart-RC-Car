#include <iostream>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <fstream>
#include <unistd.h>
#include <string.h>
#include <thread>
#include <cmath>
using namespace std;

static std::thread* readingThread;
static int x_acc = 0;
static int y_acc = 0;
static int z_acc = 0;

static float accAngleX = 0;
static float accAngleY = 0; 

void acc_init();
void acc_cleanup();

int get_xacc();
int get_yacc();
int get_zacc();

int get_accAngleX();
int get_accAngleY();

