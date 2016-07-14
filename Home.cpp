/*********************************************************************
* home.cpp
*
* Home is an object representing a special return-to-base location 
* in the gridworld. The object remembers the position of the homebase
* and the 'quantity' of material returned to it.
*
* Copyright (C) 2016 Abby Van Soest, Connor Yates, Tobi Oguyale

*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/

#include "Home.h"

//  constructors
Home::Home()
{
	this->quantityReturned = 0;
}

Home::Home(Position p) {
	this->position = p.copy();
	this->quantityReturned = 0;
}

// Return the value of the Position in the form of X_cord and Y_cords
Position Home::getPosition() {
	return this->position;
}

// Return the current value of the quantity at the home base
double Home::currentAmount(){
	return this->quantityReturned;
}


// Set the position of homebase, by accepting a position value
void Home::setPosition(Position pos){
	this->position = pos;

}

// Receive an amount of size newLoad from an agent and 
	//  add it to the quantity at the home base
void Home::receiveValues(int newLoad){
	this->quantityReturned += newLoad;
}


