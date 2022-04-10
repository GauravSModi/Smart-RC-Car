#include "network_js.h"

NetworkControls::NetworkControls(std::string remoteAddress){

  // Address
  struct sockaddr_in sin;
  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;                   // Connection may be from network
  sin.sin_addr.s_addr = htonl(INADDR_ANY);    // Host to Network long
  sin.sin_port = htons(CONTROLLER_PORT);      // Host to Network short
  
  // Create the server socket for UDP
  this->socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);

  // Bind the server socket to the port (PORT) that we specify
  bind (this->socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));	

  // set non-blocking with timeout
  // https://stackoverflow.com/questions/15941005/making-recvfrom-function-non-blocking
  struct timeval read_timeout;
  read_timeout.tv_sec = 1;
  read_timeout.tv_usec = 0;
  setsockopt(this->socketDescriptor, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof(read_timeout));





  // setup remote (the) networking thread
  memset(&sin, 0, sizeof(this->sinRemote));
  this->sinRemote.sin_family = AF_INET;                   // Connection may be from network
  //this->sinRemote.sin_addr.s_addr = htonl(remoteAddress);    // Host to Network long
  inet_pton(AF_INET, remoteAddress.c_str(), &(this->sinRemote.sin_addr));
  this->sinRemote.sin_port = htons(ROVER_PORT);
  
  // // send to network module and recieve a message to handshake
  // std::string response = this->sendMessage("alive");

  // printf("Constructor Recieved: %s\n",response.c_str());
  
}


void NetworkControls::sendMessage(std::string message){
  int sin_len = sizeof(sinRemote);
  sendto(socketDescriptor,
    message.c_str(), message.length(),
    0,
    (struct sockaddr *) &sinRemote, sin_len);
}