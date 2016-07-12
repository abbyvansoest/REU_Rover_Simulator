/*
 * Home.cpp
 *
 *  Created on: Jun 30, 2016
 *      Author: tobi
 */

#include "Home.h"

Home::Home()
{
	this->quantityReturned = 0;
}

Home::Home(Position p) {
	// TODOd Auto-generated constructor stub
	this->position = p.copy();
	this->quantityReturned = 0;
}

// Returns the value of the Position in the form of X_cord and Y_cords
Position Home::getPosition() {
	return this->position;
}

// Returns the current value of the quantity at the home base
double Home::currentAmount(){
	return this->quantityReturned;
}


// Sets the position of homebase, by accepting a position value
void Home::setPosition(Position pos){
	this->position = pos;

}

// Receives an amount of size newLoad from an agent and 
//  adds it to the quantity at the home base
void Home::receiveValues(int newLoad){
	this->quantityReturned += newLoad;
}


