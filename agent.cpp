/*
 *  This simulator implements a rover domain gridworld for theoretical research purposes.
 *  Copyright (C) 2016  Connor Yates, Abby Van Soest
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

#include "agent.h"

Agent::Agent() {
	this->broadcasting = false;
	this->carrying = false;
	this->holding = NULL;
}

Agent::Agent(bool broadcasting, bool carrying, POI* holding) {
	this->broadcasting = broadcasting;
	this->carrying = carrying;
	this->holding = holding;
}

Agent Agent::copy() {
	return Agent(this->isBroadcasting(), this->isCarrying(), this->holding);
}

//  is the agent in the middle of broadcasting?
bool Agent::isBroadcasting() {
	return this->broadcasting;
}

//  set the broadcast value appropriately
void Agent::setBroadcast(bool set) {
	this->broadcasting = set;
}

//  is the agent carrying anything?
bool Agent::isCarrying() {
	return this->carrying;
}

//  set the carrying signal appropriately
void Agent::setCarrying(bool set) {
	this->carrying = set;
}

//  set the POI the agent is holding
void Agent::setHoldingPOI(POI* poi) {
	if (!this->isCarrying()) this->holding = poi;
}

//  return the POI the agent is holding
POI* Agent::getHoldingPOI() {
	return this->holding;
}

/* get next action based on state and return to Gridworld
 * The action is represented as a single integer, found as the index in the
 * ouput array with the max value. The output is the 6-array of [0,1] continuous 
 * values. and the highest value represents the most favorable action the
 * policy has chosen */

int Agent::nextAction(State s, FANN::neural_net*& net, Position self_pos, Home home, double eps) {

	//stateTrajectory.push_back(s);
	
	//TODO if adding threading, change to rand_r()
	/*
	if (( (double)rand()/(double)RAND_MAX+1.0) < eps)
	{
		return rand() % 6;
	}
	*/
	
	if (this->isCarrying())
	{
		Position home_pos = home.getPosition();
		//std::cout << "HOME: ("<<home_pos.getX()<<","<<home_pos.getY()<<") ROVER: ("<<self_pos.getX()<<","<<self_pos.getY()<<")"<<std::endl;
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

	/* Picks the output from the neural net */
	fann_type* output = net->run( (fann_type*) s.array);

	int max_i = 0;
	for (int i = 0; i < 6; ++i)
	{
		if (output[i] > output[max_i]) { max_i = i; }
	}

	return max_i;
}

Position Agent::getP() { return this->p; }

void Agent::setP(Position pos) {
	this->p = pos;
}
