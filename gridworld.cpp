#include <iostream>
#include <cstdlib>
#include <string>
#include <fann.h>
#include <fann_cpp.h>
#include "time.h"
#include "gridworld.h"
#include "home.h"
#include "position.h"
#include "agent.h"
#include "poi.h"

using namespace std;

//  define constants for homebase location
int HOME_X = 0;
int HOME_Y = 0;

//  definition of action behaviors
int MOVE_UP = 0;
int MOVE_LEFT = 1;
int MOVE_DOWN = 2;
int MOVE_RIGHT = 3;
int PICKUP = 4;
int BROADCAST = 5;

// define the indices for the state array
int AGENTS_1 = 0;
int POI_1 = 1;
int BROADCASTING_1 = 2;
int AGENTS_2 = 3;
int POI_2 = 4;
int BROADCASTING_2 = 5;
int AGENTS_3 = 6;
int POI_3 = 7;
int BROADCASTING_3 = 8;
int AGENTS_4 = 9;
int POI_4 = 10;
int BROADCASTING_4 = 11;
int CARRYING = 12;

// constructor
Gridworld::Gridworld(int numAgents, int numPOI, int width, int height, 
	bool randHome, FANN::neural_net net) {
		
	this.numAgents = numAgents;
	this.numPOI = numPOI;
	this.width = width;
	this.height = height;

	initAgents();
	initPOI();
	initHome(randHome);

	this.nn = net;

}

//  randomly initalize agents in the grid
private void Gridworld::initAgents() {

	int x, y;
	string str;

	// random seed
	srand(time(NULL));

	for (int i = 0; i < this.numAgents; i++) {

		//  find open position on the board
		Position pos;
		bool open = false;
		while (!open) {
			x = rand() % width;
			y = rand() % height;
			pos = new Position(x, y);
			open = positionAvailable(pos);
			if (!open) delete pos;
		}
		
		// add an agent to the open position
		str = pos.toString();
		Agent addAgent = new Agent();
		this.agents.insert(str, addAgent);

		delete pos;
	}

	//  set state of all agents
	Position* pos;
	Agent* ag;
	for (auto it = agents.begin(); it != agents.end(); ++it) {
		pos = &it->first;
		ag = &it->second;
		(*ag).setState(getState(*pos, *ag));
	}

}

//  randomly initialize POI positions in the grid
private void Gridworld::initPOI() {

	int x, y;
	string str;

	// random seed
	srand(time(NULL));

	for (int i = 0; i < this.numPOI; i++) {
		//  find open position on the board
		Position pos;
		bool open = false;
		while (!open) {
			x = rand() % width;
			y = rand() % height;
			pos = new Position(x, y);
			open = positionAvailable(pos);
			if (!open) delete pos;
		}

		// add a POI to the open position
		str = pos.toString();
		POI addPOI = new POI();
		this.poi.insert(str, addPOI);

		delete pos;
	}

}

//  initialize home base to random location
//  or pre-set global value
private void initHome(bool rand) {

	if (!rand) {
		Position p = new Position(HOME_X, HOME_Y);
		this.home = new Home(p);
		delete p;
	}

	else {
		// random seed
		srand(time(NULL));
		x = rand() % width;
		y = rand() % height;
		Position p = new Position(x, y);
		this.home = new Home(p);
		delete p;
	}
}

//  check if the position is occupied by an agent, a POI, or the homebase
private bool Gridworld::positionAvailable(Position p) {

	bool available;

	string posString = p.toString();
	if (this.agents.find(posString) != agents.end() 
		|| this.poi.find(posString) != poi.end()) return false;
	if (this.home.getPosition() == p) return false;
	return true;

}

//  return the 13-dim state representation for ag
private State Gridworld::getState(Position pos, Agent ag) {

	// go through all agents and POI
	// increment variables to account for counts
	// based on relative quadrant location
	int agentCountA, agentCountB, agentCountC, agentCountD;
	int poiCountA, poiCountB, poiCountC, poiCountD;
	int broadcastCountA, broadcastCountB, broadcastCountC, broadcastCountD;

	Agent* compAgent;

	// get count of agents and count of broadcasting agents in each quadrant
	for (auto it = agents.begin(); it != agents.end(); ++it) {  

		//  values for the comparing agent
		compAgent = it->second;
		Position p = new Position(it->first);

		if (p.getX < pos.getX && p.getY >= pos.getY) agentCountA++;
		if (p.getX >= pos.getX && p.getY > pos.getY) agentCountB++;
		if (p.getX <= pos.getX && p.getY < pos.getY) agentCountC++;
		if (p.getX > pos.getX && p.getY <= pos.getY) agentCountD++;

		//  determine the number of broadcasting agents
		if ((*compAgent).isBroadcasting()) broadcastCountA++;
		if ((*compAgent).isBroadcasting()) broadcastCountB++;
		if ((*compAgent).isBroadcasting()) broadcastCountC++;
		if ((*compAgent).isBroadcasting()) broadcastCountD++;

		delete p;
	}

	//  get count of POI in each quadrant
	for (auto it = poi.begin(); it != poi.end(); ++it) {  			

		Position p = new Position(it->first);

		if (p.getX < pos.getX && p.getY >= pos.getY) poiCountA++;
		if (p.getX >= pos.getX && p.getY > pos.getY) poiCountB++;
		if (p.getX <= pos.getX && p.getY < pos.getY) poiCountC++;
		if (p.getX > pos.getX && p.getY <= pos.getY) poiCountD++;

		delete p;
		
	}

	State state;

	//  information on quadrant 1 
	state[AGENTS_A] = agentCountA;
	state[POI_A] = poiCountA;
	state[BROADCASTING_A] = broadcastCountA;
	//  information on quadrant 2
	state[AGENTS_B] = agentCountB;
	state[POI_B] = poiCountB;
	state[BROADCASTING_B] = broadcastCountB;
	//  information on quadrant 3
	state[AGENTS_C] = agentCountC;
	state[POI_C] = poiCountC;
	state[BROADCASTING_C] = broadcastCountC;
	//  information on quadrant 4
	state[AGENTS_D] = agentCountD;
	state[POI_D] = poiCountD;
	state[BROADCASTING_D] = broadcastCountD;
	// agent carrying information
	state[CARRYING] = (int)ag.isCarrying();

	return state;
}

// step all agents in the world
//  potential for threading
void Gridworld::stepAgents() {

	Agent* agent;
	State state;
	Position oldPos;
	Position nextPos;

	//  iterate through and step all agents
	//  CHECK POSITION OF INCREMENT
	for (auto it = agents.begin(); it != agents.end(); ++it) {

		oldPos = new Position(it->first);
		agent = &it->second;
		state = getState(oldPos, *agent);
		int action = (*agent).nextAction(state, this.nn);

		// execute action described by the action value

		if (action == BROADCAST) {
			(*agent).setBroadcast(true);
			return;
		}

		if (action == PICKUP) {
			(*agent).setCarrying(true);
			return;
		}

		if (action == MOVE_RIGHT) {
			nextPos = new Position(oldPos.getX() - 1, oldPos.getY());
		}
		if (action == MOVE_DOWN) {
			nextPos = new Position(oldPos.getX(), oldPos.getY() - 1);
		}
		if (action == MOVE_LEFT) {
			nextPos = new Position(oldPos.getX() + 1, oldPos.getY());
		}
		if (action == MOVE_UP) {
			nextPos = new Position(oldPos.getX(), oldPos.getY() + 1);
		}

		//  remove agent, construct new agent and insert to new position
		string str = nextPos.toString();
		Agent newAgent = (*agent).copy();
		agents.erase(it);
		agents.insert(str, newAgent);

		delete oldPos;

		return;
	}
}

// at the end of a simulation, get reward for this grid
double Gridworld::getGridReward() {


}

//  entirely clear the gridworld of agents and POIs
void Gridworld::clear() {

	//  while agents and POI lists are not empty, scan through and 
	//  delete their members
	for (auto it = agents.begin(); it != agents.end(); ++it) {
		delete it->second;
	}
	for (auto it = poi.begin(); it != poi.end(); ++it) {
		delete it->second;
	}
	delete this.home;
}

//  reset the world with the given neural net, 
//  or keep using the same neural net if NULL
void Gridworld::reset(bool random, FANN::neural_net net) {

	//  clear gridworld
	Gridworld::clear();

	//  reset POI and agents
	initAgents();
	initPOI();
	initHome(random);

	//  use a new neural net
	if (net != NULL) this.nn = net;
}




