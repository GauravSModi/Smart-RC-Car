#include <iostream>
#include "sharpIR_ds.h"
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <unistd.h>

using namespace std;

#define A2D_FILE_VOLTAGE1 "/sys/bus/iio/devices/iio:device0/in_voltage1_raw"
#define A2D_VOLTAGE_REF_V 1.8
#define A2D_MAX_READING 4095
#define TRAIN_DATA_LENGTH 14

//static SHARPDistanceSensor* instance=NULL;

/*SHARPDistanceSensor* SHARPDistanceSensor::getInstance(){
    if(instance==NULL){
        instance=new SHARPDistanceSensor();
    }
    return instance;
}*/

SHARPDistanceSensor::SHARPDistanceSensor(){
    this->reading = 0;
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

// static double power(double x, double y){
//     double val=1.0;

// }


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
    ret=29.988*pow(correspondVoltage,-1);
    if((ret<=10)||(ret>=80)){
        ret=0;
    }

    return ret;
    //return correspondVoltage;


}

/*int main(int argc, char* argv[]){

    SHARPDistanceSensor * dis= new SHARPDistanceSensor();
    while(1){
        
        double i=dis->getSensorValues();
        cout<<i<<endl;
        
    }

    return 0;
}*/
