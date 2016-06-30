/*
 *  This file implements the simple poi class for the simulation.
 *  Classes were chosen for potential extensibility.
 *  Copyright (C) 2016  Connor Yates
 *
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
 */

#include "poi.h"

POI::POI()
{
	this->complete = false;
	this->weight = 1;
}


/* Non-default constructor calls on the 
* position non-default constructor */
POI::POI(int weight, int x, int y) : p(x, y)
{
	this->weight = weight;
}

int POI::get_weight()
{
	return this->weight;
}

void POI::completed()
{
	this->complete = true;
}

bool POI::is_complete()
{
	return this->complete;
}


