#include <Joystick/Joysticks.h>

Joystick::Joystick(){

}

Joystick::~Joystick(){

}

void Joystick::initialize(){

}

void Joystick::shutdown(){
  //TODO: other cleanup?
  delete Joystick::instance;
  Joystick::instance = nullptr;
}

void Joystick::run(){
  // event loop:

  // wait for input 
  // -> process input
  // pass to input.c

}