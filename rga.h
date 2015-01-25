/*!
 * @file rga.h
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.8.1
 * @brief Main Header.
 */

/*!
 * @mainpage Racersleague GTL evaluation tool
 *
 * @section INTRODUCTION
 *
 * Reads and evaluates GTL logfiles from online races.
 *
 * @section INSTALLATION
 *
 * Copy the file rga
 *
 * @section USE
 *
 * - Initialize season and create new option file "NewOptionFile.csv":
 * rga -i \n
 *
 * - Update season after changing teams, adding penalties, etc.:
 * rga -u inputOverall.csv \n
 *
 * - Read racelog:
 * rga -r inputRacelog.txt inputOverall.csv outputOverall.csv \n
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Crga.h"
using namespace std;


#ifndef RGA_H_
#define RGA_H_

#define VERSION "0.8.1"

/*!
 * @var sProgramUse
 * @brief Contains a short description of how the program can be run
 */
string sProgramUse =
	string("rga") + VERSION + "\n" +
	"------------\n" +
	"Init Season:\trga -i [outputOverall]\n" +
	"Read Race:\trga -r inputRacelog.csv inputOverall.csv outputOverall.csv\n" +
	"Update season:\trga -u inputOverall.csv\n";


/*!
 * @brief Main function for Racersleague GTL evalualtion.
 *
 * @param argc
 *		Number of arguments as integer
 * @param **argv
 *		Array of arguments as chars
 * 		- rga -i
 * 			Initialize Season
 * 		- rga -u inputOverall.csv
 * 			Update Season
 * 		- rga -r inputRacelog.txt inputOverall.csv outputOverall.csv
 * 			Read Race
 * @return
 *		The return value indicates how the program exited
 * 		- 0: Normal termination
 * 		- -1: Not enough arguments
 * 		- -2: Wrong arguments
 */
int main(int argc, char** argv);

/*!
 * @brief Initializes a new season.
 * 
 * Creates an template with standard parameters and title for driver list.
 * Confirms that the file is written.
 *
 * @param ofileOverall
 *		Output csv filename as char array*
 * @return
 *		The return value indicates how the program exited
 * 		- 0: Normal termination
 */
int initSeason(const char* ofileOverall);

/*!
 * @brief Updates season.
 * 
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
int updateSeason(const char* ifileOverall);

/*!
 * @brief Reads race and updates season.
 * 
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
		const char* ofileOverall);

#endif
