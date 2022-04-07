#include "wireless.h"
#include <linux/wireless.h>


#define NET_INTERFACE_NAME "wlan0"

WirelessManager* WirelessManager::instance = NULL;

bool WirelessManager::connectTo(std::string name){
  
  auto scanResult = scanWifi();
  int scanSize = scanResult.size();
  for(int i = 0; i < scanSize; i++){
    if(name.compare(scanResult[i].b.essid) == 0){
      int skfd = iw_sockets_open();/*
      wireless_config iwconf;
      iw_get_basic_config(skfd,NET_INTERFACE_NAME,&iwconf);

      // connecting to access point referenced
      // github.com/AutonomyLab/ardronelib/blob/master/ARDroneLib/VP_SDK/VP_Com/linux/vp_com_wifi.c

      iwconf.has_nwid = 1;//scanResult[i].b.has_nwid;
      iwconf.has_freq = 1;//scanResult[i].b.has_freq;
      iwconf.has_key = 1;//scanResult[i].b.has_key;

      iwconf.has_mode = scanResult[i].b.has_mode;
      iwconf.mode = scanResult[i].b.mode;//IW_MODE_AUTO;//??

      iwconf.has_essid = 1; //scanResult[i].b.has_essid;
      iwconf.essid_on = 1; //scanResult[i].b.essid_on; // 1
      strncpy(&iwconf.essid[0], scanResult[i].b.essid, IW_ESSID_MAX_SIZE+1);
*/
      //iw_set_basic_config(skfd,NET_INTERFACE_NAME,&scanResult[i].b);
    
      iw_sockets_close(skfd);
      return true;
    }
  }

  return false;
}

std::vector<wireless_scan> WirelessManager::scanWifi(){
  int skfd = iw_sockets_open();
  wireless_scan_head scanHead;

  char searchString[] = NET_INTERFACE_NAME;
  iw_scan(skfd,searchString,WE_VERSION,&scanHead);
//  iw_process_scan(skfd,searchString,WE_VERSION,&scanHead);

  wireless_scan* curr = scanHead.result;
  std::vector<wireless_scan> result;
  while (curr != NULL){
    result.push_back(*curr);
    curr = curr->next;
  }

  iw_sockets_close(skfd);

  return result;
}
