#include "gyro.h"
//#include "accelerometer.h"

char data[6] = {0};
char results[6];

double elapsed_t = 0;
std::chrono::_V2::system_clock::time_point prev_time ;
std::chrono::_V2::system_clock::time_point current_time ;


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

static unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr)
{
	// To read a register, must first write the address
	int res = write(i2cFileDesc, &regAddr, sizeof(regAddr));
	if (res != sizeof(regAddr)) {
		perror("Unable to write i2c register.");
		exit(-1);
	}

	// Now read the value and return it
	char value = 0;
	res = read(i2cFileDesc, &value, sizeof(value));
	if (res != sizeof(value)) {
		perror("Unable to read i2c register");
		exit(-1);
	}
	return value;
}



float get_xGyro(){
    return xGyro;
}
float get_yGyro(){
    return yGyro;
}
float get_zGyro(){
    return zGyro;
}


void calculateAngle() {  
  
  
  gyroAngleX = gyroAngleX + (get_xGyro()-error_x)*elapsed_t; // deg/s * s = deg
  gyroAngleY = gyroAngleY + (get_yGyro()-error_y)*elapsed_t;

  //add roll and pitch by including the acc module
  //roll = 0.96 * gyroAngleX + 0.04 * get_accAngleX();
  //pitch = 0.96 * gyroAngleY + 0.04 * get_accAngleY();

  //gyroAngleX = 0.96 * gyroAngleX + 0.04 * get_accAngleX();
  //gyroAngleY = 0.96 * gyroAngleY + 0.04 * get_accAngleY();

  roll = gyroAngleX;
  pitch = gyroAngleY;

  //yaw= yaw + (get_zGyro()-error_z)*elapsed_t;
  yaw= yaw + (get_zGyro()-error_z)*elapsed_t;

}

void readGyroData(int file){

	//data[0] = getRegisterValue(file, 0x43);
	//lseek(file, 67, SEEK_SET);
    /*if(read(file, data, 6) != 6)
	{
		printf("Error : Input/Output error \n");
	}
	else
	{*/
		prev_time = current_time;
		auto temp = std::chrono::system_clock::now();
		current_time =temp;

		std::chrono::duration<double> elapsed_seconds = current_time - prev_time;
		elapsed_t = (double)elapsed_seconds.count();

		for(int i = 0; i <6; i++){
			results[i] = readI2cReg(file, data[i]);
		}  

		xGyro = ((results[0] << 8) | results[1]) / 131; //angular velocity
		if(xGyro > 250){
			xGyro -= 500;
		}

		 
		yGyro = ((results[2] << 8) + results[3]) / 131;
		if(yGyro > 250){
			yGyro -= 500;
		}


		zGyro = ((results[4] << 8) + results[5]) / 131;
		if(zGyro > 250){
			zGyro -= 500;
		}
		 

		printf("[%d, %d, %d] \n", xGyro, yGyro, zGyro);
		
}



void avg_error(int file){


    for(int i = 0 ; i < 5000; i++){

		readGyroData(file);

		error_x = error_x + xGyro;
		error_y = error_y + yGyro;
		error_z = error_z + zGyro;
    }

	error_x = error_x/5000;
	error_y = error_y/5000;
	error_z = error_z/5000;
}

void gyro_routine(){
    int file = initI2cBus();
    
    writeI2cReg(file, PWR_MGMT_1, 0);

	//avg_error(file);
	
	data[0] = 0x43;
	data[1] = 0x44;
	data[2] = 0x45;
	data[3] = 0x46;
	data[4] = 0x47;
	data[5] = 0x48;

    while(1){

        readGyroData(file);
		//calculateAngle();

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
	//acc_init();

	//acc_cleanup();
	gyro_cleanup();

	return 0;
}