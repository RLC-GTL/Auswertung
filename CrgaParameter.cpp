/*!
 * @file CrgaParameter.cpp
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.8.1
 * @brief Source for the CParameter class.
 */

#include "CrgaParameter.h"

// Constructor & Destructor
/*!
 * Constructor of CParameter.
 * The description of the season, number of races per void results, the start credit, as well as positive and negative RLT multiplier are stored.
 * The parameters are read only.
 *
 *
 * @param sSeasonName
 * 		The description of the season as string
 * @param iNumberOfVoidResults
 * 		The number of void results (the number of the worst results that are not taken account of)
 * @param iStartCredit
 * 		The start credit as int
 * @param dPosMult
 * 		The positive RLT multiplier as double
 * @param dNegMult
 * 		The negative RLT multiplier as double
 */
CParameter::CParameter(
	string sSeriesName,
	string sSeriesNameShort,
	string sSeasonName,
	int iNumberOfVoidResults,
	string sPageBaseDir,
	string sPageIndexFile,
	string sPageLogo,
	double dP,
	double dExp,
	vector<string> vsLobbynamesIgnored)
{
	SeriesName = sSeriesName;
	SeriesNameShort = sSeriesNameShort;
	SeasonName = sSeasonName;
	NumberOfVoidResults = iNumberOfVoidResults;
	PageBaseDir = sPageBaseDir;
	PageIndexFile = sPageIndexFile;
	PageLogo = sPageLogo;
	P = dP;
	Exp = dExp;
	LobbynamesIgnored = vsLobbynamesIgnored;
}

/*!
 * Destructor of CParameter.
 *
 *
 */
CParameter::~CParameter()
{

}

// Read stuff
/*!
 * Returns the description of the series.
 *
 *
 * @return
 *		The description of the series as string
 */
string CParameter::getSeriesName()
{
	return SeriesName;
}

/*!
 * Returns the short description of the series.
 *
 *
 * @return
 *		The short description of the series as string
 */
string CParameter::getSeriesNameShort()
{
	return SeriesNameShort;
}

/*!
 * Returns the description of the season.
 *
 *
 * @return
 *		The description of the season as string
 */
string CParameter::getSeasonName()
{
	return SeasonName;
}

/*!
 * Returns the number of races per void results.
 *
 *
 * @return
 *		The number of races per void results as int
 */
int CParameter::getNumberOfVoidResults()
{
	return NumberOfVoidResults;
}

/*!
 * Returns the base directory where the HTML page files are stored.
 *
 *
 * @return
 *		The page base director as string
 */
string CParameter::getPageBaseDir()
{
	return PageBaseDir;
}

/*!
 * Returns the index file name of the HTML page.
 *
 *
 * @return
 *		The index file name as string
 */
string CParameter::getPageIndexFile()
{
	return PageIndexFile;
}

/*!
 * Returns the logo file name of the HTML page.
 *
 *
 * @return
 *		The logo file name as string
 */
string CParameter::getPageLogo()
{
	return PageLogo;
}

/*!
 * Returns true if the Lobbyname is associated with a driver.
 *
 *
 * @return
 *		True: Lobbyname associated with a driver
 * 		False: Lobbyname associated with commentator
 */
bool CParameter::getLobbynameRacer(string sLobbyname)
{
	for (vector<string>::iterator it = LobbynamesIgnored.begin() ; it != LobbynamesIgnored.end(); ++it)
	{
		if (*it == sLobbyname)
			return false;
	}
	return true;
}

/*!
 * Returns the weight change as double.
 * This formula is used:
 * -(P * (((dPos-(1/2)-dNumOfStarters/2)/(dNumOfStarters/2-(1/2)))^Exp);
 *
 * @return
 *		The weight change as double
 */
double CParameter::getWeightChange(double dPos, double dNumOfStarters)
{
	//-(P * (((dPos-(1/2)-dNumOfStarters/2)/(dNumOfStarters/2-(1/2)))^Exp);
	double N2 = dNumOfStarters * 0.5;
	double base = -(dPos-0.5-N2) / (N2-0.5);
	double val =  P * pow(base,Exp);
	return round(val);
}

/*!
 * Creates and returns a string with the content.
 *
 *
 * @return
 *		The content of as string
 */
string CParameter::toString()
{
	stringstream ss;
	ss <<
		"// Parameterliste: \"Parameter\",ParamName,ParamValue	Strafgewichte: -(P * (((n-(1/2)-N/2)/(N/2-(1/2)))^e)  n: Position; N: Anz. Starter\n" <<
		"Parameter,SeriesName," << getSeriesName() << endl <<
		"Parameter,SeriesNameShort," << getSeriesNameShort() << endl <<
		"Parameter,SeasonName," << getSeasonName() << endl <<
		"Parameter,NumberOfVoidResults," << getNumberOfVoidResults() << endl <<
		"Parameter,PageBaseDir," << getPageBaseDir() << endl <<
		"Parameter,PageIndexFile," << getPageIndexFile() << endl <<
		"Parameter,P," << 50 << endl <<
		"Parameter,Exp," << 1 << endl;
	
	if (LobbynamesIgnored.empty())
		ss << "Parameter,DriverIgnore,Lobbyname\n";
	else
	{
		for (vector<string>::iterator it = LobbynamesIgnored.begin() ; it != LobbynamesIgnored.end(); ++it)
		{
			ss << "Parameter,DriverIgnore," << *it << endl;
		}
	}
	
	return ss.str();
}
