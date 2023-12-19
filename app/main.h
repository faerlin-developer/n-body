#pragma once

#include "simulator/simulator.h"
#include "graphics/timer.h"

void runRoot(Simulator *simulator, Particle *particles, int start, int end, int N,
             int *close_flag, int region_flag, MPI_Comm comm);

void run(Simulator *simulator, Particle *particles, int start, int end, int N, int *close_flag, MPI_Comm comm);

void printInfo(Timer &timer);

void toggle(int *flag);