#include <thread>

struct Vec3 {
  double x; 
  double y;
  double z;
};

struct Rotation {
  int idk_yet; // FIXME:
};

class Orientation {

private :
  const int sample_interval_ms = 5;
  bool isRunning = false;
  std::thread* readingThread;
  Rotation r;

public :
  Orientation(){
    //TODO:
    //initialize rotation
    //this->r = {0};
  }

  void initialize();
  void readingRun();
  void shutdown();
};