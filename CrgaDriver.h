/*!
 * @file CrgaDriver.h
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.8.1
 * @brief Header for the CDriver, CTeam & CDriverList classes; source of the intcompOverall comparison class
 */

#include <iostream>
#include <string>
#include <limits.h>
#include <float.h>
#include <vector>
#include <map>
#include <set>
#include "CrgaParameter.h"
#include "CrgaCar.h"
using namespace std;


#ifndef C_RGA_DRIVER_H_
#define C_RGA_DRIVER_H_


/*!
 * @struct intcompOverall
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.5.1
 * @brief Compares two doubles
 *
 * @section DESCRIPTION
 *
 * The intcompOverall struct compares two doubles and returns true if
 * the first value is bigger.
 */
struct intcompOverall
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
 * @struct raceResult
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.5.1
 * @brief Stores race results (credit and points)
 *
 * @section DESCRIPTION
 *
 * The raceResult struct stores the earned credit as double and points as int.
 * Two raceResults can be added or subtracted.
 */
//~ struct raceResult
//~ {
	//~ int Points;
	//~ string CarAbr;
	//~ string toString()
	//~ {
		//~ stringstream ss;
		//~ ss << Points << "(" << CarAbr << ")";
		//~ return ss.str();
	//~ }
//~ };

/*!
 * @file CrgaRace.cpp
 * @struct CResult
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.5.1
 * @brief Stores driver's result
 *
 * @section DESCRIPTION
 *
 * The CResult class stores the driver's result.
 * I.e. the division and the points.
 */
class CResult
{
private:
	// Variables
	string CarAbr;
	string CarNum;
	int Points;
public:
	// Constructor & Destructor
	CResult();
	CResult(string sResult);
	CResult(const string sCarAbr, const string sCarNum, const int iPoints);
	~CResult();

	// Read stuff
	string getCarAbr();
	string getCarNum();
	int getPoints();
	double getResult();
	string toString();
};


/*!
 * @class CDriver
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.5.1
 * @brief RaL driver
 *
 * @section DESCRIPTION
 *
 * The CDriver class manages the driver data.
 * I.e. the driver lobbyname, names of the driver and his team, the
 * credits, driver points, penalty and the number of races he competed.
 */
class CDriver
{
private:
	// Variables
	string LobbyName;
	string Name;
	string TeamName;
	int Penalty;
	int RaceCount;
	vector<CResult> Results;
	set<string> CarAbbrs;
	static CParameter* Parameter;
	static CCarList* Cars;
public:
	// Constructor & Destructor
	CDriver(
		string sLobbyName,
		string sName,
		string sTeamName,
		int iPenalty,
		int iRaceCount,
	    vector<CResult> vResults,
		CParameter* cParameter,
		CCarList* cCars);
	~CDriver();

	// Add stuff
	void addResult(CResult Result);
	void addPenalty(int iPenalty);

	// Change stuff
	void setTeam(string sTeamName);

	// Read stuff
	string getLobbyName();
	string getName();
	string getTeam();
	CResult* getRaceResult(unsigned int iRaceNumber);
	int getPenalty();
	double getPoints();
	double getOverall();
	vector<CResult> getVoidResults(int iRaceCount);
	double getFinish(int iRaceCount);
	int getRaceCount();
	string toString();
};


/*!
 * @class CTeam
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.5.1
 * @brief RaL team
 *
 * @section DESCRIPTION
 *
 * The CTeam class manages the team data.
 * I.e. the team name and the lobbynames of its members.
 */
class CTeam
{
private:
	// Variables
	string Name;
	string Member1;
	string Member2;
public:
	// Constructor & Destructor
	CTeam(
		string sTeamName,
		string sMember1LobbyName,
		string sMember2LobbyName);
	~CTeam();

	// Add stuff
	bool addMember(string sMemberLobbyName);

	// Read stuff
	string getName();
	string getMember(int iMemberNumber);
	int getMemberCount();
};


/*!
 * @class CDriverList
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.4
 * @brief List of all RaL drivers and teams
 *
 * @section DESCRIPTION
 *
 * The CDriverList class handles the list of drivers and teams.
 * I.e. the adding of new drivers, allocation to teams, adding drivers,
 * credits, points, penalty to drivers and counting the number of
 * races, adding members to teams and reading data.
 */
class CDriverList
{
private:
	vector<CDriver> vDrivers;
	vector<CTeam> vTeams;

	// Private functions
	CDriver* getDriver(string sDriverLobbyName);
	CDriver* getDriver(unsigned int iRank);
	CDriver* getDriverFinish(unsigned int iRank, int iRaceCount);
	CTeam* getTeam(string sTeamName);
	CTeam* getTeam(unsigned int iRank);
	CTeam* getTeamFinish(unsigned int iRank, int iRaceCount);
public:
	// Constructor & Destructor
	CDriverList();
	//~ CDriverList(vector<CDriver> &vCDrivers, vector<CTeam> &vCTeams);
	~CDriverList();

	// Add stuff
	bool addDriver(CDriver* cDriver);
	bool addDriverEmptyResult(string sDriverLobbyName);
	bool addDriverResult(string sDriverLobbyName, CResult rResult);
	bool addDriverPenalty(string sDriverLobbyName, int iPenalty);
	bool addTeamMember(string sTeamName, string sMemberLobbyName);

	// Change stuff
	//~ bool setDriverTeam(string sDriverLobbyName, string sTeamName);

	// Read stuff
	int getDriverCount();
	string getDriverLobbyName(int iRank);
	string getDriverLobbyNameFinish(int iRank, int iRaceCount);
	string getDriverName(string sDriverLobbyName);
	string getDriverTeam(string sDriverLobbyName);
	CResult* getDriverRaceResult(string sDriverLobbyName, int iRaceNumber);
	int getDriverPenalty(string sDriverLobbyName);
	double getDriverPoints(string sDriverLobbyName);
	double getDriverOverall(string sDriverLobbyName);
	double getDriverFinish(string sDriverLobbyName, int iRaceCount);
	vector<CResult> getDriverVoidResults(string sDriverLobbyName, int iRaceCount);
	int getDriverRaceCount(string sDriverLobbyName);
	int getTeamCount();
	string getTeamName(int iRank);
	string getTeamNameFinish(int iRank, int iRaceCount);
	double getTeamOverall(string sTeamName);
	int getTeamPenalty(string sTeamName);
	double getTeamFinish(string sTeamName, int iRaceCount);
	string getTeamMember(string sTeamName, int iMemberNumber);
	string toString();
};

#endif
