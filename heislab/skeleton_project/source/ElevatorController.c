
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "OrderManager.h"
#include "ElevatorController.h"
#include "ButtonPuller.h"



void stop(){

    orders_clear_all();
    while(1){
        if(elevio_stopButton()==1){
            elevio_stopLamp(1);
            elevio_motorDirection(0);
            continue;
        }
        break;
    }
    elevio_stopLamp(0);
    
    
}



//Åpner føren i 3 sekunder, hvis det er en obstruksjon holdes den åpen

void opendoor(void){
    elevio_doorOpenLamp(1);
    elevio_motorDirection(DIRN_STOP);

    time_t last_clear = time(NULL);

    while(1){
        if(elevio_stopButton() == 1){
            elevio_stopLamp(1);
        }
        if(elevio_stopButton() != 1){
            elevio_stopLamp(0);
        }
        if(elevio_obstruction() || elevio_stopButton() == 1){
            // Obstruction aktiv, reset klar-tid
            last_clear = time(NULL);
        }

       
        if(time(NULL) - last_clear >= 3){
            break;
        }
    }
    elevio_doorOpenLamp(0);
}
    


void startup(){

    for(int floor = 0; floor < N_FLOORS;floor++){
        for(int button = 0; button < N_BUTTONS;button ++){
            elevio_buttonLamp(floor, button,0);
        }
    }
    while(elevio_floorSensor() == -1){
        elevio_motorDirection(-1);
    }
    elevio_motorDirection(0);
    elevio_floorIndicator(elevio_floorSensor());
}


int main_func(void){
    int floor = -1;
    MotorDirection dir = DIRN_STOP;

    while(1){

        orders_pull_buttons();

        int fs = elevio_floorSensor();
        if(fs != -1){
            floor = fs;
            elevio_floorIndicator(floor);

            if(dir != DIRN_STOP && orders_should_stop(floor, dir)){
                elevio_motorDirection(DIRN_STOP);
                opendoor();
                orders_clear_at_floor(floor, dir);

                
                dir = orders_choose_direction(floor, dir);
            }
        
        if(dir == DIRN_STOP && orders_any_here(floor)){
            elevio_motorDirection(DIRN_STOP);
            opendoor();
            orders_clear_at_floor(floor, dir);
}
        }

        if(elevio_stopButton()){
            elevio_motorDirection(DIRN_STOP);
            orders_clear_all();
            if(elevio_floorSensor()!= -1){
                opendoor();
            }
            else {stop();}
            dir = DIRN_STOP;
        }

        
        if(dir == DIRN_STOP && floor != -1){
            dir = orders_choose_direction(floor, dir);
        }

        
        elevio_motorDirection(dir);
    }
}
