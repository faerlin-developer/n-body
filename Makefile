
build:
	mpicxx -o main main.cpp -I . -lsfml-graphics -lsfml-window -lsfml-system

run:
	mpiexec -n 4 ./main -s 1 -r
