/*********************************************************************
* simulation.cpp
*
* This class represents a single simulation, which can manipulate and 
* step a gridworld for a full epoch.
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

#include "simulation.h"
#include <iostream>
#include <cassert>
#include <map>

/* Calls the non-default constructors on the members with predetermined, 
 * almost arbitrary values */
Simulation::Simulation()
{
	this->reward = 0;
	this->timesteps = 250;
}

/* This non default constructor uses the information provided by the configuration structs 
 * to call the subsequent non default constructors for the members */
Simulation::Simulation(struct gridConfig GC, struct netConfig NC, int timesteps, int K)
{

	this->K = K;
	FANN::neural_net* newNet;
	//std::cout << "Call to non default constructor" << std::endl;
	for (int i = 0; i < GC.numAgents*K; i++) {
		newNet = new FANN::neural_net(NC.net_type, NC.num_layers, NC.layers);
		if (NC.randWeights) { newNet->randomize_weights(NC.randMin, NC.randMax); }
		this->nets.push_back(newNet);
	}

	this->timesteps = timesteps; 
	this->reward = 0;
	this->GC = GC;
	this->NC = NC;
}

// destructor
Simulation::~Simulation()
{
	//std::cout << "Call to destructor" << std::endl;
	for (auto it = nets.begin(); it != nets.end(); ++it) {
		delete *(it);
		*(it) = NULL;
	}
}

//  copy constructor
Simulation::Simulation(const Simulation& that)
{
	//std::cout << "call to copy constructor" << std::endl;
	this->reward = that.reward;
	this->timesteps = that.timesteps;

	for (auto it = this->nets.begin(); it != this->nets.end(); ++it) {
		if (*(it) != NULL) delete *(it);
	}

	FANN::neural_net* copyNet;
	//  it is a pointer to a pointer
	for (auto it = this->nets.begin(); it != this->nets.end(); ++it) {
		copyNet = new FANN::neural_net((struct fann*) *it);
		this->nets.push_back(copyNet);
	}

}

// copy assignment operator
Simulation& Simulation::operator=(const Simulation& that)
{
	//std::cout << "Call to equal operator" << std::endl;
	this->reward = that.reward;
	this->timesteps = that.timesteps;

	for (auto it = this->nets.begin(); it != this->nets.end(); ++it) {
		if (*it != NULL) delete *(it);
	}

	FANN::neural_net* copyNet;
	//  it is a pointer to a pointer
	for (auto it = this->nets.begin(); it != this->nets.end(); ++it) {
		copyNet = new FANN::neural_net( (struct fann*) *it);
		this->nets.push_back(copyNet);
	}

    return *this;
}

void Simulation::doublePopulation() {

	FANN::neural_net* mutateNet;
	for (auto it = this->nets.end(); it >= this->nets.begin(); --it) {
		mutateNet = this->mutate(*it);
		this->nets.push_back(mutateNet);
	}
}

void Simulation::evaluate() {

	std::map<double, int> rewardMap;

	this->doublePopulation();
	std::random_shuffle(this->nets.begin(), this->nets.end());

	for (int i = 0; i < 2*K; i += GC.numAgents) {

		double rewards[];
		int index;
		FANN::neural_net** netTeam;

		for (int j = 0; j < numAgents; j++) {
			netTeam[j] = this->nets.at(i + j);
		}

		//  run 2k epochs and collect rewards from each agent/net
		Gridworld world = Gridworld(GC, netTeam);
		this->runEpoch(world);
		rewards = world.accumulateRewards();   //  rewards 0 - numagents-1 reward 0 corresponds with net i+k

		for (int track = 0; track < numAgents; track++) {
			//  map associates net reward with net index in vector
			rewardMap.push_back(rewards[track], i + track);
		}
	}

	// sort using a custom function object
    struct {
        bool operator()(double a, double b)
        {   
            return a < b;
        }   
    } customComp;

	std::sort(rewardMap.begin(), rewardMap.end(), customComp);
	int index;
	for (auto it = rewardMap.begin(); it != rewardMap.end(); ++it) {
		while (this->nets.size() > K*GC.numAgents) {
			index = it->second;
			delete *this->nets.at(index);
			*this->nets.at(index) = NULL;
			this->nets.erase(index);
		}
	}

	assert(this->nets.size() == K*GC.numAgents);
}

/* Runs a single epoch, which runs for a given number of timesteps */
// runs the simulation until the time runs out or the simulation ends prematurely
void Simulation::runEpoch(Gridworld world)
{
	int steps = 0;

	for (steps = 0; steps < this->timesteps; ++steps)
	{
		//while (steps < 15) this->world.printWorld();

		world.stepAgents();

		if (world.worldComplete())
		{
			world.printWorld();
			break;
		}

	}

}

/* Performs an in place mutation of a single weight in the given network */
FANN::neural_net* Simulation::mutate(FANN::neural_net* net)
{
	double percent = .1;
	int length = net->get_total_connections();
	int numMutations = percent*length;

	FANN::neural_net* copy = new FANN::neural_net();

	FANN::connection connections[length];
	net->get_connection_array(connections);

	/* mutate a percentage of random weight */
	for (int i = 0; i < numMutations; i++) {
		int index = rand() % length;
		int sign = rand() % 2 ? 1 : -1;
		fann_type current = connections[index].weight;
		double random = ((double)rand()/((double)RAND_MAX+1.0))/10.0;

		connections[index].weight += (fann_type) sign*random*current;
	}

	copy->set_weight_array(connections, length);

	return copy;
}


