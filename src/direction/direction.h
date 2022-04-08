#include <stdio.h>
#include <stdlib.h>
#include <string.h>			
#include <unistd.h>			
#include <thread>

#include "../rover/rover.h"
#include "../gyroscope/gyro.h"
//#include "../common/utils.h"

static std::thread* direction_readingThread;

void init_direction();
void cleanup_direction();


