#include "network.h"
#include <vector>
#include <mutex>
//#include <sstream>
#include <gyroscope/gyro.h>

#define LISTEN_PORT 12345
#define PUBLISH_PORT 12346

#define PUBLISH_INTERVAL_MS 1000

// static variables for module
static std::thread* networkThread;
static std::thread* publishingThread;
static bool stopbarrier = false;
static Rover* _myRover;

// static variables for publishing Thread
static std::vector<sockaddr_in> _subscribers;
static std::vector<std::string> _types;
static std::mutex subscribersLatch;

// main network listening routine
static void run(std::function<void()> shutdownFunction);
// routine for publishing update about device to subsribed entities
// for example the node http server
static void publishRun();
static bool registerSubscriber(const sockaddr_in newSubscriber,std::string type);
static bool removeSubscriber(const sockaddr_in subscriber);


void networkDummy(){
  printf("network module Include success\n");
}

//udp inner operation
void udp_stop(struct sockaddr_in sinRemote,int socketDescriptor);

void init_networkModule(std::function<void()> shutdownFunction, Rover* myRover){
	if(myRover == NULL){
		throw;
	}
	networkThread = new std::thread(run, shutdownFunction);
	publishingThread = new std::thread(publishRun);
	_myRover = myRover;
}

void clean_networkModule(void){
	publishingThread->join();
	networkThread->join();
	delete publishingThread;
	delete networkThread;
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
	
	stopbarrier=true;
}

void udp_reply(struct sockaddr_in sinRemote,int socketDescriptor, std::string message){

	// Transmit a reply:
	int sin_len = sizeof(sinRemote);
	sendto(socketDescriptor,
		message.c_str(), message.length(),
		0,
		(struct sockaddr *) &sinRemote, sin_len);

}

static void run(std::function<void()> shutdownFunction) {
	// Address
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;                   // Connection may be from network
	sin.sin_addr.s_addr = htonl(INADDR_ANY);    // Host to Network long
	sin.sin_port = htons(LISTEN_PORT);                 // Host to Network short

	// Create the server socket for UDP
	int socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);

	// Bind the server socket to the port (PORT) that we specify
	bind (socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));	

	// Check for errors (-1)

	while (!stopbarrier) {
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
    // char* value = strtok(NULL," \n");
		bool reportMessage = true;

		// make some rover command here
		if(strcmp(command,"stop") == 0) {
      udp_stop(sinRemote, socketDescriptor);
    } else if (strcmp(command,"alive") == 0) {
			udp_reply(sinRemote, socketDescriptor, "alive");
			reportMessage = false;
		} else if (strcmp(command,"moveLeft") == 0) {
			_myRover->move_left();
			printf("moveLeft\n");
		} else if (strcmp(command,"moveRight") == 0) {
			printf("moveRight\n");
			_myRover->move_right();
		} else if (strcmp(command,"moveFront") == 0) {
			_myRover->move_forward();
		} else if (strcmp(command,"moveBack") == 0) {
			_myRover->move_backward();
			printf("moveBack\n");
		} else if (strcmp(command,"subscribe") == 0) {
			registerSubscriber(sinRemote,"webClient");
		} else if (strcmp(command,"unsubscribe") == 0) {
			removeSubscriber(sinRemote);
		} else if (strcmp(command,"stopMotors") == 0) {
			_myRover->stop_rover();
			printf("stopMotors\n");
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

// =============== PUBLISHING SUBROUTINE ====================
/*
void lockSubscribers(){
	subscribersLatch.lock();
}
void unlockSubscribers(){
	subscribersLatch.unlock();
}*/
// thread-safe add subscriber to list if not exist,
// do nothing if already a subscriber
// return true if sucess
// return false if already a subscriber
static bool registerSubscriber(const sockaddr_in newSubscriber,std::string type){
	subscribersLatch.lock();
	// search for same subscriber
	bool found = false;
	for(int i = 0; i < _subscribers.size(); i++){
		// compare PORT + ADDRESS
		if(_subscribers[i].sin_addr.s_addr == newSubscriber.sin_addr.s_addr && _subscribers[i].sin_port == newSubscriber.sin_port){
			found = true;
		}
	}

	if(!found){
		_subscribers.emplace_back(newSubscriber);
		_types.emplace_back(type);
	}

	subscribersLatch.unlock();
	return !found;
}
static bool removeSubscriber(const sockaddr_in subscriber){
	subscribersLatch.lock();
	bool found = false;
	auto itr = _subscribers.begin();
	auto typeItr = _types.begin();
	for(; itr != _subscribers.end(); itr++, typeItr++){
		// compare PORT + ADDRESS
		if(itr->sin_addr.s_addr == subscriber.sin_addr.s_addr && itr->sin_port == subscriber.sin_port){
			found = true;
			break;
		}
	}
	if(found){
		_subscribers.erase(itr);
		_types.erase(typeItr);
	}
	subscribersLatch.unlock();
	return found;
}

static void publishRun(){

	// Address
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;                   // Connection may be from network
	sin.sin_addr.s_addr = htonl(INADDR_ANY);    // Host to Network long
	sin.sin_port = htons(PUBLISH_PORT); 

  // Create the server socket for UDP
  int publishDiscriptor = socket(PF_INET, SOCK_DGRAM, 0);
	bind(publishDiscriptor, (struct sockaddr*) &sin, sizeof(sin));	

	std::cout<< "Publishing Thread Running\n";
		
	while(!stopbarrier){
		subscribersLatch.lock();
		std::cout<< "Subscriber count: " << _subscribers.size() << "\n";
		if(!_subscribers.empty()){
			// prepare messages for subscribers
			Vec2<double> position = _myRover->getPosition();
			std::string webClientUpdates = 
			"publish>>{"
			"\"yaw\":" + to_string(getYaw()) +","
			"\"xPos\":" + to_string(position.x) + ","
			"\"yPos\":" + to_string(position.y) +
			"}";
			//<< "}";

			for(int i = 0; i < _subscribers.size(); i++){	
				if(_types[i] == "webClient"){
					udp_reply(_subscribers[i],publishDiscriptor,webClientUpdates);
				} else if(_types[i] == "controller"){
					// nothing yet, keep alive maybe?
				}
			}
		}
		subscribersLatch.unlock();
		msleep(PUBLISH_INTERVAL_MS);
	}

}