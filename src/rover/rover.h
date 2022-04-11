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
#include <distance_sensor/sharpIR_ds.h>

#define MANUAL_MODE 0
#define AUTO_MODE 1

class Rover{
private:
	Motors* myMotors;
	bool shutdown;
	std::thread* roverThread;
	SHARPDistanceSensor * sideDistanceSensor;

	// 0 = controller/webpage (default)
	// 1 = object-avoidance routine
	int driveMode;	

	// turning
	bool turnConditionMet = false;
	bool roverTurning = false;
	bool successTurnPercise = false;

	// MAPPING
	Vec2<double> position;
	double steadyYaw = 30.0;
	std::mutex mapLatch;
	bool wasTurning = false;
	std::chrono::_V2::system_clock::time_point prev_time = std::chrono::system_clock::now();
	std::chrono::_V2::system_clock::time_point current_time = std::chrono::system_clock::now();
	
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
	int get_mode();
	void set_mode(int mode);	// use AUTO_MODE or MANUAL_MODE
	void toggle_mode();

	// Exposed subroutines
	bool objectSensedSubroutine();

	// MAPPING
	Vec2<double> getPosition();

private:
	// Internal subroutines, locks other controls for duration
	// turns <degrees> and returns the actual degree turned according to gyro
	bool rover_turn(double degrees, bool turnleft, bool slowMode);
	bool rover_turn_percise(double degrees, bool turnleft, double withinThreshold);
	bool rover_move(double seconds);
	
	void updatePosition();
	void updateSteadyYaw();
	void forceStopTurn(){
		this->turnConditionMet = true;
	}
};

void init_rover();

void clean_rover();

Rover* get_rover();

#endif