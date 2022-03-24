#include "accelerometer.h"

//TODO: get the accuracy of the positions right

const int MAX_POS = 1000;

int xPOS [MAX_POS];
int yPOS [MAX_POS];
int zPOS [MAX_POS];

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
	write(file, config, 2);
}

void activeMode(int file){
	// Select mode register(0x2A)
	// Active mode(0x01)
	config[0] = 0x2A;
	config[1] = 0x01;
	write(file, config, 2);
}

void setRange(int file){
	// Select configuration register(0x0E)
	// Set range to +/- 2g(0x00)
	config[0] = 0x0E;
	config[1] = 0x00;
	write(file, config, 2);
}

int digit12(int index1, int index2){
	int Accl = ((data[index1] * 256) + data[index2]) / 16;
	if(Accl > 2047)
	{
		Accl -= 4096;
	}
	
	return Accl;
}

//double integral of the gravity, returns position
//TODO: Do all these inside a circular array or ds.
int positions(int gravity_val){
	
	//TODO: decide the time interval to take values at, and how to update the time variable 
	//TODO: correct the units

	int a = gravity_val;
	int vel = a*time_in + 1; //assuming c = 1;
	int position = (vel*vel) + (1)*time_in + 0; //asuming d = 0;

	return position;
}


void readData(int file){
	
	// Read 7 bytes of data(0x00)
	
	char reg[1] = {0x00};
	write(file, reg, 1);
	if(read(file, data, 7) != 7)
	{
		printf("Error : Input/Output error \n");
	}
	else{
		int x_acc = digit12(1,2); 
		int y_acc = digit12(3,4);
		int z_acc = digit12(5,6);

		xPOS[time_in] = positions(x_acc);
		yPOS[time_in] = positions(y_acc);
		zPOS[time_in] = positions(z_acc);

		//Remove this once circular array or ds added
		if(time_in >= 1000){
			printf("buffer full \n");
			exit(1);
		}
		
		printf("x coordinate : %d \n", xPOS[time_in]);
		printf("y coordinate : %d \n", yPOS[time_in]);
		printf("z coordinate : %d \n", zPOS[time_in]);
		
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

 
int main(){

	thread id (routine);
	id.join();
	
	return 0;
}