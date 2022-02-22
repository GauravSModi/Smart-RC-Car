#pragma once 

void roverDummy();

class rover{

private:
	int joy_input;
	int web_input;
	double current_speed;
public:
	rover(int j_input, int w_input); //constructor
	
	void setInput(int j_input,int w_input); //instead of set input, set direction
	int getJoyInput();
	int getWebInput();
	
	void start_rover(); //pthread create
	void stop_rover();
	
	void takingInput(); //main routine for the rover.
	
	void init_gpio_pins();
	
	void move();
	void turn();
};

