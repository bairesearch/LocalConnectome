/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseQuery.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021-2022 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: H01LocalConnectome
 * Requirements: requires H01 indexed CSV database to have already been generated (see INDEXED_CSV_DATABASE_CREATE: H01indexedCSVdatabaseCreate.cpp/.hpp)
 * Compilation: see H01indexedCSVdatabase.hpp
 * Usage: see H01indexedCSVdatabase.hpp
 * Description: H01 indexed CSV database query - 
 *  INDEXED_CSV_DATABASE_QUERY_EXTRACT_INCOMING_OUTGOING_CONNECTIONS: mode 1 (lookup indexed CSV database by neuron ID, and find incoming/outgoing target connections, and write them to file)
 *  INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING: mode 2 (lookup indexed CSV database by neuron ID, find incoming target connections, and generate visualisation)
 *  INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: mode 3 (automatically generate localConnectomeConnections-typesFromPresynapticNeurons.csv/localConnectomeConnections-typesFromEMimages.csv from localConnectomeNeurons.csv and indexed CSV database)
 *  INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS: mode 4 (lookup indexed CSV database by neuron ID, count/infer proportion of incoming/outgoing excitatory/inhibitory target connections to local vs distal neurons)
 *  INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: mode 5 (lookup indexed CSV database by post/pre synaptic connection neuron ID, and identify connection with pre/post synaptic X/Y coordinates (if pre/post synaptic neuron type=UNKNOWN), add pre/post synaptic neuron ID, Z coordinates, and type coordinates to connection dataset [incomplete: awaiting release of H01 Release C3 neurons dataset; will print UNKNOWN neurons (with x/y coordinates only) along with candidate neuron_ids but not reconcile them])
 *  INDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS - mode 6 (crawl indexed CSV database by pre/post synaptic connection neuron ID, and count number of unique axons/dendrites as specified by neuron ID - not explicitly connected to local connectome [incomplete])
 *  INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL - mode 7 (generate large artificial cortical model; extrapolate z region to same size as x/y [incomplete])
 * Input: 
 *  INDEXED_CSV_DATABASE_QUERY_OUTPUT_CONNECTIONS: localConnectomeNeurons.csv - id, x, y, z, type, excitation_type | somas.csv - soma_id, base_seg_id, c2_rep_strict, c2_rep_manual, c3_rep_strict, c3_rep_manual, proofread_104_rep, x, y, z, celltype, layer | localConnectomeNeuronIDlistDistinct.csv - id
 *  INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING: localConnectomeNeurons.csv - id, x, y, z, type, excitation_type | somas.csv - soma_id, base_seg_id, c2_rep_strict, c2_rep_manual, c3_rep_strict, c3_rep_manual, proofread_104_rep, x, y, z, celltype, layer | localConnectomeNeuronIDlistDistinct.csv - id
 *  INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: localConnectomeNeurons.csv - id, x, y, z, type, excitation_type | somas.csv - soma_id, base_seg_id, c2_rep_strict, c2_rep_manual, c3_rep_strict, c3_rep_manual, proofread_104_rep, x, y, z, celltype, layer | localConnectomeNeuronIDlistDistinct.csv - id
 *  INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS: localConnectomeNeurons.csv - id, x, y, z, type, excitation_type | somas.csv - soma_id, base_seg_id, c2_rep_strict, c2_rep_manual, c3_rep_strict, c3_rep_manual, proofread_104_rep, x, y, z, celltype, layer | localConnectomeNeuronIDlistDistinct.csv - id
 *  INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: localConnectomeConnections-typesFromEMimages-useAllValuesAvailableFromInBodyCellConnection.csv - pre_id, pre_x, pre_y, pre_z, pre_type, post_id, post_x, post_y, post_z, post_type, post_class_label, syn_num, excitation_type
 *  INDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS: N/A
 *  INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL: localConnectomeNeurons.csv - id, x, y, z, type, excitation_type | somas.csv - soma_id, base_seg_id, c2_rep_strict, c2_rep_manual, c3_rep_strict, c3_rep_manual, proofread_104_rep, x, y, z, celltype, layer | localConnectomeNeuronIDlistDistinct.csv - id
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
 *  INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL: localConnectomeConnectionsLargeModel-typesFromPresynapticNeurons/typesFromEMimages.csv - pre_id, pre_x, pre_y, pre_z, pre_type, post_id, post_x, post_y, post_z, post_type, post_class_label, syn_num, excitation_type; localConnectomeNeuronsLargeModel.csv - id, x, y, z, type, excitation_type
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
#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL
#include "SHAREDvector.hpp"
#endif

#ifdef INDEXED_CSV_DATABASE_QUERY

typedef struct { int x, y, z; } vecInt;

typedef struct { double xPos, yPos, xNeg, yNeg; } Quad;
typedef struct { int numberConnections; Quad mean, variance, count; } GaussianQuad;
void initialiseGaussianQuad(GaussianQuad* gaussianQuad);
void initialiseQuad(Quad* q);


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

#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS
class H01connectivityModelClass
{
public:
	H01connectivityModelClass(void);
	~H01connectivityModelClass(void);
	
	int numberOfLocalConnectomeNeurons;
	int numberOfLocalConnectomeNeuronsExcitatory;
	int numberOfLocalConnectomeNeuronsInhibitory;
	//model synapse connections from soma as 4 independent half gaussians; in +x,-x,+y,-y directions 
	GaussianQuad neuronModelConnectionsLocalConnectome;
	GaussianQuad neuronModelConnectionsExternalConnectome;
	GaussianQuad neuronModelConnectionsLocalConnectomeExcitatory;
	GaussianQuad neuronModelConnectionsExternalConnectomeExcitatory;
	GaussianQuad neuronModelConnectionsLocalConnectomeInhibitory;
	GaussianQuad neuronModelConnectionsExternalConnectomeInhibitory;
};

#endif


class H01indexedCSVdatabaseQueryClass
{
	private: SHAREDvarsClass SHAREDvars;
	#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL
	private: SHAREDvectorClass SHAREDvector;
	#endif
	private: H01indexedCSVdatabaseOperationsClass H01indexedCSVdatabaseOperations;
	#ifdef INDEXED_CSV_DATABASE_QUERY_LAYERS
	private: H01indexedCSVdatabaseCalculateNeuronLayerClass H01indexedCSVdatabaseCalculateNeuronLayer;
	#endif
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS
	private: vector<H01connectivityModelClass>* generateNumberOfConnectionsLayers();
	private: vector<vector<H01connectivityModelClass>>* generateNumberOfConnectionsLayersLayers();
	private: void incrementNumberOfConnections(GaussianQuad* gaussianQuad, vec* posSource, vec* posTarget);
	#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL
		private: void incrementGaussianQuad(GaussianQuad* gaussianQuad, vec* posSource, vec* posTarget);
	private: void averageGaussianQuad(GaussianQuad* gaussianQuad);
	#endif
	#endif
	public: bool queryIndexedCSVdatabase(const int queryMode, const string indexed_csv_database_folder, const string local_connectome_folder_base);
		#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL
		private: bool generateLargeModelNeuronsAndConnectionsWrapper(const int queryMode, const string indexed_csv_database_folder, const string local_connectome_folder_base, const string neuronDatasetOrListFileName, const bool neuronListIsDataset, const bool write, const bool appendToFile, const string neuronListConnectionsFileName, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, vector<H01connectivityModelClass>* numberOfConnectionsLayersIncoming, vector<H01connectivityModelClass>* numberOfConnectionsLayersOutgoing);
		#endif
		private: bool queryIndexedCSVdatabaseByNeuronDatasetOrListFile(const int queryMode, const string indexed_csv_database_folder, const string local_connectome_folder_base, const string neuronDatasetOrListFileName, const bool neuronListIsDataset, const bool queryPresynapticConnectionNeurons, const bool write, const bool appendToFile, const string neuronListConnectionsFileName, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, vector<H01connectivityModelClass>* connectivityModelLayers = NULL);
			private: bool readLocalNeuronsAndConnections(const int queryMode, const string indexed_csv_database_folder, const string local_connectome_folder_base, const string neuronDatasetOrListFileName, const bool neuronListIsDataset, const bool write, const string neuronListConnectionsFileName, vector<string>* neuronList, map<string, int>* neuronMap, vector<vector<string>>* localConnectomeNeurons, vector<vector<string>>* localConnectomeConnections, map<string, int>* connectionsMap);
			private: bool queryIndexedCSVdatabaseByNeuronList(const int queryMode, const string indexed_csv_database_folder, vector<string>* neuronList, map<string, int>* neuronMap, vector<vector<string>>* localConnectomeNeurons, vector<vector<string>>* localConnectomeConnections, map<string, int>* connectionsMap, const bool queryByPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, ofstream* writeFileObject, string* writeFileString, const bool appendToFile, vector<H01connectivityModelClass>* connectivityModelLayers = NULL);
				#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL
				private: void generateLargeModelNeuronsAndConnectionsLayers(constEffective vector<H01connectivityModelClass>* connectivityModelLayersIncomingLayers, constEffective vector<H01connectivityModelClass>* connectivityModelLayersOutgoingLayers, const bool countLocalConnectomeNeurons, const bool countInternalConnectomeConnections, const bool countExternalConnectomeConnections);
					private: void generateLargeModelNeuronsAndConnections(const H01connectivityModelClass* connectivityModelLayersIncoming, const H01connectivityModelClass* connectivityModelLayersOutgoing, const bool countLocalConnectomeNeurons, const bool countInternalConnectomeConnections, const bool countExternalConnectomeConnections);
				#endif
				#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS
				private: double getZNormalisationFactor();
				private: void printNumberOfConnectionsLayers(const bool queryByPresynapticConnectionNeurons, constEffective vector<H01connectivityModelClass>* connectivityModelLayers, const bool countLocalConnectomeNeurons, const bool countInternalConnectomeConnections, const bool countExternalConnectomeConnections);
					private: void printNumberOfConnections(const bool queryByPresynapticConnectionNeurons, const H01connectivityModelClass* connectivityModel, const bool countLocalConnectomeNeurons, const bool countInternalConnectomeConnections, const bool countExternalConnectomeConnections);
				#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL
				private: bool countConnectionsLocal(vector<string>* neuronList, map<string, int>* neuronMap, vector<vector<string>>* localConnectomeNeurons, vector<vector<string>>* localConnectomeConnections, const bool queryPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages);
				#endif			
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
