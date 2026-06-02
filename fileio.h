#ifndef FILEIO_H
#define FILEIO_H

#include "simulation.h"

void loadConfig(const char* filename, SimulationConfig* config);
void readLogFile(const char* filename);

#endif