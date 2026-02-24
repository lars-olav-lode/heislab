
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "elevator.h"



void stop(){
    orders_clear_all();
    while(1){
        if(elevio_stopButton()==1){
            elevio_motorDirection(0);
            break;
        }
        else{elevio_motorDirection(1);}

    }
}


void opendoor(){  //Åpner føren i 3 sekunder, hvis det er en obstruksjon holdes den åpen
    elevio_doorOpenLamp(1);
    elevio_motorDirection(0);
    time_t start = time(NULL);
    while(time(NULL)-start < 3){}
    while(elevio_obstruction() == 1){
        elevio_doorOpenLamp(1);
        elevio_motorDirection(0);
    }
    elevio_doorOpenLamp(0);
    elevio_motorDirection(0);
}


int startup(){
    while(elevio_floorSensor() == -1){
        elevio_motorDirection(-1);
        
    }
    return elevio_floorSensor();

}

