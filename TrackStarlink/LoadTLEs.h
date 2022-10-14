/*
 * LoadTLEs.h
 *
 * Several functions used to load TLE files downloaded from https://celestrak.org/.
 *
 * CDT Peter Toth
 * 09OCT2022
 */

#ifndef LOAD_TLES_H_
#define LOAD_TLES_H_

#include "Tle.h"
#include <vector>

 /*
  * Print out the TLE file
  */
void printTLE(Tle* t);

/*
 * Read TLEs from file.
 *
 * tles: vector holding all of the TLEs. The reason this is a vector
 *		instead of returning a new vector is because there are many
 *		TLE files, and it is easier to push toan existing vector object.
 *
 * filename: self-explanatory
 */
void readTLEs(std::vector<Tle>* tles, char* filename);

/*
 * Return vector of filenames from the .dat file
 */
std::vector<std::string> getFilenames();

#endif // LOAD_TLES_H_