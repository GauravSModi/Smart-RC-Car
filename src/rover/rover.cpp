#include "rover.h"

rover* myRover;

rover::rover(){
	shutdown = false;
	myMotors = new Motors();
	roverThread = new std::thread(&rover::main_rover, this);
	//intialise other dependent modules
}

void rover::main_rover(){
	if (!myMotors) return;

	while (!shutdown){
		// do something
		// myMotors->moveForward();
		// msleep(3000);
		// myMotors->moveBackward();
		// msleep(3000);
		// myMotors->moveRight();
		// msleep(3000);
		// myMotors->moveLeft();
		// msleep(3000);
	}

	return;
}

void rover::move_forward(){
	myMotors->moveForward();
}

void rover::move_backward(){
	myMotors->moveBackward();
}

void rover::move_left(){
	myMotors->moveLeft();
}

void rover::move_right(){
	myMotors->moveRight();
}
	
rover::~rover(){
	shutdown = true;
	roverThread->join();
	delete myMotors;
}

void init_rover(void){
	myRover = new rover;
}

void clean_rover(){
	delete myRover;
}

rover* get_rover(){
	return myRover;
}