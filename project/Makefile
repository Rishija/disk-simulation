SHELL = /bin/bash
OBJ = main.o disk.o files.o directory.o
RM = rm -f
all : $(OBJ) disk.h
	g++ -std=gnu++11 -o disk ${OBJ}

%.o : %.cpp disk.h
	g++ -std=gnu++11 -c -o $@ $<

clean :
	RM *.o *.bin disk
