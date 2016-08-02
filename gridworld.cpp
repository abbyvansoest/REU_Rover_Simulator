/*********************************************************************
* gridworld.cpp
*
* This class is an object representation of an experimental gridworld, 
* in which agents are able to step around. The goal of this world is
* for the agents to return all POIs to the home base. 
*
* Copyright (C) 2016 Abby Van Soest, Connor Yates

*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/

#include "gridworld.h"

//  define constants for homebase location
int HOME_X = 1;
int HOME_Y = 1;
struct gridConfig emptyGC;

// default constructor -- bascially random values
Gridworld::Gridworld() : Gridworld(emptyGC, NULL) {}

//  constructor with arguments provided
Gridworld::Gridworld(struct gridConfig GC, FANN::neural_net** netTeam) {
		
	this->numAgents = GC.numAgents;
	this->numPOI = GC.numPOI;
	this->width = GC.width;
	this->height = GC.height;
	this->poiWeight = GC.poiWeight;
	this->netTeam = netTeam;

	this->numSteps = 0;

	initHome();
	initAgents();
	initPOI();
	
	this->pickupNet = new FANN::neural_net();
	bool success = this->pickupNet->create_from_file("Pickup.net");
	if (!success) {
		std::cout << "ERROR BUILDING PICKUP NET" << std::endl;
		exit(0);
	}
}

Gridworld::~Gridworld() {
	delete this->pickupNet;
	this->pickupNet = NULL;
}

//  initialize home base to pre-set global value
void Gridworld::initHome() {
	Position p = Position(HOME_X, HOME_Y);
	this->home = Home(p);
}

//  randomly initalize agents in the grid
void Gridworld::initAgents() {
	int x = 0, y = 0;
	string str;

	// for each agent, find open position on the board
	for (int i = 0; i < this->numAgents; i++) {
		Position pos = Position(rand() % width, rand() % height);
		while (!positionAvailable(pos)) {
			x = rand() % width;
			y = rand() % height;
			pos = Position(x, y);
			//std::cout << "agent pos: " << pos.toString() << std::endl;
		}

		Agent addAgent = Agent(false, NULL);
		addAgent.setP(pos);
		this->agents.push_back(addAgent);
	}
}

//  randomly initialize POI positions in the grid
void Gridworld::initPOI() {
	int x = rand() % width;
	int y = rand() % height;
	string str;

	//  for each POI, find open position on the board
	for (int i = 0; i < this->numPOI; i++) {
		Position pos = Position(x, y);
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

//  check if the position is occupied by an agent or a POI
bool Gridworld::positionAvailable(Position p) {
	
	if (p.getX() < 0 || p.getX() >= this->width || p.getY() < 0 || p.getY() >= this->height) {
		return false;
	}
	//  homebase has unlimited capacity for agents to fill
	if (p == this->home.getPosition()) {
		return true;
	}

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

	double agentsA = 0.0, agentsB = 0.0, agentsC = 0.0, agentsD = 0.0;
	double poiA = 0.0, poiB = 0.0, poiC = 0.0, poiD = 0.0;

	// for the given agent, get count of agents in each quadrant
	for (auto it = agents.begin(); it != agents.end(); ++it) {  
		//  values for the comparing agent
		Position p = Position(it->getP());

		if (p.getX() < pos.getX() && p.getY() >= pos.getY()) {
			agentsA += 1.0/getDistance(p, pos);
		}
		if (p.getX() >= pos.getX() && p.getY() > pos.getY()) {
			agentsB += 1.0/getDistance(p, pos);
		}
		if (p.getX() <= pos.getX() && p.getY() < pos.getY()) {
			agentsC += 1.0/getDistance(p, pos);
		}
		if (p.getX() > pos.getX() && p.getY() <= pos.getY()) {
			agentsD += 1.0/getDistance(p, pos);
		}
	}

	//  get count of POI in each quadrant
	for (auto it = poi.begin(); it != poi.end(); ++it) {  			

		Position p = Position(it->getP());

		if (p.getX() <  pos.getX() && p.getY() >= pos.getY()) { 
			poiA += 1.0/getDistance(p, pos);
		}
		if (p.getX() >= pos.getX() && p.getY() >  pos.getY()) { 
			poiB += 1.0/getDistance(p, pos);
		}
		if (p.getX() <= pos.getX() && p.getY() <  pos.getY()) { 
			poiC += 1.0/getDistance(p, pos);
		}
		if (p.getX() >  pos.getX() && p.getY() <= pos.getY()) {
			poiD += 1.0/getDistance(p, pos);
		}
	}

	State state;

	//  information on quadrant 1 
	state[AGENTS_A] = agentsA;
	state[POI_A] = poiA;

	//  information on quadrant 2
	state[AGENTS_B] = agentsB;
	state[POI_B] = poiB;

	//  information on quadrant 3
	state[AGENTS_C] = agentsC;
	state[POI_C] = poiC;

	//  information on quadrant 4
	state[AGENTS_D] = agentsD;
	state[POI_D] = poiD;

	// agent carrying information
	state[CARRYING] = (int)ag.isCarrying();

	state = normalize(state);

	return state;
}

State Gridworld::normalize(State state) {

	int STATE_LENGTH = 9;

	State normState;
	double sum = 0.0;

	for (int i = 0; i < STATE_LENGTH; i++) {
		sum += state[i]*state[i];
	}
	double norm = std::sqrt(sum);

	for (int i = 0; i < STATE_LENGTH; i++) {
		normState[i] = state[i]/norm;
	}

	return normState;
}

//  return the distance between points p1 and p2
double Gridworld::getDistance(Position p1, Position p2) {
	double deltaX = p1.getX() - p2.getX();
	double deltaY = p1.getY() - p2.getY();
	return sqrt(deltaX*deltaX + deltaY*deltaY);
}

//  step all agents in the world. Reward is not provided here
void Gridworld::stepAgents() {

	State state;
	Position oldPos, nextPos;
	int index = 0;
	int action;

	//  iterate through all agents
	for (auto it = agents.begin(); it != agents.end(); ++it) {

		oldPos = Position(it->getP());
		state = getState(oldPos, *it);

		float* output = this->pickupNet->run( (fann_type*) state.array);
		if (*output > .95) {
			action = PICKUP;
		}
		else {
			action = it->nextAction(state, oldPos, this->home, this->netTeam[index]); 
		}

		//  set down the POI a group of agents is holding
		if (action == SET_DOWN && it->getP() == this->home.getPosition()) {

			//  find all agents carrying a given POI
			POI* poi = it->getHoldingPOI();
			std::vector<Agent*> carriers = poi->getCarriers();

			//  set all their carrying values to false
			//std::cout << "carriers len: " << carriers.size() << std::endl;
			for (auto iter = carriers.begin(); iter != carriers.end(); ++iter) {
				//std::cout << "setting agent to no longer carry" << std::endl;
				Agent* agent = *iter;
				agent->setCarrying(false);
				agent->setHoldingPOI(NULL);
			}
			//  increase amount returned home
			this->home.receiveValues(1);
			if (this->worldComplete()) break;
		}

		//  set next position for all cases
		else if (action == MOVE_RIGHT) {
			nextPos = Position(oldPos.getX() + 1, oldPos.getY());
		}
		else if (action == MOVE_DOWN) {
			nextPos = Position(oldPos.getX(), oldPos.getY() + 1);
		}
		else if (action == MOVE_LEFT) {
			nextPos = Position(oldPos.getX() - 1, oldPos.getY());
		}
		else if (action == MOVE_UP) {
			nextPos = Position(oldPos.getX(), oldPos.getY() - 1);
		}

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
		index++;
	}

	//  iterate through poi to see if any new poi have been fully picked up
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
			POIit->remove();
		}

		else if (!POIit->isRemoved())
		{
			POIit->clearReadyAgents();
		}
	}

	//if (this->numSteps < 10) std::cout << std::endl;
	this->numSteps++;
}

//  do any POI border the given position?
bool Gridworld::findNearbyPOI(Position pos) {

	Position checkUp    = Position(pos.getX(), pos.getY() + 1);
	Position checkDown  = Position(pos.getX(), pos.getY() - 1);
	Position checkRight = Position(pos.getX() - 1, pos.getY());
	Position checkLeft  = Position(pos.getX() + 1, pos.getY());
	Position checkUpLeft = Position(pos.getX() - 1, pos.getY() + 1);
	Position checkDownLeft = Position(pos.getX() + 1, pos.getY() - 1);
	Position checkUpRight = Position(pos.getX() + 1, pos.getY() + 1);
	Position checkDownRight = Position(pos.getX() - 1, pos.getY() - 1);

	for (auto it = this->poi.begin(); it != this->poi.end(); ++it) {
		//  ignore if poi has been marked as complete or removed
		if (it->isComplete() || it->isRemoved()) continue;
		Position p = it->getP();
		if (p == checkUp || p == checkDown || p == checkRight || p == checkLeft
			|| p == checkUpLeft || p == checkDownLeft || p == checkUpRight || p == checkDownRight) {
			return true;
		}
	}

	return false;
}

//  return a pointer to a neighboring poi
POI* Gridworld::nearbyPOI(Position pos) {

	Position checkUp    = Position(pos.getX(), pos.getY() + 1);
	Position checkDown  = Position(pos.getX(), pos.getY() - 1);
	Position checkRight = Position(pos.getX() - 1, pos.getY());
	Position checkLeft  = Position(pos.getX() + 1, pos.getY());
	Position checkUpLeft = Position(pos.getX() - 1, pos.getY() + 1);
	Position checkDownLeft = Position(pos.getX() + 1, pos.getY() - 1);
	Position checkUpRight = Position(pos.getX() + 1, pos.getY() + 1);
	Position checkDownRight = Position(pos.getX() - 1, pos.getY() - 1);

	for (auto it = this->poi.begin(); it != this->poi.end(); ++it) {
		Position p = it->getP();
		if (p == checkUp || p == checkDown || p == checkRight || p == checkLeft
			|| p == checkUpLeft || p == checkDownLeft || p == checkUpRight || p == checkDownRight) {
			return &(*it);
		}
	}
}

//  entirely clear the gridworld of agents and POIs
void Gridworld::clear() {

	agents.clear();
	poi.clear();
}

//  reset the world entirely
void Gridworld::reset() {
	//  clear gridworld
	clear();
	this->numSteps = 0;

	//  reset POI and agents
	initAgents();
	initPOI();
	initHome();
}

//  have all POIs been returned home?
bool Gridworld::worldComplete()
{
	if (this->home.currentAmount() == this->numPOI)
	{
		std::cout << "worldComplete: " << this->home.currentAmount() << std::endl;
		this->printWorld();
		return true;
	}

	return false;
}

//  the current amount that has been returned to the home base
int Gridworld::currentAmount()
{
	return this->home.currentAmount();
}

//  print the current state of the world
void Gridworld::printWorld() {

	bool print;
	bool homePrint = false;

	for (int i = 0; i < this->height; i++) {
		for (int j = 0; j < this->width; j++) {
			Position p = Position(j, i);
			print = false;

			for (auto it = agents.begin(); it != agents.end(); ++it) {
				if (it->getP() == p) {
					if (this->home.getPosition() == p)  {
						if (it->isCarrying()) std::cout << "H+  ";
						else std::cout << "HA  ";
						homePrint = true;
					}
					else if (it->isCarrying()) std::cout << "+  ";
					else std::cout << "A  ";
					print = true;
				}
			}
			for (auto it = poi.begin(); it != poi.end(); ++it) {
				if (it->getP() == p) {
					if (!it->isRemoved()) {
						if (this->home.getPosition() == p)  {
							std::cout << "HP  ";
							homePrint = true;
						}
						else std::cout << "P  ";
						print = true;
					}
					else print = false;
				}
			}
			if (this->home.getPosition() == p && !homePrint) {
				std::cout << "H  ";
				print = true;
			}
			if (!print) std::cout << "*  ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

//  return the total number of steps taken in the world
int Gridworld::stepsTaken() {
	return this->numSteps;
}

void Gridworld::clearPOI()
{
	for (auto it = this->poi.begin(); it != this->poi.end(); ++it)
	{
		it->clearReadyAgents();
	}
}

double* Gridworld::accumulateRewards() {

	double rewards[numAgents];
	int index = 0;
	for (auto it = this->agents.begin(); it != this->agents.end(); ++it)
	{
		rewards[index] = it->getReward();
	}

	return rewards;
}


