
build:
	rm -rf bin && mkdir bin
	mpicxx -o main main.cpp -I . -lsfml-graphics -lsfml-window -lsfml-system

run:
	mpiexec -n 4 ./main
