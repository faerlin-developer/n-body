
build:
	rm -rf bin && mkdir bin
	g++ -o bin/main-serial main-serial.cpp -lsfml-graphics -lsfml-window -lsfml-system
	mpicxx -o bin/main-parallel main-parallel.cpp -lsfml-graphics -lsfml-window -lsfml-system
	g++ -o bin/main-quad main-quad.cpp -lsfml-graphics -lsfml-window -lsfml-system
	g++ -o bin/main-quad-2 main-quad-2.cpp -lsfml-graphics -lsfml-window -lsfml-system
	g++ -o bin/main-serial-quad main-serial-quad.cpp -lsfml-graphics -lsfml-window -lsfml-system
	g++ -o bin/main-serial-barnes-hut main-serial-barnes-hut.cpp -lsfml-graphics -lsfml-window -lsfml-system

run-serial:
	bin/main-serial

run-serial-quad:
	bin/main-serial-quad

run-serial-barnes-hut:
	bin/main-serial-barnes-hut

run-quad-2:
	bin/main-quad-2

run-parallel:
	mpiexec -n 14 bin/main-parallel

run-quad:
	bin/main-quad