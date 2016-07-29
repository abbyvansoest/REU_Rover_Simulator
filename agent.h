/*********************************************************************
* agent.h
*
* This class is an object representation of the agents in an 
* experimental gridworld. The agents remember their position and whether 
* or not they are carrying a POI.

* Copyright (C) 2016 Abby Van Soest, Connor Yates
*********************************************************************/

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

/* enumeration of action behaviors */
enum
{
	MOVE_UP,
	MOVE_LEFT,
	MOVE_DOWN,
	MOVE_RIGHT,
	PICKUP,
	SET_DOWN
};

class Agent {

	private:
		bool carrying;       // is the agent carrying a poi?
		Position p;          // current position in world
		POI* holding;        // points to the poi an agent is carrying (or NULL)

	public:	
		//  constructors
		Agent();
		Agent(bool, POI*);
		Agent copy();

		//  get the next action for hte agent based on the
		//  current state and the neural net
		int nextAction(State, FANN::neural_net*&, Position, Home);

		//  is the agent carrying anything?
		bool isCarrying();
		//  set the carrying signal appropriately
		void setCarrying(bool);

		//  return the POI the agent is holding
		POI* getHoldingPOI();
		//  set the POI the agent is holding
		void setHoldingPOI(POI*);

		Position getP();
		void setP(Position);

};

#endif
