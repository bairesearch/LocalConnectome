/*******************************************************************************
 *
 * File Name: SHAREDvars.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: Generic Construct Functions
 * Project Version: 3p5a 14-March-2022
 * /
 *******************************************************************************/


#ifndef HEADER_SHARED_VARS
#define HEADER_SHARED_VARS

#include "SHAREDglobalDefs.hpp"

#define MAP_VALUE_OUT_OF_RANGE (8888888.0)


#define TRAIN (1)
#define TEST (0)
#define TRAIN_AND_TEST (2)

#define RT_RAYTRACE_NO_HIT_DEPTH_T (10000000.0)
#define RT_RAYTRACE_NO_HIT_POINT_X (0.0)
#define RT_RAYTRACE_NO_HIT_POINT_Y (0.0)
#define RT_RAYTRACE_NO_HIT_POINT_Z (0.0)






#define TRUE (1)
#define FALSE (0)

#define REALLY_FAR_AWAY (99999999999.9)
#define REALLY_LARGE_DOUBLE (1000000.0)
#define REALLY_LARGE_INT (1000000)
#define REALLY_SMALL_DOUBLE (-1000000.0)

#define ATOR_METHOD2DOD_DIMENSIONS (2)
#define ATOR_METHOD3DOD_DIMENSIONS (3)

/* common definitions*/
typedef struct { uchar r, g, b; } colour;
typedef struct { double x, y, z; } vec;
typedef struct { vec a; vec b; vec c; } mat;

#ifdef COMPILE_OLC
	#define DOUBLE_MIN_PRECISION 0.005
#else
	#ifdef USE_UNREAL
		#define DOUBLE_MIN_PRECISION 0.001	//temporarily relaxed matrix comparison value for LDRAW_VR_DEBUG_AUTOMATICALLY_CALCULATE_UNREAL_LD_TRANSFORM_BASED_ON_EXPECTED_RESULT
	#else
		#define DOUBLE_MIN_PRECISION 0.00001
	#endif
#endif

class SHAREDvarsClass
{
	public: int64_t getTimeAsLong();

	public: bool compareDoubles(const double a, const double b);
	public: bool compareDoublesArbitraryError(const double a, const double b, const double error);

	public: int maxInt(int a, const int b);	//should be turned into a macro instead
	public: int minInt(int a, const int b);	//should be turned into a macro instead

	public: double minDouble(double a, const double b);	//should be turned into a macro instead
	public: double maxDouble(double a, const double b);	//should be turned into a macro instead

	public: double absDouble(const double val);

	public: bool argumentExists(const int argc, const char** argv, const string keystr);
	public: float getFloatArgument(const int argc, const char** argv, const string keystr);
	public: string getStringArgument(const int argc, const char** argv, const string keystr);
	public: void getStringArrayArgument(const int argc, const char** argv, const string keystr, vector<string>* inputFileNamesVector);
	public: void changeDirectory(const string newDirectory);

	public: string getCurrentDirectory();
	public: void setCurrentDirectory(string folder);
		public: void setCurrentDirectory(const string* folder);
	public: void createDirectory(const string* folder);
	public: bool directoryExists(const string* folder);
	public: bool checkIfFolderExistsAndIfNotMakeAndSetAsCurrent(const string* folderName);

	public: void copyColours(colour* colToModify, colour* colToCopy);

	public: bool isWhiteSpace(const char c);
	public: string convertStringToLowerCase(const string* arbitraryCaseString);
	public: string convertStringToFirstUpperCase(const string* arbitraryCaseString);

	public: string convertCharToString(char c);
	public: string convertFloatToString(const float number, const string format);
	public: string convertDoubleToString(const double number, const string format);
	public: string convertIntToString(const int number);
	public: string convertIntToString(const int number, const string format);
	public: string convertUnsignedIntToString(const int number);
	public: string convertBoolToString(const bool number);
	public: string convertLongToString(const int64_t number);
	public: string convertLongToString(const int64_t number, const string format);

	public: int convertStringToInt(const string number);
	public: bool convertStringToBool(string number);
	public: int64_t convertStringToLong(const string number);
	public: float convertStringToFloat(const string number);
	public: double convertStringToDouble(const string number);


	public: bool textInTextArray(const string text, const string* textArray, const int arraySize);
	public: bool textInTextArray(const string text, const string* textArray, const int arraySize, int* arrayIndexOfResultFound);
	public: bool charInCharArray(const char c, const char* charArray, const int arraySize);
	public: bool intInIntArray(const int iTest, const int* intArray, const int arraySize);
	public: string replaceAllOccurancesOfString(const string* textOrig, string stringToFind, string replacementString);
	public: string replaceAllOccurancesOfString(const string* textOrig, string stringToFind, string replacementString, bool* foundAtLeastOneInstance);
	public: string replaceAllOccurancesOfChar(const string* textOrig, char charToFind, char replacementChar);
	public: string replaceAllOccurancesOfChar(const string* textOrig, char charToFind, char replacementChar, bool* foundAtLeastOneInstance);


	public: void writeByteArrayToFile(const string fileName, char* fileByteArray, int fileByteArraySize);
	public: void writeStringToFileObject(string s, ofstream* writeFileObject);
	public: void writeStringToFile(const string fileName, const string* s);
	public: void appendStringToFile(const string fileName, const string* s);
	public: void prependStringToFile(const string fileName, const string* s);

	public: string getFileContents(const string inputFileName);
		public: string getFileContents(const string inputFileName, int* numberLines);
	public: bool getLinesFromFile(const string fileName, string* fileLinesArray, int* numberOfLinesInFile);
	public: bool getLinesFromFile(const string fileName, vector<string>* fileLinesList, int* numberOfLinesInFile);
	public: bool getLinesFromFile(const string* fileContentsString, vector<string>* fileLinesList, int* numberOfLinesInFile);		
	public: void writeStringListToFile(const string fileName, const vector<string>* stringList);

		
	public: bool fileExists(const string inputFileName);

	public: void sprintfSafeDouble(char* stringCharStar, const char* type, double number);
	public: void sprintfSafeInt(char* stringCharStar, const char* type, int number);
	public: void sprintfSafeLong(char* stringCharStar, const char* type, int64_t number);

	public: void copyFiles(const string fileToCopyName, const string newFileName);
	public: void copyFiles(const string folderToCopyName, const string fileToCopyName, const string newFolderName, const string newFileName);
		public: bool openFileAndCopyDataIntoCurrentFileObject(const string fileToOpenName, ofstream* writeFileObject);
		
	public: void copyStringArray(const string stringArrayToCopy[], string stringArray[], const int arraySize);
	
	public: bool getBitValue(int64_t number, int x);
	public: int64_t setBitValue(int64_t number, int x, bool val);
	
	public: string removeFileNameExtension(const string fileName);
	
	public: bool isCapitalised(const string text);
	
	public: void printTime(const string startString, const string endString);

	public: void getLinesFromFileCSV(const string fileName, vector<vector<string>>* CSVdatasetFile, int* CSVdatasetFileSize, const char delimiter, const bool expectFirstLineHeader);
	public: void writeLinesToFileCSV(const string fileName, vector<vector<string>>* CSVdatasetFile, const char delimiter, const bool expectFirstLineHeader, const string headerString);


};


#endif
