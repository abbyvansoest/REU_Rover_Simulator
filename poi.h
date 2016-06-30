#ifndef _POI_H
#define _POI_H

#include <iostream>
#include "position.h"

class POI {
	private:
		Position p;
		bool complete;
		int weight;
	public:
		POI();
		POI(int,int,int);
		int get_weight();
		void compleated();
		bool is_complete();
};

#endif
