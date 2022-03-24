#include <iostream>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <fstream>
#include <unistd.h>
#include <string.h>
#include <thread>
using namespace std;

static std::thread* readingThread;
static int x_acc = 0;
static int y_acc = 0;
static int z_acc = 0;

void acc_init();
void acc_cleanup();

int get_xacc();
int get_yacc();
int get_zacc();


//implement init function
//implement cleanup function