#pragma once


#define N_FLOORS 4

typedef enum { 
    DIRN_DOWN   = -1,
    DIRN_STOP   = 0,
    DIRN_UP     = 1
} MotorDirection;


#define N_BUTTONS 3

typedef enum { 
    BUTTON_HALL_UP      = 0,
    BUTTON_HALL_DOWN    = 1,
    BUTTON_CAB          = 2
} ButtonType;


void elevio_init(void);

void elevio_motorDirection(MotorDirection dirn);//Tar inn motordirection, gir ut -1 ned,0 stopp. 1 opp
void elevio_buttonLamp(int floor, ButtonType button, int value); //Denne får knapper til å lyse og skal brukes til bestillinger
void elevio_floorIndicator(int floor); // -1, 0, 1, 2, 3
void elevio_doorOpenLamp(int value); // 0 lukket, 1 for åpen
void elevio_stopLamp(int value);//stop button trykkes


int elevio_callButton(int floor, ButtonType button); 
int elevio_floorSensor(void); // returnerer når, 0-3 og -1
int elevio_stopButton(void);
int elevio_obstruction(void); // Det er en oppstruksjon


