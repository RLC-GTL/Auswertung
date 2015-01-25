/*!
 * @file rga.cpp
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.8.1
 * @brief Main source.
 */

#include <iostream>
#include "rga.h"
using namespace std;

int main(int argc, char** argv)
{	// Argumente überprüfen
	if (argc < 2)
	{	// Not enough arguments
		cout << "Not enough arguments. Use:\n" << sProgramUse;
		return -1;
	}


	string sOption = argv[1];


	if ((sOption == "-i"))
	{	// Initialize Season
		const char* sNewOptionFile = "00_Init.csv";

		if (argc == 3)
			sNewOptionFile = argv[2];

		initSeason(sNewOptionFile);
	}

	else if (sOption == "-u" && argc == 3)
	{	// Update Season
		const char* ifileInit = argv[2];

		updateSeason(ifileInit);
	}

	else if (sOption == "-r" && argc == 5)
	{	// Read Race
		const char* ifileRacelog = argv[2];
		const char* ifileOverall = argv[3];
		const char* ofileOverall = argv[4];

		readRace(ifileRacelog, ifileOverall, ofileOverall);
	}

	else
	{	// Wrong arguments
		cout << "Wrong arguments. Use:\n" << sProgramUse;
		return -2;
	}

	return 0;
}


int initSeason(const char* ofileOverall)
{
	// Define class
	Crga eval("");

	// Create new option file
	eval.writeOverall(ofileOverall);

	// Report
	cout << ofileOverall << " created\n";

	return 0;
}


int updateSeason(const char* ifileOverall)
{
	// Define class
	Crga eval(ifileOverall);

	// Report
	cout << eval.toString();

	// Update RaL evaluation pages pages
	cout << eval.writeHTML();

	// Save changes
	//~ eval.writeOverall(ofileOverall);

	return 0;
}


int readRace(const char* ifileRacelog, const char* ifileOverall,
		const char* ofileOverall)
{
	// Define class
	Crga eval(ifileOverall);

	// Read and report GTL racelog and evaluate it
	cout << eval.readRacelog(ifileRacelog);

	// Update evaluation pages pages
	cout << eval.writeHTML();

	// Save changes
	eval.writeOverall(ofileOverall);

	return 0;
}

