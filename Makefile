all:	tri
tri:		tri.o
	g++ -o tri tri.o -g -lGL -lglut -lGLEW
tri.o:		tri.cpp
	g++ -g -c tri.cpp
clean:
	rm tri tri.o
