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
	int direction;		// 0 = backward, 1 = forward
	int power;			// 0 = off, 	 1 = on
	int forwardPin;		// GPIO pin for forward motion
	int backwardPin;	// GPIO pin for backward motion
	std::thread motorsThread;

public:
	Motors();

	// All four motors move forwards
	void moveForward();

	// All four motors move backwards
	void moveBackward();

	// Move left two motors backwards, 
	// 		right two motors forwards
	void moveLeft();

	// Move left two motors forwards, 
	// 		right two motors backwards
	void moveRight();

	// int getDirection(){
	// 	return direction;
	// }

	// void turnOn(){

	// }

	// void turnOff(){

	// }

	// int powerStatus(){

	// }

	~Motors();

};

void Motors_init(void);

// Motors* Motors_init(void);

#endif