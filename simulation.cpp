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
#include <map>

/* Calls the non-default constructors on the members with predetermined, 
 * almost arbitrary values */
Simulation::Simulation()
{
	this->reward = 0;
	this->timesteps = 250;
	this->avg = 0;
	this->globalAvg = 0;
	this->completed = 0;
	this->comp_total = 0;
}

/* This non default constructor uses the information provided by the configuration structs 
 * to call the subsequent non default constructors for the members */
Simulation::Simulation(struct gridConfig GC, struct netConfig NC, int timesteps, int K, int proj)
{

	this->K = K;
	this->avg = 0;
	this->globalAvg = 0;
	this->completed = 0;
	this->comp_total = 0;
	this->projection = proj;
	//std::cout << "Call to non default constructor" << std::endl;
	//std::cout << "OG nets " << GC.numAgents*K << std::endl;
	for (int i = 0; i < GC.numAgents*K; i++) {
		FANN::neural_net* newNet = new FANN::neural_net(NC.net_type, NC.num_layers, NC.layers);
		if (NC.randWeights) { newNet->randomize_weights(NC.randMin, NC.randMax); }
		this->nets.push_back(newNet);
		//std::cout << newNet << std::endl;
	}

//	std::cout << "net size " << this->nets.size() << std::endl;

	this->pickupNet = new FANN::neural_net();
	bool success = this->pickupNet->create_from_file("Pickup.net");
	if (!success) {
		std::cout << "ERROR BUILDING PICKUP NET" << std::endl;
		exit(0);
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
	delete this->pickupNet;
	this->pickupNet = NULL;
}

//  copy constructor
Simulation::Simulation(const Simulation& that)
{
	//std::cout << "call to copy constructor" << std::endl;
	this->reward = that.reward;
	this->timesteps = that.timesteps;
	this->avg = that.avg;

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
	this->avg = that.avg;

	for (auto it = this->nets.begin(); it != this->nets.end(); ++it) {
		if (*it != NULL) delete *(it);
	}

	//  it is a pointer to a pointer
	for (auto it = this->nets.begin(); it != this->nets.end(); ++it) {
		FANN::neural_net* copyNet = new FANN::neural_net( (struct fann*) *it);
		this->nets.push_back(copyNet);
	}

    return *this;
}

void Simulation::doublePopulation() {

	FANN::neural_net* mutateNet;
	//std::cout << "mutate nets" << std::endl;

	this->nets.reserve((this->nets.size() * 2) + 1);
	auto it = this->nets.begin();
	int size = this->nets.size();
	for (int i = 0; i < size; ++i) {
		mutateNet = this->mutate(*it);
		this->nets.push_back(mutateNet);
		++it;
	}
}

void Simulation::evaluate(std::ofstream &reward_file, std::ofstream &complete_file) {

	std::vector<std::pair<double, int>> rewardVector;
	Gridworld world = Gridworld(GC, NULL);

	this->completed = 0;

	this->doublePopulation();
	std::random_shuffle(this->nets.begin(), this->nets.end());

	for (int i = 0; i < 2*K*GC.numAgents; i += GC.numAgents) {

		FANN::neural_net* netTeam[GC.numAgents];
		for (int j = 0; j < GC.numAgents; j++) {
			// the jth pointer points to the address of the net at i+j
			netTeam[j] = this->nets.at(i + j);
		}

		// run 2k epochs and collect rewards from each agent/net
		world = Gridworld(GC, netTeam);
		this->runEpoch(&world);
		std::vector<double> rewards = world.accumulateRewards();   
		// rewards 0 thru numagents-1 reward 0 corresponds with net i+k

		for (int track = 0; track < GC.numAgents; track++) {
			// map associates net reward with net index in vector
			rewardVector.push_back(std::pair<double, int>(rewards.at(track), i + track));
		}
	}

	// sort by reward (smallest in front)
	std::sort(rewardVector.begin(), rewardVector.end());

	int index;
	int count = 0;
	this->avg = 0;
	std::cout << this->nets.size() <<  " VS " << K*GC.numAgents  << " VS " << rewardVector.size()<< std::endl;
	for (auto it = rewardVector.begin(); it != rewardVector.end(); ++it) {
		std::cout << "reward " << it->first << std::endl;
		this->avg += it->first;
		// free the memory first
		if (count < rewardVector.size()/2) {
			index = it->second;
			delete this->nets[index];
			this->nets[index] = NULL;
			++count;
		}
	}

	//then loop through and remove the instances in a safe manner, so indexes dont get corrupted
	std::cout << count << std::endl;
	count = 0;
	for (auto it = this->nets.begin(); it != this->nets.end(); )
	{
		if (*it == NULL)
		{
			++count;
			it = this->nets.erase(it);
		}
		else
		{
			++it;
		}
	}

	std::cout << count << std::endl;
	this->avg /= (2*K*GC.numAgents);
	std::cout << "Avg: " << this->avg << "\t" << " Max: " << (--rewardVector.end())->first << "\tCompleted/total: " << this->completed << "/" << this->comp_total << std::endl;
	std::cout << this->nets.size() <<  " VS " << K*GC.numAgents << std::endl;
	assert(this->nets.size() == K*GC.numAgents);
	complete_file << this->completed << std::endl;
	reward_file << (--rewardVector.end())->first << std::endl;
}

void Simulation::evaluate() {

	std::vector<std::pair<double, int>> rewardVector;
	Gridworld world = Gridworld(GC, NULL);

	this->completed = 0;

	this->doublePopulation();
	std::random_shuffle(this->nets.begin(), this->nets.end());

	for (int i = 0; i < 2*K*GC.numAgents; i += GC.numAgents) {

		FANN::neural_net* netTeam[GC.numAgents];
		for (int j = 0; j < GC.numAgents; j++) {
			// the jth pointer points to the address of the net at i+j
			netTeam[j] = this->nets.at(i + j);
		}

		// run 2k epochs and collect rewards from each agent/net
		world = Gridworld(GC, netTeam);
		this->runEpoch(&world);
		std::vector<double> rewards = world.accumulateRewards();   
		// rewards 0 thru numagents-1 reward 0 corresponds with net i+k

		for (int track = 0; track < GC.numAgents; track++) {
			// map associates net reward with net index in vector
			rewardVector.push_back(std::pair<double, int>(rewards.at(track), i + track));
		}
	}

	// sort by reward (smallest in front)
	std::sort(rewardVector.begin(), rewardVector.end());

	int index;
	int count = 0;
	this->avg = 0;
	std::cout << this->nets.size() <<  " VS " << K*GC.numAgents  << " VS " << rewardVector.size()<< std::endl;
	for (auto it = rewardVector.begin(); it != rewardVector.end(); ++it) {
		std::cout << "reward " << it->first << std::endl;
		this->avg += it->first;
		// free the memory first
		if (count < rewardVector.size()/2) {
			index = it->second;
			delete this->nets[index];
			this->nets[index] = NULL;
			++count;
		}
	}

	//then loop through and remove the instances in a safe manner, so indexes dont get corrupted
	std::cout << count << std::endl;
	count = 0;
	for (auto it = this->nets.begin(); it != this->nets.end(); )
	{
		if (*it == NULL)
		{
			++count;
			it = this->nets.erase(it);
		}
		else
		{
			++it;
		}
	}

	std::cout << count << std::endl;
	this->avg /= (2*K*GC.numAgents);
	std::cout << "Avg: " << this->avg << "\t" << " Max: " << (--rewardVector.end())->first << "\tCompleted/total: " << this->completed << "/" << this->comp_total << std::endl;
	std::cout << this->nets.size() <<  " VS " << K*GC.numAgents << std::endl;
	assert(this->nets.size() == K*GC.numAgents);

}

double Simulation::getAvg(int num_epochs) { return this->globalAvg/(double)num_epochs; }

/* Runs a single epoch, which runs for a given number of timesteps */
// runs the simulation until the time runs out or the simulation ends prematurely
void Simulation::runEpoch(Gridworld* world) {

	int steps = 0;
	std::vector<double> sValues;

	for (steps = 0; steps < this->timesteps; ++steps) {

		//std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
		//std::cout << "timestep " << steps << std::endl;
		//  cacluate set of S values (contains info on distance to closest POI)
			// based on expected locations of agents in next timestep
		sValues = world->calculateS(this->projection);
		world->stepAgents(this->pickupNet, sValues);

		 if (world->worldComplete()) {

		 	++this->completed;
			++this->comp_total;
		 	//world->printWorld();
		 	break;

		 }
	}
}

/* Performs a mutation of 10% of the weights in the network, 
 * and returns a new, mutated version of the net parameter.
 * The origional is not changed.*/

FANN::neural_net* Simulation::mutate(FANN::neural_net* net)
{
	double percent = .1;
	int length = net->get_total_connections();
	int numMutations = percent*length;

	FANN::neural_net* copy = new FANN::neural_net(*net);

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

//  reset simulation for next epoch
void Simulation::reset() {

	this->reward = 0;

}


