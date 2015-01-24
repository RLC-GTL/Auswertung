/*!
 * @file proeval.cpp
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.5.1
 * @brief Main source.
 */

#include <iostream>
#include "proeval.h"
using namespace std;


/*!
 * Main function for proeval.
 *
 * @param argc
 *		Number of arguments as integer
 * @param **argv
 *		Array of arguments as chars
 * 		- proeval -i
 * 			Initialize Season
 * 		- proeval -u inputOverall.csv outputOverall.csv outputHTML.html
 * 			Update Season
 * 		- proeval -r inputRacelog.txt inputOverall.csv outputOverall.csv outputHTML.html
 * 			Read Race
 * 		- proeval -f inputOverall.csv outputHTML.html
 * 			Finish Season
 * @return
 *		The return value indicates how the program exited
 * 		- 0: Normal termination
 * 		- -1: Not enough arguments
 * 		- -2: Wrong arguments
 */
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
		const char* sNewOptionFile = "NewGentleevalOptionFile.csv";

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

	else if (sOption == "-f" && argc == 3)
	{	// Finish Season
		const char* ifileOverall = argv[2];

		finishSeason(ifileOverall);
	}

	else
	{	// Wrong arguments
		cout << "Wrong arguments. Use:\n" << sProgramUse;
		return -2;
	}

	cout << "\nTODO: Sortierung (Gesamt, Multi, Positionen), Saisonendabrechnung und Meldung für unerlaubtes Wagenwechseln\n";

	return 0;
}


/*!
 * Initializes a new season.
 * Creates an template with standard parameters and title for driver list.
 * Confirms that the file is written.
 *
 * @param ofileOverall
 *		Output csv filename as char array*
 * @return
 *		The return value indicates how the program exited
 * 		- 0: Normal termination
 */
int initSeason(const char* ofileOverall)
{
	// Define class
	CProEval eval("");

	// Create new option file
	eval.writeOverall(ofileOverall);

	// Report
	cout << ofileOverall << " created\n";

	return 0;
}


/*!
 * Updates season.
 * Reads the input file, creates the output and writes the output file.
 *
 * @param ifileOverall
 * 		Input csv filename as char array*
 * @param ofileOverall
 *		Output csv filename as char array*
 * @param ofileHTML
 * 		Output html filename as char array*
 * @return
 *		The return value indicates how the program exited
 * 		- 0: Normal termination
 */
int updateSeason(const char* ifileOverall)
{
	// Define class
	CProEval eval(ifileOverall);

	// Report
	cout << eval.toString();

	// Update RaL evaluation pages pages
	cout << eval.writeHTML(false);

	// Save changes
	//~ eval.writeOverall(ofileOverall);

	return 0;
}


/*!
 * Reads race and updates season.
 * Reads the racelog and the input file, creates the output and writes the output file.
 *
 * @param ifileRacelog
 * 		Input racelog filename as char array*
 * @param ifileOverall
 * 		Input csv filename as char array*
 * @param ofileOverall
 *		Output csv filename as char array*
 * @param ofileHTML
 * 		Output html filename as char array*
 * @return
 *		The return value indicates how the program exited
 * 		- 0: Normal termination
 */
int readRace(const char* ifileRacelog, const char* ifileOverall,
		const char* ofileOverall)
{
	// Define class
	CProEval eval(ifileOverall);

	// Read and report GTL racelog and evaluate it
	cout << eval.readRacelog(ifileRacelog);

	// Update evaluation pages pages
	cout << eval.writeHTML(false);

	// Save changes
	eval.writeOverall(ofileOverall);

	return 0;
}


/*!
 * Finishes season.
 * Reads the input file, finishes the season and creates the output.
 *
 * @param ifileOverall
 * 		Input csv filename as char array*
 * @param ofileOverall
 * 		Output csv filename as char array*
 * @param ofileHTML
 * 		Output html filename as char array*
 * @return
 *		The return value indicates how the program exited
 * 		- 0: Normal termination
 */
int finishSeason(const char* ifileOverall)
{
	// Define class
	CProEval eval(ifileOverall);

	// Report
	cout << eval.finishSeason();

	// Update Pro evaluation pages pages
	cout << eval.writeHTML(true);

	return 0;
}
