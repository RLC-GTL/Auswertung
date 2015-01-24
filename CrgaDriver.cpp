/*!
 * @file CralevalDriver.cpp
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.5.1
 * @brief Source for the CDriver, CTeam & CDriverList classes.
 */

#include "CproevalDriver.h"

// Constructor & Destructor
CResult::CResult()
{
	CarAbr = "";
	Mult = 0.0;
	Points = 0;
}

//~ CResult::CResult(string sResult)
//~ {
	//~ stringstream ssResult(sResult);
	//~ string s;
	//~
	//~ // Points
	//~ getline(ssResult, s, '(');
	//~ stringstream ssPoints(s);
	//~ ssPoints >> Points;
	//~
	//~ // Car abbreviation
	//~ getline(ssResult, s, ')');
	//~ stringstream ssCarAbr(s);
	//~ ssCarAbr >> CarAbr;
//~ }

CResult::CResult(const string sCarAbr, const double dMult, const int iPoints)
{
	CarAbr = sCarAbr;
	Mult = dMult;
	Points = iPoints;
}

CResult::~CResult()
{

}

// Read stuff
string CResult::getCarAbr()
{
	return CarAbr;
}

int CResult::getPoints()
{
	return Points;
}

double CResult::getMult()
{
	return Mult;
}

double CResult::getResult()
{
	return Points;
}

string CResult::toString()
{
	stringstream ss;
	ss << Points << "(" << CarAbr << ")";
	return ss.str();
}

// Constructor & Destructor
/*!
 * Constructor of CDriver.
 * The driver's lobbyname and name, the team name, the credit, driver points, penalty points, the number of races and a reference to the parameters are stored.
 *
 *
 * @param sLobbyName
 * 		The lobbyname as string
 * @param sName
 * 		The name as string
 * @param sTeamName
 * 		The team name as string
 * @param dCredit
 * 		The credit as double
 * @param iPoints
 * 		The driver points as int
 * @param iPenalty
 * 		The penalty points as int
 * @param iRaceCount
 * 		The number of races as int
 * @param cParameter
 * 		The reference to CRaLEval::Parameter as CParameter*
 */
CDriver::CDriver(
		string sLobbyName,
		string sName,
		string sTeamName,
		int iPenalty,
		int iRaceCount,
		vector<CResult> vResults,
		CParameter* cParameter,
		CCarList* cCars)
{
	if (Parameter == NULL)
		Parameter = cParameter;
	if (Cars == NULL)
		Cars = cCars;

	LobbyName = sLobbyName;
	Name = sName;
	TeamName = sTeamName;
	Penalty = iPenalty;
	RaceCount = iRaceCount;
	Results = vResults;

	CResult rZero;
	for (int i = RaceCount - Results.size(); i > 0; i--)
		Results.push_back(rZero);
}

/*!
 * Destructor of CDriver.
 *
 *
 */
CDriver::~CDriver()
{

}

/*!
 * Initialization of the static pointer to CRaLEval::Parameter as CParameter*
 *
 *
 */
CParameter* CDriver::Parameter = NULL;
CCarList* CDriver::Cars = NULL;

// Add stuff
/*!
 * Adds race result (credit and points). Increases race count.
 *
 *
 * @param rResult
 * 		The result as raceResult
 */
void CDriver::addResult(CResult rResult)
{
	Results.push_back(rResult);
	RaceCount++;
}

/*!
 * Adds penalty points.
 *
 *
 * @param iPenalty
 * 		The penalty points as int
 */
void CDriver::addPenalty(int iPenalty)
{
	Penalty = Penalty + iPenalty;
}

// Change stuff
/*!
 * Set's the driver's team name.
 *
 *
 * @param sTeamName
 * 		The team name as string
 */
void CDriver::setTeam(string sTeamName)
{
	TeamName = sTeamName;
}

// Read stuff
/*!
 * Reads the lobbyname.
 *
 *
 * @return
 * 		The lobbyname as string
 */
string CDriver::getLobbyName()
{
	return LobbyName;
}

/*!
 * Reads the name.
 *
 *
 * @return
 * 		The name as string
 */
string CDriver::getName()
{
	return Name;
}

/*!
 * Reads the team name.
 *
 *
 * @return
 * 		- The team name as string
 * 		- If the driver isn't member of any team "Kein Team" is returned
 */
string CDriver::getTeam()
{
	if (TeamName == "")
		return "Kein Team";
	else
		return TeamName;
}

/*!
 * Returns the race result from race iRaceNumber.
 *
 *
 * @param iRaceNumber
 * 		The race number as int
 * @return
 * 		- The team name as string
 * 		- If the driver isn't member of any team "Kein Team" is returned
 */
CResult* CDriver::getRaceResult(unsigned int iRaceNumber)
{
	if (Results.size() >= iRaceNumber)
		return &(Results.at(iRaceNumber - 1));

	return NULL;
}

/*!
 * Reads the penalty points.
 *
 *
 * @return
 * 		The penalty points as int
 */
int CDriver::getPenalty()
{
	return Penalty;
}

double CDriver::getMultMean()
{
	double dMult = 0;
	double dRaces = 0;
	string sCarAbr;
	for (vector<CResult>::iterator it = Results.begin() ; it != Results.end(); ++it)
	{
		sCarAbr = it->getCarAbr();
		if (sCarAbr != "")
		{
			dMult += Cars->getMult(sCarAbr);
			dRaces++;
		}
	}
	return dMult / dRaces;
}

/*!
 * Calculates the overall points.
 *
 *
 * @return
 * 		The driver points as double
 */
double CDriver::getPoints()
{
	double dPoints = 0;
	for (vector<CResult>::iterator it = Results.begin() ; it != Results.end(); ++it)
		dPoints += it->getResult();
	return dPoints;
}

/*!
 * Calculates the overall points and substracts penalty points.
 *
 *
 * @return
 * 		The overall points as double
 */
double CDriver::getOverall()
{
	//~ double dOverall = -(double)getPenalty();
	//~ for (vector<CResult>::iterator it = Results.begin() ; it != Results.end(); ++it)
		//~ dOverall += it->getResult();
	//~ return dOverall;

	return getPoints() - (double)getPenalty();
}

/*!
 * Returns the void races.
 *
 *
 * @param
 * 		The number of races as int
 * @return
 * 		The overall points after considering void results as double
 */
vector<CResult> CDriver::getVoidResults(int iRaceCount)
{
	//~ cout << "DEBUG CDriver::getVoidResults: " << getRaceCount() << " Races for " << getName() << endl;

	vector<CResult> vReturn;
	// Return if not enough races (more void results available)
	if (Parameter->getNumberOfVoidResults() >= iRaceCount)
		return vReturn;

	multimap<double,CResult,intcompOverall> mResults;

	// Sort races by results
	for (int i = 1; i <= getRaceCount(); i++)
	{
		CResult* r = getRaceResult(i);
		//~ int key = getRaceResult(i)->getResult();
		double key = r->getResult();
		//~ mResults.insert(pair<int,CResult>(key,*getRaceResult(i)));
		mResults.insert(pair<double,CResult>(key,*r));
	}

	// Choose worst results
	for (multimap<double,CResult>::reverse_iterator ritM = mResults.rbegin(); ritM != mResults.rend(); ++ritM)
	{
		if (vReturn.size() == (unsigned int)Parameter->getNumberOfVoidResults())
			return vReturn;;
		vReturn.push_back((*ritM).second);
	}

	return vReturn;
}

/*!
 * Returns the overall points.
 *
 * @param
 * 		The number of races as int
 * @return
 * 		The overall points after considering void results as double
 */
double CDriver::getFinish(int iRaceCount)
{
	vector<CResult> vVoidResults = getVoidResults(iRaceCount);
	double dFinish = getOverall();

	for (vector<CResult>::iterator it = vVoidResults.begin(); it != vVoidResults.end(); it++)
	{
		dFinish -= it->getResult();
	}

	return dFinish;
}

/*!
 * Returns the number of races.
 *
 *
 * @return
 * 		The number of races as int
 */
int CDriver::getRaceCount()
{
	return RaceCount;
}

/*!
 * Creates and returns a string with the content.
 *
 *
 * @return
 *		The content of as string
 */
string CDriver::toString()
{
	stringstream ss;
	ss <<
		"Driver," <<
		getName() << "," <<
		getTeam() << "," <<
		getLobbyName() << "," <<
		getPenalty() << "," <<
		getRaceCount();

		if (getRaceCount() > 0)
		{
			vector<CResult>::iterator it;

			for (it = Results.begin(); it < Results.end(); it++)
			{
				ss << "," << it->toString();
			}
		}

	return ss.str();
}


// Constructor & Destructor
/*!
 * Constructor of CTeam.
 * The team name and the names of the two members are stored.
 *
 *
 * @param sTeamName
 * 		The team name as string
 * @param sMember1LobbyName
 * 		The name of member 1 as string
 * @param sMember2LobbyName
 * 		The name of member 2 as string
 */
CTeam::CTeam(
	string sTeamName,
	string sMember1LobbyName,
	string sMember2LobbyName)
{
	Name = sTeamName;
	Member1 = sMember1LobbyName;
	Member2 = sMember2LobbyName;
}

/*!
 * Destructor of CCar.
 *
 *
 */
CTeam::~CTeam()
{

}

// Add stuff
/*!
 * Checks whether the team has an available position.
 * If so the new member is added and TRUE returned.
 * Else FALSE is returned.
 *
 *
 * @param sMemberLobbyName
 * 		The lobbyname of the new member as string
 * @return
 * 		- TRUE if the new member is successfully added
 * 		- FALSE if the team has already two members
 */
bool CTeam::addMember(string sMemberLobbyName)
{
	if (Member1 == "")
	{
		Member1 = sMemberLobbyName;
		return true;
	}
	else if (Member2 == "")
	{
		Member2 = sMemberLobbyName;
		return true;
	}
	return false;
}

// Read stuff
/*!
 * Reads the name.
 *
 *
 * @return
 * 		The name as string
 */
string CTeam::getName()
{
	return Name;
}

/*!
 * Reads the name of a team member.
 * If the parameter iMemberNumber is not 0 or 1 an empty string is returned.
 *
 *
 * @param iMemberNumber
 * 		The number of the desired member as int
 * @return
 * 		- The lobbyname as string
 * 		- If the parameter is not 0 or 1 an empty string is returned
 */
string CTeam::getMember(int iMemberNumber)
{
	if (iMemberNumber == 0)
		return Member1;
	else if (iMemberNumber == 1)
		return Member2;
	else
		return "";
}

/*!
 * Reads the number of team members.
 *
 *
 * @return
 * 		The number of team members as int
 */
int CTeam::getMemberCount()
{
	if ((Member1 != "") && (Member2 != ""))
		return 2;
	else if ((Member1 != "") || (Member2 != ""))
		return 1;
	else
		return 0;
}


// Constructor & Destructor
/*!
 * Constructor of CDriverList.
 * An empty list is created.
 *
 *
 */
CDriverList::CDriverList()
{

}

//~ CDriverList::CDriverList(vector<CDriver> &vCDrivers, vector<CTeam> &vCTeams)
//~ {
	//~ vDrivers = vCDrivers;
	//~ vTeams = vCTeams;
//~ }

/*!
 * Destructor of CDriverList.
 *
 *
 */
CDriverList::~CDriverList()
{

}

// Private functions
/*!
 * Searches the driver's lobbyname and returns the pointer to the driver.
 * If the driver couldn't be found a NULL pointer is returned.
 *
 *
 * @param sDriverLobbyName
 * 		The lobbyname as string
 * @return
 *		The driver as CDriver*
 */
CDriver* CDriverList::getDriver(string sDriverLobbyName)
{
	vector<CDriver>::iterator it;

	// Driver existing?
	for (it = vDrivers.begin(); it < vDrivers.end(); it++)
	{
		if (it->getLobbyName() == sDriverLobbyName)
			return &(*it);
	}

	return NULL;
}

/*!
 * Searches the driver's overall rank and returns the pointer to the driver.
 * If the driver couldn't be found a NULL pointer is returned.
 *
 *
 * @param iRank
 * 		The overall rank of the driver as int
 * @return
 *		The driver as CDriver*
 */
CDriver* CDriverList::getDriver(unsigned int iRank)
{
	if (iRank > vDrivers.size())
		return NULL;

	multimap<double,CDriver,intcompOverall> overall;
	vector<CDriver>::iterator itV;
	multimap<double,CDriver>::iterator itM;

	for (itV = vDrivers.begin(); itV < vDrivers.end(); itV++)
	{
		CDriver* driver = &(*itV);
		double dSortKey = driver->getOverall();
		overall.insert(pair<double,CDriver>(dSortKey,*driver));
	}

	itM = overall.begin();
	for (unsigned int i = 1; i < iRank; i++)
	{
		itM++;
	}

	return &(itM->second);
}

/*!
 * Searches the driver's finish rank and returns the pointer to the driver.
 * If the driver couldn't be found a NULL pointer is returned.
 *
 *
 * @param iRank
 * 		The finish rank of the driver as int
 * @return
 *		The driver as CDriver*
 */
CDriver* CDriverList::getDriverFinish(unsigned int iRank, int iRaceCount)
{
	if (iRank > vDrivers.size())
		return NULL;

	multimap<double,CDriver,intcompOverall> overall;
	vector<CDriver>::iterator itV;
	multimap<double,CDriver>::iterator itM;

	for (itV = vDrivers.begin(); itV < vDrivers.end(); itV++)
	{
		CDriver* driver = &(*itV);
		double dSortKey = itV->getFinish(iRaceCount);
		overall.insert(pair<double,CDriver>(dSortKey,*driver));
	}

	itM = overall.begin();
	for (unsigned int i = 1; i < iRank; i++)
	{
		itM++;
	}

	return &(itM->second);
}

/*!
 * Searches the team's name and returns the pointer to the team.
 * If the team couldn't be found a NULL pointer is returned.
 *
 *
 * @param sTeamName
 * 		The name of the team as string
 * @return
 *		The team as CTeam*
 */
CTeam* CDriverList::getTeam(string sTeamName)
{
	vector<CTeam>::iterator it;

	// Team existing?
	for (it = vTeams.begin(); it < vTeams.end(); it++)
	{
		if ((*it).getName() == sTeamName)
			return &(*it);
	}

	return NULL;
}

/*!
 * Searches the team's overall rank and returns the pointer to the team.
 * If the team couldn't be found a NULL pointer is returned.
 *
 *
 * @param iRank
 * 		The overall rank of the driver as int
 * @return
 *		The driver as CTeam*
 */
CTeam* CDriverList::getTeam(unsigned int iRank)
{
	if (iRank > vTeams.size())
		return NULL;

	multimap<double,CTeam,intcompOverall> overall;
	vector<CTeam>::iterator itV;
	multimap<double,CTeam>::iterator itM;

	for (itV = vTeams.begin(); itV < vTeams.end(); itV++)
	{
		CTeam* team = &(*itV);
		double dSortKey = getTeamOverall(team->getName());
		overall.insert(pair<double,CTeam>(dSortKey,*team));
	}

	itM = overall.begin();
	for (unsigned int i = 1; i < iRank; i++)
	{
		itM++;
	}

	return &(itM->second);
}

/*!
 * Searches the team's finish rank and returns the pointer to the team.
 * If the team couldn't be found a NULL pointer is returned.
 *
 *
 * @param iRank
 * 		The overall rank of the driver as int
 * @return
 *		The driver as CTeam*
 */
CTeam* CDriverList::getTeamFinish(unsigned int iRank, int iRaceCount)
{
	if (iRank > vTeams.size())
		return NULL;

	multimap<double,CTeam,intcompOverall> finish;
	vector<CTeam>::iterator itV;
	multimap<double,CTeam>::iterator itM;

	for (itV = vTeams.begin(); itV < vTeams.end(); itV++)
	{
		CTeam* team = &(*itV);
		double dSortKey = getTeamFinish(team->getName(), iRaceCount);
		finish.insert(pair<double,CTeam>(dSortKey,*team));
	}

	itM = finish.begin();
	for (unsigned int i = 1; i < iRank; i++)
	{
		itM++;
	}

	return &(itM->second);
}

// Add stuff
/*!
 * Adds a driver to the list.
 *
 *
 * @param cDriver
 * 		The driver to be added as CDriver*
 * @return
 * 		- TRUE if the driver is added or the driver is already in the list and he is successfully added to the team
 * 		- FALSE if the driver is already in the list and the team already has two members
 */
bool CDriverList::addDriver(CDriver* cDriver)
{
	bool b;
	string sDriverLobbyName = cDriver->getLobbyName();
	string sDriverName = cDriver->getName();

	// Driver not already existing?
	if (cDriver != getDriver(sDriverLobbyName))
	{
		vDrivers.push_back(*cDriver);
		b = true;
	}

	if (addTeamMember(cDriver->getTeam(), sDriverLobbyName))
		return true;
	else return b;

	//~ return addTeamMember(cDriver->getTeam(), sDriverLobbyName) || b;
}

/*!
 * Adds an empty race result (0 credit and 0.0 points) to a driver and increases his race count.
 *
 *
 * @param sDriverLobbyName
 * 		The lobbyname as string
 * @return
 * 		- TRUE if the driver is found and the credit added
 * 		- FALSE if the driver couldn't be found
 */
bool CDriverList::addDriverEmptyResult(string sDriverLobbyName)
{
	CResult rZero;

	return addDriverResult(sDriverLobbyName, rZero);
}

/*!
 * Adds race result (credit and points) to a driver and increases his race count.
 *
 *
 * @param sDriverLobbyName
 * 		The lobbyname as string
 * @param dCredit
 * 		The credit as double
 * @return
 * 		- TRUE if the driver is found and the credit added
 * 		- FALSE if the driver couldn't be found
 */
bool CDriverList::addDriverResult(string sDriverLobbyName, CResult rResult)
{
	CDriver *driver = getDriver(sDriverLobbyName);
	if (driver == NULL)
		return false;
	driver->addResult(rResult);
	return true;
}

/*!
 * Adds penalty points to a driver.
 *
 *
 * @param sDriverLobbyName
 * 		The lobbyname as string
 * @param iPenalty
 * 		The penalty points as int
 * @return
 * 		- TRUE if the driver is found and the penalty points added
 * 		- FALSE if the driver couldn't be found
 */
bool CDriverList::addDriverPenalty(string sDriverLobbyName, int iPenalty)
{
	CDriver *driver = getDriver(sDriverLobbyName);
	if (driver == NULL)
		return false;
	driver->addPenalty(iPenalty);
	return true;
}

/*!
 * Finds the team and checks whether it has an available position.
 * If so the new member is added and TRUE returned.
 * Else FALSE is returned.
 *
 *
 * @param sTeamName
 * 		The team's name as string
 * @param sMemberLobbyName
 * 		The lobbyname of the new member as string
 * @return
 * 		- TRUE if the new member is successfully added
 * 		- FALSE if the team couldn't be found, the driver isn't part of any team or the team has already two members
 */
bool CDriverList::addTeamMember(string sTeamName, string sMemberLobbyName)
{
	CTeam* team = getTeam(sTeamName);
	//~ CDriver* driver = getDriver(sMemberLobbyName);

	if (sTeamName == "Kein Team")
		return false;

	if (team == NULL)
	{
		team = new CTeam(sTeamName, sMemberLobbyName, "");
		vTeams.push_back(*team);
		return true;
	}
	else
		return team->addMember(sMemberLobbyName);
}

// Change stuff
//~ bool CDriverList::setDriverTeam(string sDriverLobbyName, string sTeamName)
//~ {
	//~ CDriver* driver = getDriver(sDriverLobbyName);
	//~ if (driver != NULL)
	//~ {
		//~ driver->setTeam(sTeamName);
		//~ return true;
	//~ }
	//~ else
		//~ return false;
//~ }

// Read stuff
/*!
 * Counts the number of drivers.
 *
 *
 * @return
 * 		The number of drivers as int
 */
int CDriverList::getDriverCount()
{
	return vDrivers.size();
}

/*!
 * Searches the driver's overall rank and returns the lobbyname.
 *
 *
 * @param iRank
 * 		The overall rank as int
 * @return
 * 		- The lobbyname as string
 * 		- If the driver couldn't be found an empty string is returned
 */
string CDriverList::getDriverLobbyName(int iRank)
{
	CDriver* driver = getDriver(iRank);
	if (driver != NULL)
		return driver->getLobbyName();
	else
		return "";
}

/*!
 * Searches the driver's finish rank and returns the lobbyname.
 *
 *
 * @param iRank
 * 		The finish rank as int
 * @return
 * 		- The lobbyname as string
 * 		- If the driver couldn't be found an empty string is returned
 */
string CDriverList::getDriverLobbyNameFinish(int iRank, int iRaceCount)
{
	CDriver* driver = getDriverFinish(iRank, iRaceCount);
	if (driver != NULL)
		return driver->getLobbyName();
	else
		return "";
}

/*!
 * Searches the driver's lobbyname and returns the name.
 *
 *
 * @param sDriverLobbyName
 * 		The lobbyname as string
 * @return
 * 		- The name as string
 * 		- If the driver couldn't be found an empty string is returned
 */
string CDriverList::getDriverName(string sDriverLobbyName)
{
	CDriver* driver = getDriver(sDriverLobbyName);
	if (driver != NULL)
		return driver->getName();
	else
		return "";
}

/*!
 * Searches the driver's lobbyname and returns the team name.
 *
 *
 * @param sDriverLobbyName
 * 		The lobbyname as string
 * @return
 * 		- The team name as string
 * 		- If the driver couldn't be found an empty string is returned
 */
string CDriverList::getDriverTeam(string sDriverLobbyName)
{
	CDriver* driver = getDriver(sDriverLobbyName);
	if (driver != NULL)
		return driver->getTeam();
	else
		return "";
}

/*!
 * Returns the driver's race result from race iRaceNumber.
 *
 *
 * @param sDriverLobbyName
 * 		The lobbyname as string
 * @param iRaceNumber
 * 		The race number as int
 * @return
 * 		- The team name as string
 * 		- If the driver isn't member of any team "Kein Team" is returned
 */
CResult* CDriverList::getDriverRaceResult(string sDriverLobbyName, int iRaceNumber)
{
	CDriver* driver = getDriver(sDriverLobbyName);
	if (driver != NULL)
		return driver->getRaceResult(iRaceNumber);
	else
		return NULL;
}

/*!
 * Searches the driver's lobbyname and returns the penalty points.
 *
 *
 * @param sDriverLobbyName
 * 		The lobbyname as string
 * @return
 * 		- The penalty points as int
 * 		- If the driver couldn't be found 0 is returned
 */
int CDriverList::getDriverPenalty(string sDriverLobbyName)
{
	CDriver* driver = getDriver(sDriverLobbyName);
	if (driver != NULL)
		return driver->getPenalty();
	else
		return 0;
}

double CDriverList::getDriverMultMean(string sDriverLobbyName)
{
	CDriver* driver = getDriver(sDriverLobbyName);
	if (driver != NULL)
		return driver->getMultMean();
	return 0.0;
}

/*!
 * Searches the driver's lobbyname and returns the driver points.
 *
 *
 * @param sDriverLobbyName
 * 		The lobbyname as string
 * @return
 * 		- The driver points as double
 * 		- If the driver couldn't be found 0.0 is returned
 */
double CDriverList::getDriverPoints(string sDriverLobbyName)
{
	CDriver* driver = getDriver(sDriverLobbyName);
	if (driver != NULL)
		return driver->getPoints();
	else
		return 0.0;
}

/*!
 * Searches the driver's lobbyname and returns the overall points.
 *
 *
 * @param sDriverLobbyName
 * 		The lobbyname as string
 * @return
 * 		- The overall points as double
 * 		- If the driver couldn't be found 0.0 is returned
 */
double CDriverList::getDriverOverall(string sDriverLobbyName)
{
	CDriver* driver = getDriver(sDriverLobbyName);
	if (driver != NULL)
		return driver->getOverall();
	else
		return 0.0;
}

/*!
 * Searches the driver's lobbyname and returns the overall points after considering the void results.
 *
 *
 * @param sDriverLobbyName
 * 		The lobbyname as string
 * @param iRaceCount
 * 		The number of races in the season as int
 * @return
 * 		- The overall points after considering the void results as double
 * 		- If the driver couldn't be found 0.0 is returned
 */
double CDriverList::getDriverFinish(string sDriverLobbyName, int iRaceCount)
{
	CDriver* driver = getDriver(sDriverLobbyName);
	if (driver != NULL)
		return driver->getFinish(iRaceCount);
	else
		return 0.0;
}

vector<CResult> CDriverList::getDriverVoidResults(string sDriverLobbyName, int iRaceCount)
{
	CDriver* driver = getDriver(sDriverLobbyName);
	if (driver != NULL)
		return driver->getVoidResults(iRaceCount);

	vector<CResult> r;
	return r;
}

/*!
 * Searches the driver's lobbyname and returns the races he competed.
 *
 *
 * @param sDriverLobbyName
 * 		The lobbyname as string
 * @return
 * 		- The overall points after considering the void results as double
 * 		- If the driver couldn't be found 0 is returned
 */
int CDriverList::getDriverRaceCount(string sDriverLobbyName)
{
	CDriver* driver = getDriver(sDriverLobbyName);
	if (driver != NULL)
		return driver->getRaceCount();
	else
		return -1;
}

/*!
 * Counts the number of teams.
 *
 *
 * @return
 * 		The number of teams as int
 */
int CDriverList::getTeamCount()
{
	return vTeams.size();
}

/*!
 * Searches the team's overall rank and returns the name.
 *
 *
 * @param iRank
 * 		The overall rank as int
 * @return
 * 		- The name as string
 * 		- If the team couldn't be found an empty string is returned
 */
string CDriverList::getTeamName(int iRank)
{
	CTeam* team = getTeam(iRank);
	if (team != NULL)
		return team->getName();
	else
		return "";
}

/*!
 * Searches the team's finish rank and returns the name.
 *
 *
 * @param iRank
 * 		The finish rank as int
 * @return
 * 		- The name as string
 * 		- If the team couldn't be found an empty string is returned
 */
string CDriverList::getTeamNameFinish(int iRank, int iRaceCount)
{
	CTeam* team = getTeamFinish(iRank, iRaceCount);
	if (team != NULL)
		return team->getName();
	else
		return "";
}

/*!
 * Searches the team name and returns the overall points.
 *
 *
 * @param sTeamName
 * 		The team name as string
 * @return
 * 		- The overall points as double
 * 		- If the team couldn't be found 0.0 is returned
 */
double CDriverList::getTeamOverall(string sTeamName)
{
	CTeam* team = getTeam(sTeamName);
	if (team != NULL)
	{
		double dOverall = 0.0;
		CDriver* driver1 = getDriver(team->getMember(0));
		CDriver* driver2 = getDriver(team->getMember(1));
		if (driver1 != NULL)
			dOverall = driver1->getOverall();
		if (driver2 != NULL)
			dOverall = dOverall + driver2->getOverall();
		return dOverall;
	}
	else
		return 0.0;
}

int CDriverList::getTeamPenalty(string sTeamName)
{
	CTeam* team = getTeam(sTeamName);
	if (team != NULL)
	{
		int iPen = 0;
		CDriver* driver1 = getDriver(team->getMember(0));
		CDriver* driver2 = getDriver(team->getMember(1));
		if (driver1 != NULL)
			iPen = driver1->getPenalty();
		if (driver2 != NULL)
			iPen = iPen + driver2->getPenalty();
		return iPen;
	}
	else
		return 0;
}

/*!
 * Searches the team name and returns the overall points after considering the void results.
 *
 *
 * @param sTeamName
 * 		The lobbyname as string
 * @param iRaceCount
 * 		The number of races in the season as int
 * @return
 * 		- The overall points after considering the void results as double
 * 		- If the team couldn't be found 0.0 is returned
 */
double CDriverList::getTeamFinish(string sTeamName, int iRaceCount)
{
	CTeam* team = getTeam(sTeamName);
	if (team != NULL)
	{
		double dFinish = 0.0;
		CDriver* driver1 = getDriver(team->getMember(0));
		CDriver* driver2 = getDriver(team->getMember(1));
		if (driver1 != NULL)
			dFinish = driver1->getFinish(iRaceCount);
		if (driver2 != NULL)
			dFinish = dFinish + driver2->getFinish(iRaceCount);
		return dFinish;
	}
	else
		return 0.0;
}

/*!
 * Searches the team name and returns the the name of a team member.
 * If the parameter iMemberNumber is not 0 or 1 an empty string is returned.
 *
 *
 * @param sTeamName
 * 		The name of the car as string
 * @param iMemberNumber
 * 		The number of the desired member as int
 * @return
 *		- The lobbyname as string
 * 		- If the team couldn't be found or the parameter is not 0 or 1 an empty string is returned
 */
string CDriverList::getTeamMember(string sTeamName, int iMemberNumber)
{
	CTeam* team = getTeam(sTeamName);
	if (team != NULL)
		return team->getMember(iMemberNumber);
	else
		return "";
}

/*!
 * Creates and returns a string with the content.
 *
 *
 * @return
 *		The content of as string
 */
string CDriverList::toString()
{
	string s = "// Liste der Fahrer und Teams: \"Driver\",DriverName,TeamName,LobbyName,Penalty,RaceCount,Points(Credit)\n";
	vector<CDriver>::iterator it;
	for (it = vDrivers.begin(); it < vDrivers.end(); it++)
	{
		s = s + (*it).toString() + "\n";
	}
	return s;
}
