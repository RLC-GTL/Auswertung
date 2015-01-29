/*!
 * @file Crga.cpp
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.8.1
 * @brief Source for the Crga class.
 */

#include "Crga.h"

// Constructor & Destructor
/*!
 * Constructor of Crga.
 * The csv file containing the overall status is read and the data stored.
 *
 *
 * @param ifileOverall
 * 		The input csv filename as char array*
 */
Crga::Crga(const char* ifileOverall)
{
	readOverall(ifileOverall);
}

/*!
 * Destructor of Crga.
 * Releases the previously allocated storage space.
 *
 *
 */
Crga::~Crga()
{
	delete Parameter;
	delete Cars;
	delete Points;
}

// Private functions
/*!
 * The csv file containing the overall status (parameters, prices, points, drivers, races) is read and the data stored.
 *
 *
 * @param ifileOverall
 * 		The input csv filename as char array*
 */
void Crga::readOverall(const char* ifileOverall)
{
	int iLine = 1;		// Start line
	const int iTitleLoc = 0;
	const int iUnusedLines = 2;
	string sTitle;

	// Read CSV file
	Overall.readCSV(ifileOverall);

	// Set parameter
	iLine = setParameters(iLine, iTitleLoc, "Parameter") + iUnusedLines;

	// Set Multis
	iLine = setMultis(iLine, iTitleLoc, "Car") + iUnusedLines;

	// Set points
	iLine = setPoints(iLine, iTitleLoc, "Point") + iUnusedLines;

	// Set drivers
	iLine = setDrivers(iLine, iTitleLoc, "Driver") + iUnusedLines;

	// Set races
	iLine = setRaces(iLine, iTitleLoc, "Race");
}

/*!
 * The parameters are read and stored.
 *
 *
 * @param iLine
 * 		The actual line in the overall list as int
 * @param iTitleLoc
 * 		The location of the line title as int
 * @param sTitle
 * 		The title of the data block as string
 */
int Crga::setParameters(int iLine, const int iTitleLoc, const string sTitle)
{
	const int iNameLoc = 1;
	const int iValueLoc = 2;

	string sSeriesName = "Serienname Platzhalter";
	string sSeriesNameShort = "Ser.Nam.P.H.";
	string sSeasonName = "Saisonnamen Platzhalter";
	int iNumberOfVoidResults = 2;
	string sPageBaseDir = "Page/";
	string sPageIndexFile = "Gesamtwertung.html";
	string sPageLogo = "rga_logo.png";

	while (Overall.getElement(iLine, iTitleLoc) == sTitle)
	{
		string sName = Overall.getElement(iLine, iNameLoc);

		if (sName == "SeriesName")
		{
			sSeriesName = Overall.getElement(iLine, iValueLoc);
		}
		else if (sName == "SeriesNameShort")
		{
			sSeriesNameShort = Overall.getElement(iLine, iValueLoc);
		}
		else if (sName == "SeasonName")
		{
			sSeasonName = Overall.getElement(iLine, iValueLoc);
		}
		else if (sName == "NumberOfVoidResults")
		{
			stringstream ss(Overall.getElement(iLine, iValueLoc));
			ss >> iNumberOfVoidResults;
		}
		else if (sName == "PageBaseDir")
		{
			sPageBaseDir = Overall.getElement(iLine, iValueLoc);
		}
		else if (sName == "PageIndexFile")
		{
			sPageIndexFile = Overall.getElement(iLine, iValueLoc);
		}
		else if (sName == "PageLogo")
		{
			sPageLogo = Overall.getElement(iLine, iValueLoc);
		}

		iLine++;
	}

	Parameter = new CParameter(
					sSeriesName,
					sSeriesNameShort,
					sSeasonName,
					iNumberOfVoidResults,
					sPageBaseDir,
					sPageIndexFile,
					sPageLogo);
					
	//~ cout << "DEBUG:";
	//~ cout << Parameter->toString();

	return iLine;
}

/*!
 * The car prices are read and stored.
 *
 *
 * @param iLine
 * 		The actual line in the overall list as int
 * @param iTitleLoc
 * 		The location of the line title as int
 * @param sTitle
 * 		The title of the data block as string
 */
int Crga::setMultis(int iLine, const int iTitleLoc, const string sTitle)
{
	const int iNameLoc = 1;
	const int iAbbrLoc = 2;
	const int iMultiLoc = 3;
	vector<CCar> newCarList;

	while (Overall.getElement(iLine, iTitleLoc) == sTitle)
	{
		double dMulti = 0.0;
		stringstream ssMulti(Overall.getElement(iLine, iMultiLoc));
		ssMulti >> dMulti;

		CCar newCar(
				Overall.getElement(iLine, iNameLoc),
				Overall.getElement(iLine, iAbbrLoc),
				dMulti,
				Parameter);
		newCarList.push_back(newCar);

		iLine++;
	}

	Cars = new CCarList(newCarList);
	
	//~ cout << "DEBUG:";
	//~ cout << Cars->toString();

	return iLine;
}

/*!
 * The driver points are read and stored.
 *
 *
 * @param iLine
 * 		The actual line in the overall list as int
 * @param iTitleLoc
 * 		The location of the line title as int
 * @param sTitle
 * 		The title of the data block as string
 */
int Crga::setPoints(int iLine, const int iTitleLoc, const string sTitle)
{
	const int iPositionLoc = 1;
	const int iPointsLoc = 2;
	map<int,int> miPoints;

	while (Overall.getElement(iLine, iTitleLoc) == sTitle)
	{
		int iPosition = 0;
		int iPoints = 0;
		stringstream ssPosition(Overall.getElement(iLine, iPositionLoc));
		stringstream ssPoints(Overall.getElement(iLine, iPointsLoc));
		ssPosition >> iPosition;
		ssPoints >> iPoints;

		miPoints[iPosition] = iPoints;

		iLine++;
	}

	Points = new CPoint(miPoints);
	
	//~ cout << "DEBUG:";
	//~ cout << Points->toString();

	return iLine;
}

/*!
 * The drivers are read and stored.
 *
 *
 * @param iLine
 * 		The actual line in the overall list as int
 * @param iTitleLoc
 * 		The location of the line title as int
 * @param sTitle
 * 		The title of the data block as string
 */
int Crga::setDrivers(int iLine, const int iTitleLoc, const string sTitle)
{
	const int iDriverNameLoc = 1;
	const int iDriverTeamLoc = 2;
	const int iDriverLobbyNameLoc = 3;
	const int iDriverPenaltyLoc = 4;
	const int iDriverRaceCountLoc = 5;
	const int iDriverResultStartLoc = 6;
	vector<CDriver> vDrivers;
	vector<CTeam> vTeams;

	while (Overall.getElement(iLine, iTitleLoc) == sTitle)
	{
		int iDriverResultLoc = iDriverResultStartLoc;
		CResult rScore();
		string sTeam = "Kein Team";
		vector<CResult> vResults;
		int iPenalty = 0;
		int iRaceCount = 0;

		stringstream ssPenalty(Overall.getElement(iLine, iDriverPenaltyLoc));
		stringstream ssRaceCount(Overall.getElement(iLine, iDriverRaceCountLoc));
		if (ssPenalty.str() != "Element not found")
			ssPenalty >> iPenalty;
		if (ssRaceCount.str() != "Element not found")
			ssRaceCount >> iRaceCount;
		if (Overall.getElement(iLine, iDriverTeamLoc) != "Element not found")
			sTeam = Overall.getElement(iLine, iDriverTeamLoc);
		while (Overall.getElement(iLine, iDriverResultLoc) != "Element not found")
		{
			string sCarAbr;
			string sPoints;
			int iPoints;

			stringstream ssResult(Overall.getElement(iLine, iDriverResultLoc));
			getline(ssResult, sPoints, '(');
			stringstream ssPoints(sPoints);
			ssPoints >> iPoints;
			getline(ssResult, sCarAbr, ')');
			CResult rResult(sCarAbr, Cars->getMult(sCarAbr), iPoints);
			vResults.push_back(rResult);

			iDriverResultLoc++;
		}

		CDriver* newDriver = new CDriver(
					Overall.getElement(iLine, iDriverLobbyNameLoc),
					Overall.getElement(iLine, iDriverNameLoc),
					sTeam,
					iPenalty,
					iRaceCount,
					vResults,
					Parameter,
					Cars);

		Drivers.addDriver(newDriver);
		iLine++;
	}

	return iLine;
}

/*!
 * The races are read and stored.
 *
 *
 * @param iLine
 * 		The actual line in the overall list as int
 * @param iTitleLoc
 * 		The location of the line title as int
 * @param sTitle
 * 		The title of the data block as string
 */
int Crga::setRaces(int iLine, const int iTitleLoc, const string sTitle)
{
	const int iRaceLocationLoc = 1;
	const int iRaceDateTimeLoc = 2;

	while (Overall.getElement(iLine, iTitleLoc) == sTitle)
	{
		string sRaceInfo =
			Overall.getElement(iLine, iRaceLocationLoc) + "," +
			Overall.getElement(iLine, iRaceDateTimeLoc) + "\n";
		Race.addRace(sRaceInfo);
		iLine++;
	}

	return iLine;
}

/*!
 * Fixes a string to a defined length.
 * If the string is shorter than the desired length it is expanded with space characters.
 * Else the string is shortened.
 * In both cases the string can either be left- of right-justified.
 *
 *
 * @param sString
 * 		The original string as string
 * @param iLength
 * 		The desired length as int
 * @param bRightJustified
 * 		The desired alignment as bool
 * 		- TRUE right-justified
 * 		- FALSE left-justified
 */
string Crga::limitStringLength(string sString, unsigned int iLength, bool bRightJustified)
{	// FUNKTIONIERT NICHT MIT UMLAUTEN: length() zählt diese doppelt!!!
	string s = sString;
	if (sString.length() > iLength)
	{	// Trim
		if (bRightJustified)
		{
			s = sString.substr(sString.length() - iLength, iLength);
		}
		else
		{
			s = sString.substr(0, iLength);
		}
	}
	else
	{	// Fill
		if (bRightJustified)
		{
			for (unsigned int i = 0; i < iLength - sString.length(); i++)
				s = " " + s;
		}
		else
		{
			for (unsigned int i = 0; i < iLength - sString.length(); i++)
				s = s + " ";
		}
	}
	return s;
}

/*!
 * Rounds a number to the desired number decimal places.
 *
 *
 * @param dNumber
 * 		The number as double
 * @param iDigits
 * 		The desired number decimal places as unsigned int
 */
double Crga::round(double dNumber, unsigned int iDigits)
{
    double v[] = { 1, 10, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8 };  // mgl. verlängern
    return floor(dNumber * v[iDigits] + 0.5) / v[iDigits];
}

// Read stuff
/*!
 * The overall ranking of each driver and team is calculated.
 * A preliminary result with a title, a list of the drivers, teams, races and cars is generated.
 *
 *
 * @return
 *		An preliminary overview of the season as string
 */
string Crga::toString()
{
	stringstream ss;
	ss << "\nRLC GTL " << Parameter->getSeasonName() <<
		" (Rennen " << Race.getRaceNumber() << ")\n";
	unsigned int issLength = ss.str().length() - 2;
	for (unsigned int i = 0; i < issLength; i++)
		ss << '-';
	ss << endl;

	// Einzelwertung
	ss << "Einzelwertung:\n";
	ss << "Pos ";
	ss << limitStringLength("Team", 25, false) << "\t";
	ss << limitStringLength("Fahrer", 25, false) << "\t";
	ss << limitStringLength("Rennen", 10, true);
	ss << limitStringLength("Pen", 10, true);
	ss << limitStringLength("Punkte", 10, true);
	ss << endl;

	for (int i = 1; i <= Drivers.getDriverCount(); i++)
	{
		string sLobbyName = Drivers.getDriverLobbyName(i);

		stringstream ssPos;
		ssPos << i;
		ss << limitStringLength(ssPos.str(), 2, true) << ". ";

		ss << limitStringLength(Drivers.getDriverTeam(sLobbyName), 25, false) << "\t";
		ss << limitStringLength(Drivers.getDriverName(sLobbyName), 25, false) << "\t";

		int iNumOfRaces = Drivers.getDriverRaceCount(sLobbyName);
		stringstream ssNumOfRaces;
		ssNumOfRaces << iNumOfRaces;
		ss << limitStringLength(ssNumOfRaces.str(), 10, true);

		int iPen = Drivers.getDriverPenalty(sLobbyName);
		stringstream ssPen;
		ssPen << iPen;
		ss << limitStringLength(ssPen.str(), 10, true);

		double dOverall = round(Drivers.getDriverOverall(sLobbyName), 1);
		stringstream ssOverall;
		ssOverall << dOverall;
		ss << limitStringLength(ssOverall.str(), 10, true);

		ss << endl;
	}
	ss << endl;

	// Teamwertung
	if (Drivers.getTeamCount() > 0)
	{
		ss << "Teamwertung:\n";
		ss << "Pos ";
		ss << limitStringLength("Team", 25, false) << "\t";
		ss << limitStringLength("Fahrer", 53, false) << "\t";
		ss << limitStringLength("Punkte", 10, true);
		ss << endl;
		for (int i = 1; i <= Drivers.getTeamCount(); i++)
		{
			string sTeamName = Drivers.getTeamName(i);

			stringstream ssPos;
			ssPos << i;
			ss << limitStringLength(ssPos.str(), 2, true) << ". ";

			ss << limitStringLength(sTeamName, 25, false) << "\t";
			if (Drivers.getTeamMember(sTeamName, 1) == "")
			{
				string sMember1 = Drivers.getDriverName(Drivers.getTeamMember(sTeamName, 0));
				ss << limitStringLength(sMember1, 53, false) << "\t";
			}
			else
			{
				string sMember1 = Drivers.getDriverName(Drivers.getTeamMember(sTeamName, 0));
				string sMember2 = Drivers.getDriverName(Drivers.getTeamMember(sTeamName, 1));
				ss << limitStringLength(sMember1 + " & " + sMember2, 53, false) << "\t";
			}

			double dOverall = round(Drivers.getTeamOverall(sTeamName), 1);
			stringstream ssOverall;
			ssOverall << dOverall;
			ss << limitStringLength(ssOverall.str(), 10, true);
			ss << endl;
		}
		ss << endl;
	}

	// Bisherige Rennen
	if (Race.getRaceNumber() > 0)
	{
		ss << "Rennliste:\n";
		ss << "Num ";
		ss << limitStringLength("Strecke", 25, false);
		ss << limitStringLength("Datum", 11, false);
		ss << limitStringLength("Uhrzeit", 8, true);
		ss << endl;

		for (int i = 1; i <= Race.getRaceNumber(); i++)
		{
			stringstream ssRaceInfo(Race.getRace(i));
			string sRaceInfo;

			stringstream ssNum;
			ssNum << i;
			ss << limitStringLength(ssNum.str(), 3, true) << " ";

			getline(ssRaceInfo, sRaceInfo, ',');
			ss << limitStringLength(sRaceInfo, 25, false);

			getline(ssRaceInfo, sRaceInfo, ',');
			ss << limitStringLength(sRaceInfo, 25, false);

			ss << endl;
		}
		ss << endl;
	}

	// Autoliste
	if (Cars->getCarCount() > 0)
	{
		ss << "Autoliste:\n";
		ss << limitStringLength("Abk.", 10, false);
		ss << limitStringLength("Wagen", 25, false);
		ss << limitStringLength("Multi", 15, true);
		ss << endl;
		for (int i = 0; i < Cars->getCarCount(); i++)
		{
			string sAbr = Cars->getAbrByID(i);
			ss << limitStringLength(sAbr, 10, false);
			
			string sCar = Cars->getName(sAbr);
			ss << limitStringLength(sCar, 25, false);

			double dMult = Cars->getMult(sAbr);
			stringstream ssMult;
			ssMult << dMult;
			ss << limitStringLength(ssMult.str(), 15, true);

			ss << endl;
		}
		//ss << endl;
	}

	return ss.str();
}

/*!
 * The season will be terminated by considering the void and overall results of each driver and team.
 * A general survey with a list of the drivers and teams is generated.
 *
 *
 * @return
 *		An overview at the end of the season as string
 */
string Crga::finishSeason()
{
	// Variables
	int iRaces = Race.getRaceNumber();

	stringstream ss;
	ss << "\nRLC GTL (Saisonendergebnis nach " << Race.getRaceNumber() << " Rennen)\n";
	unsigned int issLength = ss.str().length() - 2;
	for (unsigned int i = 0; i < issLength; i++)
		ss << '-';
	ss << endl;

	// Einzelwertung
	ss << "Einzelwertung:\n";
	ss << "Pos ";
	ss << limitStringLength("Team", 25, false) << "\t";
	ss << limitStringLength("Fahrer", 25, false) << "\t";
	ss << limitStringLength("Streicherg", 10, true);
	ss << limitStringLength("Pen", 10, true);
	ss << limitStringLength("Punkte", 10, true);
	ss << endl;
	for (int i = 1; i <= Drivers.getDriverCount(); i++)
	{
		// Variables
		string sLobbyName = Drivers.getDriverLobbyName(i);

		stringstream ssPos;
		ssPos << i;
		ss << limitStringLength(ssPos.str(), 2, true) << ". ";

		ss << limitStringLength(Drivers.getDriverTeam(sLobbyName), 25, false) << "\t";
		ss << limitStringLength(Drivers.getDriverName(sLobbyName), 25, false) << "\t";

		stringstream ssVoidResults;
		double dVoidResults = Drivers.getDriverOverall(sLobbyName) - Drivers.getDriverFinish(sLobbyName, iRaces);
		if (dVoidResults > 0)
			ssVoidResults << dVoidResults;
		else
			ssVoidResults << 0;
		ss << limitStringLength(ssVoidResults.str(), 10, true);
		//~ ss << limitStringLength("666", 10, true);

		int iPen = Drivers.getDriverPenalty(sLobbyName);
		stringstream ssPen;
		ssPen << iPen;
		ss << limitStringLength(ssPen.str(), 10, true);

		double dOverall = round(Drivers.getDriverFinish(sLobbyName, iRaces), 1);
		stringstream ssOverall;
		ssOverall << dOverall;
		ss << limitStringLength(ssOverall.str(), 10, true);

		ss << endl;
	}
	ss << endl;

	// Teamwertung
	if (Drivers.getTeamCount() > 0)
	{
		ss << "Teamwertung:\n";
		ss << "Pos ";
		ss << limitStringLength("Team", 25, false) << "\t";
		ss << limitStringLength("Fahrer", 53, false) << "\t";
		ss << limitStringLength("Punkte", 10, true);
		ss << endl;
		for (int i = 1; i <= Drivers.getTeamCount(); i++)
		{
			string sTeamName = Drivers.getTeamName(i);

			stringstream ssPos;
			ssPos << i;
			ss << limitStringLength(ssPos.str(), 2, true) << ". ";

			ss << limitStringLength(sTeamName, 25, false) << "\t";
			if (Drivers.getTeamMember(sTeamName, 1) == "")
			{
				string sMember1 = Drivers.getDriverName(Drivers.getTeamMember(sTeamName, 0));
				ss << limitStringLength(sMember1, 53, false) << "\t";
			}
			else
			{
				string sMember1 = Drivers.getDriverName(Drivers.getTeamMember(sTeamName, 0));
				string sMember2 = Drivers.getDriverName(Drivers.getTeamMember(sTeamName, 1));
				ss << limitStringLength(sMember1 + " & " + sMember2, 53, false) << "\t";
			}

			double dOverall = round(Drivers.getTeamFinish(sTeamName, iRaces), 1);
			stringstream ssOverall;
			ssOverall << dOverall;
			ss << limitStringLength(ssOverall.str(), 10, true);
			ss << endl;
		}
		ss << endl;
	}

	return ss.str();
}

// File handling
/*!
 * The given racelog is read and evaluated.
 * If a driver isn't listed in CDriverList Drivers he ist added.
 * Each driver pays the car, earns credit and points.
 * The changes are returned as string or an empty string is returned if the racelog couldn't be read.
 *
 *
 * @param Racelog
 * 		The name of the racelog-file that will be read and evaluated as char array*
 * @return
 *		- The changes caused by the race results as string
 * 		- If the racelog can't be read an empty string is returned
 */
string Crga::readRacelog(const char* Racelog)
{
	// Read racelog if unsuccessful return empty string
	if (Race.readLog(Racelog) != 0)
		return "";

	// Variables
	double dWinnerLaps = Race.getDriverLaps(1);
	stringstream ssRet;
	ssRet << "MaxLaps: " << dWinnerLaps;
	ssRet << endl;

	for (int iPosition = 1; iPosition <= Race.getDriverCount(); iPosition++)
	{
		// Variables
		double dDriverLaps = Race.getDriverLaps(iPosition);
		string sDriverLobbyName = Race.getDriverLobbyName(iPosition);
		string sDriverCarChosen = Race.getDriverCarChosen(iPosition);
		int iPointEarn = 0;
		string sCarAbr = Cars->getAbr(sDriverCarChosen);

		// Add driver if not already listed
		if (!Drivers.addDriverPenalty(sDriverLobbyName, 0))
		{
			vector<CResult> vResults;
			CDriver *driver = new CDriver(
									sDriverLobbyName,
									Race.getDriverName(iPosition),
									"Kein Team",
									0,
									Race.getRaceNumber() - 1,
									vResults,
									Parameter,
									Cars);

			Drivers.addDriver(driver);
			
			ssRet << "+ ";
		}

		// Earn points
		if (Race.getDriverBeforeFinishers(iPosition))
			iPointEarn = Points->getPoints(iPosition);	// Driver finished

		// Update driver statistics
		CResult rScore(sCarAbr, Cars->getMult(sCarAbr), iPointEarn);
		Drivers.addDriverResult(sDriverLobbyName, rScore);
		

		// Prepare return string
		
		ssRet << iPosition << ".: " <<
				Race.getDriverName(iPosition) << " (" << sDriverLobbyName << "), " <<
				//sDriverCarChosen << " (" << sCarAbr << "," << Cars->getMult(sCarAbr) << "), " <<
				sCarAbr << " (" << Cars->getMult(sCarAbr) << "), " <<
				dDriverLaps << " Laps";
		if (!Race.getDriverBeforeFinishers(iPosition))
			ssRet << ", no Points";
		else
			ssRet << ", " << iPointEarn << " Point(s)";
		ssRet << endl;
	}

	// For drivers not attending fill results up with empty result to allow "result to race" allocation
	for (int i = 1; i <= Drivers.getDriverCount(); i++)
	{
		string sLobbyName =  Drivers.getDriverLobbyName(i);
		if (Drivers.getDriverRaceCount(sLobbyName) < Race.getRaceNumber())
			Drivers.addDriverEmptyResult(sLobbyName);
	}

	return ssRet.str();
}

/*!
 * The save file for the actual season standing is generated.
 * It contains the parameters, cars with the cost, position with the points, drivers and races with the important data.
 *
 *
 * @param ofileOverall
 * 		The name of the csv-file that will be generated as char array*
 * @return
 *		The return value indicates how the function exited
 * 		- 0: Normal termination
 * 		- -1: Couldn't open file
 */
int Crga::writeOverall(const char* ofileOverall)
{	// TODO CcsvList.addLines & CcsvList.writeCSV
	ofstream file(ofileOverall);
	if (!file.is_open())
		return -1;

	file << Parameter->toString() + "\n";
	file << Cars->toString() + "\n";
	file << Points->toString() + "\n";
	if (Race.getRaceNumber() > 0)
	{
		file << Drivers.toString() + "\n";
		file << Race.toString() + "\n";
	}
	file.close();

	return 0;
}

/*!
 * Creates the Racersleague GTL evaluation pages with the class CrgaHTML and it's function CrgaHTML::writeHTML().
 *
 *
 * @return
 *		The return code from CrgaHTML::writeHTML()
 */
string Crga::writeHTML()
{
	CrgaHTML Page;

	return Page.writeHTML(Parameter, Points, Cars, &Drivers, &Race);
}
