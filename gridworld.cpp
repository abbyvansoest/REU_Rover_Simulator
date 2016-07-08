#include "gridworld.h"

//  define constants for homebase location
//  TODO load from config file eventually
int HOME_X = 0;
int HOME_Y = 0;


// constructor
Gridworld::Gridworld() : Gridworld(2, 1, 5, 5, true) {}

Gridworld::Gridworld(int numAgents, int numPOI, int width, int height, 
	bool randHome) {
		
	this->numAgents = numAgents;
	this->numPOI = numPOI;
	this->width = width;
	this->height = height;

	initAgents();
	initPOI();
	initHome(randHome);
	this->numSteps = 0;
}

Gridworld::~Gridworld()
{
	/*
	for (auto it = this->poi.begin(); it != this->poi.end(); ++it)
		delete it->second;

	for (auto it = this->agents.begin(); it != this->agents.end(); ++it)
		delete it->second;
		*/
}

//  randomly initalize agents in the grid
void Gridworld::initAgents() {
	int x, y;
	string str;

	for (int i = 0; i < this->numAgents; i++) {
		//  find open position on the board
		Position pos;
		bool open = false;
		while (!open) {
			x = rand() % width;
			y = rand() % height;
			pos = Position(x, y);
			open = positionAvailable(pos);
		}
		
		// add an agent to the open position
		str = pos.toString();
		//Agent* addAgent = new Agent();
		//this->agents[str] = addAgent;
		this->agents.push_back(Agent());
	}

	//  set state of all agents
	Position pos;
	for (auto it = agents.begin(); it != agents.end(); ++it) {
		pos = Position(it->getP());
		it->setState(getState(pos, *it));
	}
}

//  randomly initialize POI positions in the grid
void Gridworld::initPOI() {
	int x, y;
	string str;

	for (int i = 0; i < this->numPOI; i++) {
		//  find open position on the board
		Position pos;
		bool open = false;
		while (!open) {
			x = rand() % width;
			y = rand() % height;
			pos = Position(x, y);
			open = positionAvailable(pos);
		}

		// add a POI to the open position
		this->poi.push_back(POI(2,x,y));
	}
}

//  initialize home base to random location
//  or pre-set global value
void Gridworld::initHome(bool rand_flag) {
	if (!rand_flag) {
		Position p = Position(HOME_X, HOME_Y);
		this->home = Home(p);
	}

	else {
		int x, y;
		x = rand() % width;
		y = rand() % height;
		Position p(x, y);
		this->home = Home(p);
	}
}

//  check if the position is occupied by an agent, a POI, or the homebase
bool Gridworld::positionAvailable(Position p) {
	/*
	string posString = p.toString();
	if ((this->agents.find(posString) != agents.end()) ||
		(this->poi.find(posString) != poi.end()))
	{
		return false;
	}
	if (this->home.getPosition() == p)
	{
		return false;
	}
	*/
	return true;
}

//  return the 13-dim state representation for ag
State Gridworld::getState(Position &pos, Agent &ag) {
	// go through all agents and POI
	// increment variables to account for counts based on relative quadrant location
	double agentsA = 0.0, agentsB = 0.0, agentsC = 0.0, agentsD = 0.0;
	double poiA = 0.0, poiB = 0.0, poiC = 0.0, poiD = 0.0;
	double broadcastCountA, broadcastCountB, broadcastCountC, broadcastCountD;

	// get count of agents and count of broadcasting agents in each quadrant
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

/*  step all agents in the world.
 *  Does not calculate or provide a reward. */
void Gridworld::stepAgents(FANN::neural_net net) {

	State state;
	Position oldPos, nextPos;

	//  iterate through all agents
	for (auto it = agents.begin(); it != agents.end(); ++it) {

		oldPos = Position(it->getP());
		state = getState(oldPos, *it);
		int action = it->nextAction(state, net, oldPos, this->home.getPosition(), .1); // a default epsilon value is a placeholder for now

		//  set next position for all cases
		if (action == MOVE_RIGHT) {
			nextPos = Position(oldPos.getX() - 1, oldPos.getY());
		}
		if (action == MOVE_DOWN) {
			nextPos = Position(oldPos.getX(), oldPos.getY() - 1);
		}
		if (action == MOVE_LEFT) {
			nextPos = Position(oldPos.getX() + 1, oldPos.getY());
		}
		if (action == MOVE_UP) {
			nextPos = Position(oldPos.getX(), oldPos.getY() + 1);
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
			//  Agent remains in original location.
			string foundPOI = findNearbyPOI(nextPos);
			if (!foundPOI.empty()) {
				for (auto it_poi = this->poi.begin(); it_poi != this->poi.end(); ++it)
				{
					if (foundPOI == it_poi->getP().toString())
					{
						int success = it_poi->addAvailableAgent();
						if (success == -1) {
							it_poi->completed();
						}
					}
				}
			}
		}
	}

	//  iterate through POI to see if any have been fully picked up
	//  remove from the table if this is the case
	std::vector<POI> newpoi;
	for (auto it = this->poi.begin(); it != this->poi.end(); ++it) {
		if (it->isComplete())
		{
			/*
			//  set carrying information for all agents in vector list
			std::vector<Agent*> carriers = point.getCarriers();
			for (int i = 0; i < carriers.size(); i++) {
			carriers[i]->setCarrying(true);
			}
			*/
			bool picked_up = false;
			for (auto it_a = this->agents.begin(); it_a != this->agents.end(); ++it_a)
			{
				Position pos(it_a->getP());
				if (   it->getP() == Position(pos.getX(), pos.getY() + 1)
						|| it->getP() == Position(pos.getX(), pos.getY() - 1)
						|| it->getP() == Position(pos.getX() - 1, pos.getY())
						|| it->getP() == Position(pos.getX() + 1, pos.getY())
				   )
				{
					picked_up = true;
					it_a->setCarrying(true);
				}
				
			}
			if (!picked_up)
			{
				newpoi.push_back(*it);
			}

			//  set new agent and POI states
			//this->agents = newAgents;
			//this->poi = newPoi;
			this->numSteps++;
		}
	}
	this->poi = newpoi;
}

//  return the string of the first neighboring POI for the given position,
	//  or null if no neighboring POI exists
string Gridworld::findNearbyPOI(Position pos) {

	/*
	string checkUp    = Position(pos.getX(), pos.getY() + 1).toString();
	string checkDown  = Position(pos.getX(), pos.getY() - 1).toString();
	string checkRight = Position(pos.getX() - 1, pos.getY()).toString();
	string checkLeft  = Position(pos.getX() + 1, pos.getY()).toString();

	auto end = newPoi.end();

	if (newPoi.find(checkUp) != end)    return checkUp;
	if (newPoi.find(checkDown) != end)  return checkDown;
	if (newPoi.find(checkRight) != end) return checkRight;
	if (newPoi.find(checkLeft) != end)  return checkLeft;
	*/

	return "";

}

//  check if a position is outside the bounds of the gridworld
bool Gridworld::inDomain(Position p) {

	if (p.getX() < 0 || p.getX() >= this->width || p.getY() < 0 || p.getY() >= this->height) {
		return false;
	}
	return true;
}

// at the end of a simulation, get reward for this grid
double Gridworld::getGridReward() {
	return (-.05*this->numSteps + this->home.currentAmount());
}

//  entirely clear the gridworld of agents and POIs
void Gridworld::clear() {

	agents.clear();
	poi.clear();
}

//  reset the world with the given neural net, 
//  or keep using the same neural net if NULL
void Gridworld::reset(bool randomHome) {

	//  clear gridworld
	clear();

	//  reset POI and agents
	initAgents();
	initPOI();
	initHome(randomHome);
}

int Gridworld::currentAmount()
{
	return this->home.currentAmount();
}

bool Gridworld::worldComplete()
{
	if (this->home.currentAmount() == this->numPOI)
		return true;
	return false;
}

