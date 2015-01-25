/*!
 * @file CcsvList.cpp
 * @author Ramon Hofer <ramonhofer@bluewin.ch>
 * @version 0.1
 * @brief Source for the CcsvList class.
 */ 

#include "CcsvList.h"


// Constructor & Destructor
/*!
 * Constructor of CcsvList.
 * An empty list is created.
 * 
 *
 */
CcsvList::CcsvList()
{
	
}

/*!
 * Constructor of CcsvList.
 * The csv file is read and the list filled.
 * 
 *
 * @param ifileName
 * 		The file name of the csv file that will be read as char array*
 */
CcsvList::CcsvList(const char* ifileName)
{
	CcsvList();
	readCSV(ifileName);
}

/*!
 * Destructor of CcsvList.
 * 
 *
 */
CcsvList::~CcsvList()
{
	
}

// Add stuff
/*!
 * Adds a line to the end of the list.
 * 
 *
 * @param lineToAdd
 * 		The line to be added as string
 */
void CcsvList::addLine(const string lineToAdd)
{
	lines.push_back(lineToAdd);
}

/*!
 * Deletes the whole list.
 * 
 *
 */
void CcsvList::clear()
{
	lines.erase(lines.begin(), lines.end());
}

// Read stuff
/*!
 * Counts the number of lines in the list.
 * 
 *
 * @return
 *		The number of line as int
 */
int CcsvList::getLineCount()
{
	return lines.size();
}

/*!
 * Searches and returns the line.
 * 
 *
 * @param iLine
 * 		The number of the line as int
 * @return
 *		- The line as string
 * 		- If the line couldn't be found "Line not found" is returned
 */
string CcsvList::getLine(const int iLine)
{
	try
	{
		return lines.at(iLine);
	}
	catch(exception& e)
	{
		return "Line not found";
	}
}

/*!
 * Searches and returns the element at position iElement in line iLine.
 * 
 *
 * @param iLine
 * 		The number of the line as int
 * @param iElement
 * 		The number of the element in the line as int
 * @return
 *		- The element as string
 * 		- If the line couldn't be found "Line not found" is returned
 * 		- If the element couldn't be found in the line "Element not found" is returned
 */
string CcsvList::getElement(const int iLine, const int iElement)
{
	// Get line
	istringstream ssLine(getLine(iLine));
	
	// Variables
	string sNotFound = "Element not found";
	string element;
	vector<string> elements;

	// Line found?
	if (ssLine.str() == "Line not found")
		return sNotFound;

	// Line found.
	while (getline(ssLine, element, ','))
	{
		elements.push_back(element);
	}
	
	// Search element
	try
	{	
		// Return element
		return elements.at(iElement);
	}
	catch(exception& e)
	{
		// Return error message
		return sNotFound;
	}
}

/*!
 * Creates and returns a string with the content.
 * 
 *
 * @return
 *		Content of the list as string
 */
string CcsvList::toString()
{
	string s = "";
	
	for (int i = 0; i < getLineCount(); i++)
		s = s + getLine(i) + "\n";

	return s;
}

// File handling
/*!
 * Reads the content of a csv file.
 * 
 *
 * @param fileName
 * 		The file name of the csv file that will be read as string
 * @return
 *		The return value indicates how the function exited
 * 		- 0: Normal termination
 * 		- -1: Couldn't open file
 * 		- -2: Empty file
 */
int CcsvList::readCSV(const string fileName)
{
	// Open file
	ifstream file(fileName.c_str());
	if (!file.is_open())
		return -1;			// Couldn't open file
	if (file.peek() == EOF)
		return -2;			// Empty file

	// Variables
	string fileLine;
	
	// Read lines from file
	while (getline(file, fileLine))
		lines.push_back(fileLine);

	return 0;
}

/*!
 * Writes the content to a csv file.
 * 
 *
 * @param fileName
 * 		The file name of the csv file that will be written as string
 * @return
 *		The return value indicates how the function exited
 * 		- 0: Normal termination
 * 		- -1: Couldn't open file
 */
int CcsvList::writeCSV(const string fileName)
{	// Create new or overwrites existing file
	ofstream file(fileName.c_str());
	if (!file.is_open())
		return -1;
	file << toString();
	file.close();
	
	return 0;
}
