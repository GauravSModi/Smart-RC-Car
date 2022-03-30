#ifndef SHARPIR_DS_H
#define SHARPIR_DS_H

#include <stdio.h>

class SHARPDistanceSensor{
    FILE* sensorFD;

    public:
        static SHARPDistanceSensor* getInstance();
        unsigned int getVoltageValues();
        double getSensorValues();
        double pwlAlgorithm(double s,double a,double b,double m,double n);
    
    private:
        ~SHARPDistanceSensor();
        SHARPDistanceSensor();

};




#endif