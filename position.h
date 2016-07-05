#ifndef POSITION_INCLUDED
#define POSITION_INCLUDED

class Position 
{

private:
	int x;
	int y;

public:
	Position();
	Position(int, int);

	int getX(void);
	int getY(void);
	Position copy();

	bool operator ==(const Position& p);
	bool operator !=(const Position& p);

};

#endif
