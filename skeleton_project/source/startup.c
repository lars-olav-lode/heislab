
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
<<<<<<< HEAD
#include "elevator.h"
=======
>>>>>>> d499b1971c2259a94cf449628e23040bcdb2f8b5



void stop(){
<<<<<<< HEAD
    orders_clear_all();
=======
>>>>>>> d499b1971c2259a94cf449628e23040bcdb2f8b5
    while(1){
        if(elevio_stopButton()==1){
            elevio_motorDirection(0);
            break;
        }
        else{elevio_motorDirection(1);}

    }
}


<<<<<<< HEAD
void opendoor(){  //Åpner føren i 3 sekunder, hvis det er en obstruksjon holdes den åpen
=======
void opendoor(){
>>>>>>> d499b1971c2259a94cf449628e23040bcdb2f8b5
    elevio_doorOpenLamp(1);
    elevio_motorDirection(0);
    time_t start = time(NULL);
    while(time(NULL)-start < 3){}
<<<<<<< HEAD
    while(elevio_obstruction() == 1){
        elevio_doorOpenLamp(1);
        elevio_motorDirection(0);
    }
=======
>>>>>>> d499b1971c2259a94cf449628e23040bcdb2f8b5
    elevio_doorOpenLamp(0);
    elevio_motorDirection(0);
}


<<<<<<< HEAD
int startup(){
=======
void startup(){
>>>>>>> d499b1971c2259a94cf449628e23040bcdb2f8b5
    while(elevio_floorSensor() == -1){
        elevio_motorDirection(-1);
        
    }
<<<<<<< HEAD
    return elevio_floorSensor();

}

=======
    opendoor();

}}
    while(elevio_stopButton==1){
        elevio_motorDirection(0);
>>>>>>> d499b1971c2259a94cf449628e23040bcdb2f8b5
