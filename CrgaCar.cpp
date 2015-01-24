/*!
 * @file CralevalCar.cpp
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.5.1
 * @brief Source for the CCar & CCarList classes.
 */

#include "CproevalCar.h"

// Constructor & Destructor
/*!
 * Constructor of CCar.
 * The name, price, the number of uses, amount of earning and a reference to the parameters are stored.
 *
 *
 * @param sName
 * 		The car name as string
 * @param sAbr
 * 		The abbreviation as string
 * @param dMult
 * 		The multiplier as double
 * @param cParameter
 * 		The reference to CRaLEval::Parameter as CParameter*
 */
CCar::CCar(const string sName, const string sAbr, const double dMult, CParameter* cParameter)
{
	Name = sName;
	Abr = sAbr;
	Mult = dMult;

	if (Parameter == NULL)
		Parameter = cParameter;
}

/*!
 * Destructor of CCar.
 *
 *
 */
CCar::~CCar()
{

}

/*!
 * Initialization of the static pointer to CRaLEval::Parameter as CParameter*
 *
 *
 */
CParameter* CCar::Parameter = NULL;

// Read stuff
/*!
 * Returns the name.
 *
 *
 * @return
 *		The name of the car as string
 */
string CCar::getName()
{
	return Name;
}

/*!
 * Returns the abbreviation.
 *
 *
 * @return
 *		The abbreviation of the car as string
 */
string CCar::getAbr()
{
	return Abr;
}

/*!
 * Returns the multiplier.
 *
 *
 * @return
 *		The multiplier of the car as double
 */
double CCar::getMult()
{
	return Mult;
}

/*!
 * Creates and returns a string with the content.
 *
 *
 * @return
 *		The content of as string
 */
string CCar::toString()
{
	stringstream ss;
	ss <<
		"Car," <<
		getName() << "," <<
		getAbr() << "," <<
		getMult();
	return ss.str();
}


// Constructor & Destructor
/*!
 * Constructor of CCarList.
 * The car list is stored.
 *
 *
 * @param vCCars
 * 		The list of cars as vector<CCar>
 */
CCarList::CCarList(vector<CCar> &vCCars)
{
	vCars = vCCars;
}

/*!
 * Destructor of CCarList.
 *
 *
 */
CCarList::~CCarList()
{

}

// Private functions
/*!
 * Searches the car's abbreviation and returns the pointer to the car.
 * If the car couldn't be found a NULL pointer is returned.
 *
 *
 * @param sAbr
 * 		The abbreviation of the car as string
 * @return
 *		The car as CCar*
 */
CCar* CCarList::getCar(string sAbr)
{
	vector<CCar>::iterator it;

	// Car existing?
	for (it = vCars.begin(); it < vCars.end(); it++)
	{
		if (it->getAbr() == sAbr)
			return &(*it);
	}

	return NULL;
}

/*!
 * Searches the car's name and returns the pointer to the car.
 * If the car couldn't be found a NULL pointer is returned.
 *
 *
 * @param sCar
 * 		The name of the car as string
 * @return
 *		The car as CCar*
 */
CCar* CCarList::getCarByName(string sCar)
{
	vector<CCar>::iterator it;

	// Car existing?
	for (it = vCars.begin(); it < vCars.end(); it++)
	{
		if (it->getName() == sCar)
			return &(*it);
	}

	return NULL;
}

// Read stuff
/*!
 * Counts the number of cars.
 *
 *
 * @return
 *		The number of cars as int
 */
int CCarList::getCarCount()
{
	return vCars.size();
}

/*!
 * Searches the car abbreviation and returns the name.
 *
 *
 * @param sAbr
 * 		The abbreviation of the car as string
 * @return
 *		- The name of the car as string
 * 		- If the car couldn't be found "" is returned
 */
string CCarList::getName(string sAbr)
{
	if (getCar(sAbr) == NULL)
		return "";
	return getCar(sAbr)->getName();
}

/*!
 * Searches the car abbreviation and returns the multiplier.
 *
 *
 * @param sAbr
 * 		The abbreviation of the car as string
 * @return
 *		- The multiplier of the car as double
 * 		- If the car couldn't be found 0.0 is returned
 */
double CCarList::getMult(string sAbr)
{
	if (getCar(sAbr) == NULL)
		return 0.0;
	return getCar(sAbr)->getMult();
}

/*!
 * Searches the car name and returns the abbreviation.
 *
 *
 * @param sCar
 * 		The name of the car as string
 * @return
 *		- The amount of earning during the season as double
 * 		- If the car couldn't be found -1.0 is returned
 */
string CCarList::getAbr(string sCar)
{
	if (getCarByName(sCar) == NULL)
		return "";
	return getCarByName(sCar)->getAbr();
}

/*!
 * Searches the car ID and returns the abbreviation.
 *
 *
 * @param iID
 * 		The name of the car as string
 * @return
 *		- The amount of earning during the season as double
 * 		- If the car couldn't be found -1.0 is returned
 */
string CCarList::getAbrByID(int iID)
{
	vector<CCar>::iterator it;
	int i = 0;

	// Car existing?
	for (it = vCars.begin(); it < vCars.end(); it++)
	{
		if (i == iID)
			return (&(*it))->getAbr();
		i++;
	}

	return NULL;
}

/*!
 * Creates and returns a string with the content.
 *
 *
 * @return
 *		The content of as string
 */
string CCarList::toString()
{
	string s = "// Multis: \"Car\",CarName,CarAbbr,Multiplier\n";
	vector<CCar>::iterator it;
	for (it = vCars.begin(); it < vCars.end(); it++)
	{
		s = s + (*it).toString() + "\n";
	}
	return s;
}
