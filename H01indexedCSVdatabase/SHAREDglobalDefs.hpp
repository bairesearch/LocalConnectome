/*******************************************************************************
 *
 * File Name: SHAREDglobalDefs.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: Generic Construct Functions
 * Project Version: 3p4a 07-August-2021
 * Description: Generic Construct Functions: shared global definitions (configure to compile different BAI projects)
 * /
 *******************************************************************************/


#ifndef HEADER_SHARED_GLOBAL_DEFS
#define HEADER_SHARED_GLOBAL_DEFS


class SHAREDglobalDefsClass
{
};


	/**********
	OS DEPENDENT VARIABLES;
	***********/

#define LINUX
#ifndef LINUX
	//else assume windows (affects default parts directory only - this is ISO C++ so it should compile on any platform)
	//#include <windows.h>	//incompatible with LDjpeg.hpp (must only include windows.h when need to)
	#include <cstdint>
#endif



#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <string>
#include <cstring>
#include <vector>
#ifdef USE_LRP
#include <unordered_map>	//requires C++11
#include <utility> // make_pair
#endif
#include <algorithm>
#include <stack>
#include <map>
#include <limits>
#include <sstream>
#include <iomanip>
using namespace std;





	//ASCII variables
#define CHAR_TAB '\t'
#define CHAR_COMMA ','
#define CHAR_NEWLINE '\n'
#define CHAR_OPEN_BRACKET '('
#define CHAR_CLOSE_BRACKET ')'
#define CHAR_OPEN_SQUARE_BRACKET '['
#define CHAR_CLOSE_SQUARE_BRACKET ']'
#define CHAR_DASH '-'
#define CHAR_VERTICAL_BAR '|'
#define CHAR_UNDERSCORE '_'
#define CHAR_SPACE ' '
#define CHAR_END_OF_STRING '\0'
#define CHAR_DEL (127)
#define CHAR_IRRELEVANT (CHAR_DEL)
#define CHAR_FORWARDSLASH '/'
#define CHAR_FULLSTOP '.'
#define CHAR_SEMICOLON ';'
#define CHAR_COLON ':'
#define CHAR_QUESTIONMARK '?'
#define CHAR_STAR '*'
#define CHAR_INVERTED_COMMAS '"'
#define CHAR_APOSTROPHE '\''
#define CHAR_BACKSLASH '\\'
#define CHAR_EQUALS '='
#define CHAR_INVERTEDCOMMAS '"'
#define CHAR_DECIMALPLACE (CHAR_FULLSTOP)
#define CHAR_DOLLAR '$'
#define CHAR_GREATER_THAN '>'
#define CHAR_LESS_THAN '<'
#define CHAR_PLUS '+'
#define CHAR_EXCLAMATION_HIGHLIGHT2 '!'
#define CHAR_AMPERSAND '&'
#define CHAR_TILDA '~'
#define CHAR_FOLDER_DELIMITER '/'

#define STRING_TAB "\t"
#define STRING_NEWLINE "\n"
#define STRING_SPACE " "
#define STRING_FULLSTOP "."
#define STRING_COMMA ","
#define STRING_INVERTEDCOMMAS "\""
#define STRING_APOSTROPHE "'"
#define STRING_INVERTED_COMMAS "\""
#define STRING_QUESTIONMARK "?"

#define TM_STRUCT_YEAR_OFFSET 1900

#define INT_DEFAULT_VALUE (-1)
#define CPP_STRING_FIND_RESULT_FAIL_VALUE string::npos	//-1
#define BOOL_IRRELEVANT (false)
#define INT_IRRELEVANT (0)
#define BOOL_UNKNOWN_LEGACY (false)

#define EXIT_OK (0)
#define EXIT_ERROR (1)

#define EXE_FOLDER_PATH_MAX_LENGTH (1000)
extern string exeFolder;
extern string inputFolder;
extern string outputFolder;

//required for CS:
#define constEffective
#define uchar unsigned char
#define schar signed char

//required for SANI:
#define ordered_map map

//required for multiple projects (based on LD specification, see LDreferenceClass.hpp):
#define SHARED_COLOUR_UNDEFINED (0)
#define SHARED_COLOUR_BLACK (0)
#define SHARED_COLOUR_BLUE (1)
#define SHARED_COLOUR_GREEN (2)
#define SHARED_COLOUR_CYAN (11)
#define SHARED_COLOUR_RED (4)
#define SHARED_COLOUR_MAGENTA (5)
#define SHARED_COLOUR_BROWN (6)
#define SHARED_COLOUR_LIGHTGREY (7)
#define SHARED_COLOUR_DARKGREY (8)
#define SHARED_COLOUR_YELLOW (14)
#define SHARED_COLOUR_WHITE (15)
#define SHARED_COLOUR_ORANGE (25)
#define SHARED_COLOUR_PURPLE (22)
#define SHARED_COLOUR_DARKBLUE (272)
#define SHARED_COLOUR_DARKGREEN (288)
#define SHARED_COLOUR_DARKRED (320)
#define SHARED_COLOUR_DARKYELLOW (84)
#define SHARED_COLOUR_AQUA (3)

#define SHARED_COLOUR_LIGHT_BLUE (9)
#define SHARED_COLOUR_LIGHT_GREEN (17)
#define SHARED_COLOUR_LIGHT_CYAN (323)
#define SHARED_COLOUR_LIGHT_RED (13)
#define SHARED_COLOUR_LIGHT_MAGENTA (29)
#define SHARED_COLOUR_LIGHT_BROWN (86)
#define SHARED_COLOUR_LIGHT_YELLOW (18)
#define SHARED_COLOUR_LIGHT_ORANGE (125)
#define SHARED_COLOUR_LIGHT_PURPLE (31)
#define SHARED_COLOUR_LIGHT_AQUA (118)

#endif

