/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseMain.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021-2023 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: H01LocalConnectome
 * Requirements: see H01indexedCSVdatabaseGlobalDefs.hpp
 * Compilation: see H01indexedCSVdatabaseGlobalDefs.hpp
 * Usage: see H01indexedCSVdatabaseGlobalDefs.hpp
 * Description: H01 indexed CSV database Main (INDEXED_CSV_DATABASE_CREATE/INDEXED_CSV_DATABASE_QUERY/INDEXED_CSV_DATABASE_VISUALISE_LOCAL_CONNECTOME/INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME)
 * Comments:
 * /
 *******************************************************************************/

#include "H01indexedCSVdatabaseMain.hpp"
#ifdef INDEXED_CSV_DATABASE_CREATE
#include "H01indexedCSVdatabaseCreate.hpp"
#endif
#ifdef INDEXED_CSV_DATABASE_QUERY
#include "H01indexedCSVdatabaseQuery.hpp"
#endif
#ifdef INDEXED_CSV_DATABASE_VISUALISE_LOCAL_CONNECTOME
#include "H01indexedCSVdatabaseVisualiseLocalConnectome.hpp"
#endif
#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME
#include "H01indexedCSVdatabaseTraceLocalConnectome.hpp"
#endif
#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME
#include "H01indexedCSVdatabaseReadLocalConnectome.hpp"
#endif
#ifdef INDEXED_CSV_DATABASE_PREPROCESS
#include "H01indexedCSVdatabasePreprocess.hpp"
#endif
#include "SHAREDvars.hpp"



int main(const int argc, const char** argv)
{
 currentDirectory = SHAREDvarsClass().getCurrentDirectory();

	bool passInputReq = true;

	int executionMode = EXECUTION_MODE_DEFAULT;
	#ifdef INDEXED_CSV_DATABASE_QUERY
	int queryMode = QUERY_MODE_DEFAULT;
	#endif
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME
	int readMode = READ_MODE_DEFAULT;
	#endif
	#ifdef INDEXED_CSV_DATABASE_PREPROCESS
	int preprocessMode = PREPROCESS_MODE_DEFAULT;
	#endif

	#ifdef INDEXED_CSV_DATABASE_CREATE
	string avro_json_database_folder = AVRO_JSON_DATABASE_FOLDER;
	string indexed_csv_database_folder = INDEXED_CSV_DATABASE_FOLDER;
	#endif
	string local_connectome_folder_base = LOCAL_CONNECTOME_FOLDER_BASE;

	if(SHAREDvarsClass().argumentExists(argc, argv, "-mode"))
	{
		executionMode = SHAREDvarsClass().getFloatArgument(argc, argv, "-mode");
	}
	else
	{
		passInputReq = false;
	}
	
	#ifdef INDEXED_CSV_DATABASE_QUERY
	if(SHAREDvarsClass().argumentExists(argc, argv, "-query"))
	{
		queryMode = SHAREDvarsClass().getFloatArgument(argc, argv, "-query");
	}
	else
	{
		if(executionMode == EXECUTION_MODE_INDEXED_CSV_DATABASE_QUERY)
		{
			passInputReq = false;
		}
	}
	#endif
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME
	if(SHAREDvarsClass().argumentExists(argc, argv, "-read"))
	{
		readMode = SHAREDvarsClass().getFloatArgument(argc, argv, "-read");
	}
	else
	{
		if(executionMode == EXECUTION_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME)
		{
			passInputReq = false;
		}
	}
	#endif
	#ifdef INDEXED_CSV_DATABASE_PREPROCESS
	if(SHAREDvarsClass().argumentExists(argc, argv, "-preprocess"))
	{
		preprocessMode = SHAREDvarsClass().getFloatArgument(argc, argv, "-preprocess");
	}
	else
	{
		if(executionMode == EXECUTION_MODE_INDEXED_CSV_DATABASE_PREPROCESS)
		{
			passInputReq = false;
		}
	}
	#endif
		
	#ifdef INDEXED_CSV_DATABASE_CREATE
	if(SHAREDvarsClass().argumentExists(argc, argv, "-avro_json_database_folder"))
	{
		avro_json_database_folder = SHAREDvarsClass().getStringArgument(argc, argv, "-avro_json_database_folder");
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-indexed_csv_database_folder"))
	{
		indexed_csv_database_folder = SHAREDvarsClass().getStringArgument(argc, argv, "-indexed_csv_database_folder");
	}
	#endif
	if(SHAREDvarsClass().argumentExists(argc, argv, "-local_connectome_folder_base"))
	{
		local_connectome_folder_base = SHAREDvarsClass().getStringArgument(argc, argv, "-local_connectome_folder_base");
	}

	if(!passInputReq)
	{
		cerr << errmessage << endl;
		cerr << infomessage << endl;
		exit(EXIT_ERROR);
	}
	
	cout << "executionMode = " << executionMode << " " << executionModesArray[executionMode-1] << endl;
	#ifdef INDEXED_CSV_DATABASE_QUERY
	cout << "queryMode = " << queryMode << " " << queryModesArray[queryMode-1] << endl;
	#endif
	cout << "readMode = " << readMode << " " << readModesArray[readMode-1] << endl;
	
	#ifdef INDEXED_CSV_DATABASE_CREATE
	cout << "avro_json_database_folder = " << avro_json_database_folder << endl;
	cout << "indexed_csv_database_folder = " << indexed_csv_database_folder << endl;
	#endif
	cout << "local_connectome_folder_base = " << local_connectome_folder_base << endl;

	//exit(EXIT_ERROR);
	
	if(false) ;
	#ifdef INDEXED_CSV_DATABASE_CREATE
	else if(executionMode == EXECUTION_MODE_INDEXED_CSV_DATABASE_CREATE)
	{	
		#ifndef INDEXED_CSV_DATABASE_CREATE_DEBUG
		char userAnswer;
		cout << "The user has indicated they wish to create an indexed CSV database. This will take approximately 6 hours to generate, and will overwrite any existing indexed CSV database. Are you sure you wish to continue? (y/n)" << endl;
    	cin >> userAnswer;	
		if(userAnswer == 'y')
		{
		#endif
			H01indexedCSVdatabaseCreateClass().createIndexedCSVdatabase(avro_json_database_folder, indexed_csv_database_folder);
		#ifndef INDEXED_CSV_DATABASE_CREATE_DEBUG
		}
		else
		{
			exit(EXIT_ERROR);
		}
		#endif
	}
	#endif
	#ifdef INDEXED_CSV_DATABASE_QUERY
	else if(executionMode == EXECUTION_MODE_INDEXED_CSV_DATABASE_QUERY)
	{
		H01indexedCSVdatabaseQueryClass().queryIndexedCSVdatabase(queryMode, indexed_csv_database_folder, local_connectome_folder_base);
	}
	#endif
	#ifdef INDEXED_CSV_DATABASE_VISUALISE_LOCAL_CONNECTOME
	else if(executionMode == EXECUTION_MODE_INDEXED_CSV_DATABASE_VISUALISE_LOCAL_CONNECTOME)
	{
		H01indexedCSVdatabaseVisualiseLocalConnectomeClass().visualiseLocalConnectomeCSVdataset(local_connectome_folder_base);
	}
	#endif
	#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME
	else if(executionMode == EXECUTION_MODE_INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME)
	{
		H01indexedCSVdatabaseTraceLocalConnectomeClass().traceLocalConnectomeCSVdataset(local_connectome_folder_base);
	}
	#endif
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME
	else if(executionMode == EXECUTION_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME)
	{
		H01indexedCSVdatabaseReadLocalConnectomeClass().readLocalConnectome(readMode, local_connectome_folder_base);
	}
	#endif
	#ifdef INDEXED_CSV_DATABASE_PREPROCESS
	else if(executionMode == EXECUTION_MODE_INDEXED_CSV_DATABASE_PREPROCESS)
	{
		H01indexedCSVdatabasePreprocessClass().preprocess(preprocessMode, local_connectome_folder_base);
	}
	#endif
	else
	{
		cerr << "main error: executionMode unknown: " << executionMode << endl;
		exit(EXIT_ERROR);
	}
	
}


