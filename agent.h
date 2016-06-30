
#ifndef AGENT_INCLUDED
#define AGENT_INCLUDED

#include <deque>

typedef int[13] State;

class Agent {

private:
	static XXX nn;
	Position position;
	std::deque<State> stateTrajectory;
	int calculateAction(State);

public:	
	Agent();
	int nextAction(State);
	void updateNN(Deque<State>, double);
};

#endif
