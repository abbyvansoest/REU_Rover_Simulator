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

using namespace std;

//  non-default constructor for agent
Agent::Agent(Position initialPos, State initialState) {

	position = initialPos;
	stateTrajectory.push_back(initialState);
	//  initalize NN as needed 

}

//  get next action based on state and return to Gridworld
int Agent::nextAction(State state) {
	stateTrajectory.push_back(state);
	return calculateAction(state);
}

// based on current neural network and current agent state,
// compute and return the next desired action
int Agent::calculateAction(State state) {


}

//  based on the actions taken and the rewards received, 
//  update collective neural network
void Agent::updateNN(Deque<State> actions, double reward) {


}


