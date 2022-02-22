#pragma once

#include <stdlib.h>
#include <unistd.h>

#include <common/utils.h>

void ledDummy();

void turnOnLED(char* lEDPath);
void turnOffLED(char* lEDPath);
bool flashLED(char* lEDPath, int forMiliseconds);
