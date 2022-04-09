/*udp file: mainly focus on showing history sample data, and packet handling*/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <cstring>			// for strncmp()
#include <unistd.h>			// for close()
#include <functional>
#include <string>
#include <thread>
#include "rover/rover.h"
/*udp file: mainly focus on showing history sample data, and packet handling*/

#define MSG_MAX_LEN 1500

void init_networkModule(std::function<void()> shutdownFunction, Rover* myRover);
void clean_networkModule(void);

void udp_reply(struct sockaddr_in sinRemote,int socketDescriptor, std::string message);

