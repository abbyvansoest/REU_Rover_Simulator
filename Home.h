/*********************************************************************
* Home.h
*
* Home is an object representing a special return-to-base location 
* in the gridworld. The object remembers the position of the homebase
* and the 'quantity' of material returned to it.
*
* Copyright (C) 2016 Abby Van Soest, Connor Yates, Tobi Oguyale
*********************************************************************/

#ifndef HOME_INCLUDED
#define HOME_INCLUDED

#include  "position.h"

class Home {

private:
	int quantityReturned;  //  amount of POI material returned to the home base
	Position position;     // position of Home in the gridworld

public:

	// constructors
	Home();
	Home(Position);


	// Return the value of the position of the base
	Position getPosition();

	// Set the position of homebase, by accepting a position value
	void setPosition(Position pos);

	//  return the current amount stored in the base
	double currentAmount();

	// Receive an int value from an agent and 
		//  add the value held by the agent to the capacity
	void receiveValues(int);

};

#endif
