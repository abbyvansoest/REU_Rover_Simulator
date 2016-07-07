#include "controller.h"


/* comparator to sort simNodes in reverse order of their weights */
struct minComparator {
	bool operator()(const Simulation& sim1, const Simulation& sim2) {  
		return sim1.getReward() > sim2.getReward();
	}
}; 

void evolve_population(std::vector<Simulation> &simulations, int X)
{
	std::sort(simulations.begin(), simulations.end());
	for (auto it = simulations.begin(); it != (simulations.end()-X); ++it)
	{
		it->mutate();
	}
}


/* run simulations for the full number of epochs, performing neuro-evolutionary
   techniques between each epoch */ 
int main(void) {
	//  control experiment data collection
	int MAX_STEPS = 250;
	int NUM_SIMULATIONS = 100;
	int NUM_EPOCHS = 1000;
	int X_TOP_PERFORMERS = 10;

	//  control gridworld
	int NUMBER_OF_AGENTS = 10;
	int NUMBER_OF_POI = 10;

	int WORLD_WIDTH = 10;
	int WORLD_HEIGHT = 10;

	bool RANDOM_HOME_LOCATION = true;

	//  control neural nets
	int NUMBER_OF_LAYERS = 3;
	bool RANDOM_WEIGHTS = true;
	int RANDOM_NET_MIN = -0.5;
	int RANDOM_NET_MAX =  0.5;

	//  set up gridworld configuration
	struct gridConfig GC;
	GC.numAgents = NUMBER_OF_AGENTS;
	GC.numPOI = NUMBER_OF_POI;
	GC.width = WORLD_WIDTH;
	GC.height = WORLD_HEIGHT;
	GC.randHome = RANDOM_HOME_LOCATION;

	//  set up initial net configuration
	struct netConfig NC;
	NC.net_type = FANN::LAYER;
	NC.num_layers = NUMBER_OF_LAYERS;
	NC.layers = new unsigned int[3];
	NC.layers[0] = 13;
	NC.layers[1] = 8;
	NC.layers[2] = 6;
	NC.randWeights = RANDOM_WEIGHTS;
	NC.randMin = RANDOM_NET_MIN; 
	NC.randMax = RANDOM_NET_MAX;

	std::vector<Simulation> simulations;

	// initiaize simulations
	for (int i = 0; i < NUM_SIMULATIONS; i++) {
		simulations[i] = Simulation(GC, NC, MAX_STEPS);
	}

	//  for each learning epoch, we run the set of simulations and 
	//  then evolve the population based on basic neuroevolutionary 
	//  algorithms.
	for (int i = 0; i < NUM_EPOCHS; i++) {

		//  run each simulation
		for (int j = 0; j < NUM_SIMULATIONS; j++) {
			simulations[j].runEpoch();
		}

		evolve_population(simulations, X_TOP_PERFORMERS);
		for (auto it = simulations.begin(); it != simulations.end(); ++it)
			it->reset(RANDOM_HOME_LOCATION);

	}

	return 0;
}


//  simulations.mutate()

