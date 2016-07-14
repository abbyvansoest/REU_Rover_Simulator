#include "gridworld.h"

//  define constants for homebase location
//  TODO load from config file eventually
int HOME_X = 0;
int HOME_Y = 0;

// constructor
Gridworld::Gridworld() : Gridworld(2, 1, 5, 5, true) {}

Gridworld::Gridworld(int numAgents, int numPOI, int width, int height, 
	int weight) {
		
	this->numAgents = numAgents;
	this->numPOI = numPOI;
	this->width = width;
	this->height = height;
	this->poiWeight = weight;

	initHome();
	initAgents();
	initPOI();
	this->numSteps = 0;
}

//  initialize home base to random location
//  or pre-set global value
void Gridworld::initHome() {
	
	Position p = Position(HOME_X, HOME_Y);
	this->home = Home(p);

}

//  randomly initalize agents in the grid
void Gridworld::initAgents() {
	int x, y;
	string str;

	for (int i = 0; i < this->numAgents; i++) {
		//  find open position on the board
		Position pos;
		while (!positionAvailable(pos)) {
			x = rand() % width;
			y = rand() % height;
			pos = Position(x, y);
		}
		
		// add an agent to the open position
		//str = pos.toString();
	//	std::cout << "AGENT POSITION: " << str << std::endl;
		Agent addAgent;
		addAgent.setP(pos);
		this->agents.push_back(addAgent);
	}
}

//  randomly initialize POI positions in the grid
void Gridworld::initPOI() {
	int x, y;
	string str;

	for (int i = 0; i < this->numPOI; i++) {
		//  find open position on the board
		Position pos;
		while (!positionAvailable(pos)) {
			x = rand() % width;
			y = rand() % height;
			pos = Position(x, y);
		}

		// add a POI to the open position
		POI addPOI = POI(this->poiWeight, x, y);
		this->poi.push_back(addPOI);
	}
}

//  check if the position is occupied by an agent, a POI, or the homebase
bool Gridworld::positionAvailable(Position p) {
	
	if (p.getX() < 0 || p.getX() >= this->width || p.getY() < 0 || p.getY() >= this->height) {
		return false;
	}

	// if (this->home.getPosition() == p) {
	// 	return false;
	// }

	for (auto it = agents.begin(); it != agents.end(); ++it) {  
		Position pos = Position(it->getP());
		if (pos == p) return false;
	}

	for (auto it = poi.begin(); it != poi.end(); ++it) { 
		Position pos = Position(it->getP());
		if (pos == p) {
			if (it->isRemoved() || it->isComplete()) return true; 
			return false;
		}
	}

	return true;
}

//  return the 13-dim state representation for ag
State Gridworld::getState(Position pos, Agent ag) {
	// go through all agents and POI
	// increment variables to account for counts based on relative quadrant location
	double agentsA = 0.0, agentsB = 0.0, agentsC = 0.0, agentsD = 0.0;
	double poiA = 0.0, poiB = 0.0, poiC = 0.0, poiD = 0.0;
	double broadcastCountA = 0.0, broadcastCountB = 0.0, broadcastCountC = 0.0, broadcastCountD = 0.0;

	// get count of agents and count of broadcasting agents in each quadrant
	for (auto it = agents.begin(); it != agents.end(); ++it) {  
		//  values for the comparing agent
		Position p = Position(it->getP());

		if (p.getX() < pos.getX() && p.getY() >= pos.getY()) {
			agentsA += 1/getDistance(p, pos);
			if (it->isBroadcasting()) { broadcastCountA++; }
		}
		if (p.getX() >= pos.getX() && p.getY() > pos.getY()) {
			agentsB += 1/getDistance(p, pos);
			if (it->isBroadcasting()) { broadcastCountB++; }
		}
		if (p.getX() <= pos.getX() && p.getY() < pos.getY()) {
			agentsC += 1/getDistance(p, pos);
			if (it->isBroadcasting()) { broadcastCountC++; }
		}
		if (p.getX() > pos.getX() && p.getY() <= pos.getY()) {
			agentsD += 1/getDistance(p, pos);
			if (it->isBroadcasting()) { broadcastCountD++; }
		}
	}

	//  get count of POI in each quadrant
	for (auto it = poi.begin(); it != poi.end(); ++it) {  			

		Position p = Position(it->getP());

		if (p.getX() <  pos.getX() && p.getY() >= pos.getY()) { 
			poiA += 1/getDistance(p, pos);
		}
		if (p.getX() >= pos.getX() && p.getY() >  pos.getY()) { 
			poiB += 1/getDistance(p, pos);
		}
		if (p.getX() <= pos.getX() && p.getY() <  pos.getY()) { 
			poiC += 1/getDistance(p, pos);
		}
		if (p.getX() >  pos.getX() && p.getY() <= pos.getY()) {
			poiD += 1/getDistance(p, pos);
		}
	}

	State state;

	//  information on quadrant 1 
	state[AGENTS_A] = agentsA;
	state[POI_A] = poiA;
	state[BROADCASTING_A] = broadcastCountA;
	//  information on quadrant 2
	state[AGENTS_B] = agentsB;
	state[POI_B] = poiB;
	state[BROADCASTING_B] = broadcastCountB;
	//  information on quadrant 3
	state[AGENTS_C] = agentsC;
	state[POI_C] = poiC;
	state[BROADCASTING_C] = broadcastCountC;
	//  information on quadrant 4
	state[AGENTS_D] = agentsD;
	state[POI_D] = poiD;
	state[BROADCASTING_D] = broadcastCountD;
	// agent carrying information
	state[CARRYING] = (int)ag.isCarrying();

	return state;
}

//  return the distance between points p1 and p2
double Gridworld::getDistance(Position p1, Position p2) {
	double deltaX = p1.getX() - p2.getX();
	double deltaY = p1.getY() - p2.getY();
	return sqrt(deltaX*deltaX + deltaY*deltaY);
}

/*  step all agents in the world.
 *  Does not calculate or provide a reward. */
void Gridworld::stepAgents(FANN::neural_net* net, double &eps) {

	State state;
	Position oldPos, nextPos;

	//this->printWorld();

	//  iterate through all agents
	for (auto it = agents.begin(); it != agents.end(); ++it) {

		oldPos = Position(it->getP());
		state = getState(oldPos, *it);
		// .1 = a default epsilon value that is a placeholder for now
		int action = it->nextAction(state, net, oldPos, this->home, eps); 
	//	std::cout << "action: " << action << std::endl;

		//  set down the POI a group of agents is holding
		if (action == SET_DOWN) {
			
			std::cout << "SET DOWN: step " << this->numSteps << std::endl;

			//  find all agents carrying a given POI
			POI* poi = it->getHoldingPOI();
			std::vector<Agent*> carriers = poi->getCarriers();
			int amt = poi->getWeight();

			for (auto iter = carriers.begin(); iter != carriers.end(); ++iter) {
				Agent* agent = *iter;
				//  set all their carrying values to false
				agent->setCarrying(false);
				agent->setHoldingPOI(NULL);
			}
			
			this->home.receiveValues(amt);
			continue;
		}

		//  set next position for all cases
		if (action == MOVE_RIGHT) {
			nextPos = Position(oldPos.getX() + 1, oldPos.getY());
		}
		if (action == MOVE_DOWN) {
			nextPos = Position(oldPos.getX(), oldPos.getY() + 1);
		}
		if (action == MOVE_LEFT) {
			nextPos = Position(oldPos.getX() - 1, oldPos.getY());
		}
		if (action == MOVE_UP) {
			nextPos = Position(oldPos.getX(), oldPos.getY() - 1);
		}

		if (action == BROADCAST) {
			it->setBroadcast(true);
			nextPos = oldPos;
		}
		else it->setBroadcast(false);  

		if (action == PICKUP) {

			nextPos = oldPos;

			//  if it has a POI within one block of it and it's action is to pickup,
			//  mark the POI as having another potential carrier.
			POI* found;
			if (findNearbyPOI(nextPos)) {
				found = nearbyPOI(nextPos);
				if (!found->isComplete()) {
					//  increment until adequate # agents
					found->addAvailableAgent(&(*it));
				}
			}
		}

		//  check for collisions in new map -- change agent's position if unoccupied
		//  insert agent to newAgents vector 
		if (!positionAvailable(nextPos)) {
			nextPos = oldPos;
		}

		it->setP(nextPos);
	}

	//  iterate through POI to see if any have been fully picked up
	for (auto POIit = this->poi.begin(); POIit != this->poi.end(); ++POIit) {
		if (POIit->isComplete() && !POIit->isRemoved())
		{
			//  set carrying information for all agents in vector list
			std::vector<Agent*> carriers = POIit->getCarriers();
			for (auto AGit = carriers.begin(); AGit != carriers.end(); ++AGit) {
				Agent* agent = *AGit;
				POI* poi = &(*POIit);
				agent->setHoldingPOI(poi);
				agent->setCarrying(true);

			}
			//  'remove' from POI table
			std::cout << "REMOVING A NEWLY COMPLETE POI" << std::endl;
			POIit->remove();
		}
	}

	this->numSteps++;
}

POI* Gridworld::nearbyPOI(Position pos) {

	Position checkUp    = Position(pos.getX(), pos.getY() + 1);
	Position checkDown  = Position(pos.getX(), pos.getY() - 1);
	Position checkRight = Position(pos.getX() - 1, pos.getY());
	Position checkLeft  = Position(pos.getX() + 1, pos.getY());

	for (auto it = this->poi.begin(); it != this->poi.end(); ++it) {
		Position p = it->getP();
		if (p == checkUp || p == checkDown 
			|| p == checkRight || p == checkLeft) {
			return &(*it);
		}
	}
}

//  check if any POI border the given position 
//  return the string of their position if so or null if not
bool Gridworld::findNearbyPOI(Position pos) {

	Position checkUp    = Position(pos.getX(), pos.getY() + 1);
	Position checkDown  = Position(pos.getX(), pos.getY() - 1);
	Position checkRight = Position(pos.getX() - 1, pos.getY());
	Position checkLeft  = Position(pos.getX() + 1, pos.getY());

	for (auto it = this->poi.begin(); it != this->poi.end(); ++it) {
		//  ignore if poi has been marked as complete or removed
		if (it->isComplete() || it->isRemoved()) continue;
		Position p = it->getP();
		if (p == checkUp || p == checkDown 
			|| p == checkRight || p == checkLeft) {
			return true;
		}
	}

	return false;
}


//  entirely clear the gridworld of agents and POIs
void Gridworld::clear() {

	agents.clear();
	poi.clear();
}

//  reset the world with the given neural net, 
//  or keep using the same neural net if NULL
void Gridworld::reset() {

	//  clear gridworld
	clear();
	this->numSteps = 0;

	//  reset POI and agents
	initAgents();
	initPOI();
	initHome();
}

int Gridworld::currentAmount()
{
	return this->home.currentAmount();
}

bool Gridworld::worldComplete()
{
	if (this->home.currentAmount() == this->poiWeight*this->numPOI)
	{
		//std::cout << "worldComplete returning true" << std::endl;
		return true;
	}

	return false;
}

void Gridworld::printWorld() {

	bool print;

	for (int i = 0; i < this->height; i++) {
		for (int j = 0; j < this->width; j++) {
			Position p = Position(j, i);
			print = false;

			for (auto it = agents.begin(); it != agents.end(); ++it) {
				if (it->getP() == p) {
					if (it->isCarrying()) std::cout << "+ ";
					else std::cout << "A ";
					print = true;
				}
			}
			for (auto it = poi.begin(); it != poi.end(); ++it) {
				if (it->getP() == p) {
					if (!it->isRemoved()) {
						std::cout << "P ";
						print = true;
					}
					else print = false;
				}
			}
			if (this->home.getPosition() == p) {
				std::cout << "H ";
				print = true;
			}
			if (!print) std::cout << "* ";
		}
		std::cout << std::endl;
	}

}

int Gridworld::stepsTaken() {
	return this->numSteps;
}

