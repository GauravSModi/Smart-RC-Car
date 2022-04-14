#ifndef SHARPIR_DS_H
#define SHARPIR_DS_H

#include <stdio.h>


class SHARPDistanceSensor{
    FILE* sensorFD;

    public:
      
        unsigned int getVoltageValues();
        double getSensorValues();
        double pwlAlgorithm(double s,double a,double b,double m,double n);

        bool AlertPassedObject();


        ~SHARPDistanceSensor();
        SHARPDistanceSensor();
    
    private:
        
        double reading = 0;
        double prev_reading = 0;

};




#endif