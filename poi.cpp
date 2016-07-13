/*
 *  This simulator implements a rover domain gridworld for theoretical research purposes.
 *  Copyright (C) 2016  Connor Yates
 *
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
 */

#include "poi.h"

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

int POI::getWeight()
{
	return this->weight;
}

void POI::addAvailableAgent(Agent* ag) {

	this->agentsReady.push_back(ag);

	std::cout << "ADDED NEW AG SIZE IS " <<this->agentsReady.size()<< std::endl;
	
	/* if we have enough agents lifting the poi, return -1 */
	// TODO Do we want this to return -1? or should agents be made available
	// for more than is necessary, and then allow only a subset to lift if?
	if (this->agentsReady.size() >= this->weight) {
		this->completed();
	}
}

std::vector<Agent*> POI::getCarriers() {
	return this->agentsReady;
}

void POI::completed()
{
	this->complete = true;
}

bool POI::isComplete()
{
	return this->complete;
}

bool POI::isRemoved() {
 	return this->removed;
}

void POI::remove() {
	this->removed = true;
}

Position POI::getP() { return this->p; }
