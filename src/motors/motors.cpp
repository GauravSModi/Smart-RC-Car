// motors.cpp
// 
// Allows for control of the four motors 
// in a coordinated fashion

#include "motors.h"

#define NUM_PINS 4
enum direction{LEFT_BACKWARD = 66, LEFT_FORWARD = 67, RIGHT_FORWARD = 68, RIGHT_BACKWARD = 69};
static const int gpio_pins[] = {LEFT_BACKWARD, LEFT_FORWARD, RIGHT_FORWARD, RIGHT_BACKWARD};
	


// Motors::Motors(int forPin, int backPin) : motorsThread{}{
Motors::Motors(){
	exit = false;

	for (int i = 0; i < NUM_PINS; i ++){
		int maxLength = 50;
		char buffer[maxLength];
		std::snprintf(buffer, maxLength, "echo %d > /sys/class/gpio/export", gpio_pins[i]);
		system(buffer);
	}

	// give system time to export
	msleep(500);

	// set options
	char dir[] = "out";
	for (int i = 0; i < NUM_PINS; i ++){
		int maxLength = 50;
		char buffer[maxLength];
		std::snprintf(buffer, maxLength, "echo %s > /sys/class/gpio/gpio%d/direction", dir, gpio_pins[i]);
		system(buffer);
	}
	char value[] = "0";
	for (int i = 0; i < NUM_PINS; i ++){
		int maxLength = 50;
		char buffer[maxLength];
		std::snprintf(buffer, maxLength, "echo %s > /sys/class/gpio/gpio%d/value", value, gpio_pins[i]);
		system(buffer);
	}
}

void Motors::moveBackward(){
	for (int i = 0; i < NUM_PINS; i ++){
		int maxLength = 50;
		char buffer[maxLength];
		if (gpio_pins[i] == LEFT_FORWARD || gpio_pins[i] == RIGHT_FORWARD){
			std::snprintf(buffer, maxLength, "echo 1 > /sys/class/gpio/gpio%d/value", gpio_pins[i]);
		}
		else{
			std::snprintf(buffer, maxLength, "echo 0 > /sys/class/gpio/gpio%d/value", gpio_pins[i]);
		}
		system(buffer);
	}
	printf("Moving forward\n");
}

void Motors::moveForward(){
	for (int i = 0; i < NUM_PINS; i ++){
		int maxLength = 50;
		char buffer[maxLength];
		if (gpio_pins[i] == LEFT_BACKWARD || gpio_pins[i] == RIGHT_BACKWARD){
			std::snprintf(buffer, maxLength, "echo 1 > /sys/class/gpio/gpio%d/value", gpio_pins[i]);
		}
		else{
			std::snprintf(buffer, maxLength, "echo 0 > /sys/class/gpio/gpio%d/value", gpio_pins[i]);
		}
		system(buffer);
	}
	printf("Moving backward\n");
}

void Motors::moveLeft(){
	for (int i = 0; i < NUM_PINS; i ++){
		int maxLength = 50;
		char buffer[maxLength];
		if (gpio_pins[i] == LEFT_BACKWARD || gpio_pins[i] == RIGHT_FORWARD){
			std::snprintf(buffer, maxLength, "echo 1 > /sys/class/gpio/gpio%d/value", gpio_pins[i]);
		}
		else{
			std::snprintf(buffer, maxLength, "echo 0 > /sys/class/gpio/gpio%d/value", gpio_pins[i]);
		}
		system(buffer);
	}
	printf("Moving left\n");
}

void Motors::moveRight(){
	for (int i = 0; i < NUM_PINS; i ++){
		int maxLength = 50;
		char buffer[maxLength];
		if (gpio_pins[i] == LEFT_FORWARD || gpio_pins[i] == RIGHT_BACKWARD){
			std::snprintf(buffer, maxLength, "echo 1 > /sys/class/gpio/gpio%d/value", gpio_pins[i]);
		}
		else{
			std::snprintf(buffer, maxLength, "echo 0 > /sys/class/gpio/gpio%d/value", gpio_pins[i]);
		}
		system(buffer);
	}
	printf("Moving right\n");
}


void Motors::stopMoving(){
	for (int i = 0; i < NUM_PINS; i ++){
		int maxLength = 50;
		char buffer[maxLength];
		std::snprintf(buffer, maxLength, "echo 0 > /sys/class/gpio/gpio%d/value", gpio_pins[i]);
		system(buffer);
	}
	printf("Stopping movement...\n");
}


// int Motors::getDirection(){
// 	return direction;
// }

// void Motors::turnOn(){

// }

// void Motors::turnOff(){

// }

// int Motors::powerStatus(){

// }

Motors::~Motors(){
	exit = true;
	for (int i = 0; i < NUM_PINS; i ++){
		int maxLength = 50;
		char buffer[maxLength];
		std::snprintf(buffer, maxLength, "echo %d > /sys/class/gpio/unexport", gpio_pins[i]);
		system(buffer);
	}
}


// void Motors_init(void){
// 	Motors newMotors;
// 	while (true){
// 		// stay here
// 		newMotors.moveForward();
// 		msleep(3000);
// 		newMotors.moveBackward();
// 		msleep(3000);
// 		newMotors.moveRight();
// 		msleep(3000);
// 		newMotors.moveLeft();
// 		msleep(3000);
// 	}
// }