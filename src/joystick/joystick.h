#pragma once
#include <thread>
#include <mutex>

#define JOYSTICK_UP_PATH "/sys/class/gpio/gpio26/"
#define JOYSTICK_DOWN_PATH "/sys/class/gpio/gpio46/"
#define JOYSTICK_LEFT_PATH "/sys/class/gpio/gpio65/"
#define JOYSTICK_RIGHT_PATH "/sys/class/gpio/gpio47/"

void joystickDummy();

enum JoystickDirection{
  UP,
  DOWN,
  LEFT,
  RIGHT,
  INVALID
};

class Joystick {
  // singleton (possibly convert this to modules/non-class style)
  private:
    // static variables
    static Joystick* instance;

    // private constructor
    Joystick(); 

    // instance variables
    std::thread* joystickThread;
    std::thread* decayerThread;
    Vec2<int> strengthVector;
    bool isRunning;
    std::mutex strengthMutex;

    // file descriptors for Joysticks value
    int upValueFD;
    int downValueFD;
    int leftValueFD;
    int rightValueFD;
  
  public:
    // instantiate self and start "run/loop" in another thread
    static void initialize(); /*TODO: arguments?*/
    // clean up modules for graceful shutdown
    static void shutdown();
    // delete copy constructor
    Joystick(Joystick const &) = delete;
    ~Joystick();

    void lockStrength(){this->strengthMutex.lock();}
    void unlockStrength(){this->strengthMutex.unlock();}
    Vec2<int> getStrength(){return this->strengthVector;}
    double getStrengthX(){return this->strengthVector.x;}
    double getStrengthZ(){return this->strengthVector.y;}
    void setStrength(double _x, double _z){this->strengthVector.x = _x, this->strengthVector.y = _z;};

  private:
    // thread function for event loop in main module thread
    void run();
    
    // thread function for simulating strength decaying over time until rest
    void strengthDecayer();

    // epoll for one Joy stick imput
    // returns enum of joystick direction
    JoystickDirection getEpollInput();
};
