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
	string sSeasonName,
	int iNumberOfVoidResults,
	string sPageBaseDir,
	string sPageIndexFile)
{
	SeasonName = sSeasonName;
	NumberOfVoidResults = iNumberOfVoidResults;
	PageBaseDir = sPageBaseDir;
	PageIndexFile = sPageIndexFile;
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
		"// Parameterliste: \"Parameter\",ParamName,ParamValue\n" <<
		"Parameter,SeasonName," << getSeasonName() << endl <<
		"Parameter,NumberOfVoidResults," << getNumberOfVoidResults() << endl <<
		"Parameter,PageBaseDir," << getPageBaseDir() << endl <<
		"Parameter,PageIndexFile," << getPageIndexFile() << endl;;
	return ss.str();
}
