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


// struct gridConfig
// {
// 	int numAgents;
// 	int numPOI;
// 	int width;
// 	int height;
// 	int poiWeight;
// };

// // struct representation of the neural net configuration 
// struct netConfig
// {
// 	FANN::network_type_enum net_type;
// 	unsigned int num_layers;
// 	unsigned int *layers;
// 	bool randWeights;
// 	float randMin;
// 	float randMax;
// };


/* run simulations for the full number of epochs, performing neuro-evolutionary
   techniques between each epoch */ 
int main(void) {

	int MAX_STEPS = 250;
//	int NUM_SIMULATIONS = 50;
	int NUM_EPOCHS = 10000;
//	double MUTATION_RATE = .1;  //  number of connections to mutate within a net
//	double PERCENT = .1; 		//  percent of total simulations to mutate

	//  control gridworld
	int NUMBER_OF_AGENTS = 6;
	int NUMBER_OF_POI = 4;

	int WORLD_WIDTH = 15;
	int WORLD_HEIGHT = 15;

	int POI_WEIGHT = 2;

	//  control neural nets
	int NUMBER_OF_LAYERS = 4;
	bool RANDOM_WEIGHTS = true;
	double RANDOM_NET_MIN = -10.0;
	double RANDOM_NET_MAX =  10.0;

	//  number of steps to project into the future
	int PROJECTION_STEPS = 1;

	std::string pickupFile = "Pickup.net";


	srand(time(NULL));

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

	int K = 10;

	Simulation sim = Simulation(GC, NC, MAX_STEPS, K, PROJECTION_STEPS);

	std::ofstream max_reward_gen_out;
	std::ofstream world_complete_gen_out;
	std::ofstream max_reward_calls_out;
	std::ofstream world_complete_calls_out;

	std::string file_location = "data/intent_computation/1_projection/trial_1";

	std::string filename = file_location + "_max_reward_calls.csv";
	max_reward_calls_out.open(filename);

	filename = file_location + "_world_complete_calls.csv";
	world_complete_calls_out.open(filename);

	filename = file_location + "_max_reward_gen.csv";
	max_reward_gen_out.open(filename);

	filename = file_location + "_world_complete_gen.csv";
	world_complete_gen_out.open(filename);

	if (max_reward_gen_out.fail() || world_complete_gen_out.fail()
			|| max_reward_calls_out.fail() || world_complete_calls_out.fail()) {
		std::cout << "error creating file" << std::endl;
		exit(0);
	}

	//  for each learning epoch, we run around 10% of the set of simulations and 
	//  then evolve the population 
	std::cout << std::endl;
	for (int i = 0; i < NUM_EPOCHS; i++) {

		std::cout << "**********************************" << std::endl;
		std::cout << "EPOCH " << i << std::endl;

		sim.evaluate(max_reward_gen_out, world_complete_gen_out, max_reward_calls_out, world_complete_calls_out);
		//sim.evaluate();
		sim.reset();
	}

	max_reward_gen_out.close();
	world_complete_gen_out.close();
	max_reward_calls_out.close();
	world_complete_calls_out.close();

	std::cout << "GLOBAL AVERAGE " << sim.getAvg(NUM_EPOCHS) << std::endl;

	/* Cleanup configuration memory */
	delete [] NC.layers;

	return 0;
}




