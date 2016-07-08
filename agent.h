#ifndef AGENT_INCLUDED
#define AGENT_INCLUDED

#include <deque>
#include <iostream>
#include <cstdlib>
#include <fann.h>
#include <fann_cpp.h>
#include "position.h"
#include "state.h"

/* Definition of action behaviors relative numbers */
enum
{
	MOVE_UP,
	MOVE_LEFT,
	MOVE_DOWN,
	MOVE_RIGHT,
	PICKUP,
	BROADCAST
};

class Agent {

	private:
		bool broadcasting;
		bool carrying;
		Position p;

	public:	
		//  constructors
		Agent();
		Agent(bool, bool);

		//  get the next action for hte agent based on the
		//  current state and the neural net
		int nextAction(State, FANN::neural_net, Position, Position, double);

		//  is the broadcasting signal positive or negative?
		bool isBroadcasting();

		//  set broadcasting signal
		void setBroadcast(bool);

		//  is the agent carrying anything?
		bool isCarrying();

		//  set the carrying signal appropriately
		void setCarrying(bool);
		Agent copy();

		Position getP();
		void setP(Position);
};

#endif
