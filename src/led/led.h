#pragma once

#include <stdlib.h>
#include <unistd.h>

#include <common/utils.h>

void ledDummy(){
  printf("leds module Include success\n");
}

void turnOnLED(char* lEDPath);
void turnOffLED(char* lEDPath);
bool flashLED(char* lEDPath, int forMiliseconds);
