#include "simulation.h"
#include <iostream>

/* Calls the non-default constructors on the members with predetermined, 
 * almost arbitrary values */
Simulation::Simulation()
	: world(2, 1, 5, 5, false)
	, net(FANN::LAYER, 3, (const unsigned int[]) {13,9,6})
{
	this->reward = 0;
	this->timesteps = 250;
}

/* This non default constructor uses the information provided by the configuration structs 
 * to call the subsequent non default constructors for the members */
Simulation::Simulation(struct gridConfig GC, struct netConfig NC, int timesteps)
	: world(GC.numAgents, GC.numPOI, GC.width, GC.height, GC.randHome)
	, net(NC.net_type, NC.num_layers, NC.layers)
	
{
	if (NC.randWeights) { this->net.randomize_weights(NC.randMin, NC.randMax); }
	this->timesteps = timesteps;
	std::cout << "INIT WORLD\n";
	world.printWorld();

}

void Simulation::logResults()
{
	std::cout << "Reward: " << this->getReward() << 
	"    Returned: " << this->world.currentAmount() << "\n";
	if (this->world.currentAmount() > 0) std::cout << "SUCCESS OF SOME SORT!\n";
}

void Simulation::generateStats()
{

}

/* Runs a single epoch, which runs for a given number of timesteps */
int Simulation::runEpoch()
{
	// Run the simulation until the time runs out or the simulation ends prematurely
	int steps = 0;
	for (; steps < this->timesteps; ++steps)
	{
		this->world.stepAgents(this->net);
		if (this->world.worldComplete())
			break;
	}

	// Calculate the reward
	this->reward -= steps * 0.05;
	this->reward += this->world.currentAmount();

	return 0;
}

double Simulation::getReward() const 
{
	return this->reward;
}

/* Resets the gridworld and the statistics variables internal to the class */
void Simulation::reset(bool randHome)
{
	this->world.reset(randHome);
	// Reset any statistics variables here
	this->reward = 0;
}

void Simulation::mutate()
{

}

bool Simulation::operator<(const Simulation &rhs) const
{
	return this->reward < rhs.reward;
}
