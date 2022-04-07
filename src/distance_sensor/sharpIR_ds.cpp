#include <iostream>
#include "sharpIR_ds.h"
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <cmath>


#define A2D_FILE_VOLTAGE1 "/sys/bus/iio/devices/iio:device0/in_voltage1_raw"
#define A2D_VOLTAGE_REF_V 1.8
#define A2D_MAX_READING 4095
#define TRAIN_DATA_LENGTH 14

static SHARPDistanceSensor* instance=NULL;
//first index represent distance(x-axis), second index represent voltage value(y-axis)
static double data[14][2]={{20,2.5},{30,2},{40,1.5},{50,1.25},{60,1.1},{70,0.9},{80,0.8},{90,0.75},
                            {100,0.7},{110,0.6},{120,0.55},{130,0.5},{140,0.45},{150,0.4}};

SHARPDistanceSensor* SHARPDistanceSensor::getInstance(){
    if(instance==NULL){
        instance=new SHARPDistanceSensor();
    }
    return instance;
}

SHARPDistanceSensor::SHARPDistanceSensor(){
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
    double average_correspondVoltage=0;
    double correspondVoltage=0;


    for(int i=0;i<10;i++){
        int a2dValue=this->getVoltageValues();
        double voltage=((double)a2dValue/A2D_MAX_READING)*A2D_VOLTAGE_REF_V;

        correspondVoltage+=voltage*5/1.8*0.9;
    }
    correspondVoltage/=10;

    // if(correspondVoltage<=data[0][1]&&correspondVoltage>data[1][1]){
    //     return pwlAlgorithm(correspondVoltage,data[0][0],data[1][0],data[0][1],data[1][1]);
    // }else if(correspondVoltage<=data[1][1]&&correspondVoltage>data[2][1]){
    //     return pwlAlgorithm(correspondVoltage,data[1][0],data[2][0],data[1][1],data[2][1]);
    // }else if(correspondVoltage<=data[2][1]&&correspondVoltage>data[3][1]){
    //     return pwlAlgorithm(correspondVoltage,data[2][0],data[3][0],data[2][1],data[3][1]);
    // }else if(correspondVoltage<=data[3][1]&&correspondVoltage>data[4][1]){
    //     return pwlAlgorithm(correspondVoltage,data[3][0],data[4][0],data[3][1],data[4][1]);
    // }else if(correspondVoltage<=data[4][1]&&correspondVoltage>data[5][1]){
    //     return pwlAlgorithm(correspondVoltage,data[4][0],data[5][0],data[4][1],data[5][1]);
    // }else if(correspondVoltage<=data[5][1]&&correspondVoltage>data[6][1]){
    //     return pwlAlgorithm(correspondVoltage,data[5][0],data[6][0],data[5][1],data[6][1]);
    // }else if(correspondVoltage<=data[6][1]&&correspondVoltage>data[7][1]){
    //     return pwlAlgorithm(correspondVoltage,data[6][0],data[7][0],data[6][1],data[7][1]);
    // }


    // for(int i=0;i<TRAIN_DATA_LENGTH-1;i++){
    //     if(correspondVoltage<=data[i][1]&&correspondVoltage>data[i+1][1]){
    //         ret=pwlAlgorithm(correspondVoltage,data[i][0],data[i+1][0],data[i][1],data[i+1][1]);
    //         break;
    //     }
    // }


    //linear approximation: Distance=29.988*pow(voltage,-1.173);
    ret=29.988*pow(correspondVoltage,-1);
    if((ret<=10)||(ret>=80)){
        ret=1000;
    }

    return ret;
    //return correspondVoltage;


}