/*!
 * @file CrgaParameter.h
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.8.1
 * @brief Header for the CParameter class.
 */

#include <string>
#include <sstream>
using namespace std;


#ifndef C_RGA_PARAMETER_H_
#define C_RGA_PARAMETER_H_


/*!
 * @class CParameter
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.5.1
 * @brief Read only access to RaL parameters
 *
 * @section DESCRIPTION
 *
 * The CParameter class is created with the RaL parameters given.
 * It gives read only access to the stored parameters.
 */
class CParameter
{
private:
	// Variables
	string SeriesName;
	string SeriesNameShort;
	string SeasonName;
	int NumberOfVoidResults;
	string PageBaseDir;
	string PageIndexFile;
	string PageLogo;
public:
	// Constructor & Destructor
	CParameter(
		string sSeriesName,
		string sSeriesNameShort,
		string sSeasonName,
		int iNumberOfVoidResults,
		string sPageBaseDir,
		string sPageIndexFile,
		string sPageLogo);
	~CParameter();

	// Read stuff
	string getSeriesName();
	string getSeriesNameShort();
	string getSeasonName();
	int getNumberOfVoidResults();
	string getPageBaseDir();
	string getPageIndexFile();
	string getPageLogo();
	string toString();
};

#endif
