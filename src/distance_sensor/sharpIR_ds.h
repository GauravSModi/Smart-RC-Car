#ifndef SHARPIR_DS_H
#define SHARPIR_DS_H

#include <stdio.h>

//start reading dist sensor 2 values after rover has finsished turning from the object.
//while(moving forward = true) -> keep reading the sensor 2 values
//once the reading gets 0 (with the hysteresis) -> moveforward = false;

class SHARPDistanceSensor{
    FILE* sensorFD;

    public:
      
        unsigned int getVoltageValues();
        double getSensorValues();
        double pwlAlgorithm(double s,double a,double b,double m,double n);

        ~SHARPDistanceSensor();
        SHARPDistanceSensor();
    
    private:
        float reading;

};




#endif