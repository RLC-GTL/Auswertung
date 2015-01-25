/*!
 * @file CrgaHTML.cpp
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.8.1
 * @brief Source for the CPageHTML class.
 */

#include "CrgaHTML.h"


// Constructor & Destructor
/*!
 * Constructor of CrgaHTML.
 */
CrgaHTML::CrgaHTML()
{

}

/*!
 * Destructor of CrgaHTML.
 *
 *
 */
CrgaHTML::~CrgaHTML()
{

}

// Create HTML code
/*!
 * Creates the Racersleague GTL evaluation HTML pages.
 * Creates the driver pages, the race page and the overall page during the season.
 *
 *
 * @return
 *		The return value indicates how the function exited
 * 		- 0: Normal termination
 * 		- -1: Couldn't open file
 */
string CrgaHTML::writeHTML(CParameter* instParameter, CPoint* instPoints, CCarList* instCars, CDriverList* instDrivers, CRace* instRace)
{
	stringstream ssRet;
	string sFileIndex = instParameter->getPageBaseDir() + instParameter->getPageIndexFile();

	// Create drivers, race and overall pages

	// Create overall page
	CPageHTMLOverall PageOverall(instParameter, instPoints, instCars, instDrivers, instRace);
	string sFileOverall = PageOverall.getFileName(instParameter->getPageBaseDir(), instRace->getRaceNumber());
	ssRet << "Creating Pro Overview Page: " << sFileOverall << endl;

	// Create most recent overall page (k+10)_DateCode[k]_SeasonName_.html
	ofstream ofOverall(sFileOverall.c_str());
	if (!ofOverall.is_open())
		return "ERROR CrgaHTML::writeHTML: While opening file\n";
	else
		PageOverall.getPage(ofOverall);
	ofOverall.close();

	// Create index file
	ifstream ifOverall(sFileOverall.c_str());
	ofstream ofIndex(sFileIndex.c_str());
	if ((!ifOverall.is_open()) || (!ofIndex.is_open()))
		return "ERROR CrgaHTML::writeHTML: While opening file\n";
	else
	{
		string sFileLine;
		while (getline(ifOverall, sFileLine))
			ofIndex << sFileLine;
	}
	ifOverall.close();
	ofIndex.close();


	// Create race page
	CPageHTMLRace PageRace(instParameter, instPoints, instCars, instDrivers, instRace);
	string sFileRace = PageRace.getFileName(instParameter->getPageBaseDir(), instRace->getRaceNumber());
	//~ cout << "DEBUG CrgaHTML::writeHTML Creating " << sFileRace << endl;

	// Create most recent race page (k+10)_DateCode[k]_SeasonName_TrackName[k].html
	ofstream ofRace(sFileRace.c_str());
	if (!ofRace.is_open())
		return "ERROR CrgaHTML::writeHTML: While opening file\n";
	else
		PageRace.getPage(ofRace);
	ofRace.close();


	// Create driver page
	for (int i = 1; i <= instDrivers->getDriverCount(); i++)
	{
		string sDriverLobbyName = instDrivers->getDriverLobbyName(i);
		CPageHTMLDriver PageDriver(sDriverLobbyName, instParameter, instPoints, instCars, instDrivers, instRace);
		string sFileDriver = PageDriver.getFileName(instParameter->getPageBaseDir(), instRace->getRaceNumber(), sDriverLobbyName);
		//~ cout << "DEBUG CrgaHTML::writeHTML Creating " << sFileDriver << endl;

		// for each driver create new page: (k+10)_DateCode[k]_SeasonName_DriverName[i].html
		ofstream ofDriver(sFileDriver.c_str());
		if (!ofDriver.is_open())
			return "ERROR CrgaHTML::writeHTML: While opening file\n";
		else
			PageDriver.getPage(ofDriver);
		ofDriver.close();
	}

	return ssRet.str();
}


// Constructor & Destructor
/*!
 * Constructor of CPageHTML.
 *
 *
 */
CPageHTML::CPageHTML(CParameter* instParameter, CPoint* instPoints, CCarList* instCars, CDriverList* instDrivers, CRace* instRace)
{
	Charset = "UTF-8";
	DocType = "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01//EN\"\n\"http://www.w3.org/TR/html4/strict.dtd\">\n";;
	HTMLStart = "<html>\n";
	HTMLEnd = "</html>\n";

	if (Parameter == NULL)
	{
		Parameter = instParameter;
		Points = instPoints;
		Cars = instCars;
		Drivers = instDrivers;
		Race = instRace;

		SeriesName = "RLC The Duel";
		SeriesNameShort = "RLC Duel";
		SeasonName = Parameter->getSeasonName();
		SeasonNameCode = SeasonName;
		erase_all(SeasonNameCode, " ");
		erase_all(SeasonNameCode, "/");
		erase_all(SeasonNameCode, "ä");
		erase_all(SeasonNameCode, "ö");
		erase_all(SeasonNameCode, "ü");
		erase_all(SeasonNameCode, "Ä");
		erase_all(SeasonNameCode, "Ö");
		erase_all(SeasonNameCode, "Ü");
	}
}

/*!
 * Destructor of CDriverList.
 *
 *
 */
CPageHTML::~CPageHTML()
{

}

// Protected functions
/*!
 * Rounds a number to the desired number decimal places.
 *
 *
 * @param dNumber
 * 		The number as double
 * @param iDigits
 * 		The desired number decimal places as unsigned int
 */
double CPageHTML::round(double dNumber, unsigned int iDigits)
{
    double v[] = { 1, 10, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8 };  // mgl. verlängern
    return floor(dNumber * v[iDigits] + 0.5) / v[iDigits];
}

/*!
 * The HTML-header with the title is created.
 *
 *
 * @param oStream
 * 		The output stream as ostream&
 * @param sPageDescription
 * 		The description of the page as string
 */
void CPageHTML::getPageHeader(ostream &oStream, string sPageDescription)
{
	oStream << "<head>\n";
	oStream << " <meta name=\"generator\" content=\"HTML Tidy for Linux/x86 (vers 6 November 2007), see www.w3.org\">\n";
	oStream << " <meta content=\"text/html; charset=" << Charset << "\" http-equiv=\"content-type\">\n";
	oStream << " <title>" << SeriesNameShort + " " + sPageDescription << "</title>\n";
	oStream << "</head>\n\n";
	oStream << "<link rel=\"stylesheet\" href=\"style.css\" type=\"text/css\">\n\n";
}

/*!
 * The date when the page was created is written.
 *
 *
 * @param oStream
 * 		The output stream as ostream&
 */
void CPageHTML::getPageCreationDate(ostream &oStream)
{
	time_t now = time(0);
	char timestamp[22];
	strftime(timestamp, 22, "%d.%m.%Y - %H:%M:%S", localtime(&now));

	oStream << "<p class=\"modifydate\">Erstellt am " << timestamp << "</p>\n" << endl;


/*	time_t ttNow;
	tm *tmNow;
	ttNow = time(0);
	tmNow = localtime(&ttNow);

	oStream << "<p class=\"modifydate\">Erstellt am " << tmNow->tm_mday << '.' << tmNow->tm_mon+1 << '.'
		<< tmNow->tm_year+1900 << ", " << tmNow->tm_hour
		<< ':' << tmNow->tm_min << "</p>\n";*/
}

/*!
 * A caption from level iLevel is written.
 *
 *
 * @param oStream
 * 		The output stream as ostream&
 * @param sPageCaption
 * 		The caption as string
 * @param
 * 		The caption's level as int
 */
void CPageHTML::getPageCaption(ostream &oStream, string sPageCaption, int iLevel)
{
	oStream << "<span style=\"font-weight: bold; text-decoration: underline;\">" << sPageCaption << "</span><br>\n";
}

/*!
 * The rules table is written
 *
 *
 * @param oStream
 * 		The output stream as ostream&
 */
//~ void CPageHTML::getTableRules(ostream &oStream)
//~ {
	//~ int iWidthName = 70;
	//~ int iWidthValue = 30;
//~
	//~ oStream << "<table style=\"text-align: center; width: 100%;\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">\n";
	//~ oStream << "<tbody>\n";
//~
	//~ // Title
	//~ oStream << "<tr>\n";
	//~ oStream << "<th scope=\"col\"; align=\"left\"; style=\"width: " << iWidthName << "%;\">Regel</th>\n";
	//~ oStream << "<th scope=\"col\"; style=\"width: " << iWidthValue << "%;\">Wert</th>\n";
	//~ oStream << "</tr>\n";
//~
	//~ // Data
	//~ oStream << "<tr class=\"bgcolor1\">\n";
	//~ oStream << "<td><div align=\"left\">Startkredit</div></td>\n";	// Name
	//~ oStream << "<td><div align=\"center\">" << Parameter->getStartCredit() << "</div></td>\n";	// Wert
	//~ oStream << "</tr>\n";
//~
	//~ oStream << "<tr>\n";
	//~ oStream << "<td><div align=\"left\">Multiplikator (Kontostand positiv)</div></td>\n";	// Name
	//~ oStream << "<td><div align=\"center\">" << Parameter->getMult(+1.0) << "</div></td>\n";	// Wert
	//~ oStream << "</tr>\n";
//~
	//~ oStream << "<tr class=\"bgcolor1\">\n";
	//~ oStream << "<td><div align=\"left\">Multiplikator (Kontostand negativ)</div></td>\n";	// Name
	//~ oStream << "<td><div align=\"center\">" << Parameter->getMult(-1.0) << "</div></td>\n";	// Wert
	//~ oStream << "</tr>\n";
//~
	//~ oStream << "<tr>\n";
	//~ oStream << "<td><div align=\"left\">Anzahl Streichresultate</div></td>\n";	// Name
	//~ oStream << "<td><div align=\"center\">" << Parameter->getNumberOfVoidResults() << "</div></td>\n";	// Wert
	//~ oStream << "</tr>\n";
//~
//~
	//~ oStream << "</tbody>\n";
	//~ oStream << "</table>\n";
//~ }

/*!
 * Initialization of the static variables and pointers
 *
 *
 */
string CPageHTML::SeriesName = "";
string CPageHTML::SeriesNameShort = "";
string CPageHTML::SeasonName = "";
string CPageHTML::SeasonNameCode = "";
string CPageHTML::Charset = "";
string CPageHTML::DocType = "";
string CPageHTML::HTMLStart = "";
string CPageHTML::HTMLEnd = "";
CParameter* CPageHTML::Parameter = NULL;
CPoint* CPageHTML::Points = NULL;
CCarList* CPageHTML::Cars = NULL;
CDriverList* CPageHTML::Drivers = NULL;
CRace* CPageHTML::Race = NULL;


// Constructor & Destructor
/*!
 * Constructor of CPageHTMLOverall.
 * The series name, season name, the parameters, the list of cars, points, drivers and races are stored.
 *
 *
 * @param sSeasonName
 * 		The season name as string
 * @param sSeasonName
 * 		The series name as string
 * @param instParameter
 * 		The parameters as CParameter*
 * @param instCars
 * 		The list of cars as CCarList*
 * @param instPoints
 * 		The list of points as CPoint*
 * @param instDrivers
 * 		The list of drivers as CDriverList*
 * @param instRace
 * 		The list of races as CRace*
 */
CPageHTMLOverall::CPageHTMLOverall(CParameter* instParameter, CPoint* instPoints, CCarList* instCars, CDriverList* instDrivers, CRace* instRace)
	: CPageHTML(instParameter, instPoints, instCars, instDrivers, instRace)
{

}

/*!
 * Destructor of CDriverList.
 *
 *
 */
CPageHTMLOverall::~CPageHTMLOverall()
{

}

// Private functions
/*!
 * The HTML-body of the overall page with page description sPageDescription is created.
 *
 *
 * @param cPageType
 * 		The output stream as ostream&
 * @param sPageDescription
 *		The page description as string
 */
void CPageHTMLOverall::getPageBody(ostream &oStream, string sPageDescription)
{
	string sBannerName = "rlc_logo_gtl_gentlemens.png";
	int iBannerWidth = 199;
	int iBannerHeight = 122;

	oStream << "<body>\n";
	oStream << "<table border=\"0\" align=\"center\">\n";
	oStream << "<tr>\n";
	oStream << "<th valign=\"top\"; scope=\"col\"; style=\"width: 100%;\">\n";
	oStream << "<big align=\"center\"; style=\"font-weight: bold; text-decoration: underline;\">" << SeriesName << " " << SeasonName << " (Lauf "  << Race->getRaceNumber() << ")</big><br>";
	oStream << "<img align=\"center\"; style=\"width: " << iBannerWidth << "px; height: " << iBannerHeight << "px;\" " << "alt=\"" << sBannerName << "\" " << "src=\"" << sBannerName << "\" align=\"top\">";
	oStream << "</th>\n";
	oStream << "</tr>\n";
	oStream << "</table><br><br><br>\n";

	// Overall table
	getPageCaption(oStream, "Gesamtwertung", 1);
	oStream << "<br>\n";
	getTableOverall(oStream);
	oStream << "<br><br>\n";

	// Team table
	getPageCaption(oStream, "Teamwertung", 1);
	oStream << "<br>\n";
	getTableTeam(oStream);
	oStream << "<br><br>\n";

	// Drivers vs. races table
	//~ oStream << "<td valign=\"top\">\n";
	getPageCaption(oStream, "Fahrer - Rennen (Zielankunft)", 1);
	oStream << "<br>\n";
	getTableDriversRaces(oStream);
	oStream << "<br><br>\n";
	//~ oStream << "</td>\n";

	// Races table
	//~ oStream << "<td valign=\"top\">\n";
	getPageCaption(oStream, "Rennen", 1);
	oStream << "<br>\n";
	getTableRaces(oStream);
	oStream << "<br><br>\n";
	//~ oStream << "</td>\n";

	// Last changed
	oStream << "<div align=\"right\">\n";
	getPageCreationDate(oStream);
	oStream << "</div>\n";

	oStream << "</body>\n";
}

/*!
 * The overall table is written
 *
 *
 * @param oStream
 * 		The output stream as ostream&
 */
void CPageHTMLOverall::getTableOverall(ostream &oStream)
{
	int iWidthPos = 5;
	int iWidthDriver = 15;
	int iWidthTeam = 15;
	int iWidthRP = 5;
	int iWidthPen = 5;
	int iWidthVoids = 5;
	int iWidthFinish = 5;

	int iRaceNumber = Race->getRaceNumber();

	oStream << "<table style=\"text-align: center; width: 100%;\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">\n";
	oStream << "<tbody>\n";

	// Title
	oStream << "<tr>\n";
	oStream << "<th scope=\"col\"; style=\"width: " << iWidthPos << "%;\">Pos.</div></td>\n"; // Position
	oStream << "<th scope=\"col\"; align=\"left\"; style=\"width: " << iWidthDriver << "%;\">Fahrer</th>\n"; // Fahrer
	oStream << "<th scope=\"col\"; align=\"left\"; style=\"width: " << iWidthTeam << "%;\">Team</th>\n"; // Team
	oStream << "<th scope=\"col\"; style=\"width: " << iWidthRP << "%;\">Rennp.</div></td>\n"; // RP
	oStream << "<th scope=\"col\"; style=\"width: " << iWidthPen << "%;\">Strafp.</div></td>\n"; // Pen
	oStream << "<th scope=\"col\"; style=\"width: " << iWidthVoids << "%;\">Str.erg.</div></td>\n"; // Streichergebnisse
	oStream << "<th scope=\"col\"; style=\"width: " << iWidthFinish << "%;\">Bereinigt</div></td>\n"; // Endstand
	oStream << "</tr>\n";

	// Data
	for (int i = 1; i <= Drivers->getDriverCount(); i++)
	{
		string sDriverLobbyName = Drivers->getDriverLobbyNameFinish(i, iRaceNumber);
		int iDriverPenalty = Drivers->getDriverPenalty(sDriverLobbyName);
		double dDiverOverall = Drivers->getDriverPoints(sDriverLobbyName);
		double dDriverFinish = Drivers->getDriverFinish(sDriverLobbyName, iRaceNumber);
		double dDriverVoids = dDiverOverall - dDriverFinish - iDriverPenalty;

		// Line background
		if (i % 2)
			oStream << "<tr class=\"bgcolor1\">\n";
		else
			oStream << "<tr>\n";

		oStream << "<td><div align=\"center\">" << i << "</div></td>\n"; // Position
		oStream << "<td><div align=\"left\"><a href=\"" << CPageHTMLDriver::getFileName("", Race->getRaceNumber(), sDriverLobbyName) << "\">" << Drivers->getDriverName(sDriverLobbyName) << "</div></td>\n"; // Fahrer
		oStream << "<td><div align=\"left\">" << Drivers->getDriverTeam(sDriverLobbyName) << "</div></td>\n"; // Team
		oStream << "<td><div align=\"center\">" << dDiverOverall << "</div></td>\n"; // RP
		oStream << "<td><div align=\"center\">" << iDriverPenalty << "</div></td>\n"; // Pen
		oStream << "<td><div align=\"center\">" << dDriverVoids << "</div></td>\n"; // Streichergebnisse
		oStream << "<td><div align=\"center\">" << dDriverFinish << "</div></td>\n"; // Bereinigt

		oStream << "</tr>\n";
	}

	oStream << "</tbody>\n";
	oStream << "</table>\n";
}

/*!
 * The overall table is written
 *
 *
 * @param oStream
 * 		The output stream as ostream&
 */
void CPageHTMLOverall::getTableTeam(ostream &oStream)
{
	int iWidthPos = 5;
	int iWidthTeam = 20;
	int iWidthDrivers = 30;
	int iWidthRace = 5;
	int iWidthPen = 5;
	int iWidthVoids = 5;
	int iWidthFinish = 5;

	int iRaceNumber = Race->getRaceNumber();


	oStream << "<table style=\"text-align: center; width: 100%;\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">\n";
	oStream << "<tbody>\n";

	// Title
	oStream << "<tr>\n";
	oStream << "<th scope=\"col\"; style=\"width: " << iWidthPos << "%;\">Pos.</div></td>\n"; // Position
	oStream << "<th scope=\"col\"; align=\"left\"; style=\"width: " << iWidthTeam << "%;\">Team</th>\n"; // Team
	oStream << "<th scope=\"col\"; align=\"left\"; style=\"width: " << iWidthDrivers << "%;\">Fahrer</th>\n"; // Fahrer
	oStream << "<th scope=\"col\"; style=\"width: " << iWidthRace << "%;\">Rennp.</div></td>\n"; // RP
	oStream << "<th scope=\"col\"; style=\"width: " << iWidthPen << "%;\">Strafp.</div></td>\n"; // Pen
	oStream << "<th scope=\"col\"; style=\"width: " << iWidthVoids << "%;\">Str.erg.</div></td>\n"; // Streichergebnisse
	oStream << "<th scope=\"col\"; style=\"width: " << iWidthFinish << "%;\">Bereinigt</div></td>\n"; // Gesamtwertung
	oStream << "</tr>\n";

	// Data
	for (int i = 1; i <= Drivers->getTeamCount(); i++)
	{
		string sTeamName = Drivers->getTeamNameFinish(i, iRaceNumber);
		string sDriverLobbyName1 = Drivers->getTeamMember(sTeamName, 0);
		string sDriverLobbyName2 = Drivers->getTeamMember(sTeamName, 1);

		int iDriverPenalty = Drivers->getTeamPenalty(sTeamName);
		double dDiverOverall = Drivers->getDriverPoints(sDriverLobbyName1) + Drivers->getDriverPoints(sDriverLobbyName2);
		double dDriverFinish = Drivers->getDriverFinish(sDriverLobbyName1, iRaceNumber) + Drivers->getDriverFinish(sDriverLobbyName2, iRaceNumber);
		double dDriverVoids = dDiverOverall - dDriverFinish - iDriverPenalty;

		// Line background
		if (i % 2)
			oStream << "<tr class=\"bgcolor1\">\n";
		else
			oStream << "<tr>\n";

		oStream << "<td><div align=\"center\">" << i << "</div></td>\n"; // Position
		oStream << "<td><div align=\"left\">" << sTeamName << "</div></td>\n"; // Team

		oStream << "<td><div align=\"left\"><a href=\"" << CPageHTMLDriver::getFileName("", Race->getRaceNumber(), sDriverLobbyName1) << "\">" << Drivers->getDriverName(sDriverLobbyName1) << "</a>";
		if (sDriverLobbyName2 != "")
			oStream << " & " << "<a href=\"" << CPageHTMLDriver::getFileName("", Race->getRaceNumber(), sDriverLobbyName2) << "\">" << Drivers->getDriverName(sDriverLobbyName2) << "</a>";
		oStream << "</div></td>\n"; // Fahrer

		oStream << "<td><div align=\"center\">" << dDiverOverall << "</div></td>\n"; // RP
		oStream << "<td><div align=\"center\">" << Drivers->getTeamPenalty(sTeamName) << "</div></td>\n"; // Pen
		oStream << "<td><div align=\"center\">" << dDriverVoids << "</div></td>\n"; // Streichergebnisse
		oStream << "<td><div align=\"center\">" << dDriverFinish << "</div></td>\n"; // Bereinigt

		oStream << "</tr>\n";
	}

	oStream << "</tbody>\n";
	oStream << "</table>\n";
}

/*!
 * The drivers-races table is written
 *
 *
 * @param oStream
 * 		The output stream as ostream&
 */
void CPageHTML::getTableDriversRaces(ostream &oStream)
{
	int iWidthDriver = 3;
	int iWidthRaces = 1;

	oStream << "<table style=\"text-align: center; width: 100%;\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">\n";
	oStream << "<tbody>\n";

	// Title
	oStream << "<tr>\n";
	oStream << "<th scope=\"col\"; align=\"left\"; style=\"width: " << iWidthDriver << "%;\">Fahrer / Lauf</th>\n";
	for (int i = 1; i <= Race->getRaceNumber(); i++)
		oStream << "<th scope=\"col\"; style=\"width: " << iWidthRaces << "%;\"><a href=\"" << CPageHTMLRace::getFileName("", i) << "\">" << i << "</div></td>\n";
	oStream << "</tr>\n";

	// Data
	for (int i = 1; i <= Drivers->getDriverCount(); i++)
	{
		string sDriverLobbyName = Drivers->getDriverLobbyName(i);

		// Line background
		if (i % 2)
			oStream << "<tr class=\"bgcolor1\">\n";
		else
			oStream << "<tr>\n";

		oStream << "<td><div align=\"left\"><a href=\"" << CPageHTMLDriver::getFileName("", Race->getRaceNumber(), sDriverLobbyName) << "\">" << Drivers->getDriverName(sDriverLobbyName) << "</div></td>\n"; // Lauf
		for (int j = 1; j <= Race->getRaceNumber(); j++)
		{
			CResult* rResult = Drivers->getDriverRaceResult(sDriverLobbyName, j);
			if (rResult == NULL)
				return;

			int iPoints = rResult->getPoints();

			if (iPoints > 0)
			{
				//~ oStream << "<td><div align=\"center\">" << iPoints << "</div></td>\n";
				oStream << "<td><div align=\"center\">" << Points->getPosition(iPoints) << "</div></td>\n";
			}
			else
				oStream << "<td><div align=\"center\">--</div></td>\n";
		}

		oStream << "</tr>\n";
	}

	oStream << "</tbody>\n";
	oStream << "</table>\n";
}

/*!
 * The races table is written
 *
 *
 * @param oStream
 * 		The output stream as ostream&
 */
void CPageHTMLOverall::getTableRaces(ostream &oStream)
{
	int iWidthNumber = 10;
	int iWidthLocation = 45;
	int iWidthDateTime = 45;

	oStream << "<table style=\"text-align: center; width: 100%;\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">\n";
	oStream << "<tbody>\n";

	// Title
	oStream << "<tr>\n";
	oStream << "<th scope=\"col\"; style=\"width: " << iWidthNumber << "%;\">Lauf</th>\n";
	oStream << "<th scope=\"col\"; align=\"left\"; style=\"width: " << iWidthLocation << "%;\">Strecke</th>\n";
	oStream << "<th scope=\"col\"; style=\"width: " << iWidthDateTime << "%;\">Datum und Zeit</th>\n";
	oStream << "</tr>\n";

	// Data
	for (int i = 1; i <= Race->getRaceNumber(); i++)
	{
		// Line background
		if (i % 2)
			oStream << "<tr class=\"bgcolor1\">\n";
		else
			oStream << "<tr>\n";

		// Race
		oStream << "<td><div align=\"center\"><a href=\"" << CPageHTMLRace::getFileName("", i) << "\">" << i << "</div></td>\n"; // Lauf
		oStream << "<td><div align=\"left\"><a href=\"" << CPageHTMLRace::getFileName("", i) << "\">" << Race->getRaceLocation(i) << "</div></td>\n"; // Strecke
		oStream << "<td><div align=\"center\">" << Race->getRaceDateTime(i) << "</div></td>\n";	// Datum und Zeit

		oStream << "</tr>\n";
	}

	oStream << "</tbody>\n";
	oStream << "</table>\n";
}

/*!
 * The cars table is written
 *
 *
 * @param oStream
 * 		The output stream as ostream&
 */
//~ void CPageHTMLOverall::getTableCars(ostream &oStream)
//~ {
	//~ int iWidthCar = 60;
	//~ int iWidthCost = 20;
	//~ int iWidthRentings = 20;
//~
	//~ oStream << "<table style=\"text-align: center; width: 100%;\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">\n";
	//~ oStream << "<tbody>\n";
//~
	//~ // Title
	//~ oStream << "<tr>\n";
	//~ oStream << "<th scope=\"col\"; align=\"left\"; style=\"width: " << iWidthCar << "%;\">Wagen</th>\n";
	//~ oStream << "<th scope=\"col\"; style=\"width: " << iWidthCost << "%;\">Kosten</th>\n";
	//~ oStream << "<th scope=\"col\"; style=\"width: " << iWidthRentings << "%;\">Anz.Verm.</th>\n";
	//~ oStream << "</tr>\n";
//~
	//~ // Data
	//~ for (int i = 1; i <= Cars->getCarCount(); i++)
	//~ {
		//~ string sCarName = Cars->getName(i);
//~
		//~ // Line background
		//~ if (i % 2)
			//~ oStream << "<tr class=\"bgcolor1\">\n";
		//~ else
			//~ oStream << "<tr>\n";
//~
		//~ // Race
		//~ oStream << "<td><div align=\"left\">" << sCarName << "</div></td>\n";	// Wagen
		//~ oStream << "<td><div align=\"center\">" << Cars->getPrice(sCarName) << "</div></td>\n";	// Kosten
		//~ oStream << "<td><div align=\"center\">" << Cars->getUseCount(sCarName) << "</div></td>\n";	// AnzVerm
//~
		//~ oStream << "</tr>\n";
	//~ }
//~
	//~ oStream << "</tbody>\n";
	//~ oStream << "</table>\n";
//~ }

/*!
 * ...
 *
 *
 * @param sPageBaseDir
 * 		The page base directory as string
 * @return
 *		...
 */
string CPageHTMLOverall::getFileName(string sPageBaseDir, int iRaceNumber)
{
	stringstream ssRaceNumber;
	ssRaceNumber << (iRaceNumber + 10);

	string sRaceDateTimeCode = Race->getRaceDateTime(iRaceNumber);
	erase_all(sRaceDateTimeCode, "/");
	erase_all(sRaceDateTimeCode, " ");
	erase_all(sRaceDateTimeCode, ":");

	return sPageBaseDir + ssRaceNumber.str() +  "_" + sRaceDateTimeCode + "_" + SeasonNameCode + ".html";
}

// Page content
void CPageHTMLOverall::getPage(ostream &oStream)
{
	string sPageDescription = "Gesamtwertung Lauf ";
	stringstream ss;
	ss << Race->getRaceNumber();
	sPageDescription = sPageDescription + ss.str();

	oStream << DocType;
	oStream << HTMLStart;

	getPageHeader(oStream, sPageDescription);
	getPageBody(oStream, sPageDescription);

	oStream << HTMLEnd;
}


// Constructor & Destructor
/*!
 * Constructor of CPageHTMLDriver.
 * The series name, season name, the parameters, the list of cars, points, drivers and races are stored.
 *
 *
 * @param sDriverLobbyName
 * 		The driver's lobby name as string
 * @param instParameter
 * 		The parameters as CParameter*
 * @param instCars
 * 		The list of cars as CCarList*
 * @param instPoints
 * 		The list of points as CPoint*
 * @param instDrivers
 * 		The list of drivers as CDriverList*
 * @param instRace
 * 		The list of races as CRace*
 */
CPageHTMLDriver::CPageHTMLDriver(string sDriverLobbyName, CParameter* instParameter, CPoint* instPoints, CCarList* instCars, CDriverList* instDrivers, CRace* instRace)
	: CPageHTML(instParameter, instPoints, instCars, instDrivers, instRace)
{
	DriverLobbyName = sDriverLobbyName;
}

/*!
 * Destructor of CDriverList.
 *
 *
 */
CPageHTMLDriver::~CPageHTMLDriver()
{

}

// Private functions
/*!
 * The HTML-body of the driver page with page description sPageDescription is created.
 *
 *
 * @param cPageType
 * 		The output stream as ostream&
 * @param sPageDescription
 *		The page description as string
 */
void CPageHTMLDriver::getPageBody(ostream &oStream, string sPageDescription)
{
	string sBannerName = "rlc_logo_gtl_gentlemens.png";
	int iBannerWidth = 199;
	int iBannerHeight = 122;
	//~ int iTableWidth = 75;

	oStream << "<body>\n";
	oStream << "<table border=\"0\" align=\"center\">\n";
	oStream << "<tr>\n";
	oStream << "<th valign=\"top\"; scope=\"col\"; style=\"width: 100%;\">\n";
	oStream << "<big align=\"center\"; style=\"font-weight: bold; text-decoration: underline;\">" << SeriesName << " " << SeasonName << " (Lauf "  << Race->getRaceNumber() << ")</big><br>";
	oStream << "<img align=\"center\"; style=\"width: " << iBannerWidth << "px; height: " << iBannerHeight << "px;\" " << "alt=\"" << sBannerName << "\" " << "src=\"" << sBannerName << "\" align=\"top\">";
	oStream << "</th>\n";
	oStream << "</tr>\n";
	oStream << "</table><br><br><br>\n";

	//~ oStream << "<table align=\"center\"; style=\"text-align: left; width: " << iTableWidth << "%;\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">\n";

	//~ oStream << "<tr>\n";

	// Driver details table
	//~ oStream << "<td valign=\"top\">\n";
	getPageCaption(oStream, "Fahrerdetails " + Drivers->getDriverName(DriverLobbyName), 1);
	oStream << "<br>\n";
	getTableDriverRaces(oStream);
	oStream << "<br><br>\n";
	//~ oStream << "</td>\n";

	// Driver details table
	//~ oStream << "<td valign=\"top\">\n";
	//~ getPageCaption(oStream, "Fahrzeuge " + Drivers->getDriverName(DriverLobbyName), 1);
	//~ oStream << "<br>\n";
	//~ getTableDriverCars(oStream);
	//~ oStream << "<br><br>\n";
	//~ oStream << "</td>\n";

	//~ getPageCaption(oStream, "Teamwertung", 1);
	//~ oStream << "<br>\n";
	//~ getTableTeam(oStream);
	//~ oStream << "<br><br>\n";

	//~ oStream << "</tr>\n";

	//~ oStream << "</table>\n";

	// Last changed
	oStream << "<div align=\"right\">\n";
	getPageCreationDate(oStream);
	oStream << "</div>\n";

	oStream << "</body>\n";
}

/*!
 * The driver-races table is written
 *
 *
 * @param oStream
 * 		The output stream as ostream&
 */
void CPageHTMLDriver::getTableDriverRaces(ostream &oStream)
{
	int iWidthRace = 5;
	int iWidthTrack = 15;
	//~ int iWidthCar = 15;
	//~ int iWidthMulti = 5;
	int iWidthRacePos = 5;
	int iWidthRacePoints = 5;
	//~ int iWidthOverall = 5;


	oStream << "<table style=\"text-align: center; width: 100%;\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">\n";
	oStream << "<tbody>\n";

	// Title
	oStream << "<tr>\n";
	oStream << "<th scope=\"col\"; style=\"width: " << iWidthRace << "%;\">Lauf</th>\n";
	oStream << "<th scope=\"col\"; align=\"left\"; style=\"width: " << iWidthTrack << "%;\">Strecke</th>\n";
	//~ oStream << "<th scope=\"col\"; align=\"left\"; style=\"width: " << iWidthCar << "%;\">Fahrzeug</th>\n";
	//~ oStream << "<th scope=\"col\"; style=\"width: " << iWidthMulti << "%;\">Multi</th>\n";
	oStream << "<th scope=\"col\"; style=\"width: " << iWidthRacePos << "%;\">Rennpos.</th>\n";
	oStream << "<th scope=\"col\"; style=\"width: " << iWidthRacePoints << "%;\">Punkte</th>\n";
	//~ oStream << "<th scope=\"col\"; style=\"width: " << iWidthOverall << "%;\">Geamtpkte</th>\n";
	oStream << "</tr>\n";

	// Data
	vector<CResult> rVoid;
	if (Parameter->getNumberOfVoidResults() < Race->getRaceNumber())
		rVoid = Drivers->getDriverVoidResults(DriverLobbyName, Race->getRaceNumber());

	int i;
	for (i = 1; i <= Race->getRaceNumber(); i++)
	{
		CResult* rResult = Drivers->getDriverRaceResult(DriverLobbyName, i);
		if (rResult == NULL)
			return;

		bool bIsVoid = false;
		string sCarAbr = rResult->getCarAbr();

		for (vector<CResult>::iterator it = rVoid.begin(); it != rVoid.end(); it++)
		{
			if ( rResult->getResult() == it->getResult() )
			{
				bIsVoid = true;
				rVoid.erase(it);
				break;
			}
		}

		// Line background
		if (i % 2)
			oStream << "<tr class=\"bgcolor1\">\n";
		else
			oStream << "<tr>\n";

		// Race Number
		oStream << "<td><div align=\"center\"><a href=\"" << CPageHTMLRace::getFileName("", i) << "\">" << i << "</div></td>\n";

		// Track
		oStream << "<td><div align=\"left\">" << Race->getRaceLocation(i) << "</div></td>\n";

		//~ // Car
		//~ oStream << "<td><div align=\"left\">" << Cars->getName(sCarAbr) << "</div></td>\n";

		//~ // Multi
		//~ if (sCarAbr != "")
			//~ oStream << "<td><div align=\"center\">" << rResult->getMult() << "</div></td>\n";
		//~ else
			//~ oStream << "<td><div align=\"center\">--</div></td>\n";

		// Race
		if (rResult->getPoints() != 0)
		{
			oStream << "<td><div align=\"center\">" << Points->getPosition(rResult->getPoints()) << "</div></td>\n";
			//~ oStream << "<td><div align=\"center\">" << rResult->getPoints() << "</div></td>\n";
		}
		//~ else if (sCarAbr != "")
		//~ {
			//~ oStream << "<td><div align=\"center\">DNF</div></td>\n";
			//~ oStream << "<td><div align=\"center\">0</div></td>\n";
		//~ }
		else
		{
			oStream << "<td><div align=\"center\">--</div></td>\n";
			//~ oStream << "<td><div align=\"center\">--</div></td>\n";
		}

		// Overall
		if (bIsVoid)
		{
			//~ if (sCarAbr != "")
				oStream << "<td><div class=\"voids\" align=\"center\">" << rResult->getPoints() << "</div></td>\n";
			//~ else
				//~ oStream << "<td><div class=\"voids\" align=\"center\">--</div></td>\n";
		}
		else
		{
			//~ if (sCarAbr != "")
				oStream << "<td><div align=\"center\">" << rResult->getPoints() << "</div></td>\n";
			//~ else
				//~ oStream << "<td><div align=\"center\">--</div></td>\n";
		}

		oStream << "</tr>\n";
	}

	//~ // Mean Multi
	//~ if (Drivers->getDriverMultMean(DriverLobbyName) != 0.0)
	//~ {
		//~ if (i % 2)
			//~ oStream << "<tr class=\"bgcolor1\">\n";
		//~ else
			//~ oStream << "<tr>\n";
//~ 
		//~ oStream << "<td></td>\n";
		//~ oStream << "<td></td>\n";
		//~ oStream << "<td><div align=\"right\">Mittelwert</div></td>\n";
		//~ oStream << "<td><div align=\"center\">" << Drivers->getDriverMultMean(DriverLobbyName) << "</div></td>\n";
		//~ oStream << "<td></td>\n";
		//~ oStream << "<td></td>\n";
		//~ oStream << "<td></td>\n";
//~ 
		//~ oStream << "</tr>\n";
	//~ }

	oStream << "</tbody>\n";
	oStream << "</table>\n";

	oStream << "<div align=\"right\">\n";
	if (Parameter->getNumberOfVoidResults() == 1)
		oStream << "<span class=\"voids\">Streichergebnis fliesst in Endabrechnung ein</span><br>\n";
	else if (Parameter->getNumberOfVoidResults() > 1)
		oStream << "<span class=\"voids\">Streichergebnisse fliessen in Endabrechnung ein</span><br>\n";
	oStream << "</div>\n";
}

/*!
 * ...
 *
 *
 * @param sPageBaseDir
 * 		The page base directory as string
 * @return
 *		...
 */
string CPageHTMLDriver::getFileName(string sPageBaseDir, int iRaceNumber, string sDriverLobbyName)
{
	stringstream ssRaceNumber;
	ssRaceNumber << (iRaceNumber + 10);

	string sDriverNameCode = Drivers->getDriverName(sDriverLobbyName);
	erase_all(sDriverNameCode, " ");
	erase_all(sDriverNameCode, "/");
	replace_all(sDriverNameCode, "&auml;", "ae");
	replace_all(sDriverNameCode, "&ouml;", "oe");
	replace_all(sDriverNameCode, "&uuml;", "ue");
	replace_all(sDriverNameCode, "&Auml;", "Ae");
	replace_all(sDriverNameCode, "&Ouml;", "Oe");
	replace_all(sDriverNameCode, "&Uuml;", "Ue");

	string sRaceDateTimeCode = Race->getRaceDateTime(iRaceNumber);
	erase_all(sRaceDateTimeCode, "/");
	erase_all(sRaceDateTimeCode, " ");
	erase_all(sRaceDateTimeCode, ":");

	return sPageBaseDir + ssRaceNumber.str() + "_" + sRaceDateTimeCode + "_" + SeasonNameCode + "_" + sDriverNameCode + ".html";
}

// Page content
void CPageHTMLDriver::getPage(ostream &oStream)
{
	stringstream ssRaceNumber;
	ssRaceNumber << Race->getRaceNumber();
	string sPageDescription = Drivers->getDriverName(DriverLobbyName) + " Lauf " + ssRaceNumber.str();

	oStream << DocType;
	oStream << HTMLStart;

	getPageHeader(oStream, sPageDescription);
	getPageBody(oStream, sPageDescription);

	oStream << HTMLEnd;
}


// Constructor & Destructor
/*!
 * Constructor of CPageHTMLRace.
 * The series name, season name, the parameters, the list of cars, points, drivers and races are stored.
 *
 *
 * @param instParameter
 * 		The parameters as CParameter*
 * @param instCars
 * 		The list of cars as CCarList*
 * @param instPoints
 * 		The list of points as CPoint*
 * @param instDrivers
 * 		The list of drivers as CDriverList*
 * @param instRace
 * 		The list of races as CRace*
 */
CPageHTMLRace::CPageHTMLRace(CParameter* instParameter, CPoint* instPoints, CCarList* instCars, CDriverList* instDrivers, CRace* instRace)
	: CPageHTML(instParameter, instPoints, instCars, instDrivers, instRace)
{

}

/*!
 * Destructor of CPageHTMLRace.
 *
 *
 */
CPageHTMLRace::~CPageHTMLRace()
{

}

// Private functions
/*!
 * The HTML-body of the driver page with page description sPageDescription is created.
 *
 *
 * @param cPageType
 * 		The output stream as ostream&
 * @param sPageDescription
 *		The page description as string
 */
void CPageHTMLRace::getPageBody(ostream &oStream, string sPageDescription)
{
	string sBannerName = "rlc_logo_gtl_gentlemens.png";
	int iBannerWidth = 199;
	int iBannerHeight = 122;
	//~ int iTableWidth = 100;

	oStream << "<body>\n";
	oStream << "<table border=\"0\" align=\"center\">\n";
	oStream << "<tr>\n";
	oStream << "<th valign=\"top\"; scope=\"col\"; style=\"width: 100%;\">\n";
	oStream << "<big align=\"center\"; style=\"font-weight: bold; text-decoration: underline;\">" << SeriesName << " " << SeasonName << " (Lauf "  << Race->getRaceNumber() << ")</big><br>";
	oStream << "<img align=\"center\"; style=\"width: " << iBannerWidth << "px; height: " << iBannerHeight << "px;\" " << "alt=\"" << sBannerName << "\" " << "src=\"" << sBannerName << "\" align=\"top\">";
	oStream << "</th>\n";
	oStream << "</tr>\n";
	oStream << "</table><br><br><br>\n";

	// Race details table
	getPageCaption(oStream, "Laufdetails " + Race->getRaceLocation(Race->getRaceNumber()), 1);
	oStream << "<br>\n";
	getTableRaceDrivers(oStream);
	oStream << "<br><br>\n";

	//~ // Race Quali and Hotlap
	//~ oStream << "<table align=\"center\"; style=\"text-align: left; width: " << iTableWidth << "%;\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">\n";
	//~ oStream << "<tbody>\n";
	//~
	//~ oStream << "<tr>\n";
	//~ oStream << "<th valign=\"top\": scope=\"col\"; style=\"width: 50%;\">\n";
	//~ getPageCaption(oStream, "Qualifikation " + Race->getRaceLocation(Race->getRaceNumber()), 1);
	//~ oStream << "<br>\n";
	//~ getTableRaceQuali(oStream);
	//~ oStream << "</th>\n";
	//~ oStream << "<th valign=\"top\"; scope=\"col\"; style=\"width: 50%;\">\n";
	//~ getPageCaption(oStream, "Hotlaps " + Race->getRaceLocation(Race->getRaceNumber()), 1);
	//~ oStream << "<br>\n";
	//~ getTableRaceHotlap(oStream);
	//~ oStream << "</th>\n";
	//~ oStream << "</tr>\n";
	//~
	//~ oStream << "</tbody>\n";
	//~ oStream << "</table>\n";
	//~ oStream << "<br><br>\n";



	// Last changed
	oStream << "<div align=\"right\">\n";
	getPageCreationDate(oStream);
	oStream << "</div>\n";

	oStream << "</body>\n";
}

/*!
 * The race-drivers table is written
 *
 *
 * @param oStream
 * 		The output stream as ostream&
 */
void CPageHTMLRace::getTableRaceDrivers(ostream &oStream)
{
	int iWidthOverallPos = 5;
	int iWidthDriver = 15;
	int iWidthCar = 15;
	int iWidthMult = 5;
	int iWidthRacePos = 5;
	int iWidthRacePoints = 5;
	int iWidthOverall = 5;

	int iRaceNumber = Race->getRaceNumber();

	// Prepare result table
	multimap<double,string,compDouble> mRaceResult;
	for (int i = 1; i <= Drivers->getDriverCount(); i++)
	{
		string sDriverLobbyName = Drivers->getDriverLobbyName(i);
		CResult* rResult = Drivers->getDriverRaceResult(sDriverLobbyName, iRaceNumber);
		if (rResult == NULL)
			return;
		if (rResult->getCarAbr() != "")
			mRaceResult.insert(make_pair(rResult->getResult(), sDriverLobbyName));
	}

	oStream << "<table style=\"text-align: center; width: 100%;\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">\n";
	oStream << "<tbody>\n";

	// Title
	oStream << "<tr>\n";
	oStream << "<th scope=\"col\"; style=\"width: " << iWidthOverallPos << "%;\">Gesamtpos.</th>\n";
	oStream << "<th scope=\"col\"; align=\"left\"; style=\"width: " << iWidthDriver << "%;\">Fahrer</th>\n";
	oStream << "<th scope=\"col\"; align=\"left\"; style=\"width: " << iWidthCar << "%;\">Fahrzeug</th>\n";
	oStream << "<th scope=\"col\"; style=\"width: " << iWidthMult << "%;\">Multi</th>\n";
	oStream << "<th scope=\"col\"; style=\"width: " << iWidthRacePos << "%;\">Rennpos.</th>\n";
	oStream << "<th scope=\"col\"; style=\"width: " << iWidthRacePoints << "%;\">Rennpkte</th>\n";
	oStream << "<th scope=\"col\"; style=\"width: " << iWidthOverall << "%;\">Geamtpkte</th>\n";
	oStream << "</tr>\n";

	// Data
	int iPos = 1;
	for (multimap<double,string>::iterator it = mRaceResult.begin(); it != mRaceResult.end(); it++)
	{
		string sDriverLobbyName = it->second;

		CResult* rResult = Drivers->getDriverRaceResult(sDriverLobbyName, iRaceNumber);
		if (rResult == NULL)
			return;

		 string sCarAbr = rResult->getCarAbr();

		// Line background
		if (iPos % 2)
			oStream << "<tr class=\"bgcolor1\">\n";
		else
			oStream << "<tr>\n";

		// Position
		if (rResult->getResult() != 0)
			oStream << "<td><div align=\"center\">" << iPos << "</div></td>\n";	// Position
		else
			oStream << "<td><div align=\"center\">--</div></td>\n";	// Position

		// Driver
		oStream << "<td><div align=\"left\"><a href=\"" << CPageHTMLDriver::getFileName("", iRaceNumber, sDriverLobbyName) << "\">" << Drivers->getDriverName(sDriverLobbyName) << "</div></td>\n";	// Driver

		// Car
		oStream << "<td><div align=\"left\">" << Cars->getName(sCarAbr) << "</div></td>\n";

		// Multi
		if (sCarAbr != "")
			oStream << "<td><div align=\"center\">" << rResult->getMult() << "</div></td>\n";
		else
			oStream << "<td><div align=\"center\">--</div></td>\n";



		// Race
		if (rResult->getPoints() != 0)
		{
			oStream << "<td><div align=\"center\">" << Points->getPosition(rResult->getPoints()) << "</div></td>\n";
			oStream << "<td><div align=\"center\">" << rResult->getPoints() << "</div></td>\n";
		}
		else
		{
			oStream << "<td><div align=\"center\">DNF</div></td>\n";
			oStream << "<td><div align=\"center\">0</div></td>\n";
		}

		// Overall
		oStream << "<td><div align=\"center\">" << rResult->getResult() << "</div></td>\n";


		oStream << "</tr>\n";

		iPos++;
	}

	oStream << "</tbody>\n";
	oStream << "</table>\n";


}

//~ void CPageHTMLRace::getTableRaceQuali(ostream &oStream)
//~ {
	//~ int iWidthQualiPos = 5;
	//~ int iWidthDriver = 15;
	//~ int iWidthCar = 15;
	//~ int iWidthQualTime = 5;
	//~ int iWidthQualPoints = 5;
//~
	//~ int iRaceNumber = Race->getRaceNumber();
//~
	//~ // Prepare result table
	//~ multimap<double,string,compTime> mRaceResult;
	//~ for (int i = 1; i <= Drivers->getDriverCount(); i++)
	//~ {
		//~ string sDriverLobbyName = Drivers->getDriverLobbyName(i);
		//~ CResult* rResult = Drivers->getDriverRaceResult(sDriverLobbyName, iRaceNumber);
		//~ if (rResult == NULL)
			//~ return;
		//~ if (rResult->getCar() != "")
			//~ mRaceResult.insert(make_pair(rResult->getTimeSec(QUALI), sDriverLobbyName));
	//~ }
//~
	//~ oStream << "<table style=\"text-align: center; width: 100%;\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">\n";
	//~ oStream << "<tbody>\n";
//~
	//~ // Title
	//~ oStream << "<tr>\n";
	//~ oStream << "<th scope=\"col\"; style=\"width: " << iWidthQualiPos << "%;\">Qualipos.</th>\n";
	//~ oStream << "<th scope=\"col\"; align=\"left\"; style=\"width: " << iWidthDriver << "%;\">Fahrer</th>\n";
	//~ oStream << "<th scope=\"col\"; align=\"left\"; style=\"width: " << iWidthCar << "%;\">Fahrzeug</th>\n";
	//~ oStream << "<th scope=\"col\"; style=\"width: " << iWidthQualTime << "%;\">Qualizeit</th>\n";
	//~ oStream << "<th scope=\"col\"; style=\"width: " << iWidthQualPoints << "%;\">Qualipkte</th>\n";
	//~ oStream << "</tr>\n";
//~
	//~ // Data
	//~ int iPos = 1;
	//~ for (multimap<double,string>::iterator it = mRaceResult.begin(); it != mRaceResult.end(); it++)
	//~ {
		//~ string sDriverLobbyName = it->second;
		//~ CResult* rResult = Drivers->getDriverRaceResult(sDriverLobbyName, iRaceNumber);
		//~ if (rResult == NULL)
			//~ return;
//~
		//~ // Line background
		//~ if (iPos % 2)
			//~ oStream << "<tr class=\"bgcolor1\">\n";
		//~ else
			//~ oStream << "<tr>\n";
//~
		//~ // Position
		//~ if (rResult->getTime(QUALI) != "")
			//~ oStream << "<td><div align=\"center\">" << iPos << "</div></td>\n";	// Position
		//~ else
			//~ oStream << "<td><div align=\"center\">--</div></td>\n";	// Position
		//~
		//~ // Driver
		//~ oStream << "<td><div align=\"left\"><a href=\"" << CPageHTMLDriver::getFileName("", iRaceNumber, sDriverLobbyName) << "\">" << Drivers->getDriverName(sDriverLobbyName) << "</div></td>\n";	// Driver
		//~
		//~ // Car
		//~ oStream << "<td><div align=\"left\">" << rResult->getCar() << "</div></td>\n";
		//~
		//~ // Quali
		//~ if (rResult->getTime(QUALI) != "")
		//~ {
			//~ oStream << "<td><div align=\"center\">" << rResult->getTime(QUALI) << "</div></td>\n";
			//~ oStream << "<td><div align=\"center\">" << rResult->getPoints(QUALI) << "</div></td>\n";
		//~ }
		//~ else
		//~ {
			//~ oStream << "<td><div align=\"center\">DNQ</div></td>\n";
			//~ oStream << "<td><div align=\"center\">0</div></td>\n";
		//~ }
//~
		//~ oStream << "</tr>\n";
//~
		//~ iPos++;
	//~ }
	//~
	//~ oStream << "</tbody>\n";
	//~ oStream << "</table>\n";
//~ }
//~
//~ void CPageHTMLRace::getTableRaceHotlap(ostream &oStream)
//~ {
	//~ int iWidthHotlapPos = 5;
	//~ int iWidthDriver = 15;
	//~ int iWidthCar = 15;
	//~ int iWidthHotlapTime = 5;
	//~ int iWidthHotlapPoints = 5;
//~
	//~ int iRaceNumber = Race->getRaceNumber();
//~
	//~ // Prepare result table
	//~ multimap<double,string,compTime> mRaceResult;
	//~ for (int i = 1; i <= Drivers->getDriverCount(); i++)
	//~ {
		//~ string sDriverLobbyName = Drivers->getDriverLobbyName(i);
		//~ CResult* rResult = Drivers->getDriverRaceResult(sDriverLobbyName, iRaceNumber);
		//~ if (rResult == NULL)
			//~ return;
		//~ if (rResult->getCar() != "")
			//~ mRaceResult.insert(make_pair(rResult->getTimeSec(HOTLAP), sDriverLobbyName));
	//~ }
//~
	//~ oStream << "<table style=\"text-align: center; width: 100%;\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">\n";
	//~ oStream << "<tbody>\n";
//~
	//~ // Title
	//~ oStream << "<tr>\n";
	//~ oStream << "<th scope=\"col\"; style=\"width: " << iWidthHotlapPos << "%;\">Hotlappos.</th>\n";
	//~ oStream << "<th scope=\"col\"; align=\"left\"; style=\"width: " << iWidthDriver << "%;\">Fahrer</th>\n";
	//~ oStream << "<th scope=\"col\"; align=\"left\"; style=\"width: " << iWidthCar << "%;\">Fahrzeug</th>\n";
	//~ oStream << "<th scope=\"col\"; style=\"width: " << iWidthHotlapTime << "%;\">Hotlapzeit</th>\n";
	//~ oStream << "<th scope=\"col\"; style=\"width: " << iWidthHotlapPoints << "%;\">Hotlappkte</th>\n";
	//~ oStream << "</tr>\n";
//~
	//~ // Data
	//~ int iPos = 1;
	//~ for (multimap<double,string>::iterator it = mRaceResult.begin(); it != mRaceResult.end(); it++)
	//~ {
		//~ string sDriverLobbyName = it->second;
		//~ CResult* rResult = Drivers->getDriverRaceResult(sDriverLobbyName, iRaceNumber);
		//~ if (rResult == NULL)
			//~ return;
//~
		//~ // Line background
		//~ if (iPos % 2)
			//~ oStream << "<tr class=\"bgcolor1\">\n";
		//~ else
			//~ oStream << "<tr>\n";
//~
		//~ // Position
		//~ if (rResult->getTime(HOTLAP) != "")
			//~ oStream << "<td><div align=\"center\">" << iPos << "</div></td>\n";	// Position
		//~ else
			//~ oStream << "<td><div align=\"center\">--</div></td>\n";	// Position
		//~
		//~ // Driver
		//~ oStream << "<td><div align=\"left\"><a href=\"" << CPageHTMLDriver::getFileName("", iRaceNumber, sDriverLobbyName) << "\">" << Drivers->getDriverName(sDriverLobbyName) << "</div></td>\n";	// Driver
		//~
		//~ // Car
		//~ oStream << "<td><div align=\"left\">" << rResult->getCar() << "</div></td>\n";
		//~
		//~ // Hotlap
		//~ if (rResult->getTime(HOTLAP) != "")
		//~ {
			//~ oStream << "<td><div align=\"center\">" << rResult->getTime(HOTLAP) << "</div></td>\n";
			//~ oStream << "<td><div align=\"center\">" << rResult->getPoints(HOTLAP) << "</div></td>\n";
		//~ }
		//~ else
		//~ {
			//~ oStream << "<td><div align=\"center\">DNS</div></td>\n";
			//~ oStream << "<td><div align=\"center\">0</div></td>\n";
		//~ }
//~
		//~ oStream << "</tr>\n";
//~
		//~ iPos++;
	//~ }
	//~
	//~ oStream << "</tbody>\n";
	//~ oStream << "</table>\n";
//~ }

/*!
 * ...
 *
 *
 * @param sPageBaseDir
 * 		The page base directory as string
 * @return
 *		...
 */
string CPageHTMLRace::getFileName(string sPageBaseDir, int iRaceNumber)
{
	stringstream ssRaceNumber;
	ssRaceNumber << (iRaceNumber + 10);

	string sRaceLocationCode = Race->getRaceLocation(iRaceNumber);
	erase_all(sRaceLocationCode, " ");
	erase_all(sRaceLocationCode, "/");
	replace_all(sRaceLocationCode, "_", "");

	string sRaceDateTimeCode = Race->getRaceDateTime(iRaceNumber);
	erase_all(sRaceDateTimeCode, "/");
	erase_all(sRaceDateTimeCode, " ");
	erase_all(sRaceDateTimeCode, ":");

	return sPageBaseDir + ssRaceNumber.str() + "_" + sRaceDateTimeCode + "_" + SeasonNameCode + "_" + sRaceLocationCode + ".html";
}

// Page content
void CPageHTMLRace::getPage(ostream &oStream)
{
	int iRaceNumber = Race->getRaceNumber();
	stringstream ssRaceNumber;
	ssRaceNumber << iRaceNumber;
	string sPageDescription = Race->getRaceLocation(iRaceNumber) + " Lauf " + ssRaceNumber.str();

	oStream << DocType;
	oStream << HTMLStart;

	getPageHeader(oStream, sPageDescription);
	getPageBody(oStream, sPageDescription);

	oStream << HTMLEnd;
}


// Constructor & Destructor
/*!
 * Constructor of CPageHTMLFinish.
 * The series name, season name, the parameters, the list of cars, points, drivers and races are stored.
 *
 *
 * @param instParameter
 * 		The parameters as CParameter*
 * @param instCars
 * 		The list of cars as CCarList*
 * @param instPoints
 * 		The list of points as CPoint*
 * @param instDrivers
 * 		The list of drivers as CDriverList*
 * @param instRace
 * 		The list of races as CRace*
 */
CPageHTMLFinish::CPageHTMLFinish(CParameter* instParameter, CPoint* instPoints, CCarList* instCars, CDriverList* instDrivers, CRace* instRace)
	: CPageHTML(instParameter, instPoints, instCars, instDrivers, instRace)
{

}

/*!
 * Destructor of CPageHTMLRace.
 *
 *
 */
CPageHTMLFinish::~CPageHTMLFinish()
{

}

// Private functions
/*!
 * The HTML-body of the driver page with page description sPageDescription is created.
 *
 *
 * @param cPageType
 * 		The output stream as ostream&
 * @param sPageDescription
 *		The page description as string
 */
void CPageHTMLFinish::getPageBody(ostream &oStream, string sPageDescription)
{
	string sBannerName = "rlc_logo_gtl_gentlemens.png";
	int iBannerWidth = 199;
	int iBannerHeight = 122;

	oStream << "<body>\n";
	oStream << "<table border=\"0\" align=\"center\">\n";
	oStream << "<tr>\n";
	oStream << "<th valign=\"top\"; scope=\"col\"; style=\"width: 100%;\">\n";
	oStream << "<big align=\"center\"; style=\"font-weight: bold; text-decoration: underline;\">" << SeriesName << " " << SeasonName << " (Lauf "  << Race->getRaceNumber() << ")</big><br>";
	oStream << "<img align=\"center\"; style=\"width: " << iBannerWidth << "px; height: " << iBannerHeight << "px;\" " << "alt=\"" << sBannerName << "\" " << "src=\"" << sBannerName << "\" align=\"top\">";
	oStream << "</th>\n";
	oStream << "</tr>\n";
	oStream << "</table><br><br><br>\n";

	// Finish table
	getPageCaption(oStream, "Saisonendstand", 1);
	oStream << "<br>\n";
	getTableFinish(oStream);
	//~ oStream << "<br><br>\n";
//~
	// Team table
	getPageCaption(oStream, "Teamwertung", 1);
	oStream << "<br>\n";
	getTableTeam(oStream);
	oStream << "<br><br>\n";

	oStream << "<table style=\"text-align: left; width: 100%;\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">\n";

	oStream << "<tr>\n";

	// Drivers vs. races table
	oStream << "<td valign=\"top\">\n";
	getPageCaption(oStream, "Fahrer - Rennen (Zielankunft)", 1);
	oStream << "<br>\n";
	getTableDriversRaces(oStream);
	oStream << "<br><br>\n";
	oStream << "</td>\n";

	//~ // Rules table
	//~ oStream << "<td valign=\"top\">\n";
	//~ getPageCaption(oStream, "Regeln", 1);
	//~ oStream << "<br>\n";
	//~ getTableRules(oStream);
	//~ oStream << "<br><br>\n";
	//~ oStream << "</td>\n";

	oStream << "</tr>\n";

	oStream << "</table>\n";

	// Last changed
	oStream << "<div align=\"right\">\n";
	getPageCreationDate(oStream);
	oStream << "</div>\n";

	oStream << "</body>\n";
}

/*!
 * The finish table is written
 *
 *
 * @param oStream
 * 		The output stream as ostream&
 */
void CPageHTMLFinish::getTableFinish(ostream &oStream)
{
	int iWidthPos = 5;
	int iWidthDriver = 15;
	int iWidthTeam = 15;
	int iWidthFP = 5;
	int iWidthPen = 5;
	int iWidthVoid = 10;
	int iWidthOverall = 10;

	int iRaceNumber = Race->getRaceNumber();

	oStream << "<table style=\"text-align: center; width: 100%;\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">\n";
	oStream << "<tbody>\n";

	// Title
	oStream << "<tr>\n";
	oStream << "<th scope=\"col\"; style=\"width: " << iWidthPos << "%;\">Pos.</div></td>\n"; // Position
	oStream << "<th scope=\"col\"; align=\"left\"; style=\"width: " << iWidthDriver << "%;\">Fahrer</th>\n"; // Fahrer
	oStream << "<th scope=\"col\"; align=\"left\"; style=\"width: " << iWidthTeam << "%;\">Team</th>\n"; // Team
	oStream << "<th scope=\"col\"; style=\"width: " << iWidthFP << "%;\">Fahrerp.</div></td>\n"; // FP
	if (Parameter->getNumberOfVoidResults() == 1)
		oStream << "<th scope=\"col\"; style=\"width: " << iWidthVoid << "%;\">Streicherg.</div></td>\n"; // Streichergebnis
	else
	{
		for (int l = 1; l <= Parameter->getNumberOfVoidResults(); l++)
			oStream << "<th scope=\"col\"; style=\"width: " << iWidthVoid << "%;\">Streicherg. " << l << "</div></td>\n"; // Streichergebnis
	}
	oStream << "<th scope=\"col\"; style=\"width: " << iWidthPen << "%;\">Strafp.</div></td>\n"; // Pen
	oStream << "<th scope=\"col\"; style=\"width: " << iWidthOverall << "%;\">Endst.</div></td>\n"; // Endstand
	oStream << "</tr>\n";

	// Data
	for (int i = 1; i <= Drivers->getDriverCount(); i++)
	{
		string sDriverLobbyName = Drivers->getDriverLobbyNameFinish(i, iRaceNumber);
		vector<CResult> vVoid = Drivers->getDriverVoidResults(sDriverLobbyName, iRaceNumber);

		// Line background
		if (i % 2)
			oStream << "<tr class=\"bgcolor1\">\n";
		else
			oStream << "<tr>\n";

		oStream << "<td><div align=\"center\">" << i << "</div></td>\n"; // Position
		oStream << "<td><div align=\"left\"><a href=\"" << CPageHTMLDriver::getFileName("", Race->getRaceNumber(), sDriverLobbyName) << "\">" << Drivers->getDriverName(sDriverLobbyName) << "</div></td>\n"; // Fahrer
		oStream << "<td><div align=\"left\">" << Drivers->getDriverTeam(sDriverLobbyName) << "</div></td>\n"; // Team
		oStream << "<td><div align=\"center\">" << Drivers->getDriverPoints(sDriverLobbyName) << "</div></td>\n"; // FP
		for (int l = Parameter->getNumberOfVoidResults() - 1; l >= 0; l--)
			oStream << "<td><div align=\"center\">" << vVoid.at(l).getResult() << "</div></td>\n"; // Streichergebnis
		oStream << "<td><div align=\"center\">" << Drivers->getDriverPenalty(sDriverLobbyName) << "</div></td>\n"; // Pen
		oStream << "<td><div align=\"center\">" << round(Drivers->getDriverFinish(sDriverLobbyName, iRaceNumber), 1) << "</div></td>\n"; // Gesamtwertung

		oStream << "</tr>\n";
	}

	oStream << "</tbody>\n";
	oStream << "</table>\n";
}

/*!
 * The team finish table is written
 *
 *
 * @param oStream
 * 		The output stream as ostream&
 */
void CPageHTMLFinish::getTableTeam(ostream &oStream)
{
	int iWidthPos = 5;
	int iWidthTeam = 15;
	int iWidthDrivers = 30;
	int iWidthFinish = 5;

	int iRaceNumber = Race->getRaceNumber();

	oStream << "<table style=\"text-align: center; width: 100%;\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">\n";
	oStream << "<tbody>\n";

	// Title
	oStream << "<tr>\n";
	oStream << "<th scope=\"col\"; style=\"width: " << iWidthPos << "%;\">Pos.</div></td>\n"; // Position
	oStream << "<th scope=\"col\"; align=\"left\"; style=\"width: " << iWidthTeam << "%;\">Team</th>\n"; // Team
	oStream << "<th scope=\"col\"; align=\"left\"; style=\"width: " << iWidthDrivers << "%;\">Fahrer</th>\n"; // Fahrer
	oStream << "<th scope=\"col\"; style=\"width: " << iWidthFinish << "%;\">Endst.</div></td>\n"; // Endstand
	oStream << "</tr>\n";

	// Data
	for (int i = 1; i <= Drivers->getTeamCount(); i++)
	{
		string sTeamName = Drivers->getTeamNameFinish(i, iRaceNumber);
		string sDriverLobbyName1 = Drivers->getTeamMember(sTeamName, 0);
		string sDriverLobbyName2 = Drivers->getTeamMember(sTeamName, 1);

		// Line background
		if (i % 2)
			oStream << "<tr class=\"bgcolor1\">\n";
		else
			oStream << "<tr>\n";

		oStream << "<td><div align=\"center\">" << i << "</div></td>\n"; // Position
		oStream << "<td><div align=\"left\">" << sTeamName << "</div></td>\n"; // Team
		oStream << "<td><div align=\"left\"><a href=\"" << CPageHTMLDriver::getFileName("", Race->getRaceNumber(), sDriverLobbyName1) << "\">" << Drivers->getDriverName(sDriverLobbyName1) << "</a>";
		if (sDriverLobbyName2 != "")
			oStream << " & " << "<a href=\"" << CPageHTMLDriver::getFileName("", Race->getRaceNumber(), sDriverLobbyName2) << "\">" << Drivers->getDriverName(sDriverLobbyName2) << "</a>";
		oStream << "<td><div align=\"center\">" << round(Drivers->getTeamFinish(sTeamName, iRaceNumber), 1) << "</div></td>\n"; // Gesamtwertung

		oStream << "</tr>\n";
	}

	oStream << "</tbody>\n";
	oStream << "</table>\n";
}

/*!
 * ...
 *
 *
 * @param sPageBaseDir
 * 		The page base directory as string
 * @return
 *		...
 */
string CPageHTMLFinish::getFileName(string sPageBaseDir, int iRaceNumber)
{
	return sPageBaseDir + "99_" + SeasonNameCode + ".html";
}

// Page content
void CPageHTMLFinish::getPage(ostream &oStream)
{
	string sPageDescription = "Saisonendstand";

	oStream << DocType;
	oStream << HTMLStart;

	getPageHeader(oStream, sPageDescription);
	getPageBody(oStream, sPageDescription);

	oStream << HTMLEnd;
}

