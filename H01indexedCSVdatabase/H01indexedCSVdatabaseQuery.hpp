/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseQuery.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: H01LocalConnectome
 * Requirements: requires H01 indexed CSV database to have already been generated (see H01indexedCSVdatabaseCreate.cpp/.hpp)
 * Compilation: see H01indexedCSVdatabase.hpp
 * Usage: see H01indexedCSVdatabase.hpp
 * Description: H01 indexed CSV database query - lookup indexed CSV database by pre/postsynaptic neuron ID, and find target/source connections
 * Input: 
 * 	INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING: localConnectomeNeuronIDlistDistinct.csv
 * 	INDEXED_CSV_DATABASE_QUERY_OUTPUT_CONNECTIONS: localConnectomeNeuronIDlist1.csv/localConnectomeNeuronIDlist2.csv
 * Output Format:
 * 	INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING:
 * 		INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_3D_LINEAR_REGRESSION:
 *			INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING_LDR: localConnectomeIncomingAxonMapping.ldr - LDR_TYPE_LINE ldrawColor plot3DpointStart.x plot3DpointStart.y plot3DpointStart.z plot3DpointEnd.x plot3DpointEnd.y plot3DpointEnd.z
 * 			INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING_CSV: localConnectomeIncomingAxonMapping.csv - polyFit.connectionNeuronID, polyFit.estSynapseType, polyFit.origin.x, polyFit.origin.y, polyFit.origin.z, polyFit.axis.x, polyFit.axis.y, polyFit.axis.z
 *		INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_2D_POLY_REGRESSION:
 * 			INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING_CSV: localConnectomeIncomingAxonMapping.csv - polyFit.connectionNeuronID, polyFit.estSynapseType, polyFit.a, polyFit.b, polyFit.c
 *	INDEXED_CSV_DATABASE_QUERY_OUTPUT_CONNECTIONS: localConnectomeNeuronIDlist1connectionsPresynaptic.csv/localConnectomeNeuronIDlist1connectionsPostsynaptic.csv/localConnectomeNeuronIDlist2connectionsPresynaptic.csv/localConnectomeNeuronIDlist2connectionsPostsynaptic.csv - connectionNeuronID1, connectionType1 [, locationObjectContentsXcoordinatesContent1, locationObjectContentsYcoordinatesContent1, locationObjectContentsZcoordinatesContent1], connectionNeuronID2, connectionType2 [, locationObjectContentsXcoordinatesContent2, locationObjectContentsYcoordinatesContent2, locationObjectContentsZcoordinatesContent2], etc 
 * Comments:
 * /
 *******************************************************************************/

#include "H01indexedCSVdatabase.hpp"

#ifdef INDEXED_CSV_DATABASE_QUERY

#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING
class H01indexedCSVdatabaseQueryObject
{
public:

	H01indexedCSVdatabaseQueryObject(void);
	~H01indexedCSVdatabaseQueryObject(void);

	long neuronID;
	int type;
	int locationObjectContentsXcoordinates;	//note coordinates are stored as integers in C3 connections database
	int locationObjectContentsYcoordinates;	//note coordinates are stored as integers in C3 connections database
	int locationObjectContentsZcoordinates;	//note coordinates are stored as integers in C3 connections database
	/*
	string neuronIDstring;
	string typeString;
	string locationObjectContentsXcoordinatesContent;
	string locationObjectContentsYcoordinatesContent;
	string locationObjectContentsZcoordinatesContent;
	*/
};
#endif


bool queryIndexedCSVdatabase();
	bool queryIndexedCSVdatabase(const string neuronListFileName, const bool queryPresynapticConnectionNeurons, const string neuronListConnectionsFileName);
		bool queryIndexedCSVdatabaseByNeuronID(const string neuronID, const bool queryPresynapticConnectionNeurons, vector<vector<string>>* neuronConnectionTargetSourceList);
			bool convertCSVlineToVector(const string* csvLineText, vector<string>* csvLineVector);
			
#ifndef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_FILE_OBJECT
void appendStringBufferToPreallocatedString(string* s, string stringToAppend);
#endif
#endif
