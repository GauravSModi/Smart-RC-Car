#include <iostream>
#include <iwlib.h>
#include <wireless/wireless.h>

#define WIFI_NAME "baden"
#define INTERFACE_NAME "wlan0"

// wireless-tools example referenced https://github.com/simonschuang/iwlib-example/blob/master/main.c

void testwifistats(){
  
  int skfd = iw_sockets_open();
  iwrange range;
  iw_get_range_info(skfd, "wlan0", &range);

  iwstats stats;
  std::cout << "getstats returns: " << iw_get_stats(skfd,"wlan0",&stats,NULL,false) << "\n";

  std::cout << "Status: " << stats.status << "\n";
  std::cout << "Level: " << std::hex << (int) stats.qual.level << "\n";
  std::cout << "Quality: " << std::hex << (int)  stats.qual.qual << "\n";
  //std::cout << "Discard: " << stats.di << "\n";

  iw_sockets_close(skfd);
}

void testscanwifi(){

  int skfd = iw_sockets_open();
  wireless_scan_head scanHead;

  char searchString[] = "wlan0";

  iw_scan(skfd,searchString,WE_VERSION,&scanHead);

  wireless_scan* curr = scanHead.result;
  while (curr != NULL){
    std::cout << "Wifi: " << (char*) curr->b.essid << "\n";
    curr = curr->next;
  }
}

void testClassScanWifi(){
  WirelessManager* manager = WirelessManager::getInstance();
  auto result = manager->scanWifi();

  for(int i = 0; i < (int) result.size();i++){
    std::cout << "wifi: "<< result[i].b.name << "::::" << result[i].b.essid << "\n";
    
  }
}

void testConnectTo(){
  WirelessManager* manager = WirelessManager::getInstance();
  bool status = manager->connectTo(WIFI_NAME);
  std::cout << "wifi status: " << status << "\n";
}

void printCurrent(){
  int skfd = iw_sockets_open();

  wireless_config iwconfig;
  iw_get_basic_config(skfd, INTERFACE_NAME, &iwconfig);

  
  std::cout 
  << "name: " << iwconfig.essid << "\n"
  << "mode: " << iwconfig.mode << "\n"
  << "essid_on: " << iwconfig.essid_on << "\n";

  iw_sockets_close(skfd);
}

int main(int argc, char const *argv[]){

  //testwifistats();
  //testscanwifi();
  //testClassScanWifi();
  printCurrent();
  //testConnectTo();

  return 0;
}