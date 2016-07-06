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

struct gridConf
{
	int numAgents;
	int numPOI;
	int width;
	int height;
	bool randHome;
};

struct netConf
{
	FANN::network_type_enum net_type;
	unsigned int num_layers;
	const unsigned int *layers;
	bool randWeights;
	float randMin;
	float randMax;
};



class Simulation
{
	private:
		Gridworld world;
		FANN::neural_net net;
		int timesteps;
		//double stats[];


	public:
		Simulation();
		Simulation(struct gridConf, struct netConf, int timesteps);
		void logResults();
		void generateStats();
		void saveModel();
		int runEpoch();


};

#endif
