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
    int obstruction;
    int stop_button;
    double door_timer;
};