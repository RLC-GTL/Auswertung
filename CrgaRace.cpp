/*!
 * @file CrgaRace.cpp
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.8.1
 * @brief Source for the CRace class.
 */

#include "CrgaRace.h"

// Constructor & Destructor
/*!
 * Constructor of CRace.
 * An empty race is created.
 *
 *
 */
CRace::CRace()
{
	RaceHistory = "";
	RaceNumber = 0;
	LapMax = 0;
	LapMax = 0;
}

/*!
 * Destructor of CcsvList.
 *
 *
 */
CRace::~CRace()
{

}

CParameter* CRace::Parameter = NULL;

// Private functions
/*!
 * Searches the driver who finished the race at position iPosition and returns the driver.
 *
 *
 * @param iPosition
 * 		Driver's position in the race as int
 * @return
 * 		The driver as multimap<double,string>::iterator
 */
multimap<double,string>::iterator CRace::getDriver(int iPosition)
{
	int i = 1;
	multimap<double,string>::iterator it;
	for (it = Payoff.begin(); it != Payoff.end(); it++ )
	{
		if (i == iPosition)
			return it;
		i++;
	}
	return it;
}

/*!
 * Searches the driver who finished the race at position iPosition and returns the stored information.
 *
 *
 * @param iPosition
 * 		Driver's position in the race as int
 * @return
 * 		The data as string
 */
string CRace::getDriverData(int iPosition)
{
	if (getDriver(iPosition) == Payoff.end())
			return "";
	return getDriver(iPosition)->second;
}

/*!
 * Searches the driver who finished the race at position iPosition and returns the information stored at location iDataLoc.
 *
 *
 * @param iPosition
 * 		Driver's position in the race as int
 * @param iDataLoc
 * 		Location of the information in the set as int
 * @return
 * 		The information as string
 */
string CRace::getDriverData(int iPosition, int iDataLoc)
{
	stringstream ssDriverData(getDriverData(iPosition));
	string sData;
	for (int i = 0; i <= iDataLoc; i++)
		getline(ssDriverData, sData, ',');

	return sData;
}

// Add stuff
/*!
 * Adds a past race to the race history and increments the actual race number by one.
 *
 *
 * @param sRaceInfo
 * 		The race information contains the location, the date and the time as string.
 * 		E.g. Spa,2009/12/03 21:00
 */
void CRace::addRace(string sRaceInfo)
{
	RaceHistory = RaceHistory + sRaceInfo;
	RaceNumber++;
}

// Read stuff
/*!
 * Searches the driver who finished the race at position iPosition and returns his lobbyname.
 *
 *
 * @param iPosition
 * 		Driver's position in the race as int
 * @return
 * 		Driver's lobbyname as string
 */
string CRace::getDriverLobbyName(int iPosition)
{
	return getDriverData(iPosition, 1);
}

/*!
 * Searches the driver who finished the race at position iPosition and returns his name.
 *
 *
 * @param iPosition
 * 		Driver's position in the race as int
 * @return
 * 		Driver's name as string
 */
string CRace::getDriverName(int iPosition)
{
	return getDriverData(iPosition, 0);
}

/*!
 * Searches the driver who finished the race at position iPosition and returns the name of the car he rented.
 *
 *
 * @param iPosition
 * 		Driver's position in the race as int
 * @return
 * 		Driver's car name as string
 */
string CRace::getDriverCarChosen(int iPosition)
{
	return getDriverData(iPosition, 2);
}

string CRace::getDriverCarNumberChosen(int iPosition)
{
	return getDriverData(iPosition, 3);
}

/*!
 * Searches the driver who finished the race at position iPosition and returns the number of laps he drove.
 *
 *
 * @param iPosition
 * 		Driver's position in the race as int
 * @return
 * 		Driver's laps he drove as int
 */
int CRace::getDriverLaps(int iPosition)
{
	return floor(getDriver(iPosition)->first);
}

/*!
 * Searches the driver who finished the race at position iPosition and returns whether he finished or not.
 *
 *
 * @param iPosition
 * 		Driver's position in the race as int
 * @return
 * 		- TRUE if driver did not finish
 * 		- FALSE if driver finished
 */
bool CRace::getDriverDNF(int iPosition)
{
	string sDriverDNF = getDriverData(iPosition, 5);
	if (sDriverDNF == "DNF" || sDriverDNF == "DQ")
		return true;
	return false;
}

/*!
 * Tells whether driver has finishers behind or not.
 * If the driver has finished the race, he will get points.
 * If the driver hasn't finished the race but run more laps than the last finisher, he will get points.
 * If the driver hasn't finished the race and has run less laps than the last finisher, he won't get any points.
 *
 *
 * @param iPosition
 * 		Desired position in this race as int
 * @return
 * 		- TRUE if the driver will get points
 * 		- FALSE if the driver won't get any points
 */
bool CRace::getDriverBeforeFinishers(int iPosition)
{
	int i = getDriverCount();
	bool bBeforeLastFinisher = false;

	// From last to first
	multimap<double, string>::reverse_iterator rit;
	for (rit = Payoff.rbegin(); rit != Payoff.rend(); rit++)
	{
		if ((rit->first - floor(rit->first)) < 0.5)
			bBeforeLastFinisher = true;	// Finisher found
		if (i == iPosition)
			return bBeforeLastFinisher;	// Driver found
		i--;
	}
	return false;
}

/*!
 * Returns the number of drivers who competed in the race
 *
 *
 * @return
 * 		Number of drivers as int
 */
int CRace::getDriverCount()
{
	return Payoff.size();
}

/*!
 * Searches the race with iRaceNumber in the season and return the race information
 *
 *
 * @param iRaceNumber
 * 		Season race number as int
 * @return
 * 		The race information contains the location, the date and the time as string.
 * 		E.g. Spa,2009/12/03 21:03:12
 */
string CRace::getRace(int iRaceNumber)
{
	if (iRaceNumber > RaceNumber)
		return "";

	// Variables
	stringstream ssRaceHistory;
	string sRace;
	ssRaceHistory << RaceHistory;

	if (DateTime != "")
		ssRaceHistory << Location + "," + DateTime;

	for (int i = 1; i <= iRaceNumber; i++)
		getline(ssRaceHistory, sRace);
	return sRace;
}

/*!
 * Searches the race with iRaceNumber in the season and return the race location
 *
 *
 * @param iRaceNumber
 * 		Season race number as int
 * @return
 * 		The race location as string.
 * 		E.g. Spa
 */
string CRace::getRaceLocation(int iRaceNumber)
{
	if (iRaceNumber > RaceNumber)
		return "";

	stringstream ssRaceInformation(getRace(iRaceNumber));
	string sReturn;
	getline(ssRaceInformation, sReturn, ',');

	return sReturn;
}

/*!
 * Searches the race with iRaceNumber in the season and return the race date and time
 *
 *
 * @param iRaceNumber
 * 		Season race number as int
 * @return
 * 		The race date and time as string.
 * 		E.g. 2009/12/03 21:03:12
 */
string CRace::getRaceDateTime(int iRaceNumber)
{
	if (iRaceNumber > RaceNumber)
		return "";

	stringstream ssRaceInformation(getRace(iRaceNumber));
	string sReturn;
	getline(ssRaceInformation, sReturn, ',');
	getline(ssRaceInformation, sReturn);

	return sReturn;
}

/*!
 * Counts the number of races that took place in the season.
 * The actual race is included.
 *
 *
 * @return
 * 		The race number in the season as int.
 */
int CRace::getRaceNumber()
{
	return RaceNumber;
}

/*!
 * Creates a string for each race in chronological order, e.g. Spa,2009/12/03 21:00.
 * Each race is written to a new line.
 *
 *
 * @return
 * 		The race informations as string.
 */
string CRace::toString()
{
	stringstream ssRaceHistory(RaceHistory);
	string sRaceLine;
	string s = "// Liste der Rennen: \"Race\",Location,DateTime\n";
	while (getline(ssRaceHistory, sRaceLine))
		s = s + "Race," + sRaceLine + "\n";

	if (DateTime != "")
		s = s + "Race," + Location + "," + DateTime + "\n";

	return s;
}

// File handling
/*!
 * Reads the racelog and stores the date & time information, track location and the track length.
 * For each driver the name, lobbyname, carname, carnumber, number of laps, lap distance travelled and the race time (or "DNF" respectively "DQ") is stored.
 * The drivers are stored in a list: The number of laps, race time is considered and drivers with the same number of laps who finished are listed before driver who didn't finish.
 *
 *
 * @param Racelog
 * 		Season race number as char array*
 * @return
 *		The return value indicates how the function exited
 * 		- 0: Normal termination
 * 		- -1: Couldn't open file
 * 		- -2: Empty file
 * 		- -3: Racelog not complete
 */
int CRace::readLog(const char* Racelog, CParameter* cParameter)
{
	if (Parameter == NULL)
		Parameter = cParameter;
	
	// Open file
	ifstream file(Racelog);
	if (!file.is_open())
		return -1;			// Couldn't open file
	if (file.peek() == EOF)
		return -2;			// Empty file

	// Variables
	bool bLogComplete = false;
	string sFileLine;
	queue<string> qLines;
	RaceNumber++;

	// Read lines from file
	while (getline(file, sFileLine))
		qLines.push(sFileLine);

	//~ // Analyze line
	while (!qLines.empty())
	{
		string sSlot;
		stringstream ssLineForSlot(qLines.front());
		getline(ssLineForSlot, sSlot, '0');

		// Read Header
		if (qLines.front() == "[Header]")
		{
			while (qLines.front() != "")
			{
				string sTitle;
				stringstream ssLine(qLines.front());
				getline(ssLine, sTitle, '=');

				if (sTitle == "TimeString")
				{
					getline(ssLine, sTitle, '=');
					DateTime = sTitle;
				}

				qLines.pop();
			}
		}

		// Read Race
		else if (qLines.front() == "[Race]")
		{
			while (qLines.front() != "")
			{
				string sTitle;
				stringstream ssLine(qLines.front());
				getline(ssLine, sTitle, '=');

				if (sTitle == "Scene")
				{
					getline(ssLine, sTitle, '\\');	// sTitle = "GAMEDATA"
					getline(ssLine, sTitle, '\\');	// sTitle = "LOCATIONS"
					getline(ssLine, sTitle, '\\');	// sTitle = Location
					Location = sTitle;
				}
				else if (sTitle == "Track Length")
				{
					getline(ssLine, sTitle);
					stringstream ssTrackLength(sTitle);
					ssTrackLength >> TrackLength;
				}

				qLines.pop();
			}
		}

		// Read Slot xx
		else if (sSlot == "[Slot")
		{
			int iLapsTravalled;
			double dInvRaceTime = 0.0;
			double dLapDistanceTravelled = 0.0;
			string sDriverData;
			string sDriverLobbyName;

			while (qLines.front() != "")
			{
				string sTitle;
				stringstream ssLine(qLines.front());
				getline(ssLine, sTitle, '=');

				if (sTitle == "Driver")
				{
					getline(ssLine, sTitle, '=');
					sDriverData = sTitle;
					sDriverLobbyName = sTitle;
				}

				else if (sTitle == "Lobby Username")
				{
					getline(ssLine, sTitle, '=');
					sDriverData = sDriverData + "," + sTitle;
				}

				else if (sTitle == "Vehicle")
				{
					getline(ssLine, sTitle, '=');
					sDriverData = sDriverData + "," + sTitle;
				}

				else if (sTitle == "VehicleNumber")
				{
					getline(ssLine, sTitle, '=');
					sDriverData = sDriverData + "," + sTitle;
				}

				else if (sTitle == "Laps")
				{
					getline(ssLine, sTitle, '=');
					stringstream ssLapsTravalled(sTitle);
					ssLapsTravalled >> iLapsTravalled;
					if (iLapsTravalled > LapMax)
						LapMax = iLapsTravalled;
				}

				else if (sTitle == "LapDistanceTravelled")
				{
					getline(ssLine, sTitle, '=');
					stringstream ssLapDistanceTravelled(sTitle);
					ssLapDistanceTravelled >> dLapDistanceTravelled;
					sDriverData = sDriverData + "," + sTitle;
				}

				// Racetime: DNFs before finishers with same number of laps
				// [xx.5,xx+1): DNFs
				// [xx.0,xx.5): finishers
				else if (sTitle == "RaceTime")
				{
					getline(ssLine, sTitle, '=');
					if (sTitle == "DNF")
					{
						dInvRaceTime = 0.5 + (0.5 * dLapDistanceTravelled / TrackLength);
					}
					else if (sTitle == "DQ")
					{
						iLapsTravalled = 0;
						dInvRaceTime = 0.9;
					}
					else
					{
						stringstream ssRaceTime(sTitle);
						string sTimePart;
						while (getline(ssRaceTime, sTimePart, ':'))
						{
							double dTimePart;
							stringstream ssConvert(sTimePart);
							ssConvert >> dTimePart;
							dInvRaceTime = dInvRaceTime*60 + dTimePart;
						}
						dInvRaceTime = 0.5 / dInvRaceTime;
					}
					sDriverData = sDriverData + "," + sTitle;
				}

				qLines.pop();
			}
			
			//~ cout << "DEBUG CRace::readLog: " << iLapsTravalled << "L   " << sDriverData << endl;
			
			if (Parameter->getLobbynameRacer(sDriverLobbyName))
				Payoff.insert(pair<double,string>(iLapsTravalled+dInvRaceTime,sDriverData));
		}

		// Racelog complete
		else if (qLines.front() == "[END]")
		{
			bLogComplete = true;
			qLines.pop();
		}

		// Prevent infinite loop
		else
			qLines.pop();
	}

	//~ // TEST ////////////////////////////////////////////////////////////
	//~ int i = 0;
	//~ pair<multimap<double,string>::iterator,multimap<double,string>::iterator> ret;
	//~ multimap<double, string>::iterator it;
	//~ for (it = Payoff.begin(); it != Payoff.end(); it++)
	//~ {
		//~ cout << ++i << ".: " << (*it).second << " (" << (*it).first << ")";
		//~ if (!getDriverBeforeFinishers(i))
			//~ cout << " No Points ";
		//~ cout << endl;
	//~ }
	//~ // TEST ////////////////////////////////////////////////////////////

	if (bLogComplete)
		return 0;
	return -3;	// Racelog not complete
}
