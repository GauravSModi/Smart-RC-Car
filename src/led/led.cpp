#include "led.h"

#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <common/utils.h>


void ledDummy(){
  printf("leds module Include success\n");
}

void turnOnLED(char* lEDPath){
    char valuePath[MAX_PATH_LENGTH];
    strcpy(valuePath,lEDPath);
    strcat(valuePath,"brightness");
    truncateToFile(valuePath,"1");
}

void turnOffLED(char* lEDPath){
    char valuePath[MAX_PATH_LENGTH];
    strcpy(valuePath,lEDPath);
    strcat(valuePath,"brightness");
    truncateToFile(valuePath,"0");
}

bool flashLED(char* lEDPath, int forMiliseconds){
    // mostly referenced Example in LED Guide provide by Brian Fraser
    char valuePath[MAX_PATH_LENGTH];
    strcpy(valuePath,lEDPath);
    strcat(valuePath,"brightness");

    truncateToFile(valuePath,"1");
    usleep(forMiliseconds * 1000);
    truncateToFile(valuePath,"0");

    return true;
}