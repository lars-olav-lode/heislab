
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"



void stop(){
    while(1){
        if(elevio_stopButton()==1){
            elevio_motorDirection(0);
            break;
        }
        else{elevio_motorDirection(1);}

    }
}


void opendoor(){
    elevio_doorOpenLamp(1);
    elevio_motorDirection(0);
    time_t start = time(NULL);
    while(time(NULL)-start < 3){}
    elevio_doorOpenLamp(0);
    elevio_motorDirection(0);
}


void startup(){
    while(elevio_floorSensor() == -1){
        elevio_motorDirection(-1);
        
    }
    opendoor();

}}
    while(elevio_stopButton==1){
        elevio_motorDirection(0);