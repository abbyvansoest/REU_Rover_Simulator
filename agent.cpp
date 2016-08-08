/*********************************************************************
* agent.cpp
*
* This class is an object representation of the agents in an 
* experimental gridworld. 
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

#include "agent.h"

Agent::Agent() {

	this->carrying = false;
	this->numCarried = 0;
	this->holding = NULL;
	this->p = Position(0, 0);
	this->reward = 0;

}

Agent::Agent(bool carrying, POI* holding, Position pos) {

	this->carrying = carrying;
	this->holding = holding;
	this->numCarried = 0;
	this->p = pos.copy();
	this->reward = 0;
}

//  copy constructor
Agent::Agent(const Agent& that)
{

	this->carrying = that.carrying;
	this->holding = that.holding;
	this->numCarried = that.numCarried;
	this->p = that.p;
	this->reward = that.reward;
}

// copy assignment operator
Agent& Agent::operator=(const Agent& that)
{
	this->carrying = that.carrying;
	this->holding = that.holding;
	this->numCarried = that.numCarried;
	this->p = that.p;
	this->reward = that.reward;
    return *this;
}

/* get next action based on state and return to Gridworld
 * The action is represented as a single integer, found as the index in the
 * ouput array with the max value. The output is the 6-array of [0,1] continuous 
 * values. and the highest value represents the most favorable action the
 * policy has chosen */

int Agent::nextAction(State s, Position self_pos, Home home, FANN::neural_net* net) {

	
	//  if the agent is carrying a POI, force the agent to step toward home
	if (this->isCarrying())
	{
		Position home_pos = home.getPosition();

		//  if the agent is on top of the home base, force the agent to set 
		//  down the poi
		if (home_pos == self_pos) return SET_DOWN;

		if (abs(self_pos.getX() - home_pos.getX()) > abs(self_pos.getY() - home_pos.getY()))
		{
			if (self_pos.getX() > home_pos.getX())
			{
				return MOVE_LEFT;
			}
			else { 
				return MOVE_RIGHT; }
		}
		else{
			if (self_pos.getY() > home_pos.getY())
			{
				return MOVE_UP;
			}
			else { 
				return MOVE_DOWN; }
		}
	}

	/* random baseline */
	return rand() % SET_DOWN;


	/* Picks the output from the neural net */
	fann_type* output = net->run( (fann_type*) s.array);

	int max_i = 0;
	for (int i = 0; i < 6; ++i)
	{
		if (output[i] > output[max_i]) { max_i = i; }
	}

	return max_i;
}

//  is the agent carrying anything?
bool Agent::isCarrying() { return this->carrying; }
//  set the carrying signal appropriately
void Agent::setCarrying(bool set) { 
	this->carrying = set; 
}

//  return the POI the agent is holding
POI* Agent::getHoldingPOI() { return this->holding;}
//  set the POI the agent is holding
void Agent::setHoldingPOI(POI* poi) { if (!this->isCarrying()) this->holding = poi; }

Position Agent::getP() { return this->p; }

void Agent::setP(Position pos) { this->p = pos; }

void Agent::hasCarried() {
	this->numCarried++;
}

int Agent::numberCarried() {
	return this->numCarried;
}

double Agent::getReward()
{
	/* This calculates a quick and dirty form of D.
	 * It returns the scaled reward based on how many
	 * POI the agent carried in its lifetime.
	 * This is the same as G(v)-G(v-i). */
	return (double)(this->numCarried * 5)/2.0 + this->reward;
}

void Agent::incReward(double inc)
{
	this->reward += inc;
}
