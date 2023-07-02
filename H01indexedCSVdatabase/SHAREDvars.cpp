/*******************************************************************************
 *
 * File Name: SHAREDvars.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: Generic Construct Functions
 * Project Version: 3p5a 14-March-2022
 * /
 *******************************************************************************/


#ifdef COMPILE_UNREAL_PROJECT //comment with COMPILE_UNREAL_PROJECT
#include "ldrawVRv0.h"
#endif //comment with COMPILE_UNREAL_PROJECT
#include "SHAREDvars.hpp"

#ifdef LINUX
#include <sys/time.h>
#else
#include <time.h>
#include <windows.h>
#endif

//for file i/o;
#include <sys/stat.h>
#include <sys/types.h>
#ifdef LINUX
	#include <unistd.h>	//added for Ubuntu 13.1 file i/o
#else
	#include <windows.h>
#endif

using namespace std;


int64_t SHAREDvarsClass::getTimeAsLong()
{
	#ifdef LINUX
	struct timeval tv;
	struct timezone tz;
	struct tm* tm;
	gettimeofday(&tv, &tz);
	tm=localtime(&tv.tv_sec);
	int64_t timeAsLong = int64_t(tm->tm_hour)* 60* 60* 1000000 + int64_t(tm->tm_min)* 60* 1000000 + int64_t(tm->tm_sec)* 1000000 + int64_t(tv.tv_usec);	//time in microseconds
	return timeAsLong;

	#else

	SYSTEMTIME systemTime;
	GetSystemTime(&systemTime);

	FILETIME fileTime;
	SystemTimeToFileTime(&systemTime, &fileTime);

	ULARGE_INTEGER uli;
	uli.LowPart = fileTime.dwLowDateTime; // could use memcpy here!
	uli.HighPart = fileTime.dwHighDateTime;

	ULONGLONG systemTimeIn_ms(uli.QuadPart/10000);

	return systemTimeIn_ms;

	#endif


}

void SHAREDvarsClass::copyColours(colour* colToModify, colour* colToCopy)
{
	colToModify->r = colToCopy->r;
	colToModify->g = colToCopy->g;
	colToModify->b = colToCopy->b;
}


bool SHAREDvarsClass::compareDoublesArbitraryError(const double a, const double b, const double error)
{
	bool result;
	if((a < (b+error)) && (a > (b-error)))
	{
		result = true;
	}
	else
	{
		result = false;
	}

	return result;
}


bool SHAREDvarsClass::compareDoubles(const double a, const double b)
{
	bool result;
	if((a < (b+DOUBLE_MIN_PRECISION)) && (a > (b-DOUBLE_MIN_PRECISION)))
	{
		result = true;
	}
	else
	{
		result = false;
	}

	return result;
}


//generic functions

int SHAREDvarsClass::maxInt(int a, const int b)
{
	if(a > b)
	{
		return a;
	}
	else
	{
		return b;
	}
}

int SHAREDvarsClass::minInt(int a, const int b)
{
	if(a < b)
	{
		return a;
	}
	else
	{
		return b;
	}
}

double SHAREDvarsClass::minDouble(double a, const double b)
{
	if(a < b)
	{
		return a;
	}
	else
	{
		return b;
	}
}
double SHAREDvarsClass::maxDouble(double a, const double b)
{
	if(a > b)
	{
		return a;
	}
	else
	{
		return b;
	}
}

double SHAREDvarsClass::absDouble(const double val)
{
	if(val < 0.0)
	{
		return (-val);
	}
	else
	{
		return val;
	}
}

bool SHAREDvarsClass::argumentExists(const int argc, const char** argv, const string keystr)
{
	for(int i=1; i<argc; i++)
	{
		if(string(argv[i]) == keystr)
		{
			return true;
		}
	}
	return false;
}

float SHAREDvarsClass::getFloatArgument(const int argc, const char** argv, const string keystr)
{
	float result=0.0;
	bool foundArgument = false;

	for(int i=1; i<argc; i++)
	{
		if(!foundArgument)
		{
			if(string(argv[i]) == keystr)
			{
				result = atof(argv[i+1]);
				foundArgument = true;
			}
		}
	}
	if(!foundArgument)
	{
		cout << "Error: getFloatArgument{}; " << keystr << endl;
		result = -999999999.0F;
		return result;
	}
	else
	{
		return result;
	}
}

string SHAREDvarsClass::getStringArgument(const int argc, const char** argv, const string keystr)
{
	string stringArgument = "";
	bool foundArgument = false;
	for(int i=1;i<argc;i++)
	{
		if(!foundArgument)
		{
			if(string(argv[i]) == keystr)
			{
				stringArgument = string(argv[i+1]);
				foundArgument = true;
			}
		}
	}
	if(!foundArgument)
	{
		cout << "Error: getStringArgument{}; " << keystr << endl;
	}
	return stringArgument;
}

void SHAREDvarsClass::getStringArrayArgument(const int argc, const char** argv, const string keystr, vector<string>* inputFileNamesVector)
{
	bool foundArgument = false;
	for(int i=1;i<argc;i++)
	{
		if(!foundArgument)
		{
			if(string(argv[i]) == keystr)
			{
				int j=1;
				while(((argv[i+j])[0] != CHAR_DASH) && (i+j < argc))
				{
					string stringArgument = string(argv[i+j]);
					inputFileNamesVector->push_back(stringArgument);
					foundArgument = true;
					j++;
				}
			}
		}
	}
	if(!foundArgument)
	{
		cout << "Error: getStringArrayArgument{}; " << keystr << endl;
	}
}

void SHAREDvarsClass::changeDirectory(const string newDirectory)
{
	char* newDirectoryCharStar = const_cast<char*>(newDirectory.c_str());
	#ifdef LINUX
	chdir(newDirectoryCharStar);
	#else
	::SetCurrentDirectoryA(newDirectoryCharStar);
	#endif
}

string SHAREDvarsClass::getCurrentDirectory()
{
	char currentFolderCharStar[EXE_FOLDER_PATH_MAX_LENGTH];
	#ifdef LINUX
	getcwd(currentFolderCharStar, EXE_FOLDER_PATH_MAX_LENGTH);
	#else
	::GetCurrentDirectoryA(EXE_FOLDER_PATH_MAX_LENGTH, currentFolderCharStar);
	#endif
	string currentFolder = string(currentFolderCharStar);
	return currentFolder;
}

void SHAREDvarsClass::setCurrentDirectory(string folder)
{
	setCurrentDirectory(&folder);
}

void SHAREDvarsClass::setCurrentDirectory(const string* folder)
{
	const char* folderCharStar = folder->c_str();
	#ifdef LINUX
	chdir(folderCharStar);
	#else
	::SetCurrentDirectoryA(folderCharStar);
	#endif
}

void SHAREDvarsClass::createDirectory(const string* folder)
{
	const char* folderCharStar = folder->c_str();
	#ifdef LINUX
	mkdir(folderCharStar, 0755);	//NB GIAdatabase.cpp and ATORdatabaseFileIO uses 0755, ATORdatabaseDecisionTree.cpp use 0770 [CHECKTHIS]
	#else
	::CreateDirectoryA(folderCharStar, NULL);
	#endif
}

bool SHAREDvarsClass::directoryExists(const string* folder)
{
	const char* folderCharStar = folder->c_str();
	bool folderExists = false;

	/*
	#ifdef LINUX
	struct stat st;
	if(stat(folderCharStar, &st) == 0)
	{
		folderExists = true;
	}
	#else
	DWORD ftyp = GetFileAttributes(folderCharStar);
	if(ftyp != INVALID_FILE_ATTRIBUTES)
	{
		if(ftyp & FILE_ATTRIBUTE_DIRECTORY)
		{
			folderExists = true;
		}
	}
	#endif
	*/
	struct stat info;
	if(stat(folderCharStar, &info) != 0)
	{
		//printf( "cannot access %s\n", pathname );
	}
	else if(info.st_mode & S_IFDIR)  // S_ISDIR() doesn't exist on my windows
	{
		folderExists = true;
		//printf( "%s is a directory\n", pathname );
	}
	else
	{
		//printf( "%s is no directory\n", pathname )
	}
	return folderExists;
}

bool SHAREDvarsClass::checkIfFolderExistsAndIfNotMakeAndSetAsCurrent(const string* folderName)
{
	bool result = true;
	if(!directoryExists(folderName))
	{
		createDirectory(folderName);
	}
	setCurrentDirectory(folderName);

	return result;
}

bool SHAREDvarsClass::isWhiteSpace(const char c)
{
	bool result = false;
	if((c == CHAR_SPACE) || (c == CHAR_TAB))
	{
		result = true;
	}
	return result;
}

string SHAREDvarsClass::convertStringToLowerCase(const string* arbitraryCaseString)
{
	string lowerCaseString = *arbitraryCaseString;
	for(int i=0; i<arbitraryCaseString->length(); i++)
	{
		lowerCaseString[i] = tolower((*arbitraryCaseString)[i]);
	}
	return lowerCaseString;
}

string SHAREDvarsClass::convertStringToFirstUpperCase(const string* arbitraryCaseString)
{
	string firstUpperCaseString = *arbitraryCaseString;
	if(arbitraryCaseString->length() > 0)
	{
		firstUpperCaseString[0] = toupper((*arbitraryCaseString)[0]);
	}
	return firstUpperCaseString;
}

string SHAREDvarsClass::convertCharToString(char c)
{
	return string(1, c);
}

string SHAREDvarsClass::convertFloatToString(const float number, const string format)
{
	char stringCharStar[100];
	sprintfSafeDouble(stringCharStar, format.c_str(), number);
	return string(stringCharStar);
}
string SHAREDvarsClass::convertDoubleToString(const double number, const string format)
{
	char stringCharStar[100];
	sprintfSafeDouble(stringCharStar, format.c_str(), number);
	return string(stringCharStar);
}
string SHAREDvarsClass::convertIntToString(const int number, const string format)
{
	char stringCharStar[100];
	sprintfSafeInt(stringCharStar, format.c_str(), number);
	return string(stringCharStar);
}
string SHAREDvarsClass::convertIntToString(const int number)
{
	char stringCharStar[100];
	sprintfSafeInt(stringCharStar, "%d", number);
	return string(stringCharStar);
}
string SHAREDvarsClass::convertUnsignedIntToString(const int number)
{
	char stringCharStar[100];
	sprintfSafeInt(stringCharStar, "%u", number);
	return string(stringCharStar);
}
string SHAREDvarsClass::convertBoolToString(const bool number)
{
	if(number)
	{
		return "true";
	}
	else
	{
		return "false";
	}
}
string SHAREDvarsClass::convertLongToString(const int64_t number)
{
	//return to_string(number);	//C++11

	char tempString[100];
	sprintfSafeLong(tempString, "%ld", number);
	return string(tempString);
}
string SHAREDvarsClass::convertLongToString(const int64_t number, const string format)
{
	char tempString[100];
	sprintfSafeLong(tempString, format.c_str(), number);
	return string(tempString);
}


int SHAREDvarsClass::convertStringToInt(const string number)
{
	return atoi(number.c_str());
}
bool SHAREDvarsClass::convertStringToBool(string number)
{
	bool boolean = false;

	for(int i=0; i<number.length(); i++)
	{
		number[i] = tolower(number[i]);
	}

	if(number == "true")
	{
		boolean = true;
	}
	else if(number == "false")
	{
		boolean = false;
	}
	else if(number == "1")
	{
		boolean = true;	//boolean = (bool)convertStringToInt(number);
	}
	else if(number == "0")
	{
		boolean = false;
	}
	else
	{
		cerr << "convertStringToBool{} error: number != \"true\" or  \"false\" or  \"1\" or  \"0\"" << endl;
		cerr << "number = " << number << endl;
		exit(EXIT_ERROR);
	}

	return boolean;
}
int64_t SHAREDvarsClass::convertStringToLong(const string number)
{
	return atol(number.c_str());
}
float SHAREDvarsClass::convertStringToFloat(const string number)
{
	return (float)convertStringToDouble(number);
}
double SHAREDvarsClass::convertStringToDouble(const string number)
{
	return atof(number.c_str());
}






bool SHAREDvarsClass::textInTextArray(const string text, const string* textArray, const int arraySize)
{
	int arrayIndexOfResultFound = INT_DEFAULT_VALUE;
	return textInTextArray(text, textArray, arraySize, &arrayIndexOfResultFound);
}

bool SHAREDvarsClass::textInTextArray(const string text, const string* textArray, const int arraySize, int* arrayIndexOfResultFound)
{
	bool result = false;
	for(int i=0; i<arraySize; i++)
	{
		if(text == textArray[i])
		{
			*arrayIndexOfResultFound = i;
			result = true;
		}
	}
	return result;
}

bool SHAREDvarsClass::charInCharArray(const char c, const char* charArray, const int arraySize)
{
	bool result = false;
	for(int i=0; i<arraySize; i++)
	{
		if(c == charArray[i])
		{
			result = true;
		}
	}
	return result;
}

bool SHAREDvarsClass::intInIntArray(const int iTest, const int* intArray, const int arraySize)
{
	bool result = false;
	for(int i=0; i<arraySize; i++)
	{
		if(iTest == intArray[i])
		{
			result = true;
		}
	}
	return result;
}


string SHAREDvarsClass::replaceAllOccurancesOfString(const string* textOrig, string stringToFind, string replacementString)
{
	bool foundAtLeastOneInstance = false;
	string text = replaceAllOccurancesOfString(textOrig, stringToFind, replacementString, &foundAtLeastOneInstance);
	return text;
}

string SHAREDvarsClass::replaceAllOccurancesOfString(const string* textOrig, string stringToFind, string replacementString, bool* foundAtLeastOneInstance)
{
	*foundAtLeastOneInstance = false;
	string text = *textOrig;
	int pos = 0;
	while((pos = text.find(stringToFind, pos)) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
	{
		text.replace(pos, stringToFind.length(), replacementString);
		pos = pos + replacementString.length();
		*foundAtLeastOneInstance = true;
	}
	/*
	cout << "\n*textOrig = " <<* textOrig << endl;
	cout << "stringToFind = " << stringToFind << endl;
	cout << "replacementString = " << replacementString << endl;
	cout << "text = " << text << endl;
	*/
	return text;
}

string SHAREDvarsClass::replaceAllOccurancesOfChar(const string* textOrig, char charToFind, char replacementChar)
{
	bool foundAtLeastOneInstance = false;
	string text = replaceAllOccurancesOfChar(textOrig, charToFind, replacementChar, &foundAtLeastOneInstance);
	return text;
}

string SHAREDvarsClass::replaceAllOccurancesOfChar(const string* textOrig, char charToFind, char replacementChar, bool* foundAtLeastOneInstance)
{
	*foundAtLeastOneInstance = false;
	string text = *textOrig;
	for(int i=0; i<textOrig->length(); i++)
	{
		if((*textOrig)[i] == charToFind)
		{
			*foundAtLeastOneInstance = true;
			text[i] = replacementChar;
		}
	}
	return text;
}


void SHAREDvarsClass::writeByteArrayToFile(const string fileName, char* fileByteArray, int fileByteArraySize)
{
	ofstream parseFileObject(fileName.c_str());

	// Exactly 8 bytes written
	parseFileObject.write(fileByteArray, (sizeof(char)*fileByteArraySize));

	parseFileObject.close();

}

void SHAREDvarsClass::writeStringToFileObject(string s, ofstream* writeFileObject)
{
	//writeFileObject.write(*s);
	for(int i=0; i < s.size(); i++)
	{
		writeFileObject->put(s[i]);
	}
}

void SHAREDvarsClass::writeStringToFile(const string fileName, const string* s)
{
	ofstream writeFileObject(fileName.c_str());

	//writeFileObject.write(*s);
	for(int i=0; i < s->size(); i++)
	{
		writeFileObject.put((*s)[i]);
	}

	writeFileObject.close();
}

void SHAREDvarsClass::appendStringToFile(const string fileName, const string* s)
{
	ofstream writeFileObject(fileName.c_str(), ofstream::app);

	//writeFileObject.write(*s);
	for(int i=0; i < s->size(); i++)
	{
		writeFileObject.put((*s)[i]);
	}

	writeFileObject.close();
}

//inefficient
void SHAREDvarsClass::prependStringToFile(const string fileName, const string* s)
{
	string fileString = getFileContents(fileName);
	fileString = fileString + *s;
	writeStringToFile(fileName, &fileString);
}

string SHAREDvarsClass::getFileContents(const string inputFileName)
{	
	ifstream parseFileObject(inputFileName);
	string fileContents(static_cast<stringstream const&>(stringstream() << parseFileObject.rdbuf()).str());
	
	/*
	//not cross-platform compatible (as Windows uses multicharacter line endings CR+LF, binary cannot be used)  
	ifstream parseFileObject(inputFileName, ios::in | ios::binary);
	if(parseFileObject)
	{
		parseFileObject.seekg(0, ios::end);
		fileContents.resize(parseFileObject.tellg());
		parseFileObject.seekg(0, ios::beg);
		parseFileObject.read(&fileContents[0], fileContents.size());
		parseFileObject.close();
	}
	*/
  
    	return fileContents;
}

string SHAREDvarsClass::getFileContents(const string inputFileName, int* numberLines)
{
	string fileContents = getFileContents(inputFileName);
	
	*numberLines = 0;
	for(int i=0; i<fileContents.length(); i++)
	{
		if(fileContents[i] == CHAR_NEWLINE)
		{
			*numberLines = *numberLines + 1;
		}
	}
	
	#ifdef CS_DEBUG_GENERATE_OBJECT_ORIENTED_CODE
	//cout << "fileContents = " << fileContents << endl;
	#endif
	
	return fileContents;
}

bool SHAREDvarsClass::getLinesFromFile(const string fileName, string* fileLinesArray, int* numberOfLinesInFile)
{
	bool result = true;
	*numberOfLinesInFile = 0;
	ifstream parseFileObject(fileName.c_str());
	if(!parseFileObject.rdbuf()->is_open())
	{
		//txt file does not exist in current directory.
		result = false;
	}
	else
	{
		char currentToken;
		int fileNameIndex = 0;
		int charCount = 0;
		string currentFileName = "";
		while(parseFileObject.get(currentToken))
		{
			if(currentToken == CHAR_NEWLINE)
			{
				fileLinesArray[fileNameIndex] = currentFileName;
				currentFileName = "";
				fileNameIndex++;
			}
			else
			{
				currentFileName = currentFileName + currentToken;
			}
			charCount++;
		}
		*numberOfLinesInFile = fileNameIndex;
	}
	return result;
}

bool SHAREDvarsClass::getLinesFromFile(const string fileName, vector<string>* fileLinesList, int* numberOfLinesInFile)
{
	bool result = true;
	*numberOfLinesInFile = 0;
	ifstream parseFileObject(fileName.c_str());
	if(!parseFileObject.rdbuf()->is_open())
	{
		//txt file does not exist in current directory.
		result = false;
	}
	else
	{
		char currentToken;
		int fileNameIndex = 0;
		int charCount = 0;
		string currentFileName = "";
		while(parseFileObject.get(currentToken))
		{
			if(currentToken == CHAR_NEWLINE)
			{
				fileLinesList->push_back(currentFileName);
				currentFileName = "";
				fileNameIndex++;
			}
			else
			{
				currentFileName = currentFileName + currentToken;
			}
			charCount++;
		}
		*numberOfLinesInFile = fileNameIndex;
	}
	return result;
}

bool SHAREDvarsClass::getLinesFromFile(const string* fileContentsString, vector<string>* fileLinesList, int* numberOfLinesInFile)
{
	bool result = true;
	
	int fileNameIndex = 0;
	string currentFileName = "";
		
	for(int i=0; i<fileContentsString->length(); i++)
	{
		char currentToken = (*fileContentsString)[i];
		if(currentToken == CHAR_NEWLINE)
		{
			fileLinesList->push_back(currentFileName);
			currentFileName = "";
			fileNameIndex++;
		}
		else
		{
			currentFileName = currentFileName + currentToken;
		}
	}
	
	*numberOfLinesInFile = fileNameIndex;

	return result;
}



void SHAREDvarsClass::writeStringListToFile(const string fileName, const vector<string>* stringList)
{
	ofstream writeFileObject(fileName.c_str());
	/*
	for(int i=0; i<stringList->size(); i++)
	{
		writeFileObject << stringList->at(i) << endl;
	}
	*/  
	for(vector<string>::const_iterator stringListIter = stringList->begin(); stringListIter != stringList->end(); stringListIter++)
	{		
		string line = *stringListIter;
		writeFileObject << line << endl;
	}
	writeFileObject.close();

}



bool SHAREDvarsClass::fileExists(const string inputFileName)
{
	bool result = true;
	ifstream parseFileObject(inputFileName.c_str());
	if(!parseFileObject.rdbuf()->is_open())
	{
		result = false;
	}
	else
	{
		parseFileObject.close();
	}

	return result;
}

/*
	bool result = false;
 	FILE* pFile = NULL;
	const char* fileNameCharStar = fileName.c_str();
  	pFile = fopen(fileNameCharStar,"r");
	if(pFile != NULL)
	{
		result = true;
	}
	return result;
*/


void SHAREDvarsClass::sprintfSafeDouble(char* stringCharStar, const char* type, double number)
{
	#ifdef LINUX
	sprintf(stringCharStar, type, number);	//snprintf
	#else
	#ifdef USE_UNREAL
	snprintf(stringCharStar, sizeof(stringCharStar), type, number);
	#else
	sprintf(stringCharStar, type, number);
	#endif
	#endif
}

void SHAREDvarsClass::sprintfSafeInt(char* stringCharStar, const char* type, int number)
{
	#ifdef LINUX
	sprintf(stringCharStar, type, number);	//snprintf
	#else
	#ifdef USE_UNREAL
	snprintf(stringCharStar, sizeof(stringCharStar), type, number);
	#else
	sprintf(stringCharStar, type, number);
	#endif
	#endif
}

void SHAREDvarsClass::sprintfSafeLong(char* stringCharStar, const char* type, int64_t number)
{
	#ifdef LINUX
	sprintf(stringCharStar, type, number);	//snprintf
	#else
	#ifdef USE_UNREAL
	snprintf(stringCharStar, sizeof(stringCharStar), type, number);
	#else
	sprintf(stringCharStar, type, number);
	#endif
	#endif
}

void SHAREDvarsClass::copyFiles(const string fileToCopyName, const string newFileName)
{
	ofstream writeFileObject(newFileName.c_str());

	openFileAndCopyDataIntoCurrentFileObject(fileToCopyName, &writeFileObject);

	writeFileObject.close();
}

void SHAREDvarsClass::copyFiles(const string folderToCopyName, const string fileToCopyName, const string newFolderName, const string newFileName)
{
	string newFileNameFull = newFolderName + CHAR_FOLDER_DELIMITER + newFileName;
	ofstream writeFileObject(newFileNameFull.c_str());

	string fileToCopyNameFull = folderToCopyName + CHAR_FOLDER_DELIMITER + fileToCopyName;
	openFileAndCopyDataIntoCurrentFileObject(fileToCopyNameFull, &writeFileObject);

	writeFileObject.close();
}

bool SHAREDvarsClass::openFileAndCopyDataIntoCurrentFileObject(const string fileToOpenName, ofstream* writeFileObject)
{
	bool result = true;
	char c;	//current character being read in
	int currentLine = 1;
	int index = 0;

	ifstream parseFileObject(fileToOpenName.c_str());

	//2. fill in the data array
	if(!parseFileObject.rdbuf()->is_open())
	{
		//file does not exist in current directory.
		cout << "file, " << fileToOpenName << " cannot be opened" << endl;
		result = false;
	}
	else
	{
		while((parseFileObject).get(c))
		{
			writeFileObject->put(c);
			if(c == CHAR_NEWLINE)
			{
				currentLine++;
			}
			index++;
		}
		parseFileObject.close();
	}

	return result;
}

void SHAREDvarsClass::copyStringArray(const string stringArrayToCopy[], string stringArray[], const int arraySize)
{
	for(int i=0; i<arraySize; i++)
	{
		stringArray[i] = stringArrayToCopy[i];
	}
}

bool SHAREDvarsClass::getBitValue(int64_t number, int x) 
{
	int64_t value = (number >> x) & 1U;
	bool result = value;
	return result;
}

int64_t SHAREDvarsClass::setBitValue(int64_t number, int x, bool val) 
{
	number = number | (int64_t(val) << x);
	return number;
}

string SHAREDvarsClass::removeFileNameExtension(const string fileName)
{
	string fileNameWithoutExtension = fileName;
	int indexOfFirstFileExtension = fileName.find(STRING_FULLSTOP);
	if(indexOfFirstFileExtension != CPP_STRING_FIND_RESULT_FAIL_VALUE)
	{
		fileNameWithoutExtension = fileName.substr(0, indexOfFirstFileExtension);
	}
	return fileNameWithoutExtension;
}

bool SHAREDvarsClass::isCapitalised(const string text)
{
	bool result = false;
	if(text.length() > 0)
	{
		if(isupper(text[0]))
		{
			result = true;	
		}
	}
	return result;
}


void SHAREDvarsClass::printTime(const string startString, const string endString)
{	
	struct tm* current;
	time_t now;
	time(&now);
	current = localtime(&now);
	char timeAndDateString[100];
	sprintf(timeAndDateString, "%02i:%02i:%02i %.2i/%.2i/%i", current->tm_hour, current->tm_min, current->tm_sec, current->tm_mday, (current->tm_mon+1), (current->tm_year + TM_STRUCT_YEAR_OFFSET));
	cout << startString << timeAndDateString << endString << endl;
}

void SHAREDvarsClass::getLinesFromFileCSV(const string fileName, vector<vector<string>>* CSVdatasetFile, int* CSVdatasetFileSize, const char delimiter, const bool expectFirstLineHeader)
{
	vector<string> dataSetFile;
	int dataSetFileSize = 0;
	getLinesFromFile(fileName, &dataSetFile, &dataSetFileSize);
	
	bool firstLine = true;
	
	for(int i=0; i<dataSetFileSize; i++)
	{
		vector<string> lineContentsVector;
		string lineContents = dataSetFile[i];
		string fieldContents = "";
		int fieldIndex = 0;
		for(int charIndex=0; charIndex<lineContents.length(); charIndex++)
		{
			if(lineContents[charIndex] == delimiter)
			{	
				//cout << "fieldContents = " << fieldContents << endl;
				lineContentsVector.push_back(fieldContents);
				fieldContents = "";
				fieldIndex++;
			}
			else
			{
				fieldContents = fieldContents+lineContents[charIndex];
			}
		}
		//fill final field on line;
		lineContentsVector.push_back(fieldContents);	
		fieldContents = "";
		fieldIndex++;
		
		if(expectFirstLineHeader && firstLine)
		{
			//ignore first line header
		}
		else
		{
			CSVdatasetFile->push_back(lineContentsVector);
		}
		
		firstLine = false;
	}
	
	*CSVdatasetFileSize = CSVdatasetFile->size();
}

void SHAREDvarsClass::writeLinesToFileCSV(const string fileName, vector<vector<string>>* CSVdatasetFile, const char delimiter, const bool expectFirstLineHeader, const string headerString)
{
	vector<string> stringList;
	if(expectFirstLineHeader)
	{
		stringList.push_back(headerString);
	}
	
	for(int y=0; y<CSVdatasetFile->size(); y++)
	{
		vector<string>* lineContentsVector = &(*CSVdatasetFile)[y];
		string lineContents = "";
		for(int x=0; x<lineContentsVector->size(); x++)
		{
			lineContents += (*lineContentsVector)[x];
			if(x < lineContentsVector->size()-1)
			{
				lineContents += delimiter;
			}
		}
		stringList.push_back(lineContents);
	}
	
	writeStringListToFile(fileName, &stringList);
}

