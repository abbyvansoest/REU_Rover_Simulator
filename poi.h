#ifndef _POI_H
#define _POI_H

#include <iostream>
#include "position.h"
#include <vector>
#include "agent.h"

/* Implements a simple POI object with a position, a weight,
 * and the ability to track completed status. */
class POI {

	private:

		bool complete;
		int weight;
		std::vector<Agent> agentsReady;
		Position p;

	public:

		/* Default constructor calls on the default
		 * constructor for the position object as well */
		POI();

		/* Non-default constructor calls on the 
		* position non-default constructor */
		POI(int weight, int x, int y);

		/* Returns the weight of the POI */
		int getWeight();

		/* Triggers the POI to become completed */
		void completed();

		/* Returns the bool to check if the POI is completed */
		bool isComplete();

		/* returns the array of carriers for the POI */
		std::vector<Agent> getCarriers();

		/* Adds an agent to the list of available agents to pick up the poi if it is not full */
		int addAvailableAgent(Agent);

		Position getP();
};

#endif
