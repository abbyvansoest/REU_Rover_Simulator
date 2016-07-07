#include "controller.h"
#include <map>
#include <queue>
#include <functional>

//  control experiment data collection
MAX_STEPS = 250;
NUM_SIMULATIONS = 100;
NUM_EPOCHS = 1000;
X_TOP_PERFORMERS = 10;

//  control gridworld
NUMBER_OF_AGENTS = 10;
NUMBER_OF_POI = 10;

WORLD_WIDTH = 10;
WORLD_HEIGHT = 10;

RANDOM_HOME_LOCATION = true;

//  control neural nets
NUMBER_OF_LAYERS = 3;
RANDOM_WEIGHTS = true;
RANDOM_NET_MIN = -0.5;
RANDOM_NET_MAX = 0.5;


/* comparator to sort simNodes in reverse order of their weights */
struct minComparator {
	bool operator()(const Simulation& sim1, const Simulation& sim2) {  
    	return sim1.getReward() > sim2.getReward();
    }
}; 

//  return the top X performers in the epoch 
std::priority_queue<Simulation> getXMax(sim, X) {

	//  top element is the smallest of the current top X simulations
	std::priority_queue<Simulation, vector<Simulation>, minComparator> topXRewards; 
	
	// iterate through the simulations
	auto end = sim.end();
	for (auto it = sim.begin(); it != end; ++it) {
		//  add simNode immediately if there is space in PQ
		if (topXRewards.size() < X) {
			topXRewards.push(it->second);
		}
		// otherwise, replace the lowest performer with the new simulation
			//  if appropriate
		else {
			Simulation comp = topXRewards.pop();
			if (comp.getReward() > it->second.getReward()) topXRewards.push(comp);
			else topXRewards.push(it->second);
		}
	}

	return topXRewards;
}

/* run simulations for the full number of epochs, performing neuro-evolutionary
   techniques between each epoch */ 
int main(void) {

	//  set up gridworld configuration
	struct gridConfig GC;
	GC.numAgents = NUMBER_OF_AGENTS;
	GC.numPOI = NUMBER_OF_POI;
	GC.width = WORLD_WIDTH;
	GC.height = WORLD_HEIGHT;
	GC.randHome = RANDOM_HOME_LOCATION;

	//  set up initial net configuration
	struct netConfig NC;
	NC.net_type = FANN::layer;
	NC.num_layers = NUMBER_OF_LAYERS;
	NC.layers = {13, 8, 6};
	NC.randWeights = RANDOM_WEIGHTS;
	NC.randMin = RANDOM_NET_MIN; 
	NC.randMax = RANDOM_NET_MAX;

	std::map<int, Simulation> simulations;

	// initiaize simulations
	for (int i = 0; i < NUM_SIMULATIONS; i++) {
		simulations[i] = Simulation(GC, NC, MAX_STEPS);
	}

	//  for each learning epoch
	for (int i = 0; i < NUM_EPOCHS; i++) {
		
		//  run each simulation
		for (int j = 0; j < NUM_SIMULATIONS; j++) {
			simulations[j].runEpoch();
		}

		//  find the top X performers in the epoch
		std::priority_queue<Simulation> maxSet = getXMax(simulation, X_TOP_PERFORMERS);

		//  NEUROEVOLUTION!
		//  keep the nets of the best-performing simulations intact 
		//  mutate the other simulations' nets
		for (int j = 0; j < NUM_SIMULATIONS; j++) {
			if (!maxSet.contains(simulations[j])) simulations[j].mutate();
			simulations[j].reset();
		}
	}

	return 0;
}


//  simulations.mutate()
//  simulations.reset()
//  simulation.getReward()

