#ifndef _ROVER_H_
#define _ROVER_H_

class rover{

private:
	int joy_input;
	int web_input;
public:
	rover(int j_input, int w_input); //constructor
	void setInput(int j_input,int w_input);
	int getJoyInput();
	int getWebInput();
	//void intialization(); //pthread create

};

#endif