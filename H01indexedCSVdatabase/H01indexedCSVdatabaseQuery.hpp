/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseQuery.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: H01LocalConnectome
 * Requirements: see H01indexedCSVdatabase.hpp
 * Compilation: see H01indexedCSVdatabase.hpp
 * Usage: see H01indexedCSVdatabase.hpp
 * Description: H01 indexed CSV database query - lookup indexed CSV database by pre/postsynaptic neuron ID, and find target/source connections
 * Comments:
 * /
 *******************************************************************************/

#include "H01indexedCSVdatabase.hpp"

#ifdef INDEXED_CSV_DATABASE_QUERY
bool queryIndexedCSVdatabase();
	bool queryIndexedCSVdatabase(const string neuronListFileName, const bool queryPresynapticConnectionNeurons, const string neuronListConnectionsFileName);
		bool queryIndexedCSVdatabaseByNeuronID(const string neuronID, const bool queryPresynapticConnectionNeurons, vector<vector<string>>* neuronConnectionTargetSourceList);
			bool convertCSVlineToVector(const string* csvLineText, vector<string>* csvLineVector);
			
#ifndef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_FILE_OBJECT
void appendStringBufferToPreallocatedString(string* s, string stringToAppend);
#endif
#endif
