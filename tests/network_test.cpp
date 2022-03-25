#include <network/network.h>
//#include <unistd.h>
#include <condition_variable>
#include <mutex>


static std::mutex _shutdownLock;
static std::condition_variable shutdownCondition;
static bool isRunning = false;

void signalShutdown();

int main(){

  std::unique_lock<std::mutex> shutdownLock(_shutdownLock);

  // initialize
  init_udp(signalShutdown);

  // wait on shutdown signal
  isRunning = true;
  printf("[Main] Waiting on Condition.\n");
  shutdownCondition.wait(shutdownLock);

  // clean up and unlock shutdown mutex
  clean_udp();

  isRunning = false;
  shutdownLock.unlock();

  return 0;
}

void signalShutdown(){
  // try to acquire shutdownLatch if no other threads has called
  // 1) if main is already shutting down, lock will fail to be acquired
  // 2) if main has already shut down, isRunning will prevent shut down again
  // 3) if main has released shutdownLatch, error EINVAL will prevent acquiring lock
  if(_shutdownLock.try_lock()){
    if(isRunning){
      // signal shutdown
      printf("[main] Shutting down...\n");
      shutdownCondition.notify_all();
      _shutdownLock.unlock();

    } else {
      printf("Trying signal shutdown but system is not running.\n");
    }
  } else {
    printf("Failed to aquire shutdown lock.\n");
  }
}



