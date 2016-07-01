#ifndef AGENT_INCLUDED
#define AGENT_INCLUDED

#include <deque>
#include <iostream>
#include "position.h"

/* This struct is a nice wrapper around the state, allowing it to be treated as a singular object
 * (with additional bracketing being overloaded, if it is desired)
 * and encapsulates the int array away.
 * The internal array can be directly accessed if needed */
typedef struct State{
	int& operator[](int i) {return array[i]; }
	int array[13];
} State;

class Agent {
	private:
		Position position;
		std::deque<State> stateTrajectory;
		int calculateAction(State);

	public:	
		Agent(Position, State);
		int nextAction(State);
};

#endif
