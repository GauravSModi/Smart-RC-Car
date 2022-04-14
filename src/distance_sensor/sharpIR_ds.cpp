#include <iostream>
#include "sharpIR_ds.h"
#include <unistd.h>
#include <fcntl.h>
#include <common/utils.h>


using namespace std;

#define A2D_FILE_VOLTAGE1 "/sys/bus/iio/devices/iio:device0/in_voltage1_raw"
#define A2D_VOLTAGE_REF_V 1.8
#define A2D_MAX_READING 4095
#define TRAIN_DATA_LENGTH 14

#define OBJECT_DETECTION_DISTANCE 20
#define DS2_HYSTERIA 5


bool keepMoving = true;


SHARPDistanceSensor::SHARPDistanceSensor(){
    this->reading = 0;

}

SHARPDistanceSensor::~SHARPDistanceSensor()
{
    
}
unsigned int SHARPDistanceSensor::getVoltageValues(){
    this->sensorFD=fopen(A2D_FILE_VOLTAGE1,"r");
    if(!this->sensorFD){
        printf("ERROR: UNABLE TO OPEN VOLTAGE FILE\n");
        exit(-1);
    }

    //get reading
    int a2dReading=0;
    int itemsRead=fscanf(this->sensorFD,"%d",&a2dReading);
    if(itemsRead<0){
        printf("ERROR: UNABLE TO READ VALUES FROM VOLTAGE INPUT FILE\n");
        exit(-1);
    }

    fclose(this->sensorFD);
    return a2dReading;
}

double SHARPDistanceSensor::pwlAlgorithm(double s,double a,double b,double m,double n){
    //s is the value we wanted
    //a is x1, b is x2
    //n is y1, m is y2
    //this is a algorithm for calculating the downside linear functions
    double ret=((s-n)/(m-n)*(b-a)+b);
    return ret;
}



double SHARPDistanceSensor::getSensorValues(){
    //use voltage graph to train the data
    //in order to calculate the distance
    double ret=0;
    //double average_correspondVoltage=0;
    double correspondVoltage=0;


    for(int i=0;i<10;i++){
        int a2dValue=this->getVoltageValues();
        double voltage=((double)a2dValue/A2D_MAX_READING)*A2D_VOLTAGE_REF_V;

        correspondVoltage+=voltage*5/1.8*0.9;
    }
    correspondVoltage/=10;


    //linear approximation: Distance=29.988*pow(voltage,-1.173);
    ret=29.988*fastPow(correspondVoltage,-1.0);
    if((ret<=10)||(ret>=80)){
        ret=0;
    }

    return ret;
  


}

bool SHARPDistanceSensor::AlertPassedObject(){
    
  
    int hystreria = 0;
    bool objectInsight = false;
    while(keepMoving) {
        

        this->prev_reading = this->reading;
        this->reading = this->getSensorValues();
        printf("distance sensor #2 values : %f\n", this->reading);
        if(!objectInsight){
            if(this->reading < OBJECT_DETECTION_DISTANCE && this->reading > 0){
                hystreria++;
                if(hystreria > DS2_HYSTERIA){
                    objectInsight = true;
                    hystreria = 0;
                }
            } else {
                hystreria = 0;
            }
        } else {
            if(this->reading > OBJECT_DETECTION_DISTANCE || this->reading == 0){
                hystreria++;
                if(hystreria > DS2_HYSTERIA){
                    keepMoving = false;
                }
            } else {
                hystreria = 0;
            }
            // stop
        }

        sleep(0.001);
    }
    
    keepMoving = true;

    return true;
}

