#include "rover.h"

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
