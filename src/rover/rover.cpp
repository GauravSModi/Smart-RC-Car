#include "rover.h"
#include "gyroscope/gyro.h"
#include <mutex>

#define DIRECTION_POLLING_INTERVAL_MS 5

Rover* myRover;
std::mutex controlsLatch;

void init_rover(void){ myRover = new Rover; }
void clean_rover(){ delete myRover; }
Rover* get_rover(){	return myRover; }


Rover::Rover(){
	shutdown = false;
	myMotors = new Motors();
	roverThread = new std::thread(&Rover::main_rover, this);
	//intialise other dependent modules
}

void Rover::main_rover(){
	if (!myMotors) return;

	//this->move_left();
	//calculateAngle();
	resetYaw();

	rover_turn(90,true);

	//rover_turn_percise(90,true,2);
	
	std::cout << "Routine finished!\n";

	return;
}

bool Rover::move_forward(){
	bool success = controlsLatch.try_lock();
	if(success){
		myMotors->moveForward();
		controlsLatch.unlock();
	}
	return success;
}

bool Rover::move_backward(){
	bool success = controlsLatch.try_lock();
	if(success){
		myMotors->moveForward();
		controlsLatch.unlock();
	}
	return success;
}

bool Rover::move_left(){
	bool success = controlsLatch.try_lock();
	if(success){
		myMotors->moveLeft();
		controlsLatch.unlock();
	}
	return success;
}

bool Rover::move_right(){
	bool success = controlsLatch.try_lock();
	if(success){
		myMotors->moveRight();
		controlsLatch.unlock();
	}
	return success;
}
bool Rover::stop_rover(){
	bool success = controlsLatch.try_lock();
	if(success){
		myMotors->stopMoving();
		controlsLatch.unlock();
	}
	return success;
}

void Rover::force_stop_rover(){
	myMotors->stopMoving();
}

Rover::~Rover(){
	shutdown = true;
	roverThread->join();
	delete myMotors;
}



float Rover::rover_turn(double degrees, bool turnleft){
	// lock controls
	controlsLatch.lock();

	// Start turning
	if(turnleft){
		this->myMotors->moveLeft();
	} else {
		this->myMotors->moveRight();
	}

	// turn until degree
	bool turnConditionMet = false;
	float offset = getYaw(); // snapshot yaw when initiating routine

	float degreeCondition;
	while (!turnConditionMet){

		//cap the condition
    //assume for now that once its more than 90, set yaw back to 0.
    //originally set yaw back to 0, once the rover is not moving
		degreeCondition = getYaw() - offset;

		if(degreeCondition > degrees){	
			printf("Yaw is over 90 right now \n"); 
			//offset = getYaw();
			turnConditionMet = true;
			break;
		}
		//else if(getYaw() < -90){
		else if(degreeCondition < -degrees){  
			printf("Yaw is under -90 right now \n");
			//offset = getYaw();
			turnConditionMet = true;
			break;
		}
		
		//value = is90();
		//printf("yaw = %f, abs_yaw = %f\n", getYaw(), getAbsYaw());

		msleep(DIRECTION_POLLING_INTERVAL_MS);
	}
	//msleep(300);
	this->myMotors->stopMoving();

	controlsLatch.unlock();

	return degreeCondition;
}

bool Rover::rover_turn_percise(double degrees, bool isTurnLeft, double withinThreshold){
	bool successTurn = false;
	bool turningLeft = isTurnLeft;
	double nextTurnDegrees = degrees;
	while(!successTurn){
		float actualTurned = this->rover_turn(nextTurnDegrees, turningLeft);
 		float errorAngle = abs(nextTurnDegrees - actualTurned);

		if(errorAngle < withinThreshold){
			successTurn = true;
		} else {
			// switch direction and keep turning
			turningLeft = !turningLeft;
			nextTurnDegrees = errorAngle;
		}
	}
	return true;
}