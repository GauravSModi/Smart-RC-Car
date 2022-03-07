#pragma once
#include <string>

#define MAX_PATH_LENGTH 4096 // set to maxium linux file path length

// fileIO utilities
bool truncateToFile(std::string filePath, std::string charsToWrite);

// ========== GPIOs ============

// read one char value from GPIO specified in filePath
char readGPIOValue(std::string filePath);
// enable GPIO Edge detection mode, set to "onRising"
// this enables Edge to be triggered when GPIO value has changed(e.g. when pressed)
void enableJoyStickEdgeOnRising(std::string joyStickPath);

// ========= I2C IOs ===========

// from I2CGuide.pdf by Brian Fraser, only changing var-names
int initI2cBus(std::string busPath, int address);
// from I2CGuide.pdf by Brian Fraser, only changing var-names
void writeI2cReg(int i2cFD, unsigned char regAddr, unsigned char value);

// ======== Math operations ==========
double max(double left, double right);
double min(double left, double right);
double clamp(double value, double upper, double lower);

void utilsDummy();
// =========== sleep =============
// calls nanosleep for miliseconds
void msleep(unsigned int forMiliseconds);