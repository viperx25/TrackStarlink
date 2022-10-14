/*
 * DetermineSatellites.cpp
 * 
 * Given an observation location, and a DateTime
 * determine the satellite overhead.
 */

#include <string>
#include "CoordGeodetic.h"
#include "DateTime.h"

int main3()
{
	CoordGeodetic cg = CoordGeodetic(41.3913358, -73.9565737, 0.05);
	DateTime dt = DateTime::Now();

	return 0;
}