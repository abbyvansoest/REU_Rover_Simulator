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
		Gridworld world;        //  the simulation's gridworld
		int timesteps;          //  max timesteps in an epoch
		double reward;          //  most recent reward received

	public:
		//  constructors, destructor, copy constructor
		Simulation();
		Simulation(struct gridConfig, struct netConfig, int timesteps, std::string pickupNet);
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
		int runEpochAndPrint();
		//  how many steps the simulation took to complete in the last epoch
		int getSteps();
		//  return the simulation's reward from the last epoch
		double getReward();
		// reset the simulation
		void reset();
		//  in place mutation of the simulation's neural net
		void mutate(double);
		//  return the current amount at the grid's home base
		int amountReturned();
		
};

#endif
