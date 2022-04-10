#include <string>
#include <netdb.h>
#include <cstring>
#include <unistd.h>	
#include <arpa/inet.h>

//#include <sys/types.h>
//#include <sys/socket.h>

#define CONTROLLER_PORT 12343
#define ROVER_PORT 12345
#define MSG_MAX_LEN 1500

class NetworkControls {
  int socketDescriptor;
  struct sockaddr_in sinRemote;
public:
  // control an interface and connect to the remote UDP address
  NetworkControls(std::string remoteAddress);
  ~NetworkControls(){
    close(this->socketDescriptor);
  }

  std::string send(std::string message);

  void udp_reply(std::string message);

};
