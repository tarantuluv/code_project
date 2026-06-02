#ifndef RESULTS_H
#define RESULTS_H

#include <stdio.h>
#include "simulation.h"

typedef struct
{
    double totalTime;
    int numPass;
} DeskInfo;

typedef struct
{
    double arrivalTime;
    double queueTime;
    double service_checkin;
    double checkinTime;
    
} PassengerInfo;

int numPass;
int numDesks;
int maxQueue;
int numQPass;
double totalTime;

PassengerInfo* passengers = NULL;
DeskInfo* desks = NULL;

#endif