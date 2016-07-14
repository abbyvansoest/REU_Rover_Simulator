/*********************************************************************
* position.cpp
*
* Position is an object class representing (x, y) locations in an
* experimental gridworld.
*
* Copyright (C) 2016 Abby Van Soest, Connor Yates

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

#include "position.h"

//  default constructor
Position::Position() {
	x = 0;
	y = 0;			
}

//  constructor with (x,y) coordinates provided
Position::Position(int xcoord, int ycoord) {
	x = xcoord;
	y = ycoord;
}

//  construct a position from the string representation "(x,y)" 
Position::Position(string s)
{
	int mid = s.find(",");
	int len = mid-1;
	this->x = std::stoi(s.substr(1,len));
	len = s.length() - mid - 1;
	this->y = std::stoi(s.substr(mid+1, len));
}

/* Returns a string representation of the position, 
 * of form "(x,y)" without the quotes.
 * This is the same form that can be used in the string constructor */
string Position::toString()
{
	string s;
	s.append("(");
	s.append(std::to_string(this->x));
	s.append(",");
	s.append(std::to_string(this->y));
	s.append(")");
	return s;
}

// return X coordinate
int Position::getX() {
	return this->x;
}

// return Y coordiante
int Position::getY() {
	return this->y;
}

//  return a copy of the position
Position Position::copy() {
	return Position(this->x, this->y);
}

// overload relational operators
bool Position::operator ==(const Position& p) {
	return (this->x == p.x && this->y == p.y);
}
bool Position::operator !=(const Position& p) {
	return (this->x != p.x || this->y != p.y);
}
