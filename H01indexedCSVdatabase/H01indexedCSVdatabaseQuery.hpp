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
 *  INDEXED_CSV_DATABASE_QUERY_EXTRACT_INCOMING_OUTGOING_CONNECTIONS: mode 1 (lookup indexed CSV database by neuron ID, and find incoming/outgoing target connections, and write them to file)
 *  INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING: mode 2 (lookup indexed CSV database by neuron ID, find incoming target connections, and generate visualisation)
 *  INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: mode 3 (automatically generate localConnectomeConnections-typesFromPresynapticNeurons.csv/localConnectomeConnections-typesFromEMimages.csv from localConnectomeNeurons.csv and indexed CSV database)
 *  INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PROPORTION_LOCAL_VS_NONLOCAL_CONNECTIONS: mode 4 (lookup indexed CSV database by neuron ID, count/infer proportion of incoming/outgoing excitatory/inhibitory target connections to local vs distal neurons)
 *  INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: mode 5 (lookup indexed CSV database by post/pre synaptic connection neuron ID, and identify connection with pre/post synaptic X/Y coordinates (if pre/post synaptic neuron type=UNKNOWN), add pre/post synaptic neuron ID, Z coordinates, and type coordinates to connection dataset [incomplete: awaiting release of H01 Release C3 neurons dataset; will print UNKNOWN neurons (with x/y coordinates only) along with candidate neuron_ids but not reconcile them])
 *  INDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS_COUNT_NUMBER_UNIQUE_AXONS_DENDRITES - mode 6 (crawl indexed CSV database by pre/post synaptic connection neuron ID, and count number of unique axons/dendrites as specified by neuron ID - not explicitly connected to local connectome [incomplete])
 * Input: 
 *  INDEXED_CSV_DATABASE_QUERY_OUTPUT_CONNECTIONS: localConnectomeNeurons.csv (or localConnectomeNeuronIDlistDistinct.csv) - id, x, y, z, type, excitation_type (or id)
 *  INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING: localConnectomeNeurons.csv (or localConnectomeNeuronIDlistDistinct.csv) - id, x, y, z, type, excitation_type (or id)
 *  INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: localConnectomeNeurons.csv - id, x, y, z, type, excitation_type
 *  INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PROPORTION_LOCAL_VS_NONLOCAL_CONNECTIONS: localConnectomeNeurons.csv (or localConnectomeNeuronIDlistDistinct.csv) - id, x, y, z, type, excitation_type (or id)
 *  INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: localConnectomeConnections-typesFromEMimages-useAllValuesAvailableFromInBodyCellConnection.csv - pre_id, pre_x, pre_y, pre_z, pre_type, post_id, post_x, post_y, post_z, post_type, post_class_label, syn_num, excitation_type
 *  INDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS_COUNT_NUMBER_UNIQUE_AXONS_DENDRITES: N/A
 * Output Format:
 *  INDEXED_CSV_DATABASE_QUERY_OUTPUT_CONNECTIONS: localConnectomeNeuronIDlistConnectionsPresynaptic.csv/localConnectomeNeuronIDlistConnectionsPostsynaptic.csv - connectionNeuronID1, connectionType1 [, locationObjectContentsXcoordinatesContent1, locationObjectContentsYcoordinatesContent1, locationObjectContentsZcoordinatesContent1], connectionNeuronID2, connectionType2 [, locationObjectContentsXcoordinatesContent2, locationObjectContentsYcoordinatesContent2, locationObjectContentsZcoordinatesContent2], etc 
 *  INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING:
 *      INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_3D_LINEAR_REGRESSION:
 *          INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING_LDR: localConnectomeIncomingAxonMapping.ldr - LDR_REFERENCE_TYPE_LINE ldrawColor plot3DpointStart.x plot3DpointStart.y plot3DpointStart.z plot3DpointEnd.x plot3DpointEnd.y plot3DpointEnd.z
 *          INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING_CSV: localConnectomeIncomingAxonMapping.csv - polyFit.connectionNeuronID, polyFit.estSynapseType, polyFit.origin.x, polyFit.origin.y, polyFit.origin.z, polyFit.axis.x, polyFit.axis.y, polyFit.axis.z
 *      INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_2D_POLY_REGRESSION:
 *          INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING_CSV: localConnectomeIncomingAxonMapping.csv - polyFit.connectionNeuronID, polyFit.estSynapseType, polyFit.a, polyFit.b, polyFit.c
 *  INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: localConnectomeConnections-typesFromPresynapticNeurons.csv/localConnectomeConnections-typesFromEMimages.csv - pre_id, pre_x, pre_y, pre_z, pre_type, post_id, post_x, post_y, post_z, post_type, post_class_label, syn_num, excitation_type
 *  INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PROPORTION_LOCAL_VS_NONLOCAL_CONNECTIONS: N/A
 *  INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: localConnectomeConnections-typesFromEMimages.csv - pre_id, pre_x, pre_y, pre_z, pre_type, post_id, post_x, post_y, post_z, post_type, post_class_label, syn_num, excitation_type
 *  INDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS_COUNT_NUMBER_UNIQUE_AXONS_DENDRITES: N/A
 * Comments:
 * /
 *******************************************************************************/

#ifndef HEADER_H01indexedCSVdatabaseQuery
#define HEADER_H01indexedCSVdatabaseQuery

#include "H01indexedCSVdatabase.hpp"
#ifdef INDEXED_CSV_DATABASE_QUERY_LAYERS
#include "H01indexedCSVdatabaseCalculateNeuronLayer.hpp"
#endif
#include "H01indexedCSVdatabaseOperations.hpp"
#include "SHAREDvars.hpp"

#ifdef INDEXED_CSV_DATABASE_QUERY

typedef struct { int x, y, z; } vecInt;

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
	private: H01indexedCSVdatabaseOperationsClass H01indexedCSVdatabaseOperations;
	#ifdef INDEXED_CSV_DATABASE_QUERY_LAYERS
	private: H01indexedCSVdatabaseCalculateNeuronLayerClass H01indexedCSVdatabaseCalculateNeuronLayer;
	#endif
	
	public: bool queryIndexedCSVdatabase(const int queryMode, const string indexed_csv_database_folder, const string local_connectome_folder_base);
		private: bool queryIndexedCSVdatabaseByNeuronDatasetOrListFile(const int queryMode, const string indexed_csv_database_folder, const string local_connectome_folder_base, const string neuronDatasetOrListFileName, const bool neuronListIsDataset, const bool queryPresynapticConnectionNeurons, const bool write, const bool appendToFile, const string neuronListConnectionsFileName, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages);
			private: bool queryIndexedCSVdatabaseByNeuronList(const int queryMode, const string indexed_csv_database_folder, vector<string>* neuronList, map<string, int>* neuronMap, vector<vector<string>>* localConnectomeNeurons, vector<vector<string>>* localConnectomeConnections, const bool queryByPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, ofstream* writeFileObject, string* writeFileString, const bool appendToFile);
				#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS
				private: void printNumberOfConnections(const bool queryByPresynapticConnectionNeurons, vector<int>* numberConnectionsToLocalConnectomeLayers, vector<int>* numberConnectionsToExternalConnectomeLayers, vector<int>* numberConnectionsToLocalConnectomeExcitatoryLayers, vector<int>* numberConnectionsToExternalConnectomeExcitatoryLayers, vector<int>* numberConnectionsToLocalConnectomeInhibitoryLayers, vector<int>* numberConnectionsToExternalConnectomeInhibitoryLayers, vector<int>* numberOfLocalConnectomeNeuronsLayers, vector<int>* numberOfLocalConnectomeNeuronsExcitatoryLayers, vector<int>* numberOfLocalConnectomeNeuronsInhibitoryLayers);
				#endif		
		#ifdef INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET
		private: bool queryIndexedCSVdatabaseByConnectionDatasetFile(const int queryMode, const string indexed_csv_database_folder, const string local_connectome_folder_base, const bool connectionDatasetRead, const string connectionDatasetFileNameRead, vector<vector<string>>* localConnectomeConnections, const bool queryPresynapticConnectionNeurons, const bool connectionDatasetWrite, const bool appendToFile, const string connectionDatasetFileNameWrite, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages);
			private: bool queryIndexedCSVdatabaseByConnectionNeuronList(const int queryMode, const string indexed_csv_database_folder, vector<string>* neuronList, vector<vector<string>>* localConnectomeConnections, const bool queryByPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, const bool connectionDatasetWrite, ofstream* writeFileObject, string* writeFileString);
		#endif
				private: bool queryIndexedCSVdatabaseByNeuronID(const string indexed_csv_database_folder, const string neuronID, const bool queryByPresynapticConnectionNeurons, vector<vector<string>>* neuronConnectionList);
					private: bool convertCSVlineToVector(const string* csvLineText, vector<string>* csvLineVector);
		#ifdef INDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS_COUNT_NUMBER_UNIQUE_AXONS_DENDRITES
		private: bool crawlIndexedCSVdatabase(const int queryMode, const string indexed_csv_database_folder, const bool queryPresynapticConnectionNeurons);
			private: bool crawlIndexedCSVdatabase(const int queryMode, const string indexed_csv_database_folder, const string neuronIDstart, const bool queryByPresynapticConnectionNeurons, map<long,long>* uniqueNeuronIDmap, long* numberConnectionsExcitatory, long* numberConnectionsInhibitory);
		#endif

	#ifndef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_FILE_OBJECT
	private: void appendStringBufferToPreallocatedString(string* s, string stringToAppend);
	#endif
};

#endif

#endif
