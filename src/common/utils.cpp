#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

// fileIO
bool truncateToFile(std::string filePath, std::string charsToWrite){
  FILE *file = fopen(filePath.c_str(),"w");
  if (file == NULL) {
      printf("ERROR OPENING %s.\n", filePath.c_str());
      return false;
  }
  int charWritten = fprintf(file, "%s", charsToWrite.c_str());
  if (charWritten <= 0) {
      printf("ERROR WRITING DATA for %s.\n", filePath.c_str());
      return false;
  }
  fclose(file);
  return true;
}

char readGPIOValue(std::string filePath){
  FILE *file = fopen(filePath.c_str(),"r");
  if (file == NULL) {
      printf("ERROR OPENING %s.\n", filePath.c_str());
      return false;
  }
  
  const int MAX_LENGTH = 64;
  char buff[MAX_LENGTH];
  fgets(buff, MAX_LENGTH, file);

  printf("buffer[%c]",buff[0]);

  fclose(file);
  return buff[0];
}

void enableJoyStickEdgeOnRising(std::string joyStickPath){
  // mostly referenced Example in LED Guide provide by Brian Fraser
  if(!truncateToFile((joyStickPath + "edge"),"falling")){
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

void utilsDummy(){
  printf("utils Include success\n");
}