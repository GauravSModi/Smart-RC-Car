// motors.cpp
// 
// Allows for control of the four motors 
// in a coordinated fashion

#include "motors.h"

#define NUM_PINS 4
enum direction{LEFT_BACKWARD = 66, LEFT_FORWARD = 67, RIGHT_FORWARD = 68, RIGHT_BACKWARD = 69};
static const int gpio_pins[] = {LEFT_BACKWARD, LEFT_FORWARD, RIGHT_FORWARD, RIGHT_BACKWARD};
	

Motors::Motors(){
	direction = -1;
	power = 0;

	exportAll();

	// give system time to export
	msleep(500);

	// set options
	std::string direction= "out";
	for (int i = 0; i < NUM_PINS; i ++){
		int maxLength = 50;
		char buffer[maxLength];
		std::snprintf(buffer, maxLength, "/sys/class/gpio/gpio%d/direction", gpio_pins[i]);
		writeGPIOValue(buffer, direction);
	}
	std::string value = "0";
	for (int i = 0; i < NUM_PINS; i ++){
		int maxLength = 50;
		char buffer[maxLength];
		std::snprintf(buffer, maxLength, "/sys/class/gpio/gpio%d/value", gpio_pins[i]);
		writeGPIOValue(buffer, value);
	}
}

void Motors::moveBackward(){
	direction = 0;
	power = 1;

	for (int i = 0; i < NUM_PINS; i ++){
		int maxLength = 50;
		char buffer[maxLength];
		if (gpio_pins[i] == LEFT_FORWARD || gpio_pins[i] == RIGHT_FORWARD){
			std::snprintf(buffer, maxLength, "/sys/class/gpio/gpio%d/value", gpio_pins[i]);
			writeGPIOValue(buffer, "1");
		}
		else{
			std::snprintf(buffer, maxLength, "/sys/class/gpio/gpio%d/value", gpio_pins[i]);
			writeGPIOValue(buffer, "0");
		}
	}
	printf("Moving backwards\n");
}

void Motors::moveForward(){
	direction = 1;
	power = 1;

	for (int i = 0; i < NUM_PINS; i ++){
		int maxLength = 50;
		char buffer[maxLength];
		if (gpio_pins[i] == LEFT_BACKWARD || gpio_pins[i] == RIGHT_BACKWARD){
			std::snprintf(buffer, maxLength, "/sys/class/gpio/gpio%d/value", gpio_pins[i]);
			writeGPIOValue(buffer, "1");
		}
		else{
			std::snprintf(buffer, maxLength, "/sys/class/gpio/gpio%d/value", gpio_pins[i]);
			writeGPIOValue(buffer, "0");
		}
	}
	printf("Moving forwards\n");
}

void Motors::moveLeft(){
	direction = 2;
	power = 1;

	for (int i = 0; i < NUM_PINS; i ++){
		int maxLength = 50;
		char buffer[maxLength];
		if (gpio_pins[i] == LEFT_BACKWARD || gpio_pins[i] == RIGHT_FORWARD){
			std::snprintf(buffer, maxLength, "/sys/class/gpio/gpio%d/value", gpio_pins[i]);
			writeGPIOValue(buffer, "1");
		}
		else{
			std::snprintf(buffer, maxLength, "/sys/class/gpio/gpio%d/value", gpio_pins[i]);
			writeGPIOValue(buffer, "0");
		}
	}
	printf("Moving left\n");
}
void Motors::slowLeft(){
	direction = 2;
	power = 1;

	for (int i = 0; i < NUM_PINS; i ++){
		int maxLength = 50;
		char buffer[maxLength];
		if (gpio_pins[i] == LEFT_BACKWARD){
			std::snprintf(buffer, maxLength, "/sys/class/gpio/gpio%d/value", gpio_pins[i]);
			writeGPIOValue(buffer, "1");
		}
		else{
			std::snprintf(buffer, maxLength, "/sys/class/gpio/gpio%d/value", gpio_pins[i]);
			writeGPIOValue(buffer, "0");
		}
	}
	printf("Moving left\n");
}

void Motors::moveRight(){
	direction = 3;
	power = 1;

	for (int i = 0; i < NUM_PINS; i ++){
		int maxLength = 50;
		char buffer[maxLength];
		if (gpio_pins[i] == LEFT_FORWARD || gpio_pins[i] == RIGHT_BACKWARD){
			std::snprintf(buffer, maxLength, "/sys/class/gpio/gpio%d/value", gpio_pins[i]);
			writeGPIOValue(buffer, "1");
		}
		else{
			std::snprintf(buffer, maxLength, "/sys/class/gpio/gpio%d/value", gpio_pins[i]);
			writeGPIOValue(buffer, "0");
		}
	}
	printf("Moving right\n");
}
void Motors::slowRight(){
	direction = 3;
	power = 1;

	for (int i = 0; i < NUM_PINS; i ++){
		int maxLength = 50;
		char buffer[maxLength];
		if (gpio_pins[i] == RIGHT_BACKWARD){
			std::snprintf(buffer, maxLength, "/sys/class/gpio/gpio%d/value", gpio_pins[i]);
			writeGPIOValue(buffer, "1");
		}
		else{
			std::snprintf(buffer, maxLength, "/sys/class/gpio/gpio%d/value", gpio_pins[i]);
			writeGPIOValue(buffer, "0");
		}
	}
	printf("Moving right\n");
}

void Motors::stopMoving(){
	direction = -1;
	power = 0;

	for (int i = 0; i < NUM_PINS; i ++){
		int maxLength = 50;
		char buffer[maxLength];
		std::snprintf(buffer, maxLength, "/sys/class/gpio/gpio%d/value", gpio_pins[i]);
		writeGPIOValue(buffer, "0");
	}
	printf("Stopping movement...\n");
}


int Motors::getDirection(){
	return direction;
}

int Motors::getPowerStatus(){
	return power;
}

void Motors::exportAll(){
	std::string filePath = "/sys/class/gpio/export";
	for (int i = 0; i < NUM_PINS; i ++){
		std::string pin = std::to_string(gpio_pins[i]);
		writeGPIOValue(filePath, pin);
	}
}

void Motors::unexportAll(){
	std::string filePath = "/sys/class/gpio/unexport";
	for (int i = 0; i < NUM_PINS; i ++){
		std::string pin = std::to_string(gpio_pins[i]);
		writeGPIOValue(filePath, pin);
	}
}

Motors::~Motors(){
	stopMoving();
	unexportAll();
}
