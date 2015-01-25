/*!
 * @file CrgaPoint.cpp
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.8.1
 * @brief Header for the CPoint class.
 */

#include <string>
#include <sstream>
#include <map>
using namespace std;


#ifndef C_RGA_POINT_H_
#define C_RGA_POINT_H_


/*!
 * @file CrgaPoint.cpp
 * @class CPoint
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.3
 * @brief Read only access to driver points
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
