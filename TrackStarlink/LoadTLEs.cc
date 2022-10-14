/*
 * LoadTLEs.cc
 * 
 * Several functions used to load TLE files downloaded from https://celestrak.org/.
 * 
 * CDT Peter Toth
 * 09OCT2022
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>

#include "Tle.h"

using namespace std;

/*
 * Print out the TLE file
 */
void printTLE(Tle* t)
{
	cout << "Name: " << t->Name() << endl;
	cout << "Line 1: " << t->Line1() << endl;
	cout << "Line 2: " << t->Line2() << endl;
}

/*
 * Read TLEs from file.
 * 
 * tles: vector holding all of the TLEs. The reason this is a vector
 *		instead of returning a new vector is because there are many
 *		TLE files, and it is easier to push toan existing vector object.
 * 
 * filename: self-explanatory
 */
void readTLEs(vector<Tle>* tles, char* filename)
{
	fstream file;
	string line;

	char prefix[51] = "tles/";

	strcat(prefix, filename);
	file.open(prefix);

	if (!file.is_open())
	{
		cout << "failed to open " << filename << endl;
		return;
	}

	struct {
		string name;
		string line1;
		string line2;
	} ttle;
	bool skip = false;

	while (getline(file, line))
	{
		if (line.substr(0, 1) == "1")
		{
			ttle.line1 = line;
		}
		else if (line.substr(0, 1) == "2")
		{
			ttle.line2 = line;
			if (!skip)
			{
				tles->push_back(Tle(ttle.name, ttle.line1, ttle.line2));
			}
			skip = false;
		}
		else
		{
			if (line.substr(0, 8) != "STARLINK")
			{
				skip = true;
				// cout << "skipping " << line << endl;
			}
			ttle.name = line;
		}
	}
}

/*
 * Return vector of filenames from the .dat file
 */
vector<string> getFilenames()
{
	ifstream file("tles/filenames.dat");
	vector<string> files;
	string text;
	char* tempText;
	char* temp;

	if (!file.is_open())
	{
		cerr << "Failed to open file" << endl;
		exit(1);
	}

	file >> text;
	tempText = (char*)text.c_str();
	temp = strtok(tempText, ",");
	while (temp)
	{
		files.push_back(temp);
		temp = strtok(NULL, ",");
	}

	return files;
}