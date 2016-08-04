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

#include "config.h"
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
	MOVE_UP,  //0
	MOVE_LEFT, //1
	MOVE_DOWN, //2
	MOVE_RIGHT, //3
	PICKUP,    //4
	SET_DOWN   //5
};

class Agent {

	private:
		bool carrying;       // is the agent carrying a poi?
		int numCarried;     //  has the agent ever carried a poi?
		Position p;          // current position in world
		POI* holding;        // points to the poi an agent is carrying (or NULL)

	public:	
		//  constructors
		Agent();
		Agent(bool, POI*, Position);
		//  copy constructor
		Agent(const Agent&);
		// copy assignment operator
		Agent& operator=(const Agent&);

		//  get the next action for the agent based on the
		//  current state and the neural net
		int nextAction(State, Position, Home, FANN::neural_net*);

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

		double getReward();

		void hasCarried();

};

#endif
