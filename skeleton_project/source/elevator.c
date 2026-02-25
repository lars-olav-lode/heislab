#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "startup.h"
#include "elevator.h"
#include "driver/elevio.h"

int orders[N_FLOORS][N_BUTTONS];

void orders_init(void){
    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            orders[f][b] = 0;
            elevio_buttonLamp(f, (ButtonType)b, 0);
        }
    }
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

// Retningsvalg (samme idé som du hadde, men bygger på riktig above/below/here)
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

    // DIRN_STOP
    if(orders_any_here(current_floor))  return DIRN_STOP;
    if(orders_any_above(current_floor)) return DIRN_UP;
    if(orders_any_below(current_floor)) return DIRN_DOWN;
    return DIRN_STOP;
}

// Stopp-regler (clear skjer IKKE her!)
int orders_should_stop(int current_floor, MotorDirection dir){
    // Cab-bestilling i etasjen -> stopp alltid
    if(orders[current_floor][BUTTON_CAB]) return 1;

    if(dir == DIRN_UP){
        // Betjen hall_up mens vi går opp
        if(orders[current_floor][BUTTON_HALL_UP]) return 1;

        // End-of-run: hvis ingen over, kan vi stoppe for hall_down før vi snur
        if(!orders_any_above(current_floor) && orders[current_floor][BUTTON_HALL_DOWN]) return 1;

        return 0;
    }

    if(dir == DIRN_DOWN){
        // Betjen hall_down mens vi går ned
        if(orders[current_floor][BUTTON_HALL_DOWN]) return 1;

        // End-of-run: hvis ingen under, kan vi stoppe for hall_up før vi snur
        if(!orders_any_below(current_floor) && orders[current_floor][BUTTON_HALL_UP]) return 1;

        return 0;
    }

    // DIRN_STOP: hvis vi står stille og finnes ordre her -> stopp (åpne dør)
    return orders_any_here(current_floor);
}

// Kalles når dere faktisk har stoppet og skal “servere” etasjen
void orders_clear_at_floor(int floor, MotorDirection dir){
    // Cab alltid
    orders[floor][BUTTON_CAB] = 0;
    elevio_buttonLamp(floor, BUTTON_CAB, 0);

    if(dir == DIRN_UP){
        // Når vi går opp, clear hall_up
        orders[floor][BUTTON_HALL_UP] = 0;
        elevio_buttonLamp(floor, BUTTON_HALL_UP, 0);

        // Hvis end-of-run (ingen over), clear også hall_down her
        if(!orders_any_above(floor)){
            orders[floor][BUTTON_HALL_DOWN] = 0;
            elevio_buttonLamp(floor, BUTTON_HALL_DOWN, 0);
        }
    }
    else if(dir == DIRN_DOWN){
        // Når vi går ned, clear hall_down
        orders[floor][BUTTON_HALL_DOWN] = 0;
        elevio_buttonLamp(floor, BUTTON_HALL_DOWN, 0);

        // Hvis end-of-run (ingen under), clear også hall_up her
        if(!orders_any_below(floor)){
            orders[floor][BUTTON_HALL_UP] = 0;
            elevio_buttonLamp(floor, BUTTON_HALL_UP, 0);
        }
    }
    else { 
        // DIRN_STOP: vi står i etasje. Ofte trygt å clear begge hall-lamper i etasjen
        orders[floor][BUTTON_HALL_UP] = 0;
        orders[floor][BUTTON_HALL_DOWN] = 0;
        elevio_buttonLamp(floor, BUTTON_HALL_UP, 0);
        elevio_buttonLamp(floor, BUTTON_HALL_DOWN, 0);
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

int main_func(void){
    int floor = -1;
    MotorDirection dir = DIRN_STOP;

    while(1){

        orders_pull_buttons();

        int fs = elevio_floorSensor();
        if(fs != -1){
            floor = fs;
            elevio_floorIndicator(floor);

            // Skal vi stoppe her?
            if(dir != DIRN_STOP && orders_should_stop(floor, dir)){
                elevio_motorDirection(DIRN_STOP);
                opendoor();
                orders_clear_at_floor(floor, dir);

                // Velg ny retning etter stopp
                dir = orders_choose_direction(floor, dir);
            }
        }

        if(elevio_stopButton()){
            elevio_motorDirection(DIRN_STOP);
            orders_clear_all();
            stop();
            dir = DIRN_STOP;
        }

        // Hvis vi står stille -> velg retning
        if(dir == DIRN_STOP && floor != -1){
            dir = orders_choose_direction(floor, dir);
        }

        elevio_motorDirection(dir);
    }
}

// int main_func(void){
//     int floor = -1;                 // start som ukjent

//     while(1){
//         orders_pull_buttons();

//         int fs = elevio_floorSensor();   
//         if(fs != -1){
//             floor = fs;
//             elevio_floorIndicator(floor);
//         }

//         if(elevio_stopButton() == 1){
//             stop();
//             startup();
//         }

//         // Ikke bruk orders_any_* hvis floor er -1
//         if(floor != -1 && orders_any_below(floor)){
//             elevio_motorDirection(DIRN_DOWN);
//         }

//          if(floor != -1 && orders_any_above(floor)){
//             elevio_motorDirection(DIRN_UP);
//         }

//         if(floor != -1 && orders_any_here(floor)){
//             opendoor();
//             orders_clear_at_floor(floor, DIRN_STOP);  // se under
//         }
//     }
// }

// int main_func(void){
//     int floor;
//     while(1){

//         orders_pull_buttons();

//         if(elevio_floorSensor()!= -1){
//             elevio_floorIndicator(elevio_floorSensor());
//             floor = elevio_floorSensor();
//         }
        
//         if (elevio_stopButton() == 1){stop();}

//         if(orders_any_below(floor)){
//             elevio_motorDirection(-1);
//         }

//         if(orders_any_here(floor) && elevio_floorSensor()!= -1){
//             opendoor();
//             orders_clear_at_floor(0, DIRN_DOWN);
//         }

    //     int floor = elevio_floorSensor();
    //     MotorDirection dir = DIRN_STOP;
    //     if(floor != -1){
    //         elevio_floorIndicator(floor);
    //     }

    //     // STOP har høyeste prioritet
    //     if(elevio_stopButton()==1){
    //         printf("Stopp er trykket");
            
    //     }
       
        

    //     // L1/L2: les knapper (NB: ikke kall denne inne i STOP-loop)
    //     orders_pull_buttons();

    //     // Hvis dør er åpen: ikke kjør (S1)
    //     // (Hvis du bruker opendoor_3s_with_obstruction() som blokkerer, trenger du ikke ekstra “door_open”-state her.)

    //     // Velg retning
    //     if(dir == DIRN_STOP){
    //         if(floor != -1 && orders_any_here(floor)){
    //             opendoor();
    //             orders_clear_at_floor(floor, DIRN_STOP);
    //         } else if(floor != -1) {
    //             dir = orders_choose_direction(floor, dir);
    //             elevio_motorDirection(dir);
    //         }
    //     } else {
    //         elevio_motorDirection(dir);

    //         // Når vi ankommer definert etasje
    //         if(floor != -1 && orders_should_stop(floor, dir)){
    //             elevio_motorDirection(DIRN_STOP);
    //             opendoor();
    //             orders_clear_at_floor(floor, dir);

    //             // ny retning etter stopp
    //             dir = orders_choose_direction(floor, dir);
    //             elevio_motorDirection(dir);
    //         }
    //     }
//     }
// }