/*
 * AngleComparison.cpp
 *
 * Functions used to measure, compare, rank, and print the
 * angle between a satellite and the observer on the ground.
 *
 * CDT Peter Toth
 * 09OCT2022
 */

#include <vector>

#include "SGP4.h"
#include "AngleComparison.h"

using namespace std;


/*
 * NOTE: not currently in use
 * 
 * Filters the satellites by a minimum angle. This way, we can
 * focus on the ones that we know will be within range.
 *
 * sgpVec: address of vector containing original list
 * startTime & obsVec: time and location of observer at start
 * objectiveTheta: maximum angle
 */
vector<SGP4> filterByMinTheta(vector<SGP4>& sgpVec, DateTime startTime, Vector obsVec, double objectiveTheta)
{
	vector<SGP4> updatedVec;
	double minTheta = 999;
	double theta = 999;
	DateTime time;

	cout << "Min thetas:" << endl;

	for (int i = 0; i < sgpVec.size(); i++)
	{
		minTheta = 999;
		// determining min theta within next 7 days
		for (int t = 0; t < 360 * 24 * 7; t++)
		{
			time = startTime.AddSeconds(t);
			theta = sgpVec.at(i).FindPosition(time).Position().Angle(obsVec);
			if (theta > 20) { t += 30; } // If theta is high, it's going to take time for the satellite to come
											// in range, so wait 30 seconds and check again (makes algo way faster)
			if (theta < minTheta) { minTheta = theta; }
		}
		if (minTheta <= objectiveTheta)
		{
			updatedVec.push_back(sgpVec.at(i));
			cout << sgpVec.at(i).name << ": " << minTheta << endl;
		}
	}

	return updatedVec;
}


/*
 * Prints a vector of SatData structs.
 */
void printSatData(vector<SatData>& sd)
{
	for (int i = 0; i < sd.size(); i++)
	{
		// Display them differently if there was a min theta within the last 10 seconds.
		if (DateTime::Now() - sd.at(i).lastMinThetaDT > TimeSpan(0, 0, 10))
		{
			cout << i + 1 << ") " << sd.at(i).sgp.name << " theta=" << sd.at(i).lastTheta << " lastMin=" << sd.at(i).lastMinThetaDT << endl;
		}
		else
		{
			cout << " *** " << i + 1 << ") " << sd.at(i).sgp.name << " theta=" << sd.at(i).lastTheta << " lastMin=" << sd.at(i).lastMinThetaDT << endl;
		}
	}
}



/*
 * Returns a ranked vector of SatData structs.
 *
 * satVec: vector of satellites
 * time: current time to rank distances
 * geo: current geographic location of observer
 * maxTheta: will only return satellites whose theta (angle between satellite and observer vectors)
 *				is less than specified
 */
vector<SatData> rankThetas(vector<SatData>& satVec, DateTime time, CoordGeodetic geo, double maxTheta)
{
	vector<SatData> rankedSatVec;
	double theta;
	double deltaTheta;
	int startI = 0;

	// Update each satellite theta
	for (int i = 0; i < satVec.size(); i++)
	{
		try {
			theta = satVec.at(i).sgp.FindPosition(time).Position().Angle(Eci::Eci(time, geo).Position());
		}
		catch (DecayedException de)
		{
			cout << de.what() << ": ";
			cout << satVec.at(i).sgp.name << endl;
		}
		deltaTheta = theta - satVec.at(i).lastTheta;

		// if first derivative goes from neg to pos -> min
		if (satVec.at(i).lastDeltaTheta < 0 && deltaTheta > 0)
		{
			satVec.at(i).lastMinThetaDT = time;
		}

		// Update for next loop
		satVec.at(i).lastTheta = theta;
		satVec.at(i).lastDeltaTheta = deltaTheta;
	}

	// Locate the first element where the maxTheta requirement is met
	while (satVec.at(startI).lastTheta > maxTheta) { startI++; }
	rankedSatVec.push_back(satVec.at(startI));

	// Sort the vectors by theta, and make sure they are under the maxTheta
	for (int i = startI + 1; i < satVec.size(); i++)
	{
		if (satVec.at(i).lastTheta <= maxTheta) // check theta
		{
			// Loops through rankedSatVec until the current satellite theta is greater.
			// - then it inserts the satellite in front.
			for (int j = 0; j < rankedSatVec.size(); j++)
			{
				if (rankedSatVec.at(j).lastTheta > satVec.at(i).lastTheta)
				{
					rankedSatVec.insert(rankedSatVec.begin() + j, satVec.at(i));
					break;
				}
				if (j == rankedSatVec.size() - 1)
				{
					rankedSatVec.push_back(satVec.at(i));
					break;
				}
			}
		}
	}

	return rankedSatVec;
}