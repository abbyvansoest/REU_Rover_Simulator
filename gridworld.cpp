/////   NEED TO: make position to string / from string operations
/////            figure out how to initialize states 
//////     cahnge constructor for agent and aff setState();

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

// constructor
Gridworld::Gridworld(int numAgents, int numPOI, int width, int height, FANN::neural_net net) {
		
	this.numAgents = numAgents;
	this.numPOI = numPOI;
	this.width = width;
	this.height = height;

	initAgents();
	initPOI();

	Position p = new Position(HOME_X, HOME_Y);
	this.home = new Home(p);

	this.nn = net;

}

//  randomly initalize agent positions in the grid
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
		}
		
		// add an agent to the open position
		str = pos.toString();
		Agent addAgent = new Agent(p);
		this.agents.insert(str, addAgent);
		delete pos;
		//  DELETING AGENTS????
	}

	//  set state of all agents
	Agent* ag;
	for (auto it = agents.begin(); it != agents.end(); ++it) {
		ag = &it->second;
		ag.setState(getState(agent));
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
		}

		// add a POI to the open position
		str = pos.toString();
		POI addPOI = new POI(p);
		this.poi.insert(str, addPOI);

		delete pos;
	}

}

//  check if the position is occupied by an agent or a POI
private bool Gridworld::positionAvailable(Position p) {

	string posString = p.toString();
	if (this.agents.find(posString) != agents.end() 
		|| this.poi.find(posString) != poi.end()) return false;
	return true;

}

//  return the 13-dim state representation for ag
private State Gridworld::getState(Agent ag) {

	//  RETURN STATE OF THE AGENT SOMEHOW LOL

}

// step all agents in the world
//  potential for threading
void Gridworld::stepAgents() {

	Position pos;
	Agent agent;
	State state;

	//  iterate through and step all agents
	//  CHECK POSITION OF INCREMENT
	for (auto it = agents.begin(); it != agents.end(); ++it) {
		pos = it->first;
		agent = it->second;
		state = getState(agent);
		int action = agent.nextAction(state, this.nn);
		// CHOOSE WHAT TO DO BASED ON ACTION VALUE

		// //  if action is to move
		// if (action == MOVE_RIGHT || action == MOVE_DOWN 
		// 	|| action == MOVE_LEFT || action == MOVE_UP) {

		// }

	}

}

// at the end of a simulation, get reward for this grid
double Gridworld::getGridReward() {


}














