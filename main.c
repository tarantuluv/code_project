#include <stdio.h>
#include <stdlib.h>
#include "simulation.h"
#include "fileio.h"
int main()
{
    int choice;
    SimulationConfig config;
    loadConfig("config.txt", &config);
    while (1)
    {
        printf("\n===== AIRPORT SIMULATION MENU =====\n");
        printf("1 - Run simulation\n");
        printf("2 - Load all previous simulations\n");
        printf("3 - See simulation result\n");
        printf("4 - Exit\n");
        printf("Choose: ");

        scanf("%d", &choice);
        if (choice == 1)
        {
           FILE* logFile = fopen("simulation_log.txt", "a"); 
            if (!logFile)
            {
                printf("Cannot open log file!\n");
                continue;
            }

            fprintf(logFile, "\n\n===== NEW SIMULATION RUN =====\n\n");

            runSimulation(&config, logFile);

            fclose(logFile);

            printf("\nSimulation finished!\n");
        }
        else if (choice == 2)
        {
            readLogFile("simulation_log.txt");
        }
        else if (choice ==3) {
            printResults(&config);
        }
        else if (choice == 4)
        {
            printf("Exiting...\n");
            
            FILE* f = fopen("simulation_log.txt", "w");
            if(f) fclose(f);
            FreeResults();
            break;
        }
        else
        {
            printf("Invalid choice!\n");
        }
    }

    return 0;
}