#include "simulation.h"


Simulation::Simulation()
	: world(2, 1, 5, 5, false)
	, net(FANN::LAYER, 3, (const unsigned int[]) {13,9,6})
{

}

Simulation::Simulation(struct gridConf GC, struct netConf NC)
	: world(GC.numAgents, GC.numPOI, GC.width, GC.height, GC.randHome)
	, net(NC.net_type, NC.num_layers, NC.layers)
	
{
	if (NC.randWeights) { this->net.randomize_weights(NC.randMin, NC.randMax); }

}

void Simulation::logResults()
{

}

void Simulation::generateStats()
{

}
