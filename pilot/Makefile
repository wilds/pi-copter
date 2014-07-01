#exec file
EXEC = picopter

#include dirs
INC= -I. -IRTIMULib -Icore -Imotor -Iinput

#source : find recursive .cpp
SRC=$(shell find . -name '*.cpp')
#objects
OBJ=$(SRC:.cpp=.o)

#-----------------------------------------------------------
#compilation :
CC = g++
LIBS= -std=c++0x -lm -lrt -lpthread #-lncurses

#option debug:
OPT_Debug= -Wall -g -O2

#CXXFLAGS = -DDMP_FIFO_RATE=1

ALL:$(EXEC)

$(EXEC) : $(OBJ)
	$(CC) $(CXXFLAGS) $(INC) $(OPT_Debug) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CC) $(CXXFLAGS) $(INC) $(OPT_Debug) -o $@ -c $^ $(LIBS)

clean:
	#find . -name "*.o" -exec rm '{}' ';
	rm -rf *.o *~ *.mod
	rm -rf $(EXEC)
