/********************************************************************
* Position.h
*
* Position is an object class representing (x, y) locations in a 
* gridworld. The relational equivalence operators are overloaded, 
* simplifying collision detection.
*
* Copyright (C) 2016 Abby Van Soest Connor Yates
*********************************************************************/

#ifndef POSITION_INCLUDED
#define POSITION_INCLUDED

#include <string>
using std::string;

class Position 
{
	private:
		int x;  
		int y;

	public:
		// default constructor (empty)
		Position();
		//  constructor for a given (x, y) position
		Position(int, int);
		//  construct Position from a string in the form "(x, y)"
		Position(string);

		int getX();          //  return Position's x value
		int getY();          //  return Position's y value
		string toString();   //  return string representation
		Position copy();     //  make a copy of the position

		//  override relational operators
		bool operator ==(const Position& p);
		bool operator !=(const Position& p);
};

#endif
