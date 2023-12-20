
# Options
SIMULATOR_OPT = -s
NUM_PARTICLES_OPT = -n
DRAW_REGION_OPT = -r

# Command line arguments
NUM_MPI_PROCESS = 4
NAIVE = $(SIMULATOR_OPT) 0
BARNES_HUT = $(SIMULATOR_OPT) 1
NUM_PARTICLES = $(NUM_PARTICLES_OPT) 1500

build:
	mpicxx -o main main.cpp */*/*.cpp -I . -lsfml-graphics -lsfml-window -lsfml-system

run:
	mpiexec -n $(NUM_MPI_PROCESS) ./main $(BARNES_HUT) $(NUM_PARTICLES) $(DRAW_REGION_OPT)
