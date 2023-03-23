/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseQuery.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021-2023 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: H01LocalConnectome
 * Requirements: requires H01 indexed CSV database to have already been generated (see INDEXED_CSV_DATABASE_CREATE: H01indexedCSVdatabaseCreate.cpp/.hpp)
 * Compilation: see H01indexedCSVdatabaseGlobalDefs.hpp
 * Usage: see H01indexedCSVdatabaseGlobalDefs.hpp
 * Description: H01 indexed CSV database query - 
 *  INDEXED_CSV_DATABASE_QUERY_EXTRACT_INCOMING_OUTGOING_CONNECTIONS: mode 1 (lookup indexed CSV database by neuron ID, and find incoming/outgoing target connections, and write them to file)
 *  INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING: mode 2 (lookup indexed CSV database by neuron ID, find incoming target connections, and generate visualisation)
 *  INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: mode 3 (automatically generate localConnectomeConnections-typesFromPresynapticNeurons.csv/localConnectomeConnections-typesFromEMimages.csv from localConnectomeNeurons.csv and indexed CSV database)
 *  INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS: mode 4 (lookup indexed CSV database by neuron ID, count/infer proportion of incoming/outgoing excitatory/inhibitory target connections to local vs distal neurons)
 *  INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: mode 5 (lookup indexed CSV database by post/pre synaptic connection neuron ID, and identify connection with pre/post synaptic X/Y coordinates (if pre/post synaptic neuron type=UNKNOWN), add pre/post synaptic neuron ID, Z coordinates, and type coordinates to connection dataset [incomplete: awaiting release of H01 Release C3 neurons dataset; will print UNKNOWN neurons (with x/y coordinates only) along with candidate neuron_ids but not reconcile them])
 *  INDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS - mode 6 (crawl indexed CSV database by pre/post synaptic connection neuron ID, and count number of unique axons/dendrites as specified by neuron ID - not explicitly connected to local connectome [incomplete])
 * Input: 
 *  INDEXED_CSV_DATABASE_QUERY_OUTPUT_CONNECTIONS: localConnectomeNeurons.csv - id, x, y, z, type, excitation_type | somas.csv - soma_id, base_seg_id, c2_rep_strict, c2_rep_manual, c3_rep_strict, c3_rep_manual, proofread_104_rep, x, y, z, celltype, layer | localConnectomeNeuronIDlistDistinct.csv - id
 *  INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING: localConnectomeNeurons.csv - id, x, y, z, type, excitation_type | somas.csv - soma_id, base_seg_id, c2_rep_strict, c2_rep_manual, c3_rep_strict, c3_rep_manual, proofread_104_rep, x, y, z, celltype, layer | localConnectomeNeuronIDlistDistinct.csv - id
 *  INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: localConnectomeNeurons.csv - id, x, y, z, type, excitation_type | somas.csv - soma_id, base_seg_id, c2_rep_strict, c2_rep_manual, c3_rep_strict, c3_rep_manual, proofread_104_rep, x, y, z, celltype, layer | localConnectomeNeuronIDlistDistinct.csv - id
 *  INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS: localConnectomeNeurons.csv - id, x, y, z, type, excitation_type | somas.csv - soma_id, base_seg_id, c2_rep_strict, c2_rep_manual, c3_rep_strict, c3_rep_manual, proofread_104_rep, x, y, z, celltype, layer | localConnectomeNeuronIDlistDistinct.csv - id
 *  INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: localConnectomeConnections-typesFromEMimages-useAllValuesAvailableFromInBodyCellConnection.csv - pre_id, pre_x, pre_y, pre_z, pre_type, post_id, post_x, post_y, post_z, post_type, post_class_label, syn_num, excitation_type
 *  INDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS: N/A
 * Output Format:
 *  INDEXED_CSV_DATABASE_QUERY_OUTPUT_CONNECTIONS: localConnectomeNeuronIDlistConnectionsPresynaptic.csv/localConnectomeNeuronIDlistConnectionsPostsynaptic.csv - connectionNeuronID1, connectionType1 [, locationObjectContentsXcoordinatesContent1, locationObjectContentsYcoordinatesContent1, locationObjectContentsZcoordinatesContent1], connectionNeuronID2, connectionType2 [, locationObjectContentsXcoordinatesContent2, locationObjectContentsYcoordinatesContent2, locationObjectContentsZcoordinatesContent2], etc 
 *  INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING:
 *      INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_3D_LINEAR_REGRESSION:
 *          INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING_LDR: localConnectomeIncomingAxonMapping.ldr - LDR_REFERENCE_TYPE_LINE ldrawColor plot3DpointStart.x plot3DpointStart.y plot3DpointStart.z plot3DpointEnd.x plot3DpointEnd.y plot3DpointEnd.z
 *          INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING_CSV: localConnectomeIncomingAxonMapping.csv - polyFit.connectionNeuronID, polyFit.estSynapseType, polyFit.origin.x, polyFit.origin.y, polyFit.origin.z, polyFit.axis.x, polyFit.axis.y, polyFit.axis.z
 *      INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_2D_POLY_REGRESSION:
 *          INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING_CSV: localConnectomeIncomingAxonMapping.csv - polyFit.connectionNeuronID, polyFit.estSynapseType, polyFit.a, polyFit.b, polyFit.c
 *  INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: localConnectomeConnections-typesFromPresynapticNeurons/typesFromEMimages.csv - pre_id, pre_x, pre_y, pre_z, pre_type, post_id, post_x, post_y, post_z, post_type, post_class_label, syn_num, excitation_type
 *  INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS: N/A
 *  INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: localConnectomeConnections-typesFromEMimages.csv - pre_id, pre_x, pre_y, pre_z, pre_type, post_id, post_x, post_y, post_z, post_type, post_class_label, syn_num, excitation_type
 *  INDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS: N/A
 * Comments:
 * /
 *******************************************************************************/


#ifndef HEADER_H01indexedCSVdatabaseQuery
#define HEADER_H01indexedCSVdatabaseQuery

#include "H01indexedCSVdatabaseGlobalDefs.hpp"
#include "H01indexedCSVdatabaseCalculateNeuronLayer.hpp"	//now used universally for local connectome i/o wrapper - not just by INDEXED_CSV_DATABASE_QUERY_LAYERS
#include "H01indexedCSVdatabaseOperations.hpp"
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
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL
	private: SHAREDvectorClass SHAREDvector;
	#endif
	private: H01indexedCSVdatabaseOperationsClass H01indexedCSVdatabaseOperations;
	private: H01indexedCSVdatabaseCalculateNeuronLayerClass H01indexedCSVdatabaseCalculateNeuronLayer;
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS
	private: void incrementNumberOfConnections(GaussianQuad* gaussianQuad, const vec* posSource, const vec* posTarget, const vec* sourceNeuronCorticalFlowVector);
	#endif

	public: bool queryIndexedCSVdatabase(const int queryMode, const string indexed_csv_database_folder, const string local_connectome_folder_base);
		private: bool queryIndexedCSVdatabaseByNeuronDatasetOrListFile(const int queryMode, const string indexed_csv_database_folder, const string local_connectome_folder_base, const string neuronDatasetOrListFileName, const bool neuronListIsDataset, const bool queryPresynapticConnectionNeurons, const bool write, const bool appendToFile, const string neuronListConnectionsFileName, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, vector<H01connectivityModelClass>* connectivityModelLayers = NULL);
			#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING	
			private: void performIncomingAxonMapping(ofstream* writeFileObject);
			#endif
			private: bool queryIndexedCSVdatabaseByNeuronList(const int queryMode, const string indexed_csv_database_folder, vector<string>* neuronList, map<string, int>* neuronMap, vector<vector<string>>* localConnectomeCSVdatasetNeurons, vector<vector<string>>* localConnectomeCSVdatasetConnections, map<string, int>* connectionsMap, const bool queryByPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, ofstream* writeFileObject, const bool appendToFile, vector<H01connectivityModelClass>* connectivityModelLayers = NULL);
				#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS
				#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_NUMBER_EXCITATORY_INHIBITORY_NEURONS
				private: void countConnectionsNumberInhibitoryExcitatoryNeurons(vector<string>* localConnectomeNeuron, vector<H01connectivityModelClass>* connectivityModelLayers);
					private: void addConnectionNumberInhibitoryExcitatoryNeurons(const bool excitationType, const int localConnectomeNeuronLayer, vector<H01connectivityModelClass>* connectivityModelLayers);
				#endif
				#endif
				#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING	
				private: void performIncomingAxonMappingRead(map<string, int>* neuronMap, const string sourceNeuronID, const string targetNeuronID, const string connectionType, const string locationObjectContentsXcoordinatesContent, const string locationObjectContentsYcoordinatesContent, const string locationObjectContentsZcoordinatesContent);
				#endif					
				#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET
				private: void generateLocalConnectomeConnectionsDataset(vector<string> csvLineVector, map<string, int>* neuronMap, vector<vector<string>>* localConnectomeCSVdatasetNeurons, vector<string>* localConnectomeNeuron, const string sourceNeuronID, const string targetNeuronID, const int connectionTypeInt, const bool queryByPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, ofstream* writeFileObject);
				#endif
				#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS
				private: void countConnections(const int queryMode, map<string, int>* neuronMap, vector<vector<string>>* localConnectomeCSVdatasetNeurons, vector<string>* localConnectomeNeuron, vector<H01connectivityModelClass>* connectivityModelLayers, const string sourceNeuronID, const string targetNeuronID, const int connectionTypeInt, const bool queryByPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages);
					private: void addConnection(const bool preAndPostSynapticNeuronAreInLocalConnectome, const bool excitationTypeConnection, const int localConnectomeNeuronLayer, vector<H01connectivityModelClass>* connectivityModelLayers, const vec* posSource, const vec* posTarget, const vec* sourceNeuronCorticalFlowVector);
				#endif
					private: int calculateConnectionExcitationType1(const int connectionTypeInt, vector<string>* localConnectomeNeuronSource, vector<string>* localConnectomeNeuronTarget, const bool queryByPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages);
						private: int calculateConnectionExcitationType2(const int connectionTypeInt, vector<string>* localConnectomeNeuronSource, const string presynapticNeuronType, const bool queryByPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages);	
				#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS
				private: void printNumberOfConnectionsLayers(const bool queryByPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, constEffective vector<H01connectivityModelClass>* connectivityModelLayers, const bool countLocalConnectomeNeurons, const bool countInternalConnectomeConnections, const bool countExternalConnectomeConnections);
					private: void printNumberOfConnections(const int layerIndex, const bool queryByPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, const H01connectivityModelClass* connectivityModel, const bool countLocalConnectomeNeurons, const bool countInternalConnectomeConnections, const bool countExternalConnectomeConnections);		
				#endif

					
		#ifdef INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET
		private: bool queryIndexedCSVdatabaseByConnectionDatasetFile(const int queryMode, const string indexed_csv_database_folder, const string local_connectome_folder_base, const bool connectionDatasetRead, const string connectionDatasetFileNameRead, vector<vector<string>>* localConnectomeCSVdatasetConnections, const bool queryPresynapticConnectionNeurons, const bool connectionDatasetWrite, const bool appendToFile, const string connectionDatasetFileNameWrite, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages);
			private: bool queryIndexedCSVdatabaseByConnectionNeuronList(const int queryMode, const string indexed_csv_database_folder, vector<string>* neuronList, vector<vector<string>>* localConnectomeCSVdatasetConnections, const bool queryByPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, const bool connectionDatasetWrite, ofstream* writeFileObject);
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

