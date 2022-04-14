/**
 * @file gyro.cpp
 * @brief Start calculating the yaw once object encountered
 * Once at a certain distance from the the object, start counting the yaw
 */

#include "gyro.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
//#include <string.h>
#include <thread>
//#include <ctime>



#include <iomanip>


// ************ ERROR CACLULATION PARAMS ***********
#define RAPID_STABILIZE_EXPONENTIAL_FACTOR 0.9 // (0.0 - 1.0) how much previous values are trusted
#define INITIAL_ERROR_X 100
#define INITIAL_ERROR_Y 100
#define INITIAL_ERROR_Z 100
#define AVG_ERROR_CALC_LIMIT 1000
#define STABILIZE_CONDITION 0.1

// Declare Variables
//static double elapsed_t = 0;
// ==== MODULE ====
static std::thread* gyro_readingThread;
static int gyroFileDiscriptor;
static bool shutdown = false;
static void avg_error(int file);
static void gyro_routine();

// ==== RAW VALUES =====
static Vec3<int16_t> gyroRaw(0,0,0);
float get_xGyro_raw(){ return gyroRaw.x; }
float get_yGyro_raw(){ return gyroRaw.y; }
float get_zGyro_raw(){ return gyroRaw.z; }

static const char data[6] = {0x43,0x44,0x45,0x46,0x47,0x48};
static char results[6];
static void readGyroData(int file);

// ====== ERROR ========
static Vec3<float> error(0.0,0.0,0.0);
//static float error_x = 0;
//static float error_y = 0;
//static float error_z = 0;

// ======= YAW =========
static float yaw = 0;
float getYaw()  { return yaw; }
void resetYaw() { yaw = 0; }
// Helper function for calculateAngle
static std::chrono::_V2::system_clock::time_point prev_time;
static std::chrono::_V2::system_clock::time_point current_time;
static double elapsed_time();




// IMPLEMENTATIONS
bool gyro_isActive(){
	return !shutdown;
}

static void avg_error(int file){
	// restore IO manip config afterwards referenced
	// https://stackoverflow.com/questions/2273330/restore-the-state-of-stdcout-after-manipulating-it
	std::ios_base::fmtflags f( cout.flags() );
	std::cout << std::fixed << std::setfill(' ') << std::setprecision(4) << "Cacluating gryo Errors\n";
	const int DOUBLE_WIDTH = 8;

	Vec3<double> avg(INITIAL_ERROR_X,INITIAL_ERROR_Y,INITIAL_ERROR_Z);	
	Vec3<double> delta(0.0,0.0,0.0);

	bool stablized = false;
	int attempts = 0;
	const double expAveragingFactor = RAPID_STABILIZE_EXPONENTIAL_FACTOR;
	while(!stablized && attempts < AVG_ERROR_CALC_LIMIT){
		attempts++;
		// read new Gyro Values
		readGyroData(file);
		// update average
    for(int axis = 0; axis < 3; axis++){
			// update average based on expoential averaging
      double newAverage = avg.asArray[axis] * expAveragingFactor + gyroRaw.asArray[axis] * (1.0 - expAveragingFactor);
      delta.asArray[axis] = gyroRaw.asArray[axis] - newAverage;
      // remove extreme values
			avg.asArray[axis] = newAverage;
    }

		bool xOkay = abs(delta.x) < STABILIZE_CONDITION;
		bool yOkay = abs(delta.y) < STABILIZE_CONDITION;
		bool zOkay = abs(delta.z) < STABILIZE_CONDITION;

		if(xOkay && yOkay && zOkay){
			std::cout << "[gyro] stablized on error["
			<< std::setw(DOUBLE_WIDTH) << avg.x <<","
			<< std::setw(DOUBLE_WIDTH) << avg.y << ","
			<< std::setw(DOUBLE_WIDTH) << avg.z << "]\n";
			stablized = true;
		} else {
			if(attempts % 10 == 0){
				std::cout << "[gyro] stablizing... (current: ["
				<< std::setw(DOUBLE_WIDTH) << avg.x <<","
				<< std::setw(DOUBLE_WIDTH) << avg.y << ","
				<< std::setw(DOUBLE_WIDTH) << avg.z << "] delta["
				<< std::setw(DOUBLE_WIDTH) << delta.x <<","
				<< std::setw(DOUBLE_WIDTH) << delta.y << ","
				<< std::setw(DOUBLE_WIDTH) << delta.z << "] attepmt: "<< attempts <<")\n";
			}
		}
	}

	error.x = (int16_t)avg.x;
	error.y = (int16_t)avg.y;
	error.z = (int16_t)avg.z;

	std::cout << "Finished cacluating gryo Errors\n";
	cout.flags( f );
}

// Routine for gyro module's Thread
static void gyro_routine(){
	while(!shutdown){
		readGyroData(gyroFileDiscriptor);
		calculateAngle();
		sleep(0.01);
	}
}

void gyro_init(){
	// Get I2C device, MMA8452Q I2C address is 0x68
	gyroFileDiscriptor = initI2cBus("/dev/i2c-2",GYRO_DEVICE_ADDR);
	
	writeI2cReg(gyroFileDiscriptor, PWR_MGMT_1, 0);

	// calculate error
	avg_error(gyroFileDiscriptor);
	resetYaw();
	shutdown = false;
	gyro_readingThread = new std::thread(gyro_routine);
}

void gyro_cleanup(){
	shutdown = true;
	gyro_readingThread->join();
	close(gyroFileDiscriptor);
	printf("Exiting Gyro\n");
}

static void readGyroData(int file){

		for(int i = 0; i <6; i++){
			results[i] = readI2cReg(file, data[i]);
		}  

		gyroRaw.x = ((results[0] << 8) | results[1]) / 131; //angular velocity
		if(gyroRaw.x > 250){
			gyroRaw.x -= 500;
		}

		 
		gyroRaw.y = ((results[2] << 8) + results[3]) / 131;
		if(gyroRaw.y > 250){
			gyroRaw.y -= 500;
		}


		gyroRaw.z = ((results[4] << 8) + results[5]) / 131;
		if(gyroRaw.z > 250){
			gyroRaw.z -= 500;
		}
		
}


bool is90(float goalDegree){

	
	return true;
}



static double elapsed_time(){

	double ret_val;
	prev_time = current_time;
	auto temp = std::chrono::system_clock::now();
	current_time =temp;

	std::chrono::duration<double> elapsed_seconds = current_time - prev_time;
	ret_val = (double)elapsed_seconds.count();

	return ret_val;
	
}

void calculateAngle() {  

  double elapsed_t = elapsed_time();	


  float delta =(gyroRaw.z-error.z)*elapsed_t;
  
  float actual =  (delta > 0.0016 || delta < -0.0016)? delta :0.0;

 
  yaw= yaw + actual;
  
}


