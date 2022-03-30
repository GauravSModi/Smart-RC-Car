/*
 * UDP Listening program on port 12345
 * By Brian Fraser, Modified from Linux Programming Unleashed (book)
 *
 * Usage:
 *	On the target, run this program (netListenTest).
 *	On the host:
 *		> netcat -u 192.168.7.2 12345
 *		(Change the IP address to your board)
 *
 *	On the host, type in a number and press enter:
 *		4<ENTER>
 *
 *	On the target, you'll see a debug message:
 *	    Message received (2 bytes):
 *	    '4
 *	    '
 *
 *	On the host, you'll see the message:
 *	    Math: 4 + 1 = 5
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <cstring>			// for strncmp()
#include <unistd.h>			// for close()
#include "network.h"
#include <motors/motors.h>
// cpp thread
#include <thread>
#include <string>


#define MSG_MAX_LEN 1500
#define PORT 12345

static std::thread* networkThread;
static bool stop_barrier=false;
void run(std::function<void()> shutdownFunction);

void networkDummy(){
  printf("network module Include success\n");
}

//udp inner operation
void udp_stop(struct sockaddr_in sinRemote,int socketDescriptor);

void init_udp(std::function<void()> shutdownFunction){
	networkThread = new std::thread(run, shutdownFunction);
}

void clean_udp(void){
	networkThread->join();
}

void udp_stop(struct sockaddr_in sinRemote,int socketDescriptor){
	char messageTx[MSG_MAX_LEN];
	sprintf(messageTx,"Program terminating.\n");
	// Transmit a reply:
	int sin_len = sizeof(sinRemote);
	sendto(socketDescriptor,
		messageTx, strlen(messageTx),
		0,
		(struct sockaddr *) &sinRemote, sin_len);
	
	stop_barrier=true;
}

void udp_reply(struct sockaddr_in sinRemote,int socketDescriptor, std::string message){

	// Transmit a reply:
	int sin_len = sizeof(sinRemote);
	sendto(socketDescriptor,
		message.c_str(), message.length(),
		0,
		(struct sockaddr *) &sinRemote, sin_len);

}


void run(std::function<void()> shutdownFunction) {
	// Address
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;                   // Connection may be from network
	sin.sin_addr.s_addr = htonl(INADDR_ANY);    // Host to Network long
	sin.sin_port = htons(PORT);                 // Host to Network short
	
	// Create the server socket for UDP
	int socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);

	// Bind the server socket to the port (PORT) that we specify
	bind (socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));	

	// Check for errors (-1)

	while (!stop_barrier) {
		// Get the data (blocking)
		// Will change sin (the address) to be the address of the client.
		// Note: sin passes information in and out of call!
		struct sockaddr_in sinRemote;
		unsigned int sin_len = sizeof(sinRemote);
		char messageRx[MSG_MAX_LEN];

		// Pass buffer size - 1 for max # bytes so room for the null (string data)
		int bytesRx = recvfrom(socketDescriptor,
			messageRx, MSG_MAX_LEN - 1, 0,
			(struct sockaddr *) &sinRemote, &sin_len);

		// Check for errors (-1)

		// Make it null terminated (so string functions work)
		// - recvfrom given max size - 1, so there is always room for the null
		messageRx[bytesRx] = 0;
	
		// split into tokens
    char* command = strtok(messageRx," \n");
    //char* value = strtok(NULL," \n");
		bool reportMessage = true;

		//make some rover command here
		if(strcmp(command,"stop") == 0) {
      udp_stop(sinRemote, socketDescriptor);
    } else if (strcmp(command,"alive") == 0) {
			udp_reply(sinRemote, socketDescriptor, "alive");
			reportMessage = false;
		} else if (strcmp(command,"moveLeft") == 0) {
			printf("moveLeft\n");
		} else if (strcmp(command,"moveRight") == 0) {
			printf("moveRight\n");
		} else if (strcmp(command,"moveFront") == 0) {
			printf("moveFront\n");
		} else if (strcmp(command,"moveBack") == 0) {
			printf("moveBack\n");
		}

		if(reportMessage){
			std::string messageStr(messageRx);
			printf("Recieved: %s\n",messageStr.c_str());
		}
		
	}

	// Close
	close(socketDescriptor);

	shutdownFunction();
}