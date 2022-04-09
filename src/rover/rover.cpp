#include "rover.h"
#include "gyroscope/gyro.h"

rover* myRover;

bool value = 0; 

rover::rover(){
	shutdown = false;
	myMotors = new Motors();
	roverThread = new std::thread(&rover::main_rover, this);
	//intialise other dependent modules
}

void rover::main_rover(){
	if (!myMotors) return;

	this->move_left();
	//calculateAngle();
	magic();

	while (!shutdown){
		//calculateAngle();
		// do something
		// myMotors->moveForward();
		// msleep(3000);
		// myMotors->moveBackward();
		// msleep(3000);
		// myMotors->moveRight();
		// msleep(3000);
		// myMotors->moveLeft();
		// msleep(3000);

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
		sleep(0.01);
		
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