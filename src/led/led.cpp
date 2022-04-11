#include "led.h"

#include <iostream>
//#include <stdlib.h>
//#include <unistd.h>
#include <common/utils.h>
#include <thread>

#define FLASH_INTERVAL_MS 250

std::thread* ledThread;
static bool isRunning;
static bool shouldFlash = false;


static void ledMainThreadRun();
static void disableLED(std::string lEDPath);

void init_LEDModule(){

    // disable LEDs
    disableLED(ZERO_LED_DIR_PATH);
    disableLED(FIRST_LED_DIR_PATH);
    disableLED(SECOND_LED_DIR_PATH);
    disableLED(THIRD_LED_DIR_PATH);

    isRunning = true;
    ledThread = new std::thread(ledMainThreadRun);
}

void cleanUp_LEDModule(){
    isRunning = false;
    ledThread->join();
    delete ledThread;
}

void startFlashing(){
    shouldFlash = true;
}
void stopFlashing(){
    shouldFlash = false;
}

// interacting with LEDs
void disableLED(std::string lEDPath){
    truncateToFile((lEDPath + "trigger"),"none");
}

void ledMainThreadRun(){

    while(isRunning){
        if(!shouldFlash){
            turnOnLED(ZERO_LED_DIR_PATH);
            turnOnLED(FIRST_LED_DIR_PATH);
            turnOnLED(SECOND_LED_DIR_PATH);
            turnOnLED(THIRD_LED_DIR_PATH);
        }
        else if(shouldFlash){
            turnOffLED(ZERO_LED_DIR_PATH);
            turnOffLED(FIRST_LED_DIR_PATH);
            turnOffLED(SECOND_LED_DIR_PATH);
            turnOffLED(THIRD_LED_DIR_PATH);
            msleep(FLASH_INTERVAL_MS);
            turnOnLED(ZERO_LED_DIR_PATH);
            turnOnLED(FIRST_LED_DIR_PATH);
            turnOnLED(SECOND_LED_DIR_PATH);
            turnOnLED(THIRD_LED_DIR_PATH);
        }

        msleep(FLASH_INTERVAL_MS);
    }
}

void turnOnLED(std::string lEDPath){
    truncateToFile((lEDPath + "brightness"),"1");
}

void turnOffLED(std::string lEDPath){
    truncateToFile(lEDPath + "brightness","0");
}

bool flashLED(std::string lEDPath, int forMiliseconds){
    truncateToFile(lEDPath + "brightness","1");
    msleep(forMiliseconds);
    truncateToFile(lEDPath + "brightness","0");
    return true;
}