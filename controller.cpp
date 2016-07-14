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

/* takes in two parents to produce the child */
FANN::neural_net* crossover(FANN::neural_net *p1, FANN::neural_net *p2)
{
	/* setup memory and get the connections out of the net */
	FANN::neural_net *net = new FANN::neural_net(*p1);

	FANN::connection c1[p1->get_total_connections()];
	FANN::connection c2[p2->get_total_connections()];
	FANN::connection crossover[p2->get_total_connections()];
	p1->get_connection_array(c1);
	p2->get_connection_array(c2);
	net->get_connection_array(crossover);

	int split = rand() % p1->get_total_connections();
	/* Extract values from connections */
	int i;
	for (i = 0; i < split; ++i)
	{
		crossover[i].weight = c1[i].weight;
	}
	for (i = split; i < p1->get_total_connections(); ++i)
	{
		crossover[i].weight = c2[i].weight;
	}

	net->set_weight_array(crossover, p1->get_total_connections());

	return net;
}

void evolve_population(std::vector<Simulation> &simulations, int X, float mutation_rate)
{

	//  sort to find the top X performers
	std::sort(simulations.begin(), simulations.end());

	//  delete the nets of the lowest performing half of the population
	int halfPop = 0.5*(simulations.size());
	for (auto it = simulations.begin(); it != simulations.end() - halfPop; ++it) {
		it->destroyNet();
		int random = (rand() % X) + 1;
		it->recreateNet((simulations.end() - random)->getNet());

		/*
		int p1 = (rand() % X) + simulations.size() - X;
		int p2 = (rand() % X) + simulations.size() - X;
		//std::cout << "p1: " << p1 << " : p2: " << p2 << std::endl;
		FANN::neural_net* new_net = crossover((simulations.begin()+p1)->getNet(), (simulations.begin()+p2)->getNet());
		it->setNet(new_net);
		*/
	}

	for (auto it = simulations.begin(); it != simulations.end(); ++it)
	{
		if (( (double)rand()/(double)RAND_MAX+1.0) < mutation_rate)
		{
			it->mutate();
		}
	}

}

/* run simulations for the full number of epochs, performing neuro-evolutionary
   techniques between each epoch */ 
int main(void) {
	//  control experiment data collection
	int MAX_STEPS = 250;
	int NUM_SIMULATIONS = 100;
	int NUM_EPOCHS = 100000;
	int X_TOP_PERFORMERS = 10;
	int Y_MUTATIONS = 100;
	double mutation_rate = .1;

	//  control gridworld
	int NUMBER_OF_AGENTS = 2;
	int NUMBER_OF_POI = 2;

	int WORLD_WIDTH = 3;
	int WORLD_HEIGHT = 3;

	int POI_WEIGHT = 2;

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
	GC.poiWeight = POI_WEIGHT;

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

	srand(time(NULL));

	std::vector<Simulation> simulations(NUM_SIMULATIONS, Simulation(GC, NC, MAX_STEPS));

	//  for each learning epoch, we run the set of simulations and 
	//  then evolve the population based on basic neuroevolutionary 
	//  algorithms.
	std::cout << std::endl;
	for (int i = 0; i < NUM_EPOCHS; i++) {

		double avg = 0.0;
		double max = 0.0;

		std::cout << "EPOCH " << i << std::endl;
		std::cout << "**********************************" << std::endl;

		//  run each simulation
		for (int j = 0; j < NUM_SIMULATIONS; j++) {
			//std::cout << "------------------------------------" << std::endl;
			simulations[j].runEpoch();
			//std::cout << "simulation " << j << "   ";
			//simulations[j].logResults();
			avg += simulations[j].getReward();
			max = (max > simulations[j].getReward()) ? max : simulations[j].getReward();
		}

		avg /= NUM_SIMULATIONS;
		std::cout << "EPOCH AVERAGE " << avg << "\tMAX: " << max << std::endl;

		evolve_population(simulations, X_TOP_PERFORMERS, mutation_rate);
		for (auto it = simulations.begin(); it != simulations.end(); ++it)
		{
			it->reset();
		}
		if (mutation_rate > 0 )
			mutation_rate -= 0.001;
	}

	/* Cleanup configuration memory */
	delete [] NC.layers;

	return 0;
}




