#include <network/networkControls.h>

//#define REMOTE_ADDRESS "192.168.43.100"
#define REMOTE_ADDRESS "192.168.7.2"

int main(){

  NetworkContols* controller = new NetworkContols(REMOTE_ADDRESS);
  
  //contoller->send()
  if(controller){}

  return 0;
}



