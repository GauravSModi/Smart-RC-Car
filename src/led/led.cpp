#include "led.h"

#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <common/utils.h>


void ledDummy(){
  printf("leds module Include success\n");
}

void turnOnLED(std::string lEDPath){
    truncateToFile((lEDPath + "brightness"),"1");
}

void turnOffLED(std::string lEDPath){
    truncateToFile(lEDPath + "brightness","0");
}

bool flashLED(std::string lEDPath, int forMiliseconds){
    truncateToFile(lEDPath + "brightness","1");
    usleep(forMiliseconds * 1000);
    truncateToFile(lEDPath + "brightness","0");
    return true;
}