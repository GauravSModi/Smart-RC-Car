// motors.h

#ifndef _MOTORS_H_
#define _MOTORS_H_

#include <iostream>
#include <cstdio>
#include <string>
#include <thread>

#include "../common/utils.h"

class Motors {
private:
	bool exit;
	int direction;		// -1 = none, 0 = backward, 1 = forward, 2 = left, 3 = right
	int power;			// 0 = off, 	 1 = on
	int forwardPin;		// GPIO pin for forward motion
	int backwardPin;	// GPIO pin for backward motion
	std::thread motorsThread;

	void exportAll();
	void unexportAll();

public:
	Motors();

	// All four motors move forwards
	void moveForward();

	// All four motors move backwards
	void moveBackward();

	// Move left two motors backwards, 
	// 		right two motors forwards
	void moveLeft();
	void slowLeft();

	// Move left two motors forwards, 
	// 		right two motors backwards
	void moveRight();
	void slowRight();

	// Stop all motors
	void stopMoving();

	// -1 = none, 0 = backward, 1 = forward, 2 = left, 3 = right
	int getDirection();

	// 0 = off, 1 = on
	int getPowerStatus();

	~Motors();

};

// void Motors_init(void);

#endif