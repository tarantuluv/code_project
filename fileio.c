#include <stdio.h>
#include "fileio.h"

void loadConfig(const char* filename, SimulationConfig* config)
{
    FILE* f = fopen(filename, "r");

   if (!f)
    {
        printf("Cannot load configuration!\n");
        return;
    }

    fscanf(f, "PASSENGERS=%d\n", &config->passengers);
    fscanf(f, "CHECKIN=%d\n", &config->checkinDesks);

    fclose(f);
}


void readLogFile(const char* filename)
{
    FILE* f = fopen(filename, "r");

    if (!f)
    {
        printf("Cannot open log file!\n");
        return;
    }

    char line[LENGTH];

    printf("\n===== SIMULATION LOG =====\n\n");

    while (fgets(line, sizeof(line), f))
    {
        printf("%s", line);
    }

    fclose(f);
}