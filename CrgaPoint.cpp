#include "CproevalPoint.h"

// Constructor & Destructor
CPoint::CPoint(map<int,int> &miPoints)
{
	mPoints = miPoints;
}

CPoint::~CPoint()
{

}

// Read stuff
int CPoint::getPoints(int iPosition)
{
	if (mPoints.find(iPosition) == mPoints.end())
		return 0;

	return mPoints.find(iPosition)->second;
}

int CPoint::getPosition(int iPoints)
{
	for (map<int,int>::iterator it = mPoints.begin(); it != mPoints.end(); it ++)
	{
		if (it->second == iPoints)
			return it->first;
	}
	return -1;
}

string CPoint::toString()
{
	map<int,int>::iterator it;
	stringstream ss;
	ss <<
		"// Punkteliste: \"Point\",Position,Points\n";
	for (it = mPoints.begin(); it != mPoints.end(); it++)
	{
		ss <<
			"Point," <<
			(*it).first << "," <<
			(*it).second << endl;
	}

	return ss.str();
}
