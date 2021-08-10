/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseMain.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: H01LocalConnectome
 * Requirements: see H01indexedCSVdatabase.hpp
 * Compilation: see H01indexedCSVdatabase.hpp
 * Usage: see H01indexedCSVdatabase.hpp
 * Description: H01 indexed CSV database Main
 * Comments:
 * /
 *******************************************************************************/

#include "H01indexedCSVdatabaseMain.hpp"
#ifdef INDEXED_CSV_DATABASE_VISUALISE_LOCAL_CONNECTOME
#include "H01indexedCSVdatabaseVisualiseLocalConnectome.hpp"
#endif
#ifdef INDEXED_CSV_DATABASE_CREATE
#include "H01indexedCSVdatabaseCreate.hpp"
#endif
#ifdef INDEXED_CSV_DATABASE_QUERY
#include "H01indexedCSVdatabaseQuery.hpp"
#endif


int main()
{
	#ifdef INDEXED_CSV_DATABASE_VISUALISE_LOCAL_CONNECTOME
	H01indexedCSVdatabaseVisualiseLocalConnectomeClass().visualiseLocalConnectomeCSVdataset();
	#endif
	#ifdef INDEXED_CSV_DATABASE_CREATE
	H01indexedCSVdatabaseCreateClass().createIndexedCSVdatabase();
	#endif
	#ifdef INDEXED_CSV_DATABASE_QUERY
	H01indexedCSVdatabaseQueryClass().queryIndexedCSVdatabase();
	#endif
	
}

