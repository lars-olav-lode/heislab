#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "ElevatorController.h"
#include "OrderManager.h"
#include "driver/elevio.h"
#include "ButtonPuller.h"





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