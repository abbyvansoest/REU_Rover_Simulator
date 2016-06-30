/*
 * Home.cpp
 *
 *  Created on: Jun 30, 2016
 *      Author: tobi
 */

#include "Home.h"

Home::Home(Position p) {
	// TODOd Auto-generated constructor stub
	this->Home_Pos =p;
	this -> capacity_value =0;
}


// Returns the value of the Position in the form of X_cord and Y_cords
Position Home::getPosition(){
		return this -> Home_Pos;
	}


// Returns the current value of the capacity_value
double Home:: getCurrentamount(){
		return this ->capacity_value;
	}

/*
 *  * Sets the Position of Home Base , by accepting a  position value
*/
void Home::setPosition(Position pos){
	this ->Home_Pos = pos;

}



// Receives an amount from an agent and adds it to the current value of the capacity_value
void Home::recieveValues(int newLoad){
		this ->capacity_value += newLoad;
	}

