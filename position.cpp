#include <iostream>
#include "position.h"

using namespace std;

// constructor for Position without arguments provided
Position::Position() {
	x = 0;
	y = 0;			
}

// constructor for Position object with arguments provided
Position::Position(int xcoord, int ycoord) {
	x = xcoord;
	y = ycoord;
}

// return X coordinate
int Position::getX() {
	return this->x;
}

// return Y coordiante
int Position::getY() {
	return this->y;
}

Position Position::copy() {
	Position p = new Position(this->x, this->y);
	return p;
}

// overload relational operators
bool Position::operator ==(const Position& p) {
	return (this->x == p.x && this->y == p.y);
}
bool Position::operator !=(const Position& p) {
	return (this->x != p.x || this->y != p.y);
}






