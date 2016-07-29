CC=g++
CFLAGS=--std=c++11
EXE=pickup_trainer.out

.PHONY: debug debug-flags default all clean

default: all

debug-flags: 
	$(eval CFLAGS += -D DEBUG -Wall -g)

debug: | debug-flags all

all: train_pickup_net.o
	$(CC) $(CFLAGS) -lfann train_pickup_net.o -o $(EXE)

train_pickup_net.o: train_pickup_net.cpp
	$(CC) $(CFLAGS) -c train_pickup_net.cpp -o train_pickup_net.o

clean:
	rm -f *.o $(EXE)
