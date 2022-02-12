#ifndef _LED_H_
#define _LED_H_

#include "../common/utils.h"
#include <stdlib.h>
#include <unistd.h>

void turnOnLED(char* lEDPath);
void turnOffLED(char* lEDPath);
bool flashLED(char* lEDPath, int forMiliseconds);

#endif