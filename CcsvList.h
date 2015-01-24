/*!
 * @file CcsvList.h
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.1
 * @brief Header for the CcsvList class.
 */ 

#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <queue>

using namespace std;

#ifndef C_CSV_LIST_H_
#define C_CSV_LIST_H_


/*!
 * @class CcsvList
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.1
 * @brief Reads, writes csv files and accesses data
 * 
 * @section DESCRIPTION
 * 
 * The CcsvList class reads csv files.
 * Each line or element can be accessed.
 * Writes the csv file.
 * 
 * @section LICENSE
 * 
 * Copyright 2009 Ramon Hofer <ramonhofer@bluewin.ch>
 * 
 * This program is free software; you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation; either version 2 of the License, or 
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, 
 * MA 02110-1301, USA.
 */ 
class CcsvList
{
private:	
	// Variables
	vector<string> lines;
public:
	// Constructor & Destructor
	CcsvList();
	CcsvList(const char* ifileName);
	~CcsvList();

	// Add stuff
	void addLine(const string lineToAdd);
	
	// Change stuff
	//~ int setElementValue(const string sValue, 
			//~ const int iLine, const int iElement);
	void clear();

	// Read stuff
	int getLineCount();
	//~ int getElementCount(const int iLine);
	string getLine(const int iLine);
	string getElement(const int iLine, const int iElement);
	string toString();
	
	// File handling
	int readCSV(const string fileName);
	int writeCSV(const string fileName);
};

#endif
