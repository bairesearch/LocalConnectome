/*******************************************************************************
 *
 * File Name: SHAREDglobalDefs.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: Generic Construct Functions
 * Project Version: 3p5a 14-March-2022
 * License: 
 * Description: Generic Construct Functions: shared global definitions (configure to compile different BAI projects)
 * /
 *******************************************************************************/


#ifndef HEADER_SHARED_GLOBAL_DEFS
#define HEADER_SHARED_GLOBAL_DEFS


class SHAREDglobalDefsClass
{
};

	/**********
	SOURCE PROJECT VARIABLES;
	***********/

	//current/active projects;
//#define COMPILE_NLCI		//QTcreator NLCI.pro -> NLCI.exe
//#define COMPILE_GIAI		//QTcreator GIAI.pro -> GIAI.exe
//#define COMPILE_CF		//compileCF.bat -> CF.exe
//#define COMPILE_NLC		//makefile.NLC -> NLC.exe
#define COMPILE_SANI		//makefile.SANI -> SANI.exe
//#define COMPILE_GIA		//makefile.GIA -> GIA.exe
//#define COMPILE_GIA_WITH_ANN	//makefile.GIAwithANN -> GIA.exe
//#define COMPILE_LRP_GENERATE_POS_TAGGER_DATABASE		//makefile.LRPgeneratePOStaggerDatabase -> LRPgeneratePOStaggerDatabase.exe
//#define COMPILE_LRP_WITH_ANN_GENERATE_POS_TAGGER_DATABASE	//makefile.LRPwithANNgeneratePOStaggerDatabase -> LRPgeneratePOStaggerDatabase.exe
//#define COMPILE_ATOR		//makefile.ATOR -> ATOR.exe
//#define COMPILE_CS		//makefile.CS -> CS.exe
//#define COMPILE_RT		//makefile.RT -> RT.exe
//#define COMPILE_ANN_WITH_RT	//makefile.ANNwithRT -> ANN.exe
//#define COMPILE_ANN		//makefile.ANN -> ANN.exe

//#define COMPILE_OLC		//compileOLC.bat -> OLC.exe
//#define COMPILE_LRRC	//makefile.LRRC -> LRRC.exe
//#define COMPILE_UNREAL_PROJECT

	//inactive tests;
//#define COMPILE_LRRC_WITH_ANN
//#define COMPILE_TH_ANN_WITH_LRRC
//#define COMPILE_LRP_WITH_SANI_MASS_INPUT	//makefile.LRPwithSANImassInput -> LRPexecuteSANImassInput.exe




	/**********
	OS DEPENDENT VARIABLES;
	***********/

#define LINUX
#ifndef LINUX
	//else assume windows (affects default parts directory only - this is ISO C++ so it should compile on any platform)
	//#include <windows.h>	//incompatible with LDjpeg.hpp (must only include windows.h when need to)
	#include <cstdint>
#endif

#define SQL_MYSQL_VERSION_5_7_PLUS

	/**********
	COMPILATION TYPE DEPENDENT VARIABLES;
	***********/

#ifdef COMPILE_UNREAL_PROJECT
	#define USE_UNREAL
	#define USE_RT
	#ifdef LINUX
		#define DEFAULT_PARTS_DIRECTORY_FULL_PATH "/usr/share/local/LDRAW/PARTS/"
		#define DEFAULT_PARTS_PRIMITIVES_DIRECTORY_FULL_PATH "/usr/share/local/LDRAW/P/"
	#else
		#define DEFAULT_PARTS_DIRECTORY_FULL_PATH "D:/Program Files (x86)/LDraw/PARTS/parts/"
		#define DEFAULT_PARTS_PRIMITIVES_DIRECTORY_FULL_PATH "D:/Program Files (x86)/LDraw/PARTS/p/"
	#endif
#endif

#ifdef COMPILE_CF
	#define USE_CF
#endif

#ifdef COMPILE_NLCI
	#define USE_NLCI
	#define USE_NLC
	#define USE_GIA
	#define USE_SANI	//required for GIA_POS_REL_TRANSLATOR
	#define USE_LRP
#endif

#ifdef COMPILE_GIAI
	#define USE_GIAI
	#define USE_GIA
	#define USE_SANI	//required for GIA_POS_REL_TRANSLATOR
	#define USE_LRP
#endif

#ifdef COMPILE_NLC
	#define USE_NLC
	#define USE_GIA
	#define USE_SANI	//required for GIA_POS_REL_TRANSLATOR
	#define USE_LRP
#endif

#ifdef COMPILE_SANI
	#define USE_SANI
	#define USE_ANN
	#define USE_RT
	#define USE_LRP
#endif
#ifdef COMPILE_GIA_WITH_ANN
	#define COMPILE_GIA
	#define USE_ANN
#endif
#ifdef COMPILE_GIA
	#define USE_GIA
	#define USE_SANI	//required for GIA_POS_REL_TRANSLATOR
	#define USE_LRP
#endif
#ifdef COMPILE_LRP_WITH_ANN_GENERATE_POS_TAGGER_DATABASE
	#define COMPILE_LRP_GENERATE_POS_TAGGER_DATABASE
	//#define USE_GIA
	#define USE_ANN
	#define USE_RT
#endif
#ifdef COMPILE_LRP_GENERATE_POS_TAGGER_DATABASE
	#define USE_LRP
	//#define USE_GIA
#endif
#ifdef USE_GIA
	#define USE_RT
#endif

#ifdef COMPILE_CS
	#define USE_CS
#endif
#ifdef USE_CS
	#define USE_RT
#endif

#ifdef COMPILE_ATOR
	#define USE_ATOR
#endif
#ifdef USE_ATOR
	#define USE_RT
#endif


#ifdef COMPILE_ANN_WITH_RT
	#define COMPILE_ANN
	#define USE_RT
#endif
#ifdef COMPILE_ANN
	#define USE_ANN
#endif

#ifdef COMPILE_RT
	#define USE_RT
#endif


#ifdef COMPILE_LRRC
	#define USE_LRRC
#endif
#ifdef COMPILE_LRRC_WITH_ANN
	#define USE_LRRC
	#define USE_ANN
	#define USE_RT
#endif
#ifdef COMPILE_TH_ANN_WITH_LRRC
	#define USE_LRRC
	#define USE_ANN
	#define TH_USE_ANN_TEST_HARNESS
	#define USE_RT
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
#define CHAR_EXCLAMATION_MARK '!'
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

