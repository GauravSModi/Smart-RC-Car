#ifndef _ROVER_H_
#define _ROVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>			
#include <unistd.h>			
#include <thread>

#include "../motors/motors.h"
#include "../common/utils.h"

class Rover{
private:
	Motors* myMotors;
	bool shutdown;
	std::thread* roverThread;

public:
	Rover(); //constructor
	
	void main_rover();

	void move_forward();

	void move_backward();

	void move_left();

	void move_right();

	void stop_rover();

	~Rover();
};

void init_rover();

void clean_rover();

Rover* get_rover();

#endif