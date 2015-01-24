#include <string>
#include <sstream>
#include <map>
using namespace std;


#ifndef C_PROEVAL_POINT_H_
#define C_PROEVAL_POINT_H_


/*!
 * @file CproevalPoint.cpp
 * @class CPoint
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.3
 * @brief Read only access to RaL driver points
 *
 * @section DESCRIPTION
 *
 * The CPoint class is created with the Pro Series driver points given.
 * It gives read only access to the stored points and it allocates position to driver points.
 */
class CPoint
{
private:
	// Variables
	map<int,int> mPoints;
public:
	// Constructor & Destructor
	CPoint(map<int,int> &miPoints);
	~CPoint();

	// Read stuff
	int getPoints(int iPosition);
	int getPosition(int iPoints);
	string toString();
};

#endif
