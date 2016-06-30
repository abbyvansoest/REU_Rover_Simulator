CC=g++
CFLAGS=--std=c++11
EXE=grid_sim.out

.PHONY: debug debug-flags default all

default: all

debug-flags: 
	echo $(CFLAGS)
	CFLAGS+="-D DEBUG -Wall"

debug: | debug-flags test

all: agent.o poi.o gridworld.o simulator.o homebase.o
	$(CC) $(CFLAGS) agent.o poi.o gridworld.o simulator.o homebase.o -o $(EXE)

agent.o: agent.cpp agent.h
	$(CC) $(CFLAGS) -c agent.cpp -o agent.o

gridworld.o: gridworld.cpp gridworld.h
	$(CC) $(CFLAGS) -c gridworld.cpp -o gridworld.o

simulator.o: simulator.cpp simulator.h
	$(CC) $(CFLAGS) -c simulator.cpp -o simulator.o

homebase.o: homebase.cpp homebase.h
	$(CC) $(CFLAGS) -c homebase.cpp -o homebase.o

clean:
	rm -f *.o $(EXE)
