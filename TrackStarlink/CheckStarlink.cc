/* CheckStarlink.cc
 *
 * Track the satellites that are within a given theeta.
 * 
 * CDT Peter Toth
 */

#include <Windows.h>

#include "AngleComparison.h"
#include "LoadTLEs.h"
#include "SGP4.h"
#include "Tle.h"

using namespace std;


int main()
{
	vector<string> files = getFilenames();
	vector<Tle> tles;
	vector<SatData> satVec;
	vector<SatData> updatedSatVec;
	time_t t = time(nullptr);
	char formatStr[100];

	// Read TLEs
	for (int i = 0; i < files.size(); i++)
	{
		readTLEs(&tles, (char*)files.at(i).c_str());
	}

	for (int i = 0; i < tles.size(); i++)
	{
		satVec.push_back(SatData{ SGP4(tles.at(i)), DateTime::Now()-TimeSpan(1, 0, 0), 999, -999});
	}

	while (true)
	{
		t = time(nullptr);
		strftime(formatStr, sizeof(formatStr), "%d%b%Y %H:%M:%S", localtime(&t));
		cout << formatStr << " - Satellites Listed by Theta" << endl;

		/*
		 * current options:
		 * - location: Ike Barracks, West Point, NY
		 * - maxTheta: Only show satellites within 10 degrees of angle from us
		 */
		updatedSatVec = rankThetas(satVec, DateTime::Now(), CoordGeodetic(41.3913358, -73.9565737, 0.05), 10);
		printSatData(updatedSatVec);

		Sleep(1000);
		system("cls");
	}


}