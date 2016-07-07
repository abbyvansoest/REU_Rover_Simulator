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
		Position();
		Position(int, int);
		Position(string);

		int getX();
		int getY();
		string toString();
		Position copy();

		bool operator ==(const Position& p);
		bool operator !=(const Position& p);
};

#endif
