#include <iostream>
#include <distance_sensor/sharpIR_ds.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[]){

    SHARPDistanceSensor* dis= new SHARPDistanceSensor();
    while(1){
        //int i=dis->getVoltageValues();
        double i=dis->getSensorValues();
        cout<<i<<endl;
    }

    return 0;
}
