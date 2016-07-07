#include "position.h"

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

/* This constructs a position object from the string representation "(x,y)" */
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
