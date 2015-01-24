/*!
 * @file CproevalRace.h
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.5.1
 * @brief Header for the CRace class; source of the intcompRace comparison class
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <queue>
#include <map>
#include <math.h>
using namespace std;
using namespace boost;


#ifndef C_PROEVAL_RACE_H_
#define C_PROEVAL_RACE_H_


/*!
 * @struct intcompRace
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.5.1
 * @brief Compares the number laps and the time of each driver
 *
 * @section DESCRIPTION
 *
 * The intcompOverall struct compares two doubles and returns true if
 * the first value is bigger.
 */
struct intcompRace
{
	/*!
	 * Compares two doubles and returns true if the left handed value is bigger.
	 *
	 *
	 * @param lhs
	 * 		The left handed value as double
	 * @param rhs
	 * 		The right handed value as double
	 * @return
	 * 		Wheter the left handed value is bigger
	 */
	bool operator() (const double& lhs, const double& rhs) const
	{
		return lhs > rhs;
	}
};


/*!
 * @class CRace
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.5.1
 * @brief Pro race
 *
 * @section DESCRIPTION
 *
 * The CRace class reads the racelog and handles the data.
 * I.e. the race location and date as well as the history of races with
 * the same data.
 * The race result is created and the number of laps, the race time and
 * the car for each driver is handled.
 */
class CRace
{
private:
	// Variables
	int RaceNumber;
	string RaceHistory;
	string DateTime;
	string Location;
	double TrackLength;
	int LapMax;
	int LapMin;
	multimap<double,string,intcompRace> Payoff;	// Ranking (Name,LobbyName,ChosenCar,NumberOfChosenCar,LapDistanceTravelled,RaceTime)

	// Private function
	multimap<double,string>::iterator getDriver(int iPosition);
	string getDriverData(int iPosition);
	string getDriverData(int iPosition, int iDataLoc);
public:
	// Constructor & Destructor
	CRace();
	~CRace();

	// Add stuff
	void addRace(string sRaceInfo);

	// Read stuff
	string getDriverLobbyName(int iPosition);
	string getDriverName(int iPosition);
	string getDriverCarChosen(int iPosition);
	int getDriverLaps(int iPosition);
	bool getDriverBeforeFinishers(int iPosition);
	bool getDriverDNF(int iPosition);
	int getDriverCount();
	string getRace(int iRaceNumber);
	string getRaceLocation(int iRaceNumber);
	string getRaceDateTime(int iRaceNumber);
	int getRaceNumber();
	string toString();

	// File handling
	int readLog(const char* Racelog);
};

#endif
