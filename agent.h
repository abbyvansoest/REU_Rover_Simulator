#ifndef AGENT_INCLUDED
#define AGENT_INCLUDED

#include <deque>
#include <iostream>
#include <fann.h>
#include <fann_cpp.h>
#include "position.h"
#include <array>

/* This struct is a nice wrapper around the state, allowing it to be treated as a singular object
 * (with additional bracketing being overloaded, if it is desired)
 * and encapsulates the int array away.
 * The internal array can be directly accessed if needed */
typedef struct State{
	float& operator[](int i) { return array[i]; }
	float array[13];
} State;

class Agent {
	private:
		Position position; //Currently unused
		std::deque<State> stateTrajectory; // Do we want to keep this?

	public:	
		Agent(Position, State);
		float nextAction(State, FANN::neural_net);
};

#endif
