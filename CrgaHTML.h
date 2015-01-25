/*!
 * @file CrgaHTML.cpp
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.8.1
 * @brief Source for the CPageHTML class.
 */


#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <time.h>
#include <exception>
#include "CrgaParameter.h"
#include "CrgaPoint.h"
#include "CrgaDriver.h"
#include "CrgaRace.h"
using namespace std;
using namespace boost;


#ifndef C_RGA_HTML_H_
#define C_RGA_HTML_H_

struct compResult
{
	bool operator() (const CResult& lhs, const CResult& rhs) const
	{
		CResult leftResult = lhs;
		CResult rightResult = rhs;
		return leftResult.getPoints() > rightResult.getPoints();
	}
};

struct compDouble
{
	bool operator() (const double& lhs, const double& rhs) const
	{return lhs > rhs;}
};

class CrgaHTML
{
private:

public:
	// Constructor & Destructor
	CrgaHTML();
	~CrgaHTML();

	string writeHTML(CParameter* instParameter, CPoint* instPoints, CCarList* instCars, CDriverList* instDrivers, CRace* instRace);
};

class CPageHTML
{
protected:
	static string SeriesName;
	static string SeriesNameShort;
	static string SeasonName;
	static string SeasonNameCode;
	static string Charset;
	static string DocType;
	static string HTMLStart;
	static string HTMLEnd;
	static CParameter* Parameter;
	static CCarList* Cars;
	static CPoint* Points;
	static CDriverList* Drivers;
	static CRace* Race;

	static double round(double dNumber, unsigned int iDigits);
	static void getPageHeader(ostream &oStream, string sPageDescription);
	static void getPageCreationDate(ostream &oStream);
	static void getPageCaption(ostream &oStream, string sPageCaption, int iLevel);
	static void getTableDriversRaces(ostream &oStream);
	static void getTableRaces(ostream &oStream);
	virtual void getPageBody(ostream &oStream, string sPageDescription) =0;
public:
	CPageHTML(CParameter* instParameter, CPoint* instPoints, CCarList* instCars, CDriverList* instDrivers, CRace* instRace);
	~CPageHTML();
	//virtual string getFileName(string sPageBaseDir, int iRaceNumber) =0;
	virtual void getPage(ostream &oStream) =0;
};

class CPageHTMLOverall : protected CPageHTML
{
private:
	int iRaceNumber;
	void getPageBody(ostream &oStream, string sPageDescription);
	void getTableOverall(ostream &oStream);
	void getTableTeam(ostream &oStream);
	void getTableRaces(ostream &oStream);
public:
	CPageHTMLOverall(CParameter* instParameter, CPoint* instPoints, CCarList* instCars, CDriverList* instDrivers, CRace* instRace);
	~CPageHTMLOverall();
	static string getFileName(string sPageBaseDir, int iRaceNumber);
	void getPage(ostream &oStream);
};

class CPageHTMLDriver : protected CPageHTML
{
private:
	string DriverLobbyName;
	void getPageBody(ostream &oStream, string sPageDescription);
	void getTableDriverMultis(ostream &oStream);
	void getTableDriverRaces(ostream &oStream);
public:
	CPageHTMLDriver(string sDriverLobbyName, CParameter* instParameter, CPoint* instPoints, CCarList* instCars, CDriverList* instDrivers, CRace* instRace);
	~CPageHTMLDriver();
	static string getFileName(string sPageBaseDir, int iRaceNumber, string sDriverLobbyName);
	void getPage(ostream &oStream);
};

class CPageHTMLRace : protected CPageHTML
{
private:
	void getPageBody(ostream &oStream, string sPageDescription);
	void getTableRaceDrivers(ostream &oStream);
public:
	CPageHTMLRace(CParameter* instParameter, CPoint* instPoints, CCarList* instCars, CDriverList* instDrivers, CRace* instRace);
	~CPageHTMLRace();
	static string getFileName(string sPageBaseDir, int iRaceNumber);
	void getPage(ostream &oStream);
};

class CPageHTMLFinish : protected CPageHTML
{
private:
	void getPageBody(ostream &oStream, string sPageDescription);
	void getTableFinish(ostream &oStream);
	void getTableTeam(ostream &oStream);
public:
	CPageHTMLFinish(CParameter* instParameter, CPoint* instPoints, CCarList* instCars, CDriverList* instDrivers, CRace* instRace);
	~CPageHTMLFinish();
	static string getFileName(string sPageBaseDir, int iRaceNumber);
	void getPage(ostream &oStream);
};

#endif
