/*********************************************************************
* simulation.h
*
* This class represents a single simulation of a gridworld. The simulation
* runs an epoch, tracks a certain gridworld and steps its agents for a 
* given number of steps.
* 
* Copyright (C) 2016 Abby Van Soest, Connor Yates
*********************************************************************/

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

// struct representation of the gridworld configuration 
struct gridConfig
{
	int numAgents;
	int numPOI;
	int width;
	int height;
	int poiWeight;
};

// struct representation of the neural net configuration 
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
		Gridworld world;        //  the simulation's gridworld
		FANN::neural_net* net;  //  pointer to the neural net used (on the heap)
		int timesteps;          //  max timesteps in an epoch
		double reward;          //  most recent reward received


	public:
		//  constructors, destructor, copy constructor
		Simulation();
		Simulation(struct gridConfig, struct netConfig, int timesteps);
		~Simulation();
		Simulation(const Simulation&);

		//  override relational operator 
		bool operator<(const Simulation &) const;
		// copy assignment operator
		Simulation& operator=(const Simulation&);

		//  print the results and statistics from the simulation
		void logResults();
		//  run the simulation in an epoch with a certain number of timesteps
		int runEpoch();
		//  how many steps the simulation took to complete in the last epoch
		int getSteps();
		//  return the simulation's reward from the last epoch
		double getReward() const;
		// reset the simulation
		void reset();
		//  in place mutation of the simulation's neural net
		void mutate(double);
		//  destroy the current neural net
		void destroyNet();
		//  recreate the neural net identical to the one given
		void recreateNet(FANN::neural_net*);
		// get a pointer to the neural net
		FANN::neural_net* getNet();
		// reset the neural net to point to a new net
		void setNet(FANN::neural_net*);
		
};

#endif
