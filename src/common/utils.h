#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>

#define MAX_PATH_LENGTH 4096 // set to maxium linux file path length

void utilsDummy();

// fileIO utilities
bool truncateToFile(const char* filePath, const char* charsToWrite);

// ========== GPIOs ============

// read one char value from GPIO specified in filePath
char readGPIOValue(const char* filePath);
// enable GPIO Edge detection mode, set to "onRising"
// this enables Edge to be triggered when GPIO value has changed(e.g. when pressed)
void enableJoyStickEdgeOnRising(const char* joyStickPath);

// ======== Math operations ==========
double max(double left, double right);
double min(double left, double right);
double clamp(double value, double upper, double lower);
