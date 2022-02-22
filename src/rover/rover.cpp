#include <stdio.h>
#include <stdlib.h>
#include <string.h>			
#include <unistd.h>			
//#include <pthread.h> // FIXME:
#include <thread>
#include "rover.h"
#include "joystick/joystick.h"

pthread_t start;

void roverDummy(){
  printf("rover module Include success\n");
}

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

	
//typedef void* (*THREADFUNCPTR)(void *); // FIXME:
std::thread* tempThread; // FIXME:
void rover::start_rover(){
	// FIXME:
	// temp fix for pthread_create with non-static class methods
	// referenced https://thispointer.com/c-how-to-pass-class-member-function-to-pthread_create/
	//
	// pthread_create(&start, NULL, (THREADFUNCPTR) &takingInput, NULL);
	// , this However still gives a warning:
	//       ISO C++ forbids taking the address of an unqualified or parenthesized
	//       non-static member function to form a pointer to member function.  
	//       Say ‘&rover::takingInput’
	// Temporarily placing alternative using std::thread
	tempThread = new std::thread(&rover::takingInput, this);

	//intialise other dependent modules
}	
	
void rover::stop_rover(){
	
	//join other modules
	//pthread_join(start, NULL); //FIXME:
	if(start){} // suppress unused variable warning
	tempThread->join();
}	
	
void rover::takingInput(){
 //while(1) -> start reading input
}	

void init_gpio_pins(){
 //init all the involved gpio pins.
 }
	
void move(){
 //possible directions: 
 // forward, backward, right, left
 //forward_right, forward_left, backward_right, backward_left
 }

void turn(){
 //use angles to determine the turn
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
