#include "accelerometer.h"


int time_in = 0;

static char config[2] = {0};
static char data[7] = {0};

static int initI2cBus(void)
{
	// Create I2C bus
	int file;
	
	if((file = open("/dev/i2c-1", O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, MMA8452Q I2C address is 0x1C(28)
	ioctl(file, I2C_SLAVE, 0x1C);
	
	return file;
}

void standbyMode(int file){
	// Select mode register(0x2A)
	// Standby mode(0x00)
	config[0] = 0x2A;
	config[1] = 0x00;
	int result = write(file, config, 2);
	/*if (result != 2) {
		perror("I2C: Unable to write i2c register.");
		exit(1);
	}*/
}

void activeMode(int file){
	// Select mode register(0x2A)
	// Active mode(0x01)
	config[0] = 0x2A;
	config[1] = 0x01;
	int result = write(file, config, 2);
	/*if (result != 2) {
		perror("I2C: Unable to write i2c register.");
		exit(1);
	}*/
}

void setRange(int file){
	// Select configuration register(0x0E)
	// Set range to +/- 2g(0x00)
	config[0] = 0x0E;
	config[1] = 0x00;
	int result = write(file, config, 2);
	/*if (result != 2) {
		perror("I2C: Unable to write i2c register.");
		exit(1);
	}*/
}

int digit12(int index1, int index2){
	int Accl = ((data[index1] * 256) + data[index2]) / 16;
	if(Accl > 2047)
	{
		Accl -= 4096;
	}
	
	return Accl;
}

int get_xacc(){
	return x_acc;
}
int get_yacc(){
	return y_acc;
}
int get_zacc(){
	return z_acc;
}

int get_accAngleX(){
	return accAngleX;
}
int get_accAngleY(){
	return accAngleY;
}

//double integral of the gravity, returns position
//TODO: Do all these inside a circular array or ds.
/*int positions(int gravity_val){
	
	//TODO: decide the time interval to take values at, and how to update the time variable 
	//TODO: correct the units

	int a = gravity_val;
	int vel = a*time_in + 1; //assuming c = 1;
	int position = (vel*vel) + (1)*time_in + 0; //asuming d = 0;

	return position;
}*/


void readData(int file){
	
	// Read 7 bytes of data(0x00)
	
	char reg[1] = {0x00};
	int res = write(file, reg, 1);
	/*if (res != 2) {
		perror("I2C: Unable to write i2c register.");
		exit(1);
	}*/
	if(read(file, data, 7) != 7)
	{
		printf("Error : Input/Output error \n");
	}
	else{
		x_acc = digit12(1,2); 
	    y_acc = digit12(3,4);
		z_acc = digit12(5,6);

		//calculating roll and pitch
		//TODO : adjust the error
		accAngleX = (atan(y_acc / sqrt(pow(x_acc, 2) + pow(z_acc, 2))) * 180 / 3.14);
		accAngleY = (atan(-1 * x_acc / sqrt(pow(y_acc, 2) + pow(z_acc, 2))) * 180 / 3.14);

	}
}


void routine(){

	int file = initI2cBus();
	setRange(file);
	
	while(1){
	 standbyMode(file);
	 activeMode(file);
	 sleep(0.5);
	 time_in +=1;
	 
	 readData(file);
	 sleep(2);

	}
	
}

void acc_init(){
	readingThread = new std::thread(routine);
}

void acc_cleanup(){
	readingThread->join();
}
 
