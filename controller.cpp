#include "controller.h"

//remove comparator

void evolve_reset_population(std::vector<Simulation> &simulations, int X, bool RANDOM_HOME_LOCATION)
{
	std::sort(simulations.begin(), simulations.end());
	auto it = simulations.begin();
	for (it = simulations.begin(); it != (simulations.end()-X); ++it)
	{
		it->mutate();
		it->reset(RANDOM_HOME_LOCATION);
	}
	while (it != simulations.end()) {
		it->reset(RANDOM_HOME_LOCATION);
		++it;
	}
}

void printAvgReward(std::vector<Simulation> simulations, int num_sims, int epoch) {

	double avg = 0.0;

	for (int i = 0; i < num_sims; i++) {
		avg += simulations[i].getReward();
	}

	avg = avg / num_sims;
	std::cout << "EPOCH: " << epoch << "  " << "AVG REWARD: " << avg << "\n";
}

/* run simulations for the full number of epochs, performing neuro-evolutionary
   techniques between each epoch */ 
int main(void) {
	//  control experiment data collection
	int MAX_STEPS = 10;
	int NUM_SIMULATIONS = 1;
	int NUM_EPOCHS = 5;
	int X_TOP_PERFORMERS = 1;

	//  control gridworld
	int NUMBER_OF_AGENTS = 2;
	int NUMBER_OF_POI = 1;

	int WORLD_WIDTH = 3;
	int WORLD_HEIGHT = 3;

	bool RANDOM_HOME_LOCATION = false;

	//  control neural nets
	int NUMBER_OF_LAYERS = 3;
	bool RANDOM_WEIGHTS = true;
	double RANDOM_NET_MIN = -0.5;
	double RANDOM_NET_MAX =  0.5;

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

	std::vector<Simulation> simulations(NUM_SIMULATIONS, Simulation(GC, NC, MAX_STEPS));


	//  for each learning epoch, we run the set of simulations and 
	//  then evolve the population based on basic neuroevolutionary 
	//  algorithms.
	for (int i = 0; i < NUM_EPOCHS; i++) {

		std::cout << "EPOCH " << i << "\n";
		std::cout << "**********************************" << "\n";

		//  run each simulation
		for (int j = 0; j < NUM_SIMULATIONS; j++) {
			simulations[j].runEpoch();
			std::cout << "simulation " << j << "   ";
			simulations[j].logResults();
		}

		std::cout << "\n";

		//printAvgReward(simulations, NUM_SIMULATIONS, i);
		evolve_reset_population(simulations, X_TOP_PERFORMERS, RANDOM_HOME_LOCATION);
	}

	/* Cleanup configuration memory */
	delete [] NC.layers;

	return 0;
}




