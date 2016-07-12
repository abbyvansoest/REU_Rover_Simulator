#ifndef AGENT_INCLUDED
#define AGENT_INCLUDED

class POI;

#include <deque>
#include <iostream>
#include <cstdlib>
#include <fann.h>
#include <fann_cpp.h>
#include "position.h"
#include "poi.h"
#include "state.h"
#include "Home.h"

/* Definition of action behaviors relative numbers */
enum
{
	MOVE_UP,
	MOVE_LEFT,
	MOVE_DOWN,
	MOVE_RIGHT,
	PICKUP,
	BROADCAST,
	SET_DOWN
};

class Agent {

	private:
		bool broadcasting;
		bool carrying;
		Position p;
		POI* holding;

	public:	
		//  constructors
		Agent();
		Agent(bool, bool, POI*);

		//  get the next action for hte agent based on the
		//  current state and the neural net
		int nextAction(State, FANN::neural_net, Position, Home, double);

		//  is the broadcasting signal positive or negative?
		bool isBroadcasting();

		//  set broadcasting signal
		void setBroadcast(bool);

		//  is the agent carrying anything?
		bool isCarrying();

		//  set the POI the agent is holding
		void setHoldingPOI(POI*);
		//  return the POI the agent is holding
		POI* getHoldingPOI();

		//  set the carrying signal appropriately
		void setCarrying(bool);
		Agent copy();

		Position getP();
		void setP(Position);
};

#endif
