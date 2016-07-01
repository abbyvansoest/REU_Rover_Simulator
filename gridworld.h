#ifndef GRIDWORLD_INCLUDED
#define GRIDWORLD_INCLUDED

#include <unordered_map>
#include <string>
#include "home.h"
#include "position.h"
#include "agent.h"
#include "poi.h"
#include <fann.h>
#include <fann_cpp.h>

class Gridworld 
{

private:
	int numAgent;
	int numPOI;
	int width;
	int height;
	unordered_map<string, Agent> agents;
	unordered_map<string, Poi> poi;
	Home home;
	FANN::neural_net nn;

	void initPOI();
	void initAgents();
	bool positionAvailable(Position);

public:

	// constructor
	Gridworld(int, int, int, int, FANN::neural_net);

	// step all agents in the world
	//  potential for threading
	void stepAgents(); 

	// at the end of a simulation, get reward for this grid
	double getGridReward();

};

#endif

