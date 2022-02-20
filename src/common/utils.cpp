#include "utils.h"
#include <cstring>

// fileIO
bool truncateToFile(char* filePath, const char* charsToWrite){
  FILE *file = fopen(filePath,"w");
  if (file == NULL) {
      printf("ERROR OPENING %s.\n", filePath);
      return false;
  }
  int charWritten = fprintf(file, "%s", charsToWrite);
  if (charWritten <= 0) {
      printf("ERROR WRITING DATA for %s.\n", filePath);
      return false;
  }
  fclose(file);
  return true;
}

char readGPIOValue(const char* filePath){
  FILE *file = fopen(filePath,"r");
  if (file == NULL) {
      printf("ERROR OPENING %s.\n", filePath);
      return false;
  }
  
  const int MAX_LENGTH = 64;
  char buff[MAX_LENGTH];
  fgets(buff, MAX_LENGTH, file);

  printf("buffer[%c]",buff[0]);

  fclose(file);
  return buff[0];
}

void enableJoyStickEdgeOnRising(const char* joyStickPath){
  // mostly referenced Example in LED Guide provide by Brian Fraser
  char edgePath[MAX_PATH_LENGTH];
  strcpy(edgePath,joyStickPath);
  strcat(edgePath,"edge");

  

  if(!truncateToFile(edgePath,"falling")){
      exit(1);
  }
}

double max(double left, double right){
  return left > right ? left : right;
}
double min(double left, double right){
  return left < right ? left : right;
}

double clamp(double value, double upper, double lower){
  return max( min(value,upper), lower);
}
