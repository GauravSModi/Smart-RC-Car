#include "rover.h"
#include "gyroscope/gyro.h"
#include <mutex>

Rover* myRover;
std::mutex controlsLatch;

bool value = 0; 

// subroutines
static bool rover_turn(double degrees);
static bool rover_move(double seconds);

Rover::Rover(){
	shutdown = false;
	myMotors = new Motors();
	roverThread = new std::thread(&Rover::main_rover, this);
	//intialise other dependent modules
}

void Rover::main_rover(){
	if (!myMotors) return;

	this->move_left();
	//calculateAngle();
	magic();

	while (!shutdown){

		//turn
		// get current yaw
		// const yaw + 90 -> angleToAimFOr
		// 
		// 

		value = is90();
		//printf("yaw = %f, abs_yaw = %f\n", getYaw(), getAbsYaw());
		if(value == 1){

			printf("STOPPING NOW\n");
			this->myMotors->stopMoving();
			//sleep(5);
		}
		
		
	}

	return;
}

bool Rover::move_forward(){
	controlsLatch.lo
	myMotors->moveForward();
}

bool Rover::move_backward(){
	myMotors->moveBackward();
}

bool Rover::move_left(){
	myMotors->moveLeft();
}

bool Rover::move_right(){
	myMotors->moveRight();
}


Rover::~Rover(){
	shutdown = true;
	roverThread->join();
	delete myMotors;
}

void init_rover(void){
	myRover = new Rover;
}

void clean_rover(){
	delete myRover;
}

Rover* get_rover(){
	return myRover;
}