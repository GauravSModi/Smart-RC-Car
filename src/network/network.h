#pragma once
#include <stdio.h>
/*udp file: mainly focus on showing history sample data, and packet handling*/

void init_udp(void);
void clean_udp(void);

void networkDummy(){
  printf("network module Include success\n");
}