#include "gridworld.h"

//  define constants for homebase location
//  TODO load from config file eventually
int HOME_X = 0;
int HOME_Y = 0;


// constructor
Gridworld::Gridworld(int numAgents, int numPOI, int width, int height, 
	bool randHome, FANN::neural_net net) {
		
	this->numAgents = numAgents;
	this->numPOI = numPOI;
	this->width = width;
	this->height = height;

	initAgents();
	initPOI();
	initHome(randHome);

	this->nn = net;
}

//  randomly initalize agents in the grid
void Gridworld::initAgents() {
	int x, y;
	string str;

	for (int i = 0; i < this->numAgents; i++) {
		//  find open position on the board
		Position pos;
		bool open = false;
		while (!open) {
			x = rand() % width;
			y = rand() % height;
			pos = Position(x, y);
			open = positionAvailable(pos);
		}
		
		// add an agent to the open position
		str = pos.toString();
		Agent* addAgent = new Agent();
		this->agents[str] = addAgent;
	}

	//  set state of all agents
	Position pos;
	Agent* ag;
	for (auto it = agents.begin(); it != agents.end(); ++it) {
		pos = Position(it->first);
		ag = it->second;
		ag->setState(getState(pos, *ag));
	}
}

//  randomly initialize POI positions in the grid
void Gridworld::initPOI() {
	int x, y;
	string str;

	for (int i = 0; i < this->numPOI; i++) {
		//  find open position on the board
		Position pos;
		bool open = false;
		while (!open) {
			x = rand() % width;
			y = rand() % height;
			pos = Position(x, y);
			open = positionAvailable(pos);
		}

		// add a POI to the open position
		str = pos.toString();
		POI *addPOI = new POI();
		this->poi[str] = addPOI;
	}
}

//  initialize home base to random location
//  or pre-set global value
void Gridworld::initHome(bool rand_flag) {
	if (!rand_flag) {
		Position p = Position(HOME_X, HOME_Y);
		this->home = Home(p);
	}

	else {
		int x, y;
		x = rand() % width;
		y = rand() % height;
		Position p(x, y);
		this->home = Home(p);
	}
}

//  check if the position is occupied by an agent, a POI, or the homebase
bool Gridworld::positionAvailable(Position p) {

	bool available;

	string posString = p.toString();
	if ((this->agents.find(posString) != agents.end()) ||
		(this->poi.find(posString) != poi.end()))
	{
		return false;
	}
	if (this->home.getPosition() == p)
	{
		return false;
	}
	return true;

}

//  return the 13-dim state representation for ag
State Gridworld::getState(Position pos, Agent ag) {
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
		Position p = Position(it->first);

		if (p.getX() < pos.getX() && p.getY() >= pos.getY()) {
			agentCountA++;
			if ((*compAgent).isBroadcasting()) { broadcastCountA++; }
		}
		if (p.getX() >= pos.getX() && p.getY() > pos.getY()) {
			agentCountB++;
			if ((*compAgent).isBroadcasting()) { broadcastCountB++; }
		}
		if (p.getX() <= pos.getX() && p.getY() < pos.getY()) {
			agentCountC++;
			if (compAgent->isBroadcasting()) { broadcastCountC++; }
		}
		if (p.getX() > pos.getX() && p.getY() <= pos.getY()) {
			agentCountD++;
			if (compAgent->isBroadcasting()) { broadcastCountD++; }
		}
	}

	//  get count of POI in each quadrant
	for (auto it = poi.begin(); it != poi.end(); ++it) {  			

		Position p = Position(it->first);

		if (p.getX() <  pos.getX() && p.getY() >= pos.getY()) { poiCountA++; }
		if (p.getX() >= pos.getX() && p.getY() >  pos.getY()) { poiCountB++; }
		if (p.getX() <= pos.getX() && p.getY() <  pos.getY()) { poiCountC++; }
		if (p.getX() >  pos.getX() && p.getY() <= pos.getY()) { poiCountD++; }
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

//  step all agents in the world
void Gridworld::stepAgents() {

	Agent* agent;
	State state;
	Position oldPos, nextPos;

	unordered_map<string, Agent*> newAgents;
	unordered_map<string, POI*> newPoi = this->poi;

	//  iterate through all agents
	for (auto it = agents.begin(); it != agents.end(); ++it) {

		oldPos = Position(it->first);
		agent = it->second;
		state = getState(oldPos, *agent);
		int action = agent->nextAction(state, this->nn);

		//  set next position for all cases
		if (action == MOVE_RIGHT) {
			nextPos = Position(oldPos.getX() - 1, oldPos.getY());
		}
		if (action == MOVE_DOWN) {
			nextPos = Position(oldPos.getX(), oldPos.getY() - 1);
		}
		if (action == MOVE_LEFT) {
			nextPos = Position(oldPos.getX() + 1, oldPos.getY());
		}
		if (action == MOVE_UP) {
			nextPos = Position(oldPos.getX(), oldPos.getY() + 1);
		}
		if (action == BROADCAST) {
			(*agent).setBroadcast(true);
			nextPos = oldPos;
		}
		else (*agent).setBroadcast(false);    
		if (action == PICKUP) {
			nextPos = oldPos;
		}
		
		//  check what the next position holds
		string posString = nextPos.toString();

		//  if it has an agent / if it is at the boundary, do not move
		if (this->newAgents.find(posString) != newAgents.end() || !inDomain(nextPos)) {
			nextPos = oldPos;
		}

		//  if it has a POI, mark the POI as having another potential carrier
		//  agent remains in original location
		if ((POI point = this->newPoi.find(posString)) != newPoi.end() && action == PICKUP) {
			int success = point.addAvailableAgent(agent);
			if (ret == -1) point.completed();
			nextPos = oldPos;
		}

		// add agent to new map of the gridworld with updated location
		newAgents[posString] = newAgents;

	}

	//  iterate through POI to see if any have been fully picked up
	//  remove from the table if this is the case
	for (auto it = newPoi.begin(); it != newPoi.end(); ++it) {
		POI point = *it->second;
		if (point.isComplete()) {
			//  set carrying information for all agents in vector list
			std::vector<Agent*> carriers = point.getCarriers();
			for (int i = 0; i < carriers.size(); i++) {
				(*carriers[i]).setCarrying(true);
			}
			newPoi.erase(it);
		}
	}

	//  set new agent and POI states
	this->agents = newAgents;
	this->poi = newPoi;
}

//  check if a position is outside the bounds of the gridworld
bool Gridworld::inDomain(Position p) {

	if (p.getX() < 0 || p.getX() >= this->width || p.getY() < 0 || p.getY() >= this->height) {
		return false;
	}
	return true;
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
	agents.clear();
	poi.clear();
}

//  reset the world with the given neural net, 
//  or keep using the same neural net if NULL
void Gridworld::reset(bool random, FANN::neural_net net) {

	//  clear gridworld
	clear();

	//  reset POI and agents
	initAgents();
	initPOI();
	initHome(random);

	//  use a new neural net
	//if (net != NULL) this->nn = net;
}




