/* FANN */
#include <fann.h>
#include <fann_cpp.h>

#include <string>

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


