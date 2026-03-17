#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "ElevatorController.h"
#include "OrderManager.h"
#include "driver/elevio.h"
#include "ButtonPuller.h"


int orders[N_FLOORS][N_BUTTONS];

void orders_init(void){
    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            orders[f][b] = 0;
            elevio_buttonLamp(f, (ButtonType)b, 0);
        }
    }
}



int orders_any_here(int floor){
    for(int b = 0; b < N_BUTTONS; b++){
        if(orders[floor][b]) return 1;
    }
    return 0;
}

int orders_any_above(int floor){
    for(int f = floor + 1; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            if(orders[f][b]) return 1;
        }
    }
    return 0;
}

int orders_any_below(int floor){
    for(int f = 0; f < floor; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            if(orders[f][b]) return 1;
        }
    }
    return 0;
}

int orders_any(void){
    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            if(orders[f][b]) return 1;
        }
    }
    return 0;
}


MotorDirection orders_choose_direction(int current_floor, MotorDirection dir){
    if(dir == DIRN_UP){
        if(orders_any_above(current_floor)) return DIRN_UP;
        if(orders_any_here(current_floor))  return DIRN_STOP;
        if(orders_any_below(current_floor)) return DIRN_DOWN;
        return DIRN_STOP;
    }
    if(dir == DIRN_DOWN){
        if(orders_any_below(current_floor)) return DIRN_DOWN;
        if(orders_any_here(current_floor))  return DIRN_STOP;
        if(orders_any_above(current_floor)) return DIRN_UP;
        return DIRN_STOP;
    }

    if(orders_any_here(current_floor))  return DIRN_STOP;
    if(orders_any_above(current_floor)) return DIRN_UP;
    if(orders_any_below(current_floor)) return DIRN_DOWN;
    return DIRN_STOP;
}


int orders_should_stop(int current_floor, MotorDirection dir){
   
    if(orders[current_floor][BUTTON_CAB]) return 1;

    if(dir == DIRN_UP){
      
        if(orders[current_floor][BUTTON_HALL_UP]) return 1;

        
        if(!orders_any_above(current_floor) && orders[current_floor][BUTTON_HALL_DOWN]) return 1;

        return 0;
    }

    if(dir == DIRN_DOWN){
       
        if(orders[current_floor][BUTTON_HALL_DOWN]) return 1;

        if(!orders_any_below(current_floor) && orders[current_floor][BUTTON_HALL_UP]) return 1;

        return 0;
    }

    return orders_any_here(current_floor);
}


void orders_clear_at_floor(int floor, MotorDirection dir){
    for(int b = 0; b < N_BUTTONS; b++){
        orders[floor][b] = 0;
       elevio_buttonLamp(floor, b, 0);
    }
    }

void orders_clear_all(void){
    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            orders[f][b] = 0;
            elevio_buttonLamp(f, (ButtonType)b, 0);
        }
    }
}

