/*********************************************************************
* poi.cpp
*
* This class is an object representation of Points of Interest (POI)
* in an experimental gridworld. 
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

#include "poi.h"

//  default constructor
POI::POI()
{
	this->complete = false;
	this->weight = 1;
	this->removed = false;
}


/* Non-default constructor calls on the 
* position non-default constructor */
POI::POI(int weight, int x, int y)
{
	this->weight = weight;
	this->p = Position(x, y);
	this->removed = false;
	this->complete = false;
}

//  return the weight of the poi
int POI::getWeight()
{
	return this->weight;
}

// add another agent to the list of agents available to 
	//  carry the poi
void POI::addAvailableAgent(Agent* ag) {

	this->agentsReady.push_back(ag);
	
	//  mark the poi as completed if enough agents are available to carry
	if (this->agentsReady.size() >= this->weight) {
		this->completed();
	}
}

//  return the vector of agents carrying the poi
std::vector<Agent*> POI::getCarriers() {
	return this->agentsReady;
}

//  is the poi complete?
bool POI::isComplete()
{
	return this->complete;
}
//  mark the poi as completed
void POI::completed()
{
	this->complete = true;
}

//  is the poi removed?
bool POI::isRemoved() {
 	return this->removed;
}
//  remove the poi?
void POI::remove() {
	this->removed = true;
}

//  return the location of te poi in the world
Position POI::getP() { return this->p; }

void POI::clearReadyAgents()
{
	this->agentsReady.clear();
}
