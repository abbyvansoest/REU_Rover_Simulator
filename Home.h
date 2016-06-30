/*
 * Home.h
 *
 *  Created on: Jun 30, 2016
 *      Author: tobi
 */

#ifndef HOME_H_
#define HOME_H_
#include <iostream>
#include  "position.h"

class Home {
private :
	int capacity_value;
	Position Home_Pos;
public:
	Home(Position);

	/*
	 * Returns the value of the position of the base
	 */
	Position getPosition();


	/*
	 * Sets the Position of Home Base , by accepting a  position value
	 */
	void setPosition(Position pos);


	/*
	 * returns the current amount stored in the base
	 */
	double getCurrentamount();


	/*
	 * Receives an int value from an agent and adds the value held by the agent to the  capacity
	 */
	void recieveValues(int);

};

#endif /* HOME_H_ */
