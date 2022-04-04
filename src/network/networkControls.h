#include <string>
#include "network.h"
#include <netdb.h>
#include <cstring>
#include <unistd.h>	
#include <arpa/inet.h>

//#include <sys/types.h>
//#include <sys/socket.h>

#define CONTROLLER_PORT 12347

class NetworkContols {
  int socketDiscriptor;
  struct sockaddr_in sinRemote;
public:
  // control an interface and connect to the remote UDP address
  NetworkContols(std::string remoteAddress);
  ~NetworkContols(){
    close(this->socketDiscriptor);
  }

  std::string send(std::string message);

};

