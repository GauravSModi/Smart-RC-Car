#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <linux/joystick.h>
#include <chrono>

#include "network_js.h"

// #define ROVER_ADDRESS "192.168.43.100"
// #define ROVER_ADDRESS "192.168.7.2"
#define ROVER_ADDRESS "127.0.0.1"
#define JS_FILE_PATH "/dev/input/js0"
#define VERTICAL_AXIS 1
#define HORIZONTAL_AXIS 0
#define SELECT_BUTTON 8
#define START_BUTTON 9
#define RED_BUTTON 1
#define YELLOW_BUTTON 2

enum input_mapping{UP=1, DOWN=2, LEFT=3, RIGHT=4, STOP=0, ON=32767};

std::queue<int> input_queue;
std::queue<int> stop_queue;

static std::mutex mutex_stop;
static std::condition_variable cv_stop;

static void listen_for_shutdown(bool* quit);
static void queue_handler(bool* quit, NetworkControls* network);
static void event_handler(struct js_event event, NetworkControls* network);

int main(int argc, char **argv){

	NetworkControls network(ROVER_ADDRESS);

	int js_file = -1;
	
	while (js_file < 0){
		js_file = open(JS_FILE_PATH, O_RDONLY | O_NONBLOCK);
		if (js_file < 0){
			printf("Error: Couldn't open joystick file\n");
		}
	}

	bool quit = false;

	std::thread shutdownThread(listen_for_shutdown, &quit);
	std::thread queueThread(queue_handler, &quit, &network);

	while (!quit){
		struct js_event event;

		if (read(js_file, &event, sizeof(struct js_event)) > 0){
			event_handler(event, &network);
		}
	}

	shutdownThread.join();
	queueThread.join();

	return 0;
}


static void listen_for_shutdown(bool* quit){
	std::string buffer;
	while (!*quit){
		std::cout << "Enter 'q' to quit: ";
		std::cin >> buffer;
		if (buffer == "q"){
			*quit = true;
		}
	}
}


static void queue_handler(bool* quit, NetworkControls* network){
	std::unique_lock<std::mutex> stop_lock(mutex_stop);
	printf("got mutex\n");
	while (!*quit){
		if (input_queue.size() > 0){
			int input_direction = input_queue.front();
			input_queue.pop();
			int input_axis;
			bool ignore_input = false;

			// check which axis the input is located on (up is on VERTICAL, etc.)
			if (input_direction == UP || input_direction == DOWN){
				input_axis = VERTICAL_AXIS;
			}else{
				input_axis = HORIZONTAL_AXIS;
			}

			// cycle through all stops received to see if there is one
			// for current command
			for (unsigned int i = 0; i < stop_queue.size(); i ++){
				int stop_type = stop_queue.front();
				stop_queue.pop();
				if (stop_type == input_axis){
					ignore_input = true;
					break;
				}else{
					stop_queue.push(stop_type);
				}
			}

			// move on to next input if stop for input existed already
			if (ignore_input){
				continue;
			}

			// send message
			if (!ignore_input){
				switch(input_direction){
					case UP:
						network->sendMessage("moveFront");
						printf("JS up\n");
						break;
					case DOWN:
						network->sendMessage("moveBack");
						printf("JS down\n");
						break;
					case LEFT:
						network->sendMessage("moveLeft");
						printf("JS left\n");
						break;
					case RIGHT:
						network->sendMessage("moveRight");
						printf("JS right\n");
						break;
				}
			}

			// wait for stop command
			bool rx_stop = false;
			while (!rx_stop){
				cv_stop.wait(stop_lock);
				for (unsigned int i = 0; i < stop_queue.size(); i ++){
					int stop_type = stop_queue.front();
					stop_queue.pop();
					if (stop_type == input_axis){
						rx_stop = true;
						network->sendMessage("stopMotors");
					}else{
						stop_queue.push(stop_type);
					}
				}
			}
		}
	}
}


static void event_handler(struct js_event event, NetworkControls* network){
	if (event.type == JS_EVENT_AXIS){	// Joystick event
		if (event.number == VERTICAL_AXIS){
			if (event.value == -ON){
				input_queue.push(UP);
			}
			else if (event.value == ON){
				input_queue.push(DOWN);
			}
			else if (event.value == STOP){
				stop_queue.push(VERTICAL_AXIS);
				cv_stop.notify_all();
			}
		}
		else if (event.number == HORIZONTAL_AXIS){
			if (event.value == -ON){
				input_queue.push(LEFT);
			}
			else if (event.value == ON){
				input_queue.push(RIGHT);
			}
			else if (event.value == STOP){
				stop_queue.push(HORIZONTAL_AXIS);
				cv_stop.notify_all();
			}
		}
	}else if (event.type == JS_EVENT_BUTTON && event.value == 1){	// Button event
		switch(event.number){
			case START_BUTTON:
				// network->sendMessage("moveFront");
				// start smart routine?
				break;
			case SELECT_BUTTON:
				network->sendMessage("toggleMode");
				break;
			case RED_BUTTON:
				network->sendMessage("stop");
				break;
			case YELLOW_BUTTON:
				network->sendMessage("stopMotors");
				break;
		}
	}
}


