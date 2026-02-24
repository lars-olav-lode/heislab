#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "startup.h"
#include "elevio.h"


struct elevator{
    MotorDirection dirc;
    int door_open;
    int moving;
<<<<<<< HEAD
    int directions; // -1 for ned, 1 for opp
    int next_stop;  // etasje vi skal til
    int obstruction;
    int stop_button;
    double door_timer;
    int current_floor;
};

void main_func(struct elevator el){
    while(1){
        if (el.stop_button == 1){
            stop();
        }
        if(el.door_open==1 && el.obstruction == 1){
            halt();
        }
        
    }
}


int orders[N_FLOORS][N_BUTTONS];   // En matrix som tar inn etasje og knapp

static MotorDirection orders_choose_direction(int current_floor, MotorDirection dir){
    if(dir == DIRN_UP){
        if(orders_any_above(current_floor)) return DIRN_UP;
        if(orders_any_below(current_floor)) return DIRN_DOWN;
        return DIRN_STOP;
    }
    if(dir == DIRN_DOWN){
        if(orders_any_below(current_floor)) return DIRN_DOWN;
        if(orders_any_above(current_floor)) return DIRN_UP;
        return DIRN_STOP;
    }

    // Den er på DIRN_STOP
    if(orders_any_here(current_floor)) return DIRN_STOP; // håndteres av "stoppe og clear"
    if(orders_any_above(current_floor)) return DIRN_UP;
    if(orders_any_below(current_floor)) return DIRN_DOWN;
    return DIRN_STOP;
}

static int orders_should_stop(int current_floor, MotorDirection dir){
    // Hvis vi står stille: stopp kun hvis det finnes ordre i etasjen
    if(dir == DIRN_STOP){
        orders_clear_at_floor(current_floor);
        return orders_any_here(current_floor);
    }

    // CAB: stopp alltid hvis noen skal av/på i cab til denne etasjen
    if(orders[current_floor][BUTTON_CAB]){
        orders_clear_at_floor(current_floor);
         return 1;
        }


    if(dir == DIRN_UP){
        // Betjen kun "opp"-ordre mens vi går opp (H2)
        if(orders[current_floor][BUTTON_HALL_UP]){ 
            orders_clear_at_floor(current_floor);
            return 1;
        }

        // End-of-run: hvis vi ikke har flere ordre over oss,
        // kan vi stoppe for hall_down i denne etasjen før vi snur.
        if(!orders_any_above(current_floor) && orders[current_floor][BUTTON_HALL_DOWN]) {
            orders_clear_at_floor(current_floor);
            return 1;
        }

        return 0;
    }

    // dir == DIRN_DOWN
    if(orders[current_floor][BUTTON_HALL_DOWN]) {
        orders_clear_at_floor(current_floor);
        return 1;
    }

    if(!orders_any_below(current_floor) && orders[current_floor][BUTTON_HALL_UP]) return 1;

    return 0;
}


void orders_pull_buttons(void){
    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            if(elevio_callButton(f, (ButtonType)b)){
                orders[f][b] = 1;
                elevio_buttonLamp(f, (ButtonType)b, 1);
            }
        }
    }
} //Skjekker om noen av knappene blir trukket inn, og får knappen til å lyse



int orders_any_above_and_up(int floor){
    for(int f = floor+1; f < N_FLOORS; f++){
            if(orders[f][BUTTON_HALL_UP]) return f;
    }
    return -1;
} //Skjekker om det er noen bestillinger under, som skal samme vei

int orders_any_under_and_down(int floor){
    for(int f = floor-1; f >= 0; f--){
            if(orders[f][BUTTON_HALL_DOWN]) return f;
    }
    return -1;
} //Skjekker om det er noen bestillinger under, som skal samme vei

int orders_any_below(int floor){
    for(int f = 0; f < floor; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            if(orders[f][b]) return 1;
        }
    }
    return 0;
}

int orders_any_above(int floor){
    for(int f = floor; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            if(orders[f][b]) return 1;
        }
    }
    return 0;
}


void orders_clear_at_floor(int floor){
    for(int b = 0; b < N_BUTTONS; b++){
        orders[floor][b] = 0;
        elevio_buttonLamp(floor, (ButtonType)b, 0);
    }
} //Slukker alle lysene, samt sletter alle bestillingene for en etasje

void orders_clear_all(){   //Denne nullstiller alle bestillingsknapper
     for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            orders[f][b] = 0;
        }
    }
}

=======
    int obstruction;
    int stop_button;
    double door_timer;
};
>>>>>>> d499b1971c2259a94cf449628e23040bcdb2f8b5
