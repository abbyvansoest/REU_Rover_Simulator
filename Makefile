CC=g++
CFLAGS=--std=c++11 -lfann
EXE=grid_sim.out

.PHONY: debug debug-flags default all

default: all

debug-flags: 
	echo $(CFLAGS)
	CFLAGS+="-D DEBUG -Wall"

debug: | debug-flags test

all: agent.o poi.o gridworld.o simulation.o Home.o position.o 
	$(CC) $(CFLAGS) agent.o poi.o gridworld.o simulation.o Home.o position.o -o $(EXE)

agent.o: agent.cpp agent.h state.h
	$(CC) $(CFLAGS) -c agent.cpp -o agent.o

poi.o: poi.cpp poi.o
	$(CC) $(CFLAGS) -c poi.cpp -o poi.o

gridworld.o: gridworld.cpp gridworld.h state.h
	$(CC) $(CFLAGS) -c gridworld.cpp -o gridworld.o

#simulation.o: simulation.cpp simulation.h
	#$(CC) $(CFLAGS) -c simulation.cpp -o simulation.o

Home.o: Home.cpp Home.h
	$(CC) $(CFLAGS) -c Home.cpp -o Home.o

position.o: position.cpp position.h
	$(CC) $(CFLAGS) -c position.cpp -o position.o

clean:
	rm -f *.o $(EXE)
