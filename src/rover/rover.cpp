#include "rover.h"
#include "gyroscope/gyro.h"
#include <mutex>
#include <math.h>

#define DIRECTION_POLLING_INTERVAL_MS 1
#define DIRECTION_CORRECTION_RATE 0.78
#define JIGGLE_DURATION_MS 5

#define POSITION_UPDATE_INTERVAL_MS 5

Rover* myRover;
std::mutex controlsLatch;

void init_rover(void){ myRover = new Rover; }
void clean_rover(){ delete myRover; }
Rover* get_rover(){	return myRover; }

Rover::Rover(){
	this->position.x = 0.0;
	this->position.y = 0.0;
	shutdown = false;
	driveMode = MANUAL_MODE;
	myMotors = new Motors();
	roverThread = new std::thread(&Rover::main_rover, this);
	//intialise other dependent modules
}

void Rover::main_rover(){
	if (!myMotors) return;
	resetYaw();

	while (!shutdown){
		this->updatePosition();
		msleep(POSITION_UPDATE_INTERVAL_MS);
	}

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
	if(driveMode == MANUAL_MODE){
  		this->rover_turn_percise(90.0,true,0.5);
	}
	return true;
}

void Rover::toggle_mode(){
	driveMode = 1 - driveMode;
}

int Rover::get_mode(){
	return driveMode;
}

void Rover::updatePosition(){
	if(this->myMotors->getPowerStatus()==1){
		int direction = this->myMotors->getDirection();

		if(direction == 0 || direction == 1){
			if(this->wasTurning){
				this->wasTurning = false;
				updateSteadyYaw();
			}
			// if motors were on
			if(prev_time != current_time){
				std::chrono::duration<double> elapsed_seconds = current_time - prev_time;
				double deltaTime = (double)elapsed_seconds.count();
				
				double deltaDistance = deltaTime * UNITS_PER_SECOND * (direction==1?1:-1);

				// update values
				mapLatch.lock();
				position.x += deltaDistance * cos(steadyYaw);
				position.y += deltaDistance * sin(steadyYaw);
				mapLatch.unlock();
			}
		} else if(direction == 2 || direction == 3){
			this->wasTurning = true;
		}
	} 
	

	// update time
	prev_time = current_time;
	current_time = std::chrono::system_clock::now();
}

Vec2<double> Rover::getPosition(){
	Vec2<double> result;
	mapLatch.lock();
	result.x = this->position.x;
	result.y = this->position.y;
	mapLatch.unlock();
	return result;
}

void Rover::updateSteadyYaw(){
	mapLatch.lock();
	this->steadyYaw = getYaw();
	mapLatch.unlock();
}