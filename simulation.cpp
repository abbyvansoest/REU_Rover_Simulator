#include "simulation.h"


/* Calls the non-default constructors on the members with predetermined, 
 * almost arbitrary values */
Simulation::Simulation()
	: world(2, 1, 5, 5, false)
	, net(FANN::LAYER, 3, (const unsigned int[]) {13,9,6})
{
	this->timesteps = 250;

}

/* This non default constructor uses the information provided by the configuration structs 
 * to call the subsequent non default constructors for the members */
Simulation::Simulation(struct gridConf GC, struct netConf NC, int timesteps)
	: world(GC.numAgents, GC.numPOI, GC.width, GC.height, GC.randHome)
	, net(NC.net_type, NC.num_layers, NC.layers)
	
{
	if (NC.randWeights) { this->net.randomize_weights(NC.randMin, NC.randMax); }
	this->timesteps = timesteps;

}

void Simulation::logResults()
{

}

void Simulation::generateStats()
{

}

/* Runs a single epoch, which runs for a given number of timesteps */
int Simulation::runEpoch()
{
	// Run the simulation until the time runs out or the simulation ends prematurely
	int steps;
	for (int steps = 0; steps < this->timesteps; ++steps)
	{
		this->world.stepAgents(this->net);
		if (this->world.worldComplete())
			break;
	}

	// Calculate the reward
	double reward = 0;
	reward -= steps * 0.05;
	reward += this->world.currentAmount();


	return 0;
}
