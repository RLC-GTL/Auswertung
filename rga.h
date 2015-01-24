/*!
 * @file proeval.h
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.5.1
 * @brief Main Header.
 */

/*!
 * @mainpage Racersleague GTL Pro evaluation tool
 *
 * @section INTRODUCTION
 *
 * Reads and evaluates GTL logfiles from online races.
 *
 * @section INSTALLATION
 *
 * Copy the file proeval
 *
 * @section USE
 *
 * - Initialize season and create new option file "NewOptionFile.csv":
 * raleval -i \n
 *
 * - Update season after changing teams, adding penalties, etc.:
 * raleval -u inputOverall.csv outputOverall.csv \n
 *
 * - Read racelog:
 * raleval -r inputRacelog.txt inputOverall.csv outputOverall.csv \n
 *
 * - Finish season, i.e. calculate void results:
 * raleval -f inputOverall.csv \n
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Cproeval.h"
using namespace std;


#ifndef PROEVAL_H_
#define PROEVAL_H_


// Variables
/*!
 * @var sProgramUse
 * @brief Contains a short description of how the program can be run
 */
string sProgramUse =
	string("proeval v0.6.1\n") +
	"------------\n" +
	"Init Season:\tproeval -i [outputOverall]\n" +
	"Read Race:\tproeval -r inputRacelog.csv inputOverall.csv outputOverall.csv\n" +
	"Update season:\tproeval -u inputOverall.csv\n" +
	"Finish Season:\tproeval -f inputOverall.csv\n";


// Main
int main(int argc, char** argv);

// Initialize season
int initSeason(const char* ofileOverall);

// Update season
int updateSeason(const char* ifileOverall);

// Read & evaluate race
int readRace(const char* ifileRacelog, const char* ifileOverall,
		const char* ofileOverall);

// Finish season
int finishSeason(const char* ifileOverall);

#endif
