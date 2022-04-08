#include <iostream>
#include <distance_sensor/sharpIR_ds.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[]){

    SHARPDistanceSensor * dis=SHARPDistanceSensor::getInstance();
    while(1){
        //int i=dis->getVoltageValues();
        double i=dis->getSensorValues();
        cout<<i<<endl;
        sleep(1);
    }



    return 0;
}