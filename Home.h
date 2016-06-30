/*
 * Home.h
 *
 *  Created on: Jun 30, 2016
 *      Author: tobi
 */

#ifndef HOME_H_
#define HOME_H_

class Home {
private :
	double x_cord;
	double y_cord;
	int capacity_value;
public:
	Home(double,double);
	double getPosition();
	double getCurrentAmount();
	void recieveValues(int);

};

#endif /* HOME_H_ */
