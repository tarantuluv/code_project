#ifndef SIMULATION_H
#define SIMULATION_H

#include <stdio.h>

#define NAME_LEN 20

#define LENGTH 200

typedef struct
{
    int id;
    char name[NAME_LEN];

} Passenger;

typedef enum
{
    ARRIVAL,
    CHECKIN_DONE
} EventType;

typedef struct Event
{
    double time;
    EventType type;
    Passenger p;
    int deskId;
    struct Event* next;
} Event;

typedef struct
{
    int busy;
} Desk;

typedef struct Node
{
    Passenger p;
    struct Node* next;
} Node;

typedef struct
{
    int passengers;
    int checkinDesks;
} SimulationConfig;

static Event* pushEvent(Event* head, Event* e);
static Event* popEvent(Event* head, Event** out);

static void logEvent(int max, char** log, int* idx, const char* msg);

void runSimulation(SimulationConfig* config, FILE* logFile);

void printResults(SimulationConfig* config);
void FreeResults();
#endif