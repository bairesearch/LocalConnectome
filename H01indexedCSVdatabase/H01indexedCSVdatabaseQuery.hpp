/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseQuery.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: H01LocalConnectome
 * Requirements: requires H01 indexed CSV database to have already been generated (see INDEXED_CSV_DATABASE_CREATE: H01indexedCSVdatabaseCreate.cpp/.hpp)
 * Compilation: see H01indexedCSVdatabase.hpp
 * Usage: see H01indexedCSVdatabase.hpp
 * Description: H01 indexed CSV database query - 
 * 	INDEXED_CSV_DATABASE_QUERY_EXTRACT_INCOMING_OUTGOING_CONNECTIONS: mode 1 (lookup indexed CSV database by neuron ID, and find incoming/outgoing target connections, and write them to file)
 * 	INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING: mode 2 (lookup indexed CSV database by neuron ID, find incoming target connections, and generate visualisation)
 * 	INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: mode 3 (automatically generate localConnectomeConnections-typesFromPresynapticNeurons.csv/localConnectomeConnections-typesFromEMimages.csv from localConnectomeNeurons.csv and H01 indexed CSV database)
 * Input: 
 * 	INDEXED_CSV_DATABASE_QUERY_OUTPUT_CONNECTIONS: localConnectomeNeurons.csv (or localConnectomeNeuronIDlistDistinct.csv)
 * 	INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING: localConnectomeNeurons.csv (or localConnectomeNeuronIDlistDistinct.csv)
 * 	INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: localConnectomeNeurons.csv
 * Output Format:
 *	INDEXED_CSV_DATABASE_QUERY_OUTPUT_CONNECTIONS: localConnectomeNeuronIDlistConnectionsPresynaptic.csv/localConnectomeNeuronIDlistConnectionsPostsynaptic.csv - connectionNeuronID1, connectionType1 [, locationObjectContentsXcoordinatesContent1, locationObjectContentsYcoordinatesContent1, locationObjectContentsZcoordinatesContent1], connectionNeuronID2, connectionType2 [, locationObjectContentsXcoordinatesContent2, locationObjectContentsYcoordinatesContent2, locationObjectContentsZcoordinatesContent2], etc 
 * 	INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING:
 * 		INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_3D_LINEAR_REGRESSION:
 *			INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING_LDR: localConnectomeIncomingAxonMapping.ldr - LDR_REFERENCE_TYPE_LINE ldrawColor plot3DpointStart.x plot3DpointStart.y plot3DpointStart.z plot3DpointEnd.x plot3DpointEnd.y plot3DpointEnd.z
 * 			INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING_CSV: localConnectomeIncomingAxonMapping.csv - polyFit.connectionNeuronID, polyFit.estSynapseType, polyFit.origin.x, polyFit.origin.y, polyFit.origin.z, polyFit.axis.x, polyFit.axis.y, polyFit.axis.z
 *		INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_2D_POLY_REGRESSION:
 * 			INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING_CSV: localConnectomeIncomingAxonMapping.csv - polyFit.connectionNeuronID, polyFit.estSynapseType, polyFit.a, polyFit.b, polyFit.c
 *  INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: localConnectomeConnections.csv - pre_id, pre_x, pre_y, pre_z, pre_type, post_id, post_x, post_y, post_z, post_type, post_class_label, syn_num, excitation_type
 * Comments:
 * /
 *******************************************************************************/

#include "H01indexedCSVdatabase.hpp"
#include "SHAREDvars.hpp"

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


class H01indexedCSVdatabaseQueryClass
{
	private: SHAREDvarsClass SHAREDvars;
	
	public: bool queryIndexedCSVdatabase();
		private: bool queryIndexedCSVdatabaseByNeuronListFile(const string neuronListFileName, const bool queryPresynapticConnectionNeurons, const string neuronListConnectionsFileName, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages);
			private: bool queryIndexedCSVdatabaseByNeuronList(vector<vector<string>>* localConnectomeNeurons, vector<string>* neuronList, const bool queryByPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, ofstream* writeFileObject, string* writeFileString);
				private: bool queryIndexedCSVdatabaseByNeuronID(const string neuronID, const bool queryByPresynapticConnectionNeurons, vector<vector<string>>* neuronConnectionList);
					private: bool convertCSVlineToVector(const string* csvLineText, vector<string>* csvLineVector);
			
#ifndef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_FILE_OBJECT
	private: void appendStringBufferToPreallocatedString(string* s, string stringToAppend);
#endif
};

#endif
