#include "gyro.h"

//Start calculating the yaw once object encountered
//Once at a certain distance from the the object, start counting the yaw

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
		//exit(1);
	}
}

static unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr)
{
	// To read a register, must first write the address
	int res = write(i2cFileDesc, &regAddr, sizeof(regAddr));
	if (res != sizeof(regAddr)) {
		perror("Unable to write i2c register.");
		//exit(-1);
	}

	// Now read the value and return it
	char value = 0;
	res = read(i2cFileDesc, &value, sizeof(value));
	if (res != sizeof(value)) {
		perror("Unable to read i2c register");
		//exit(-1);
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
float getYaw(){
	return yaw;
}

/*float getAbsYaw(){
	return absolute_yaw;
}*/

void magic(){
	yaw = 0;
}


bool is90(void){

	//cap the yaw
    //assume for now that once its more than 90, set yaw back to 0.
    //originally set yaw back to 0, once the rover is not moving

	float condition  = yaw - offset;
	//if(getYaw() > 90){
	if(condition > 75){	
	  printf("Yaw is over 90 right now \n"); 
	  offset = getYaw();

	  //yaw = 0;

	 return true;
  }
  //else if(getYaw() < -90){
	else if( condition < -75){  

	  printf("Yaw is under -90 right now \n");
	  offset = getYaw();
	  //yaw = 0;
	  
	  return true;
  }
	return false;
}

double elapsed_time(){

	double ret_val;
	prev_time = current_time;
	auto temp = std::chrono::system_clock::now();
	current_time =temp;

	std::chrono::duration<double> elapsed_seconds = current_time - prev_time;
	ret_val = (double)elapsed_seconds.count();

	return ret_val;
	
}

void calculateAngle() {  

 // prev_yaw = getYaw();	
  elapsed_t = elapsed_time();	
 //Only need the yaw readings for left and right movement.

  float delta =(zGyro-error_z)*elapsed_t;
  
  float actual =  (delta > 0.0016 || delta < -0.0016)? delta :0.0;

 
  //yaw= yaw + (zGyro-error_z)*elapsed_t;
  yaw= yaw + actual;
  //float temp = yaw;
  //absolute_yaw = temp;
  //printf("delat value = %f, actual = %f, yaw = %f \n", delta,actual,yaw);
  printf("running sum = %f\n", yaw);
  
  
}




void readGyroData(int file){

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
		
}

void avg_error(int file){


    for(int i = 0 ; i < 1000; i++){

		readGyroData(file);

		error_x = error_x + xGyro;
		error_y = error_y + yGyro;
		error_z = error_z + zGyro;
    }

	error_x = error_x/1000;
	error_y = error_y/1000;
	error_z = error_z/1000;
}

void gyro_routine(){
    int file = initI2cBus();
    
    writeI2cReg(file, PWR_MGMT_1, 0);

	
	data[0] = 0x43;
	data[1] = 0x44;
	data[2] = 0x45;
	data[3] = 0x46;
	data[4] = 0x47;
	data[5] = 0x48;

	avg_error(file);
	sleep(2);
    while(1){

        readGyroData(file);
	
	    calculateAngle();
	    sleep(0.01);
    }

}

void gyro_init(){
	gyro_readingThread = new std::thread(gyro_routine);
}

void gyro_cleanup(){
	gyro_readingThread->join();
}

