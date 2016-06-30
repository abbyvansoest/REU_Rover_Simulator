#include "agent.h"
#include <iostream>
#include <deck>

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


