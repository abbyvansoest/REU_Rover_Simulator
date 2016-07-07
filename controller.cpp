#include "controller.h"
#include <map>
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
RANDOM_NET_MIN = ??;
RANDOM_NET_MAX = ??;


//  return the top performers in the epoch iteration
std::priority_queue<Simulation> getXMax(sim, rewards, X) {

	//  top element is the smallest of the current top X
	// priority queue of simulations sorted by reward! 
		// need to implement that...comparators :)
	std::priority_queue<Simulation> topXRewards; 
	
	// iterate through the simulations
	for (auto it = sim.begin(); it != sim.end(); ++it) {
		//  add simulation immediately if space in PQ
		if (topXRewards.size() < X) {
			topXRewards.push(it->first);
		}
		// otherwise, replace the lowest performer with the new simulation
			//  if appropriate
		else {
			Simulation comp = topXRewards.pop();
			if (rewards(comp) > rewards(it->first)) topXRewards.push(comp);
			else topXRewards.push(it->first);
		}
	}
	return topXRewards;
}



int main(int argc, char *argv[]) {

	//  set up gridworld configuration
	struct gridConfig GC;
	GC.numAgents = NUMBER_OF_AGENTS;
	GC.numPOI = NUMBER_OF_POI;
	GC.width = WORLD_WIDTH;
	GC.height = WORLD_HEIGHT;
	GC.randHome = RANDOM_HOME_LOCATION;

	//  set up initial net configuration
	struct netConfig NC;
	NC.net_type = //  what type of net do we want?
	NC.num_layers = NUMBER_OF_LAYERS;
	NC.layers = //  what does this refer to?
	NC.randWeights = RANDOM_WEIGHTS;
	NC.randMin = RANDOM_NET_MIN; 
	NC.randMax = RANDOM_NET_MAX;

	std::map<int, Simulation> simulations;

	double reward;
	int X = X_TOP_PERFORMERS;

	// initiaize simulations
	for (int i = 0; i < NUM_SIMULATIONS; i++) {
		simulations[i] = Simulation(GC, NC, MAX_STEPS);
	}

	//  for each learning epoch
	for (int i = 0; i < NUM_EPOCHS; i++) {

		std::map<Simulation, double> rewardTable;
		
		//  run each simulation and save reward
		for (int j = 0; j < NUM_SIMULATIONS; j++) {

			reward = simulations[j].runEpoch();
			rewardTable[simulations[j]] = reward;

		}

		//  find the top X performers in the epoch
		std::priority_queue<Simulation> maxSet = getXMax(simulation, rewardTable, X);

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




//  need to find top X simulations 
//  mutate all mutations not in the top X


//  simulations.mutate()
//  simulations.reset()
//  override comparison operators in Simulation class
//  simulation.getReward() 

