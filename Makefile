CC=g++
CFLAGS=--std=c++11
EXE=grid_sim.out

.PHONY: debug debug-flags default all

default: all

debug-flags: 
	echo $(CFLAGS)
	CFLAGS+="-D DEBUG -Wall"

debug: | debug-flags test

all: agent.o poi.o gridworld.o simulation.o homebase.o
	$(CC) $(CFLAGS) agent.o poi.o gridworld.o simulation.o homebase.o -o $(EXE)

agent.o: agent.cpp agent.h
	$(CC) $(CFLAGS) -c agent.cpp -o agent.o

gridworld.o: gridworld.cpp gridworld.h
	$(CC) $(CFLAGS) -c gridworld.cpp -o gridworld.o

simulation.o: simulation.cpp simulation.h
	$(CC) $(CFLAGS) -c simulation.cpp -o simulation.o

homebase.o: homebase.cpp homebase.h
	$(CC) $(CFLAGS) -c homebase.cpp -o homebase.o

clean:
	rm -f *.o $(EXE)
