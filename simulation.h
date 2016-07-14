#ifndef _SIMULATION_H
#define _SIMULATION_H

/* Classes */
#include "agent.h"
#include "gridworld.h"
#include "Home.h"
#include "poi.h"
#include "position.h"
#include "state.h"

/* FANN */
#include <fann.h>
#include <fann_cpp.h>

/* Libraries */
#include <string>
#include <iostream>
#include <fstream>

struct gridConfig
{
	int numAgents;
	int numPOI;
	int width;
	int height;
	int poiWeight;
};

struct netConfig
{
	FANN::network_type_enum net_type;
	unsigned int num_layers;
	unsigned int *layers;
	bool randWeights;
	float randMin;
	float randMax;
};



class Simulation
{
	private:
		Gridworld world;
		FANN::neural_net* net;
		int timesteps;
		//double stats[];
		double reward;


	public:
		Simulation();
		~Simulation();
		Simulation(const Simulation&);
		Simulation(struct gridConfig, struct netConfig, int timesteps);
		void logResults();
		void generateStats();
		FANN::neural_net* getNet();
		void saveModel();
		int runEpoch();
		double getReward() const;
		void reset();
		void mutate();
		void destroyNet();
		void recreateNet(FANN::neural_net*);
		void printGrid();
		bool operator<(const Simulation &) const;
		Simulation& operator=(const Simulation&);
};

#endif
