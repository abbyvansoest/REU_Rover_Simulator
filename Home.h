/*
 * Home.h
 *
 *  Created on: Jun 30, 2016
 *      Authors: Tobi Ogunyale, Abby Van Soest
 */

#ifndef HOME_INCLUDED
#define HOME_INCLUDED

#include  "position.h"

class Home {

private:
	int capacity_value;
	Position position;

public:

	// constructor
	Home(Position);

	// destructor
	Home::~Home();

	// Returns the value of the position of the base
	Position getPosition();

	// Sets the position of homebase, by accepting a position value
	void setPosition(Position pos);

	//  returns the current amount stored in the base
	double currentAmount();

	// Receives an int value from an agent and 
	//  adds the value held by the agent to the capacity
	void recieveValues(int);

};

#endif
