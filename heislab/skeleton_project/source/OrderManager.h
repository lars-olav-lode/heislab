#pragma once
#include "driver/elevio.h"


// order-arrayet: orders[etasje][knapp] = 0/1
extern int orders[N_FLOORS][N_BUTTONS];

// Setter alle ordre til 0 og slukker alle lamper
void orders_init(void);


// Sjekk om det finnes noen ordre i etasjen
int  orders_any_here(int floor);

// Sjekk om det finnes noen ordre i etasjene over
int  orders_any_above(int floor);

// Sjekk om det finnes noen ordre i etasjene under
int  orders_any_below(int floor);

// Sjekk om det finnes noen ordre i det hele tatt
int  orders_any(void);

// Kalles for å avgjøre hvilken retning heisen skal gå i
MotorDirection orders_choose_direction(int current_floor, MotorDirection dir);

// Kalles når dere har nådd en etasje og skal avgjøre om dere skal stoppe eller ikke
int  orders_should_stop(int current_floor, MotorDirection dir);

// Kalles når dere faktisk stopper og åpner dør
void orders_clear_at_floor(int floor, MotorDirection dir);


void orders_clear_all(void);



