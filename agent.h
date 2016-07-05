#ifndef AGENT_INCLUDED
#define AGENT_INCLUDED

#include <deque>
#include <iostream>
#include <fann.h>
#include <fann_cpp.h>
#include "position.h"

/* This struct is a nice wrapper around the state, allowing it to be treated as a singular object
 * (with additional bracketing being overloaded, if it is desired)
 * and encapsulates the int array away.
 * The internal array can be directly accessed if needed */

typedef struct State {

	float& operator[](int i) { return array[i]; }
	float array[13];

} State;


class Agent {

	private:
	
		bool broadcasting;
		bool carrying;
		std::deque<State> stateTrajectory;

	public:	

		//  constructors
		Agent();
		Agent::Agent(std::deque<State>, bool);
		Agent Agent::copy();

		//  push the newest state information onto the 
		//  trajectory deque
		void Agent::setState(State);

		//  get the next action for hte agent based on the
		//  current state and the neural net
		int Agent::nextAction(State, FANN::neural_net);

		//  is the broadcasting signal positive or negative?
		bool Agent::isBroadcasting();

		//  set broadcasting signal
		void Agent::setBroadcast(bool);

		//  is the agent carrying anything?
		bool Agent::isCarrying();

		//  set the carrying signal appropriately
		void Agent::setCarrying(bool);
};

#endif
