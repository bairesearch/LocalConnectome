/*******************************************************************************
 *
 * File Name: SHAREDglobalDefs.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * License: AGPLv3
 * Project: Generic Construct Functions
 * Project Version: 3p4a 07-August-2021
 * License: 
 * Description: Generic Construct Functions: shared global definitions (configure to compile different BAI projects)
 * /
 *******************************************************************************/


#ifdef COMPILE_UNREAL_PROJECT //comment with COMPILE_UNREAL_PROJECT
#include "ldrawVRv0.h"
#endif //comment with COMPILE_UNREAL_PROJECT
#include "SHAREDglobalDefs.hpp"

string exeFolder;
string inputFolder;
string outputFolder;

