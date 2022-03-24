#include "gyro.h"
#include "accelerometer.h"

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

void readGyroData(int file){
    if(read(file, data, 6) != 6)
	{
		printf("Error : Input/Output error \n");
	}
	else
	{
		xGyro_past = get_xGyro();
		yGyro_past = get_yGyro();
		zGyro_past = get_zGyro();

		 xGyro = ((data[0] << 8) | data[1]) / 131; //angular velocity
		 yGyro = ((data[2] << 8) + data[3]) / 131;
		 zGyro = ((data[4] << 8) + data[5]) / 131;
 
		// Output data to screen
		printf("Angle of X-Axis : %d \n", get_xGyro()); //value range = 250 deg/s   
		printf("Angle of Y-Axis : %d \n", get_yGyro()); //TODO : fix calculation
		printf("Angle of Z-Axis : %d \n", get_zGyro());
	}
}


void calculateAngle() {  
  // same equation can be written as 
  // angelZ = angelZ + ((timePresentZ - timePastZ)*(gyroZPresent + gyroZPast - 2*gyroZCalli)) / (2*1000*131);
  // 1/(1000*2*131) = 0.00000382
  // 1000 --> convert milli seconds into seconds
  // 2 --> comes when calculation area of trapezium
  // substacted the callibated result two times because there are two gyro readings
  angle_x = angle_x + ((current_time - prev_time)*(xGyro + xGyro_past - 2*error_x)) * 0.00000382;
  angle_y = angle_y + ((current_time - prev_time)*(yGyro + yGyro_past - 2*error_y)) * 0.00000382;
  angle_z = angle_z + ((current_time - prev_time)*(zGyro + zGyro_past - 2*error_z)) * 0.00000382;
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

