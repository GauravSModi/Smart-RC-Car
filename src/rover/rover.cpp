#include "rover.h"
#include "gyroscope/gyro.h"
#include <mutex>

#define DIRECTION_POLLING_INTERVAL_MS 1
#define DIRECTION_CORRECTION_RATE 0.78
#define JIGGLE_DURATION_MS 5

Rover* myRover;
std::mutex controlsLatch;

void init_rover(void){ myRover = new Rover; }
void clean_rover(){ delete myRover; }
Rover* get_rover(){	return myRover; }


Rover::Rover(){
	shutdown = false;
	driveMode = MANUAL_MODE;
	myMotors = new Motors();
	roverThread = new std::thread(&Rover::main_rover, this);
	//intialise other dependent modules
}

void Rover::main_rover(){
	if (!myMotors) return;

	//this->move_left();
	//calculateAngle();
	resetYaw();

	//rover_turn(90,false);

	//rover_turn_percise(90,true,0.5);
	
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



float Rover::rover_turn(double degrees, bool turnleft, bool slow){
	// lock controls
	controlsLatch.lock();
	
	float offset = getYaw(); // snapshot yaw when initiating routine

	// Start turning
	if(turnleft){
		if(slow){
			this->myMotors->slowLeft();
		} else {
			this->myMotors->moveLeft();
		}
	} else {
		if(slow){
			this->myMotors->slowRight();
		} else {
			this->myMotors->moveRight();
		}
	}

	// turn until degree
	bool turnConditionMet = false;

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
	this->myMotors->stopMoving();

	controlsLatch.unlock();

	return getYaw() - offset;
}

bool Rover::rover_turn_percise(double degrees, bool isTurnLeft, double withinThreshold){
	
	const double initialYaw = getYaw();
	double actualTurned = this->rover_turn(degrees, isTurnLeft, false);
	std::cout << "actualTurned: " << actualTurned << "\n";

	// correct for overshooting
	
	double errorAngle;
	bool successTurn = false;
	while(!successTurn){

		std::cout << "Getting finalize position in 4 seconds.......\n";
		msleep(100);

		errorAngle = initialYaw - getYaw() + (isTurnLeft?-degrees:degrees);
		std::cout << "errorAngle: " << errorAngle << "\n";
		
		if(errorAngle > withinThreshold){
			this->myMotors->moveRight();
			msleep(JIGGLE_DURATION_MS);
			this->myMotors->stopMoving();
		} else if (errorAngle < -withinThreshold) {
			this->myMotors->moveLeft();
			msleep(JIGGLE_DURATION_MS);
			this->myMotors->stopMoving();
		} else {
			successTurn = true;
		}		
	}
	return true;
}

bool Rover::objectSensedSubroutine(){
	this->myMotors->stopMoving();
	if(driveMode == AUTO_MODE){
  		this->rover_turn_percise(90.0,true,0.5);
	}
}

void Rover::toggle_mode(){
	driveMode = 1 - driveMode;
}

int Rover::get_mode(){
	return driveMode;
}