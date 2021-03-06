#include "networkControls.h"

NetworkContols::NetworkContols(std::string remoteAddress){

  // Address
  struct sockaddr_in sin;
  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;                   // Connection may be from network
  sin.sin_addr.s_addr = htonl(INADDR_ANY);    // Host to Network long
  sin.sin_port = htons(CONTROLLER_PORT);                 // Host to Network short
  
  // Create the server socket for UDP
  this->socketDiscriptor = socket(PF_INET, SOCK_DGRAM, 0);

  // Bind the server socket to the port (PORT) that we specify
  bind (this->socketDiscriptor, (struct sockaddr*) &sin, sizeof(sin));	

  // set non-blocking with timeout
  // https://stackoverflow.com/questions/15941005/making-recvfrom-function-non-blocking
  struct timeval read_timeout;
  read_timeout.tv_sec = 1;
  read_timeout.tv_usec = 0;
  setsockopt(this->socketDiscriptor, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof(read_timeout));





  // setup remote (the) networking thread
  memset(&sin, 0, sizeof(this->sinRemote));
  this->sinRemote.sin_family = AF_INET;                   // Connection may be from network
  //this->sinRemote.sin_addr.s_addr = htonl(remoteAddress);    // Host to Network long
  inet_pton(AF_INET, remoteAddress.c_str(), &(this->sinRemote.sin_addr));
  this->sinRemote.sin_port = htons(CONTROLLER_PORT);
  
  // send to network module and recieve a message to handshake
  std::string response = this->send("alive");

  printf("Constructor Recieved: %s\n",response.c_str());
  
}

std::string NetworkContols::send(std::string message){
  udp_reply(this->sinRemote,this->socketDiscriptor,message);
  
  unsigned int sin_len = sizeof(this->sinRemote);
  char messageRx[MSG_MAX_LEN];

  // recieve reply
  int bytesRx = recvfrom(this->socketDiscriptor,
    messageRx, MSG_MAX_LEN - 1, 0,
    (struct sockaddr *) &this->sinRemote, &sin_len);
  messageRx[bytesRx] = '\0';

  return messageRx;
}