/*********************************************************************
* controller.cpp
*
* This simulation controller implements a rover domain gridworld for 
* theoretical research purposes. It is the top-level manager of a 
* large number of experimental simulations, and uses neuroevolutionary 
* techniques on neural nets.
*
* Copyright (C) 2016 Abby Van Soest, Connor Yates
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/

#include "controller.h"
#include <cassert>


//  return the fitness for the provided simulation
int fitness(Simulation sim, int max_steps) {
	return 2*(sim.amountReturned()*sim.amountReturned()) + (int)(max_steps/sim.getSteps());
}

//  return a semi-random index between 0 and NUM SIMULATIONS - 1
//  more likely to return indices at the upper end of the array
int getIndex(int numSims, std::vector<Simulation> simulations, int max_steps) {

	std::vector<int> probabilities;

	for (int i = 0; i < numSims; i++) {
		int fit = fitness(simulations[i], max_steps);
		for (int j = 0; j < fit; j++) {
			probabilities.push_back(i);
		}
	}

	int random = rand() % probabilities.size();

	return probabilities.at(random);
}

/* run simulations for the full number of epochs, performing neuro-evolutionary
   techniques between each epoch */ 
int main(void) {
	//  control experiment data collection
	int MAX_STEPS = 800;
	int NUM_SIMULATIONS = 100;
	int NUM_EPOCHS = 100000;
	int X_TOP_PERFORMERS = 10;
	double MUTATION_RATE = .1;  //  number of connections to mutate within a net
	double PERCENT = .1; 		//  percent of total simulations to mutate

	//  control gridworld
	int NUMBER_OF_AGENTS = 3;
	int NUMBER_OF_POI = 3;

	int WORLD_WIDTH = 6;
	int WORLD_HEIGHT = 6;

	int POI_WEIGHT = 2;

	//  control neural nets
	int NUMBER_OF_LAYERS = 4;
	bool RANDOM_WEIGHTS = true;
	double RANDOM_NET_MIN = -0.5;
	double RANDOM_NET_MAX =  0.5;

	//  set up gridworld configuration
	struct gridConfig GC;
	GC.numAgents = NUMBER_OF_AGENTS;
	GC.numPOI = NUMBER_OF_POI;
	GC.width = WORLD_WIDTH;
	GC.height = WORLD_HEIGHT;
	GC.poiWeight = POI_WEIGHT;

	//  set up initial net configuration
	struct netConfig NC;
	NC.net_type = FANN::LAYER;
	NC.num_layers = NUMBER_OF_LAYERS;
	NC.layers = new unsigned int[NUMBER_OF_LAYERS];
	NC.layers[0] = 9;
	NC.layers[1] = 18;
	NC.layers[2] = 9;
	NC.layers[3] = 4;
	NC.randWeights = RANDOM_WEIGHTS;
	NC.randMin = RANDOM_NET_MIN;
	NC.randMax = RANDOM_NET_MAX;

	srand(time(NULL));

	std::string pickupNet = "Pickup.net";

	std::vector<Simulation> simulations(NUM_SIMULATIONS, Simulation(GC, NC, MAX_STEPS, pickupNet));
	//  randomize all nets
	FANN::neural_net* net;
	for (auto it = simulations.begin(); it != simulations.end(); ++it) {
		net = it->getNet();
		net->randomize_weights(RANDOM_NET_MIN, RANDOM_NET_MAX);
	}
	//  run each simulation to initialize rewards
	for (int j = 0; j < NUM_SIMULATIONS; j++) {
		simulations[j].runEpoch();
	}

	//  for each learning epoch, we run around 10% of the set of simulations and 
	//  then evolve the population 
	int totalfailures = 0;
	int something = 0;
	std::cout << std::endl;
	for (int i = 0; i < NUM_EPOCHS; i++) {

		double avg = 0.0;
		double max = -100000.0;
		double avgSteps = 0.0;

		std::cout << "**********************************" << std::endl;
		std::cout << "EPOCH " << i << std::endl;

		//  mutate some new simulations and add them to the simulations population
		for (int j = 0; j < (int)NUM_SIMULATIONS*PERCENT; j++) {

			int index = 0;
			if (simulations[0].getReward() == simulations[NUM_SIMULATIONS - 1].getReward()) {
				index = rand() % NUM_SIMULATIONS;
			}
			else {
				index = getIndex(NUM_SIMULATIONS, simulations, MAX_STEPS);
			}
			Simulation sim = Simulation(simulations[index]);
			sim.reset();
			sim.mutate(MUTATION_RATE);
			sim.runEpoch();

			simulations.push_back(sim);
		}

		//  remove the lowest performing simulations
		std::sort(simulations.begin(), simulations.end());
		auto loser = simulations.begin();
		simulations.erase(loser, loser + (int)(NUM_SIMULATIONS*PERCENT));

		//  track statistics
		int track = 0;
		for (auto sim = simulations.begin(); sim != simulations.end(); ++sim) {
			std::cout << "REWARD " << track <<  " is " << sim->getReward() << "\tSTEPS: " << sim->getSteps() << std::endl;
			track++;
			if (max < sim->getReward()) {
				max = sim->getReward();
			}
			avg += sim->getReward();
			avgSteps += sim->getSteps();
		}		

		avg /= NUM_SIMULATIONS;
		avgSteps /= NUM_SIMULATIONS;

		std::cout << "EPOCH AVERAGE " << avg << "\tMAX: " << max << " Avg steps: " << avgSteps << std::endl;//"\tat: " << max_i << std::endl;
		std::cout << std::endl;

		if (max == -100) totalfailures++;
		else something++;

		if (MUTATION_RATE > 0) MUTATION_RATE -= 0.001;
	}

	std::cout  << "TOTAL FAILURES: " << totalfailures << "\tSOME SORT OF SUCCESS: " << something << std::endl; 

	/* Cleanup configuration memory */
	delete [] NC.layers;

	return 0;
}




