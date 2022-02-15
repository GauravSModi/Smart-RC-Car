#include <stdio.h>
#include <stdlib.h>
#include <string.h>			
#include <unistd.h>			
#include <pthread.h>
#include "rover.h"
#include "joystick/joystick.h"

pthread_t start;

rover::rover(int j_input, int w_input){
	setInput(j_input, w_input);
}

void rover::setInput(int j_input, int w_input){
	joy_input = j_input;
	web_input = w_input;
}

int rover:: getJoyInput(){
	return joy_input;
}

int rover:: getWebInput(){
	return joy_input;
}

void rover::start_rover(){
	pthread_create(&start, NULL, (void*)&takingInput, NULL);
	//intialise other modules
}	
	
void rover::stop_rover(){
	
	//join other modules
	pthread_join(&start, NULL);
}	
	
void rover::takingInput(){
 //while(1) -> start reading input
}	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
