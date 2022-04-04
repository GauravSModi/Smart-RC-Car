/*udp file: mainly focus on showing history sample data, and packet handling*/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <cstring>			// for strncmp()
#include <unistd.h>			// for close()
#include <functional>
#include <string>
/*udp file: mainly focus on showing history sample data, and packet handling*/
#include <thread>
#include "rover/rover.h"


#define MSG_MAX_LEN 1500

void init_udp(std::function<void()> shutdownFunction);
void clean_udp(void);

void udp_reply(struct sockaddr_in sinRemote,int socketDescriptor, std::string message);
