/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabase.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: H01LocalConnectome
 * Requirements: BAI SHARED C++ library, Eigen 3 C++ library
 * Compilation: ./compileH01indexedCSVdatabase.sh
 * Usage: ./H01indexedCSVdatabase.exe
 * Description: H01 indexed CSV database (main)
 * Comments:
 * /
 *******************************************************************************/

#include "H01indexedCSVdatabase.hpp"
#ifdef INDEXED_CSV_DATABASE_CREATE
#include "H01indexedCSVdatabaseCreate.hpp"
#endif
#ifdef INDEXED_CSV_DATABASE_QUERY
#include "H01indexedCSVdatabaseQuery.hpp"
#endif

int main()
{
	#ifdef INDEXED_CSV_DATABASE_CREATE
	createIndexedCSVdatabase();
	#endif
	#ifdef INDEXED_CSV_DATABASE_QUERY
	queryIndexedCSVdatabase();
	#endif
}

