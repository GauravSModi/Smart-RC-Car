#include <iostream>
#include <iwlib.h>

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

int main(int argc, char const *argv[]){

  //testwifistats();
  testscanwifi();

  return 0;
}