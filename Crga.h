/*!
 * @file Crga.h
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.8.1
 * @brief Header for the Crga class.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <math.h>
#include "CcsvList.h"
#include "CrgaParameter.h"
#include "CrgaCar.h"
#include "CrgaPoint.h"
#include "CrgaDriver.h"
#include "CrgaRace.h"
#include "CrgaHTML.h"
#include "CrgaBoP.h"
using namespace std;


#ifndef C_RGA_H_
#define C_RGA_H_


/*!
 * @class Crga
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.5.1
 * @brief Manages and assigns the data.
 *
 * @section DESCRIPTION
 *
 * The Crga class reads the inputOverall file and construes the
 * data.
 * The parameters, cars, points, drivers, teams, races, outputs are handled.
 */
class Crga
{
private:
	// Variables
	CRace Race;

	// Lists
	CParameter *Parameter;
	CCarList *Cars;
	CPoint *Points;
	CDriverList Drivers;
	CcsvList Overall;

	// Functions
	void readOverall(const char* ifileOverall);
	int setParameters(
			int iLine,
			const int iTitleLoc,
			const string sTitle);	//After readOverall
	int setMultis(
			int iLine,
			const int iTitleLoc,
			const string sTitle);
	int setPoints(
			int iLine,
			const int iTitleLoc,
			const string sTitle);
	int setDrivers(
			int iLine,
			const int iTitleLoc,
			const string sTitle);	//After setParameters
	int setRaces(
			int iLine,
			const int iTitleLoc,
			const string sTitle);
	string limitStringLength(string sString, unsigned int iLength, bool bRightJustified);
	double round(double dNumber, unsigned int iDigits);
public:
	// Constructor & Destructor
	Crga(const char* ifileOverall);
	~Crga();

	string toString();
	string finishSeason();
	string readRacelog(const char* Racelog);
	int writeOverall(const char* ofileOverall);
	string writeHTML();
};



#endif
