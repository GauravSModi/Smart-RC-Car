#pragma once
#include <stdlib.h>
#include <string>
#include <vector>
#include <iwlib.h>

class WirelessManager {
  static WirelessManager* instance;

  WirelessManager(){

  }

public:

  static WirelessManager* getInstance(){
    if(instance == NULL){
      instance = new WirelessManager();
    }
    return instance;
  }

  bool connectTo(std::string name);
  std::vector<wireless_scan> scanWifi();
};