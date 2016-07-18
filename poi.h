/*********************************************************************
* poi.h
*
* This class is an object representation of the Points of Interest (POI)
* in the experimental gridworld. The simple object has a position, a 
* weight and the ability to track completion and removal from the world. 
* The POI is also able to track the agents which are available to pick it
* up and transport it home.
*
* Copyright (C) 2016 Abby Van Soest, Connor Yates
*********************************************************************/

#ifndef _POI_H
#define _POI_H

class Agent;

#include <iostream>
#include "position.h"
#include <vector>
#include "agent.h"

/* Implements a simple POI object with a position, a weight,
 * and the ability to track completed status. */
class POI {

	private:

		Position p;      //  location of the poi in the world
		int weight;      //  number of agents needed to carry poi
		std::vector<Agent*> agentsReady;   //  pointers to the agents available to carry the poi
		bool complete;   // true if the poi has enough agents to be picked up
		bool removed;    // true if the agent has been picked up

	public:

		/* Default constructor calls on the default
		 * constructor for the position object as well */
		POI();

		/* Non-default constructor calls on the 
		* position non-default constructor */
		POI(int weight, int x, int y);

		/* Returns the weight of the POI */
		int getWeight();

		/* Adds an agent to the list of available agents to pick up the poi if it is not full */
		void addAvailableAgent(Agent*);
		/* returns the array of carriers for the POI */
		std::vector<Agent*> getCarriers();

		/* Returns the bool to check if the POI is completed */
		bool isComplete();
		/* Triggers the POI to become completed */
		void completed();

		//  has the agent been completely picked up?
		bool isRemoved();
		//  remove the agent if it has not already been removed
		void remove();

		//  return the location of the poi in the world
		Position getP();

		// Clears the ready agents vector
		void clearReadyAgents();
};

#endif
