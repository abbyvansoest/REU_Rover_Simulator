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

// overload relational operators
bool Position::operator ==(const Position& p) {
	return (this->x == p.x && this->y == p.y);
}

bool Position::operator !=(const Position& p) {
	return (this->x != p.x || this->y != p.y);
}

// test main
int main() {

	Position p1(2, 3);
	Position p2(1, 2);
	Position p3(2, 3);
	Position p4;

	if (p1 != p2) cout << "good\n";
	else cout << "bad\n";

	if (p1 == p3) cout << "good\n";
	else cout << "bad\n";

	cout << "Four: " << p4.getX() << ", " << p4.getY() << "\n";
	cout << "Three: " << p3.getX() << ", " << p3.getY() << "\n";
	cout << "Two: " << p2.getX() << ", " << p2.getY() << "\n";
	cout << "One: " << p1.getX() << ", " << p1.getY() << "\n";

	return 0;

}






