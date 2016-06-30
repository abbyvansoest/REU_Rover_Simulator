/*
 * Home.cpp
 *
 *  Created on: Jun 30, 2016
 *      Author: tobi
 */

#include "Home.h"

Home::Home(double x, double y ) {
	// TODOd Auto-generated constructor stub
	x_cord = x;
	y_cord = y;
	capacity_value = 0;
}
// Returns the value of the Position in the form of X_cord and Y_cords
double Home::getPosition() {
	return x_cord, y_cord;
}
// Returns the current value of the capacity_value
double Home:: getCurrentAmount() {
	return capacity_value;
}

// Receives an amount from an agent and adds it to the current value of the capacity_value
void Home::recieveValues(int newLoad) {
	capacity_value += newLoad;
}

