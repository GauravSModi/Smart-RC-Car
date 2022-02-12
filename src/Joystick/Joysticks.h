#pragma once

#define JOYSTICK_UP_PATH "/sys/class/gpio/gpio26/"
#define JOYSTICK_DOWN_PATH "/sys/class/gpio/gpio46/"
#define JOYSTICK_LEFT_PATH "/sys/class/gpio/gpio65/"
#define JOYSTICK_RIGHT_PATH "/sys/class/gpio/gpio47/"

class Joystick {
  // singleton (possibly convert this to modules/non-class style)
  private:
    static Joystick* instance;
    Joystick();
  public:
    // instantiate self and start "run/loop" in another thread
    static void initialize(); /*TODO: arguments?*/
    // clean up modules for graceful shutdown
    static void shutdown();
    // delete copy constructor
    Joystick(Joystick const &) = delete;
    ~Joystick();

  private:
    void run();
};
