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


class Simulation
{
	private:
		int timesteps;          //  max timesteps in an epoch
		double reward;          //  most recent reward received
		std::vector<FANN::neural_net*> nets;
		FANN::neural_net* pickupNet;  //  globally shared pickup decision net
		int K;
		struct gridConfig GC;
		struct netConfig NC;
		double avg;
		double globalAvg;
		int completed;
		int comp_total;
		int projection;

	public:
		//  constructors, destructor, copy constructor
		Simulation();
		Simulation(struct gridConfig, struct netConfig, int, int, int);
		~Simulation();
		Simulation(const Simulation&);

		// copy assignment operator
		Simulation& operator=(const Simulation&);

		//  evaluate the neural nets in a simulation
			// option 1 = no data file generation
			// option 2 = data file generation
		void evaluate();
		void evaluate(std::ofstream&, std::ofstream&, std::ofstream&, std::ofstream&);
		//  run the simulation in an epoch with a certain number of timesteps
		void runEpoch(Gridworld*);
		//  in place mutation of the simulation's neural net
		void mutate(double);
		//  return the current amount at the grid's home base
		int amountReturned();

		void doublePopulation();

		FANN::neural_net* mutate(FANN::neural_net*);

		//  reset simulation for next epoch
		void reset();

		double getAvg(int);
		
};

#endif
