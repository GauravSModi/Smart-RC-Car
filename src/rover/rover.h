#ifndef _ROVER_H_
#define _ROVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>			
#include <unistd.h>			
#include <thread>
#include <mutex>

#include "../motors/motors.h"
#include "../common/utils.h"

#define MANUAL_MODE 0
#define AUTO_MODE 1
#define UNITS_PER_SECOND 1.0 // how much the rover moves

class Rover{
private:
	Motors* myMotors;
	bool shutdown;
	std::thread* roverThread;

	// 0 = controller/webpage (default)
	// 1 = object-avoidance routine
	int driveMode;	

	// MAPPING
	Vec2<double> position;
	double steadyYaw = 30.0;
	std::mutex mapLatch;
	bool wasTurning = false;
	std::chrono::_V2::system_clock::time_point prev_time;
	std::chrono::_V2::system_clock::time_point current_time;
	
public:
	Rover(); //constructor
	~Rover();

	// main loop for rover
	void main_rover();

	// privimite movement functions
	// these calls will return false if there is other high piority subroutin
	// running, expect "force_stop_rover()"
	bool move_forward();
	bool move_backward();
	bool move_left();
	bool move_right();
	bool stop_rover();
	void force_stop_rover();
	void toggle_mode();
	int get_mode();

	// Exposed subroutines
	bool objectSensedSubroutine();

	// MAPPING
	Vec2<double> getPosition();

private:
	// Internal subroutines, locks other controls for duration
	// turns <degrees> and returns the actual degree turned according to gyro
	float rover_turn(double degrees, bool turnleft, bool slowMode);
	bool rover_turn_percise(double degrees, bool turnleft, double withinThreshold);
	bool rover_move(double seconds);
	
	void updatePosition();
	void updateSteadyYaw();
};

void init_rover();

void clean_rover();

Rover* get_rover();

#endif