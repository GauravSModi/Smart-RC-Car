#pragma once
#include <stdio.h>
#include <functional>
/*udp file: mainly focus on showing history sample data, and packet handling*/

void init_udp(std::function<void()> shutdownFunction);
void clean_udp(void);

