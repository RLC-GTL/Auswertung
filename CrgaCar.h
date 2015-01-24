/*!
 * @file CralevalCar.h
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.5.1
 * @brief Header for the CCar & CCarList classes; source of the intcompUseRank comparison class
 */

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "CproevalParameter.h"
using namespace std;


#ifndef C_PROEVAL_CAR_H_
#define C_PROEVAL_CAR_H_


/*!
 * @struct intcompUseRank
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.5.1
 * @brief Compares the number of rentings
 *
 * @section DESCRIPTION
 *
 * The intcompUseRank struct compares two integers and returns true if the first value is bigger.
 */
struct intcompUseRank
{
	/*!
	 * Compares two integers and returns true if the left handed value is bigger.
	 *
	 *
	 * @param lhs
	 * 		The left handed value as double
	 * @param rhs
	 * 		The right handed value as double
	 * @return
	 * 		Wheter the left handed value is bigger
	 */
	bool operator() (const int& lhs, const int& rhs) const
	{
		return lhs > rhs;
	}
};


/*!
 * @class CCar
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.5.1
 * @brief RaL car
 *
 * @section DESCRIPTION
 *
 * The CCar class manages the car data.
 * I.e. the car name, price and the number of rentings.
 */
class CCar
{
private:
	// Variables
	string Name;
	string Abr;
	double Mult;
	static CParameter* Parameter;
public:
	// Constructor & Destructor
	CCar(const string sName, const string sAbr, const double dMult, CParameter* cParameter);
	~CCar();

	// Read stuff
	string getName();
	string getAbr();
	double getMult();
	string toString();
};


/*!
 * @class CCarList
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.5.1
 * @brief List of all Pro cars
 *
 * @section DESCRIPTION
 *
 * The CCarList class handles the list of cars.
 * I.e. collects all the multipliers for the cars.
 */
class CCarList
{
private:
	vector<CCar> vCars;

	// Private functions
	CCar* getCar(string sAbr);
	CCar* getCarByName(string sCar);
public:
	// Constructor & Destructor
	CCarList(vector<CCar> &vCCars);
	~CCarList();

	// Read stuff
	int getCarCount();
	string getName(string sAbr);
	double getMult(string sAbr);
	string getAbr(string sCar);
	string getAbrByID(int iID);
	string toString();
};

#endif
