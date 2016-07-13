#include "simulation.h"
#include <iostream>

/* Calls the non-default constructors on the members with predetermined, 
 * almost arbitrary values */
Simulation::Simulation()
	: world(2, 1, 5, 5, false)
{
	this->net = new FANN::neural_net(FANN::LAYER, 3, (const unsigned int[]) {13,9,6});
	this->reward = 0;
	this->timesteps = 250;
}

/* This non default constructor uses the information provided by the configuration structs 
 * to call the subsequent non default constructors for the members */
Simulation::Simulation(struct gridConfig GC, struct netConfig NC, int timesteps)
	: world(GC.numAgents, GC.numPOI, GC.width, GC.height, GC.randHome)
	
{

	this->net = new FANN::neural_net(NC.net_type, NC.num_layers, NC.layers);

	if (NC.randWeights) { this->net->randomize_weights(NC.randMin, NC.randMax); }
	this->timesteps = timesteps;
	std::cout << "INIT WORLD" << std::endl;
	world.printWorld();

}

// destructor
Simulation::~Simulation() {
	delete this->net;
}

//  copy constructor
Simulation::Simulation(const Simulation& that) {

	this->net = new FANN::neural_net(*that.net);
	this->reward = that.reward;
	this->world = that.world;
	this->timesteps = that.timesteps;
}

// copy assignment operator
Simulation& Simulation::operator=(const Simulation& that)
{
    this->net = new FANN::neural_net(*that.net);
	this->reward = that.reward;
	this->world = that.world;
	this->timesteps = that.timesteps;
    return *this;
}

void Simulation::logResults()
{
	std::cout << "Reward: " << this->getReward() << 
	"    Returned: " << this->world.currentAmount() << std::endl;
	if (this->world.currentAmount() > 0)
	{
		std::cout << "SUCCESS OF SOME SORT!" << std::endl;
	}
	std::cout << std::endl;
}

void Simulation::generateStats()
{

}

FANN::neural_net* Simulation::getNet() {
	return this->net;
}

/* Runs a single epoch, which runs for a given number of timesteps */
int Simulation::runEpoch()
{
	// Run the simulation until the time runs out or the simulation ends prematurely
	int steps = 0;
	for (; steps < this->timesteps; ++steps)
	{
		this->world.stepAgents(this->net);
		if (this->world.worldComplete())
			break;
	}


	// Calculate the reward
	this->reward -= steps * 0.05;
	this->reward += this->world.currentAmount();
	std::cout << "Reward: " << this-> reward << " from " << steps << " steps and " << this->world.currentAmount() << " POI found" << std::endl;

	return 0;
}

double Simulation::getReward() const 
{
	return this->reward;
}

/* Resets the gridworld and the statistics variables internal to the class */
void Simulation::reset(bool randHome)
{
	this->world.reset(randHome);
	// Reset any statistics variables here
	this->reward = 0;
}

void Simulation::destroyNet() {
	delete this->net;
	this->net = NULL;
}

void Simulation::recreateNet(FANN::neural_net* net) {
	if (this->net == NULL) {
		this->net = new FANN::neural_net(*net);
	}
}

/* Performs an in place mutation of a single weight in the given network */
void Simulation::mutate()
{
	int length = this->net->get_total_connections();

	FANN::connection connections[length];
	this->net->get_connection_array(connections);
	/* mutate a random weight */
	int index = rand() % length;
	int sign = rand() % 2 ? 1 : -1;
	fann_type magnitude = 1/((fann_type) ((rand() % 50) + 10));

	std::cout << "mag: " << magnitude << std::endl;
	connections[index].weight += (fann_type) sign*magnitude;

	this->net->set_weight_array(connections, length);
}

bool Simulation::operator<(const Simulation &rhs) const
{
	return this->reward < rhs.reward;
}
