#include <iostream>
#include <fann.h>
#include <fann_cpp.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>

enum
{
	AGENTS_A,
	POI_A,
	AGENTS_B,
	POI_B,
	AGENTS_C,
	POI_C,
	AGENTS_D,
	POI_D,
	CARRYING
};

char** createWorld_rand(int num_agents, int num_poi, int size) 
{
	// allocate memory
	char **world = new char*[size];
	for (int i = 0; i < size; ++i) { world[i] = new char[size]; }

	// populate the world
	for (int i = 0; i < size; ++i)
	{

		for (int j = 0; j < size; ++j)
		{
			world[i][j] = '.';
		}
	}
	int i = 0;
	while (i < num_agents)
	{
		// generate random points
		int x = random() % size;
		int y = random() % size;
		if (world[x][y] == '.')
		{
			world[x][y] = 'A';
			++i;
		}
	}
	i = 0;
	while (i < num_poi)
	{
		// generate poi
		int x = random() % size;
		int y = random() % size;
		if (world[x][y] == '.')
		{
			world[x][y] = 'P';
			++i;
		}
	}

	return world;
}

char **createWorld_fail(int num_agents, int num_poi, int size)
{
	char **world = new char*[size];
	for (int i = 0; i < size; ++i) { world[i] = new char[size]; }
	
	for (int i = 0; i < size; ++i)
	{

		for (int j = 0; j < size; ++j)
		{
			world[i][j] = '.';
		}
	}
	
	int i = 0;
	while (i < num_agents)
	{
		// generate random points
		int x = random() % size;
		int y = random() % size;
		if (world[x][y] == '.')
		{
			world[x][y] = 'A';
			++i;
		}
	}

	i = 0;
	while (i < num_poi)
	{
		// generate random points
		int x = random() % size;
		int y = random() % size;
		if (world[x][y] == '.')
		{
			if (x+1 < size && world[x+1][y] == '.') {
				if (x-1 >=0 && world[x-1][y] == '.') {
					if (y+1 < size && world[x][y+1] == '.') {
						if (y-x <= 0 && world[x][y-1] == '.') {
							world[x][y] = 'P';
							++i;
						}
					}
				}
			}
		}
	}

	return world;
}

char **createWorld_success(int num_agents, int num_poi, int size, int observe_requirement)
{
	char **world = new char*[size];
	for (int i = 0; i < size; ++i) { world[i] = new char[size]; }
	
	for (int i = 0; i < size; ++i)
	{

		for (int j = 0; j < size; ++j)
		{
			world[i][j] = '.';
		}
	}

	
	
	int i = 0;
	int num_agents_total = 0;
	while (i < num_poi)
	{
		int x = random() % size;
		int y = random() % size;
		if (world[x][y] == '.')
		{
			world[x][y] = 'P';
			++i;
			int agents_on_poi = 0;
			while (agents_on_poi < observe_requirement && num_agents_total < num_agents)
			{
				int dx = x, dy = y;
				int c = rand() % 4;
				switch (c)
				{
					case 0: dx = x+1; break;
					case 1: dx = x-1; break;
					case 2: dy = y+1; break;
					case 3: dy = y-1; break;
					default: std::cout << "Rand broke..." << std::endl;
				}
				//std::cout << "x, y, dx, dy" << x << y << dx << dy << std::endl;
				if (dx >= 0 && dx < size && dy >= 0 && dy < size)
				{
					if (world[dx][dy] == '.')
					{
						world[dx][dy] = 'A';
						++agents_on_poi;
						++num_agents_total;
					}
				}
			}
		}
	}

	while (num_agents_total < num_agents)
	{
		// generate random points
		int x = random() % size;
		int y = random() % size;
		if (world[x][y] == '.')
		{
			world[x][y] = 'A';
			++i;

		}
	}
	return world;
}
void print(char **world, int size)
{
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			std::cout << world[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

struct pos { int x;  int y;};

fann_type distance(struct pos& a, struct pos& b)
{
	fann_type deltaX = a.x - b.x;
	fann_type deltaY = a.y - b.y;
	return sqrt(deltaX*deltaX + deltaY*deltaY);
}

void gen_states(fann_type **states, char ** world, int size)
{
	fann_type agentsA = 0.0, agentsB = 0.0, agentsC = 0.0, agentsD = 0.0;
	fann_type poiA = 0.0, poiB = 0.0, poiC = 0.0, poiD = 0.0;
	std::vector<struct pos> agent_positions;
	std::vector<struct pos> poi_positions;
	int temp_state[9] = {0, 9};
	// loop through world untill agent shows up
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			if (world[i][j] == 'A') {
				struct pos p = {i, j};
				agent_positions.push_back(p);
			}
			else if (world[i][j] == 'P') {
				struct pos p = {i, j};
				poi_positions.push_back(p);
			}
		}
	}

	int index = 0;
	for (auto it = agent_positions.begin(); it != agent_positions.end(); ++it)
	{
		// zero out the temp state;
		for (int i = 0; i < 9; ++i) { temp_state[i] = 0; }
		for (auto it_other = agent_positions.begin(); it_other != agent_positions.end(); ++it_other)
		{
			if (it_other != it)
			{
				if (it_other->x < it->x && it_other->y >= it->y) {
					agentsA += 1.0/distance(*it, *it_other);
				}
				if (it_other->x >= it->x && it_other->y > it->y) {
					agentsB += 1.0/distance(*it, *it_other);
				}
				if (it_other->x <= it->x && it_other->y < it->y) {
					agentsC += 1.0/distance(*it, *it_other);
				}
				if (it_other->x > it->x && it_other->y <= it->y) {
					agentsD += 1.0/distance(*it, *it_other);
				}
			}
		}
		for (auto it_other = poi_positions.begin(); it_other != poi_positions.end(); ++it_other)
		{
			if (it_other->x < it->x && it_other->y >= it->y) {
				poiA += 1.0/distance(*it, *it_other);
			}
			if (it_other->x >= it->x && it_other->y > it->y) {
				poiB += 1.0/distance(*it, *it_other);
			}
			if (it_other->x <= it->x && it_other->y < it->y) {
				poiC += 1.0/distance(*it, *it_other);
			}
			if (it_other->x > it->x && it_other->y <= it->y) {
				poiD += 1.0/distance(*it, *it_other);
			}
		}

		states[index][AGENTS_A] = agentsA;
		states[index][POI_A] = poiA;
		//  information on quadrant 2
		states[index][AGENTS_B] = agentsB;
		states[index][POI_B] = poiB;
		//  information on quadrant 3
		states[index][AGENTS_C] = agentsC;
		states[index][POI_C] = poiC;
		//  information on quadrant 4
		states[index][AGENTS_D] = agentsD;
		states[index][POI_D] = poiD;
		// agent carrying information
		states[index][CARRYING] = 0;

		++index;
	}
}


int main(int argc, char**argv)
{
	int num_agents = 2;
	int num_poi = 3;
	int size = 10;
	int observation_required = 2;
	int num_worlds = 1000;

	// create the net that we will train
	srand(time(NULL));
	FANN::neural_net net1(FANN::LAYER, 3, (const unsigned int[]) {9,5,1});
	net1.randomize_weights(-0.1, 0.1);

	// set up arrays
	fann_type desired_output[1] = {1};
	fann_type *output;
	fann_type **states = new fann_type*[num_agents];
	for (int i = 0; i < num_agents; ++i) { states[i] = new fann_type[9]; }
	
	char **world;

	// Train on each of the worlds generated.
	for (int w = 0; w < num_worlds; ++w)
	{
		if (rand() % 2 == 0) { 
			world = createWorld_success(num_agents, num_poi, size, observation_required);
			desired_output[0] = 1;
		}
		else {
			world = createWorld_fail(num_agents, num_poi, size);
			desired_output[0] = 0;
		}

		// generate the state of the world for each agent.
		gen_states(states, world, size);
	
		//print(world, size);
	
		for (int i = 0; i < 10; ++i) {
			for (int agent = 0; agent < num_agents; ++agent)
			{
				net1.train((fann_type*)states[agent], desired_output);
			}
		}

		// cleanup the world
		for (int i = 0; i < size; ++i) { delete world[i]; }
		delete world;

	}

	// Testing on a new set of worlds very similar to training set
	for (int w = 0; w < num_worlds; ++w)
	{
		if (rand() % 2 == 0) { 
			world = createWorld_success(num_agents, num_poi, size, observation_required);
			desired_output[0] = 1;
		}
		else {
			world = createWorld_fail(num_agents, num_poi, size);
			desired_output[0] = 0;
		}
		gen_states(states, world, size);

		for (int i = 0; i < num_agents; ++i)
		{
			output = net1.run((fann_type*)states[i]);
			std::cout << "error: " << abs(desired_output[0] - output[0]) << std::endl;
		}
	}
	// testing on random worlds.
	for (int w = 0; w < 10; ++w)
	{
		std::cout << " ************* " << " WORLD " << w +1 << " *************" << std::endl;
		world = createWorld_rand(num_agents, num_poi, size);
		print(world, size);
		gen_states(states, world, size);
		for (int i = 0; i < num_agents; ++i)
		{
			output = net1.run(states[i]);
			std::cout << "value: " << output[0] << ", ";
		}
		std::cout << std::endl;
	}

	net1.print_connections();
	net1.save("Pickup.net");
	
	return 0;
}
