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

	bool move_forward();
	bool move_backward();
	bool move_left();
	bool move_right();
	bool stop_rover();
	void force_stop_rover();

	~Rover();

private:
	// subroutines, locks other controls for duration
	// turns <degrees> and returns the actual degree turned according to gyro
	float rover_turn(double degrees, bool turnleft, bool slowMode);
	bool rover_turn_percise(double degrees, bool turnleft, double withinThreshold);
	bool rover_move(double seconds);
};

void init_rover();

void clean_rover();

Rover* get_rover();

#endif