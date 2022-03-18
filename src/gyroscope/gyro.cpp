#include "gyro.h"

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

void readData(int file){
    if(read(file, data, 6) != 6)
	{
		printf("Error : Input/Output error \n");
	}
	else
	{
		
		 xGyro = ((data[0] << 8) | data[1]) / 131;
		 yGyro = ((data[2] << 8) + data[3]) / 131;
		 zGyro = ((data[4] << 8) + data[5]) / 131;
 
		// Output data to screen
		printf("Gyro Reading on X-Axis : %d \n", get_xGyro()); //value range = 250 deg/s   
		printf("Gyro Reading on Y-Axis : %d \n", get_yGyro()); //TODO : fix calculation
		printf("Gyro Reading on Z-Axis : %d \n", get_zGyro());
	}
}



void gyro_routine(){
    int file = initI2cBus();
    printf("loaded the file, %d \n", file);
    writeI2cReg(file, PWR_MGMT_1, 0);

    while(1){
        readData(file);
        sleep(1);
    }

}


int main(){

    thread gyro_id (gyro_routine);
	gyro_id.join();

    return 0;
}