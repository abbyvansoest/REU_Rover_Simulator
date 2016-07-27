/*********************************************************************
* gridworld.h
*
* This class is an object representation of a gridworld. The world 
* remembers all agents, poi, and the home base. It can step all agents
* and mark a world as "complete" when all POIs have been fully returned.
*
* Copyright (C) 2016 Abby Van Soest, Connor Yates
*********************************************************************/

#ifndef GRIDWORLD_INCLUDED
#define GRIDWORLD_INCLUDED

/* Gridworld implementation necessary libraries */
#include <unordered_map>
#include <string>
/* Other classes */
#include <cmath>
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
	
	int numAgents;   // number of agents in the world
	int numPOI;      // number of poi in the world
	int width;       //  width dimension
	int height;      //  height dimension
	int poiWeight;   //  weight of all POIs
	std::vector<Agent> agents;  //  vector to store all agents
	std::vector<POI> poi;;      //  vector to store all POI
	Home home;                  //  world's home base
	int numSteps;               //  number of steps taken in the world

	//  initialize POI, agents, and home base
	void initPOI();
	void initAgents();
	void initHome();
	//  check if a location in the grid is occupied
	bool positionAvailable(Position);
	//  return the state of the agent in the given position
	State getState(Position, Agent);
	//  find the distance between two (x, y) locations
	double getDistance(Position, Position);
	// is there a poi within one step?
	bool findNearbyPOI(Position);
	//  return a pointer to the poi within one step
	POI* nearbyPOI(Position);
	// clear the gridworld of all agents and POI
	void clear();

public:

	// constructor
	Gridworld();
	Gridworld(int, int, int, int, int);

	// step all agents in the world
	void stepAgents(FANN::neural_net*, double&); 

	//  reset the world 
	void reset();
	
	// check if all the POI's have been returned to the homebase
	bool worldComplete();

	//  passthrough accessor to return the current amount at home
	int currentAmount();

	// visualizor of the current gridworld
	void printWorld();

	//  the total number of steps taken by the world thus far
	int stepsTaken();

	// Resets the POI's ready agents vectors, so agents can't build up the ready
	// list by waiting around
	void clearPOI();

};

#endif








