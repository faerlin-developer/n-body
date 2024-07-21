
# Build variables
MAIN_EXE = main
MAIN_CPP = main.cpp
CPP = */*/*.cpp
INCLUDE = -I .
SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Command line options for MAIN_EXE
SIMULATOR_OPT = -s
SIZE_OPT = -n
DRAW_REGION_OPT = -r

# Default arguments for target run
duration = 0s
cores = 2
simulator = BarnesHut
size = 1000

build:
	mpicxx -o $(MAIN_EXE) $(MAIN_CPP) $(CPP) $(INCLUDE) $(SFML_FLAGS)

run:
ifdef out
	timeout $(duration) mpiexec -n $(cores) ./$(MAIN_EXE) $(SIMULATOR_OPT) $(simulator) $(SIZE_OPT) $(size) >> $(simulator)-$(duration)-$(size)-$(cores).data
else
	mpiexec -n $(cores) ./$(MAIN_EXE) $(SIMULATOR_OPT) $(simulator) $(SIZE_OPT) $(size) $(DRAW_REGION_OPT)
endif

clean:
	rm -rf main
