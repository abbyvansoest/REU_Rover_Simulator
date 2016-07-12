#include "controller.h"

//remove comparator

void evolve_reset_population(std::vector<Simulation> &simulations, int X, int Y, bool RANDOM_HOME_LOCATION)
{

	//  sort to find the top X performers
	std::sort(simulations.begin(), simulations.end());

	//  delete the nets of the lowest performing half of the population
	int halfPop = 0.5*(simulations.size());
	for (auto it = simulations.begin(); it != simulations.end() - halfPop; ++it) {
		it->destroyNet();
		int random = rand() % X;
		it->recreateNet((simulations.end() - random)->getNet());
	}

	//   mutate up to Y randomly chosen nets
	int i = 0;
	while (i < Y) {
		int chosenOne = rand() % simulations.size();
		(simulations.begin() + chosenOne)->mutate();
		i++;
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
	int MAX_STEPS = 50;
	int NUM_SIMULATIONS = 5;
	int NUM_EPOCHS = 10000;
	int X_TOP_PERFORMERS = 2;
	int Y_MUTATIONS = 5;

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

	srand(time(NULL));

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
		evolve_reset_population(simulations, X_TOP_PERFORMERS, Y_MUTATIONS, RANDOM_HOME_LOCATION);
	}

	/* Cleanup configuration memory */
	delete [] NC.layers;

	return 0;
}




