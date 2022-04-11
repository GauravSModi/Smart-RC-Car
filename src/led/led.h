#pragma once
#include <string>

#define ZERO_LED_DIR_PATH "/sys/class/leds/beaglebone:green:usr0/"
#define FIRST_LED_DIR_PATH "/sys/class/leds/beaglebone:green:usr1/"
#define SECOND_LED_DIR_PATH "/sys/class/leds/beaglebone:green:usr2/"
#define THIRD_LED_DIR_PATH "/sys/class/leds/beaglebone:green:usr3/"

void init_LEDModule();
void cleanUp_LEDModule();

void startFlashing();
void stopFlashing();

void turnOnLED(std::string lEDPath);
void turnOffLED(std::string lEDPath);
bool flashLED(std::string lEDPath, int forMiliseconds);
void ledDummy();

