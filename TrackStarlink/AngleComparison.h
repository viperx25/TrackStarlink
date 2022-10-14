/*
 * AngleComparison.h
 * 
 * Functions used to measure, compare, rank, and print the 
 * angle between a satellite and the observer on the ground.
 * 
 * CDT Peter Toth
 * 09OCT2022
 */

#ifndef ANGLE_COMPARISON_H_
#define ANGLE_COMPARISON_H_

#include <vector>
#include "DateTime.h"
#include "SGP4.h"


// Structure to keep track of each satellite's data
struct SatData
{
	SGP4 sgp;
	DateTime lastMinThetaDT;
	double lastDeltaTheta;
	double lastTheta;
};


/*
 * Filters the satellites by a minimum angle. This way, we can
 * focus on the ones that we know will be within range.
 * 
 * sgpVec: address of vector containing original list
 * startTime & obsVec: time and location of observer at start
 * objectiveTheta: maximum angle
 */
std::vector<SGP4> filterByMinTheta(std::vector<SGP4>& sgpVec, DateTime startTime, Vector obsVec, double objectiveTheta);


/*
 * Prints a vector of SatData structs.
 */
void printSatData(std::vector<SatData>& sd);


/*
 * Returns a ranked vector of SatData structs.
 *
 * satVec: vector of satellites
 * time: current time to rank distances
 * geo: current geographic location of observer
 * maxTheta: will only return satellites whose theta (angle between satellite and observer vectors)
 *				is less than specified
 */
std::vector<SatData> rankThetas(std::vector<SatData>& satVec, DateTime time, CoordGeodetic geo, double maxTheta);


#endif // ANGLE_COMPARISON_H_