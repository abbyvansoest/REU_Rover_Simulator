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

Agent::Agent(){}
//  non-default constructor for agent
Agent::Agent(Position initialPos, State initialState) {
	position = initialPos;
	stateTrajectory.push_back(initialState);
}

/* get next action based on state and return to Gridworld
 * The action is represented as a single integer, found as the index in the
 * ouput array with the max value. The output is the 6-array of [0,1] continuous 
 * values. and the highest value represents the most favorable action the
 * policy has chosen */
int Agent::nextAction(State s, FANN::neural_net net) {
	//stateTrajectory.push_back(s);
	//TODO add exploratory steps
	
	fann_type* output = net.run( (fann_type*) s.array);

	int max_i = 0;
	for (int i = 0; i < 6; ++i)
	{
		if (output[i] > output[max_i]) { max_i = i; }
	}
	return max_i;
}

