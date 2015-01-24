/*!
 * @file Cproeval.h
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.5.1
 * @brief Header for the CProEval class.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <math.h>
#include "CcsvList.h"
#include "CproevalParameter.h"
#include "CproevalCar.h"
#include "CproevalPoint.h"
#include "CproevalDriver.h"
#include "CproevalRace.h"
#include "CproevalHTML.h"
using namespace std;


#ifndef C_PROEVAL_H_
#define C_PROEVAL_H_


/*!
 * @class CProEval
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.5.1
 * @brief Manages and assigns the data.
 *
 * @section DESCRIPTION
 *
 * The CProEval class reads the inputOverall file and construes the
 * data.
 * The RaL parameters, cars, points, drivers, teams, races are handled.
 */
class CProEval
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
	CProEval(const char* ifileOverall);
	~CProEval();

	string toString();
	string finishSeason();
	string readRacelog(const char* Racelog);
	int writeOverall(const char* ofileOverall);
	string writeHTML(bool bFinishSeason);
};



#endif
