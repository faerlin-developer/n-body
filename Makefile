
build:
	rm -rf bin && mkdir bin
	mpicxx -o bin/main-serial main-serial.cpp -lsfml-graphics -lsfml-window -lsfml-system
	mpicxx -o bin/main-parallel main-parallel.cpp -lsfml-graphics -lsfml-window -lsfml-system

run-serial:
	mpiexec -n 1 bin/main-serial

run-parallel:
	mpiexec -n 14 bin/main-parallel