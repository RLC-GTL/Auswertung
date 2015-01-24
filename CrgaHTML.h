//      CRaLEvalHTML.h
//
//      Copyright 2010 Ramon Hofer <ramonhofer<at>bluewin.ch>
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.


#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <time.h>
#include <exception>
#include "CproevalParameter.h"
#include "CproevalPoint.h"
#include "CproevalDriver.h"
#include "CproevalRace.h"
using namespace std;
using namespace boost;


#ifndef C_PROEVALHTML_H_
#define C_PROEVALHTML_H_

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

class CProEvalHTML
{
private:

public:
	// Constructor & Destructor
	CProEvalHTML();
	~CProEvalHTML();

	string writeHTML(bool bFinishSeason, CParameter* instParameter, CPoint* instPoints, CCarList* instCars, CDriverList* instDrivers, CRace* instRace);
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
