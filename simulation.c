#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simulation.h"
#include "queue.h"
#include "results.h"


static Event* pushEvent(Event* head, Event* e)
{
    if (!head || e->time < head->time)
    {
        e->next = head;
        return e;
    }

    Event* cur = head;
    while (cur->next && cur->next->time <= e->time)
        cur = cur->next;

    e->next = cur->next;
    cur->next = e;

    return head;
}

static Event* popEvent(Event* head, Event** out)
{
    if (!head) 
    {
        return NULL;
        *out =NULL; 
    }
    *out = head;
    return head->next;
}


static void logEvent(int max, char** log, int* idx, const char* msg)
{
    printf("%s\n", msg);
    if(max > *idx) {
    strcpy(log[*idx], msg);
    (*idx)++;
    }
}



void runSimulation(SimulationConfig* config, FILE* logFile)
{
    maxQueue = 0;
    numQPass = 0;
    totalTime = 0;
    
    numPass = config->passengers;
    numDesks = config->checkinDesks;
    
    passengers = malloc(numPass * sizeof(PassengerInfo));
    desks = malloc(numDesks * sizeof(DeskInfo));
    
    Node* checkinQ = NULL;
    char buffer[LENGTH];
    int logCap = config->passengers * 5 + 2;
    
    char** log = malloc(logCap * sizeof(char*));
    for (int i = 0; i < logCap; i++)
        log[i] = malloc(LENGTH);
    
    int logIndex = 0;

    logEvent(logCap, log, &logIndex, "--- SIMULATION STARTED ---");



    Desk* checkin = calloc(config->checkinDesks, sizeof(Desk));

    Event* eventList = NULL;

    int idCounter = 1;

    for (int i = 0; i < config->passengers; i++)
    {
        Event* e = malloc(sizeof(Event));
        e->type = ARRIVAL;
        e->time =  i * 2+(rand() % 100) / 100.0;
        (passengers+i)->arrivalTime = e->time;
        e->p.id = idCounter++;
        sprintf(e->p.name, "P%d", e->p.id);
        e->next = NULL;

        eventList = pushEvent(eventList, e);

    }


    while (eventList)
    {
        Event* ev = NULL;
        eventList = popEvent(eventList, &ev);

        double currentTime = ev->time;


        if (ev->type == ARRIVAL )
        { 
         
            sprintf(buffer, "T=%.2f: %s arrives", currentTime, ev->p.name);
            logEvent(logCap, log, &logIndex, buffer); 

            int assigned = 0;

            for (int i = 0; i < config->checkinDesks; i++)
            {
                if (!checkin[i].busy)
                {
                    checkin[i].busy = 1;
                    double service = 2.0 + (rand() % 500) / 100.0;
                    (passengers+(ev->p.id-1))->service_checkin = service;
                    desks[i].totalTime +=service;
                    desks[i].numPass++;
                    Event* done = malloc(sizeof(Event));
                    done->type = CHECKIN_DONE;
                    done->time = currentTime + service;
                    (passengers+(ev->p.id-1))->checkinTime = done->time;
                    done->p = ev->p;
                    done->deskId = i;
                    eventList = pushEvent(eventList, done);

                    sprintf(buffer,
                        "T=%.2f: %s starts check-in at desk %d",
                        currentTime, ev->p.name, i);

                    logEvent(logCap, log, &logIndex, buffer);
                    assigned = 1;
                    break;
                }
            }
            
            if (!assigned)
            {
                checkinQ = enqueue(checkinQ, ev->p);
                int qSize = queueSize(checkinQ);
                sprintf(buffer,
                    "T=%.2f: %s enters check-in queue (len=%d)",
                    currentTime,
                    ev->p.name,
                    qSize);
                    if(maxQueue < qSize)
                    maxQueue = qSize;
                logEvent(logCap, log, &logIndex, buffer);
            }
        }


        else if (ev->type == CHECKIN_DONE)
        {
            checkin[ev->deskId].busy = 0;

            sprintf(buffer,
                "T=%.2f: %s finished check-in",
                currentTime,
                ev->p.name);
            logEvent(logCap, log, &logIndex, buffer);

            sprintf(buffer,
                "T=%.2f: Desk %d is now free",
                currentTime,
                ev->deskId);

            logEvent(logCap, log, &logIndex, buffer);
        

           if (checkinQ)
        {
            Passenger p;
            checkinQ = dequeue(checkinQ, &p);
            numQPass++;
            checkin[ev->deskId].busy = 1;
            double service = 2.0 + (rand() % 500) / 100.0;
            (passengers+(p.id-1))->service_checkin = service;
            Event* done = malloc(sizeof(Event));
            done->type = CHECKIN_DONE;
            done->time = currentTime + service;
            
            (passengers+(p.id-1))->queueTime = currentTime - (passengers+(p.id-1))->arrivalTime;
            (passengers+(p.id-1))->checkinTime = done->time;
           
            desks[ev->deskId].totalTime +=service;
            desks[ev->deskId].numPass++;
           
            done->p = p;
            done->deskId = ev->deskId;

            eventList = pushEvent(eventList, done);

            sprintf(buffer,
                "T=%.2f: %s starts check-in at desk %d (from queue)",
                currentTime, p.name, done->deskId);

            logEvent(logCap, log, &logIndex, buffer);
        }
    }
        
        totalTime = currentTime;
        free(ev);
    }

    logEvent(logCap, log, &logIndex, "=== SIMULATION FINISHED ===");

    
    fprintf(logFile, "\n--- EVENT LOG (from dynamic matrix) ---\n");
    for (int i = 0; i < logIndex; i++)
       fprintf(logFile, "%s\n", log[i]);


    for (int i = 0; i < logCap; i++)
        free(log[i]);
    free(log);
    free(checkin);
}

void printResults(SimulationConfig* config) 
{

  if(totalTime == 0) 
  {
    printf("The simulation was not started yet\n");
    return;
  }
  printf("_________________START_________________\n");
  printf("CONFIGURATION\n");
  printf("Number of passengers: %d\n", numPass);
  printf("Number of desks: %d\n", numDesks);
  printf("__________________________________\n");
  printf("Simulation time: %f minutes\n", totalTime);
  printf("__________________________________\n");
  printf("PASSENGER INFORMATION\n");
    
  double averageQueueTime = 0;
  double averageServTime = 0;
    
  for (int i = 0; i < numPass; i++) 
  {
    averageQueueTime+=(passengers+i)->queueTime;
    averageServTime+=(passengers+i)->service_checkin;
  }
  
  averageQueueTime = averageQueueTime /(double)numPass;
  averageServTime = averageServTime / (double)numPass;
     
  printf("Average queue time: %f\n", averageQueueTime);
  printf("Average service time: %f\n", averageServTime);
  printf("__________________________________\n");
  printf("INFORMATION OF EACH PASSENGER\n");
   
  for (int i = 0; i < numPass; i++)
  {
    printf("Passenger %d:\n", i+1);
    printf("Arrival time: %f\n", (passengers + i)->arrivalTime);
    if((passengers + i)->queueTime == 0)    printf("Passenger did not stand in queue\n");
    else  printf("Queue time: %f\n", (passengers + i)->queueTime);
    printf("Service time: %f\n", (passengers + i)->service_checkin);
    printf("Check in time: %f\n", (passengers + i)->checkinTime);
 }
  printf("__________________________________\n");
  printf("QUEUE INFORMATION\n");
  printf("Number of passengers standed in queue: %d\n", numQPass);
  printf("Maximum length of queue: %d\n", maxQueue);
  printf("__________________________________\n");
  printf("INFORMATION OF EACH DESK\n");
  
  for (int i = 0; i < numDesks; i++)
  {
    printf("Desk %d:\n", i);
    printf("Total busy time: %f\n", (desks+i)->totalTime);
    printf("Number of passengers served: %d\n", (desks+i)->numPass);
    printf("Average served time: %f\n", (desks+i)->totalTime / (double)(desks+i)->numPass);
  }
   printf("_________________END_________________\n");
}


void FreeResults() 
{
    free(desks);
    free(passengers);
}