#ifndef GRIDWORLD_INCLUDED
#define GRIDWORLD_INCLUDED

/* Gridworld implementation necessary libraries */
#include <unordered_map>
#include <string>
/* Other classes */
#include "Home.h"
#include "position.h"
#include "agent.h"
#include "poi.h"
#include "state.h"
/* Rand */
#include <cstdlib>
#include "time.h"
/* Neural Net */
#include <fann.h>
#include <fann_cpp.h>

using std::unordered_map;
using std::string;

class Gridworld 
{

private:
	
	int numAgents;
	int numPOI;
	int width;
	int height;
	unordered_map<string, Agent*> agents;
	unordered_map<string, POI*> poi;
	Home home;
	int numSteps;

	void initPOI();
	void initAgents();
	void initHome(bool);
	bool positionAvailable(Position);
	State getState(Position, Agent);
	bool inDomain(Position);

public:

	// constructor
	Gridworld();
	Gridworld(int, int, int, int, bool);

	// step all agents in the world
	//  potential for threading?
	void stepAgents(FANN::neural_net); 

	// at the end of a simulation, get reward for this grid
	double getGridReward();

	// clear the gridworld of all agents and POI
	void clear();

	//  reset the world with the given neural net, 
	//  or keep using the same neural net if NULL
	void reset(bool, FANN::neural_net);
	
	/* Checks if all the POI's have been returned to the homebase */
	bool worldComplete();

	/* A passthrough accessor to return the home current amount */
	int currentAmount();

};

#endif








