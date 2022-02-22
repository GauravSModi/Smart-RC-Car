#include <joystick/joystick.h>
#include <unistd.h>

// test if joystick module is include
void testInclude(){
  joystickDummy();
}

// test joystick init
void testInit(){
  printf("TEST: Initializing joystick.\n");
 
  Joystick::initialize();
  sleep(10);

  printf("TEST: Shutting down joystick.\n");
  Joystick::shutdown(); /**/
}

int main(int argc, char const *argv[]){
  testInclude();

  return 0;
}
