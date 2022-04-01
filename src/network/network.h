/*udp file: mainly focus on showing history sample data, and packet handling*/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <cstring>			// for strncmp()
#include <unistd.h>			// for close()
#include <functional>
#include <thread>
#include <string>

#include "rover/rover.h"

void init_udp(std::function<void()> shutdownFunction, rover* myRover);
void clean_udp(void);