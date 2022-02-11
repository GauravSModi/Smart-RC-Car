#include <stdio.h>
#include <stdlib.h>
#include <string>

#define MAX_PATH_LENGTH 4096 // set to maxium linux file path length

// fileIO utilities
bool truncateToFile(char* filePath, char* charsToWrite);
char readGPIOValue(char* filePath);
void enableJoyStickEdgeOnRising(char* joyStickPath);

