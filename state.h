#ifndef _STATE_H
#define _STATE_H
/* This struct is a nice wrapper around the state, allowing it to be treated as a singular object
 * (with additional bracketing being overloaded, if it is desired)
 * and encapsulates the int array away.
 * The internal array can be directly accessed if needed */

/* Define the indicies to be used by the state variable globally, so everyone
 * uses the proper index */
enum
{
	AGENTS_A,
	POI_A,
	BROADCASTING_A,
	AGENTS_B,
	POI_B,
	BROADCASTING_B,
	AGENTS_C,
	POI_C,
	BROADCASTING_C,
	AGENTS_D,
	POI_D,
	BROADCASTING_D,
	CARRYING
};

typedef struct State {

	double& operator[](int i) { return array[i]; }
	double array[13];

} State;

#endif
