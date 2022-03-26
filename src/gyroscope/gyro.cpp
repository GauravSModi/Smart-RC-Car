#include "gyro.h"
//#include "../acclerometer/accelerometer.h"

char data[6] = {43};

static int initI2cBus(void)
{
	// Create I2C bus
	int file;
	
	if((file = open("/dev/i2c-2", O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, MMA8452Q I2C address is 0x68
	ioctl(file, I2C_SLAVE, deviceADDR);
	
	return file;
}

static void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value)
{
	unsigned char buff[2];
	buff[0] = regAddr;
	buff[1] = value;
	int res = write(i2cFileDesc, buff, 2);
	if (res != 2) {
		perror("I2C: Unable to write i2c register.");
		exit(1);
	}
}

int get_xGyro(){
    return xGyro;
}
int get_yGyro(){
    return yGyro;
}
int get_zGyro(){
    return zGyro;
}

double elapsed_time(){
	auto start = std::chrono::system_clock::now();
    // Some computation here
    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end-start;
	double res = (double)elapsed_seconds.count();
	return res;
}


void calculateAngle() {  
  
  double elapsed_t = elapsed_time();
  gyroAngleX = gyroAngleX + get_xGyro()*elapsed_t; // deg/s * s = deg
  gyroAngleY = gyroAngleY + get_yGyro()*elapsed_t;

  //add roll and pitch by including the acc module
  //roll = roll + ...
  //pitch = pitch + ...
  yaw= yaw + get_zGyro()*elapsed_t;


  printf(" GyroAnglex : %f \n", gyroAngleX);
  printf(" GyroAngley : %f \n", gyroAngleY);
  printf(" yaw : %f \n", yaw);
  
}

void readGyroData(int file){
    if(read(file, data, 6) != 6)
	{
		printf("Error : Input/Output error \n");
	}
	else
	{

		 xGyro = ((data[0] << 8) | data[1]) / 131; //angular velocity
		 yGyro = ((data[2] << 8) + data[3]) / 131;
		 zGyro = ((data[4] << 8) + data[5]) / 131;
 
		// Output data to screen
		//printf("Angle of X-Axis : %d \n", get_xGyro()); //value range = 250 deg/s   
		//printf("Angle of Y-Axis : %d \n", get_yGyro()); //TODO : fix range
		//printf("Angle of Z-Axis : %d \n", get_zGyro());
		calculateAngle() ;
	}
}



void avg_error(){
    for(int i = 0 ; i < 1000; i++){
        error_x = error_x + get_xGyro();
		error_y = error_y + get_yGyro();
		error_z = error_z + get_zGyro();
    }

	error_x = error_x/1000;
	error_y = error_y/1000;
	error_z = error_z/1000;
}

void gyro_routine(){
    int file = initI2cBus();
    printf("loaded the file, %d \n", file);
    writeI2cReg(file, PWR_MGMT_1, 0);

	avg_error();
	//init current time as well

    while(1){

        readGyroData(file);
        sleep(1);
    }

}

void gyro_init(){
	gyro_readingThread = new std::thread(gyro_routine);
}

void gyro_cleanup(){
	gyro_readingThread->join();
}

int main(){
	gyro_init();
	gyro_cleanup();

	return 0;
}