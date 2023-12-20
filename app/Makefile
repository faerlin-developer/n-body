
# Build variables
MAIN_EXE = main
MAIN_CPP = main.cpp
CPP = */*/*.cpp
INCLUDE = -I .
SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Command line options for MAIN_EXE
SIMULATOR_OPT = -s
NUM_PARTICLES_OPT = -n
DRAW_REGION_OPT = -r

# Command line arguments for MAIN_EXE
NUM_MPI_PROCESS = 4
NAIVE = $(SIMULATOR_OPT) 0
BARNES_HUT = $(SIMULATOR_OPT) 1
NUM_PARTICLES = $(NUM_PARTICLES_OPT) 1500

build:
	mpicxx -o $(MAIN_EXE) $(MAIN_CPP) $(CPP) $(INCLUDE) $(SFML_FLAGS)

run:
	mpiexec -n $(NUM_MPI_PROCESS) ./$(MAIN_EXE) $(BARNES_HUT) $(NUM_PARTICLES) $(DRAW_REGION_OPT)

clean:
	rm -rf main
