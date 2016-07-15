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

// default constructor -- bascially random values
Gridworld::Gridworld() : Gridworld(2, 1, 5, 5, true) {}

//  constructor with arguments provided
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

//  initialize home base to pre-set global value
void Gridworld::initHome() {
	Position p = Position(HOME_X, HOME_Y);
	this->home = Home(p);
}

//  randomly initalize agents in the grid
void Gridworld::initAgents() {
	int x, y;
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

		Agent addAgent;
		addAgent.setP(pos);
		this->agents.push_back(addAgent);
	}
}

//  randomly initialize POI positions in the grid
void Gridworld::initPOI() {
	int x, y;
	string str;

	//  for each POI, find open position on the board
	for (int i = 0; i < this->numPOI; i++) {
		Position pos = Position(rand() % width, rand() % height);
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
	double broadcastCountA = 0.0, broadcastCountB = 0.0, broadcastCountC = 0.0, broadcastCountD = 0.0;

	// for the given agent, get count of agents and 
	   // count of broadcasting agents in each quadrant
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

//  step all agents in the world. Reward is not provided here
void Gridworld::stepAgents(FANN::neural_net* net, double &eps) {

	State state;
	Position oldPos, nextPos;
	int index = 1;

	//  iterate through all agents
	for (auto it = agents.begin(); it != agents.end(); ++it) {

		oldPos = Position(it->getP());
		state = getState(oldPos, *it);
		// .1 is a default epsilon value that is a placeholder for now
		int action = it->nextAction(state, net, oldPos, this->home, eps); 
		//if (this->numSteps < 10) std::cout << "action " << index << ": " << action << std::endl;
		index++;

		//  set down the POI a group of agents is holding
		if (action == SET_DOWN) {

			//  find all agents carrying a given POI
			POI* poi = it->getHoldingPOI();
			std::vector<Agent*> carriers = poi->getCarriers();
			int amt = poi->getWeight();

			//  set all their carrying values to false
			for (auto iter = carriers.begin(); iter != carriers.end(); ++iter) {
				Agent* agent = *iter;
				agent->setCarrying(false);
				agent->setHoldingPOI(NULL);
			}
			//  increase amount returned home
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

//  return a pointer to a neighboring poi
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
	if (this->home.currentAmount() == this->poiWeight*this->numPOI)
	{
		//std::cout << "worldComplete returning true" << std::endl;
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
	bool homePrint;

	for (int i = 0; i < this->height; i++) {
		for (int j = 0; j < this->width; j++) {
			Position p = Position(j, i);
			print = false;
			homePrint = false;

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
						std::cout << "P  ";
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

