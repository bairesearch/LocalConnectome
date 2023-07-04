/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseCalculateNeuronLayer.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021-2023 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: H01LocalConnectome
 * Requirements: see H01indexedCSVdatabaseGlobalDefs.hpp
 * Compilation: see H01indexedCSVdatabaseGlobalDefs.hpp
 * Usage: see H01indexedCSVdatabaseGlobalDefs.hpp
 * Description: H01 indexed CSV database calculate neuron layer
 * Input: corticalLayersBoundaryKeypoints.csv
 * Output Format: vector<vector<vec>> corticalLayersKeypoints
 * Comments:
 * /
 *******************************************************************************/

#ifndef HEADER_H01indexedCSVdatabaseCalculateNeuronLayer
#define HEADER_H01indexedCSVdatabaseCalculateNeuronLayer

#include "H01indexedCSVdatabaseGlobalDefs.hpp"
#include "H01indexedCSVdatabaseCalculateNeuronLayer.hpp"	//now used universally for local connectome i/o wrapper - not just by INDEXED_CSV_DATABASE_QUERY_LAYERS
#include "H01indexedCSVdatabaseOperations.hpp"
#include "SHAREDvars.hpp"
#include "SHAREDvector.hpp"


typedef struct { int x, y, z; } vecInt;

typedef struct { double xPos, yPos, xNeg, yNeg; } Quad;
typedef struct { int numberConnections; Quad mean, variance, count; } GaussianQuad;
void initialiseGaussianQuad(GaussianQuad* gaussianQuad);
void initialiseQuad(Quad* q);

class H01connectivityModelClass
{
public:
	H01connectivityModelClass(void);
	~H01connectivityModelClass(void);
	
	int numberOfLocalConnectomeNeurons;
	int numberOfLocalConnectomeNeuronsExcitatory;
	int numberOfLocalConnectomeNeuronsInhibitory;
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_NEURONS
	int numberOfNeuronsWithRecurrentConnections[INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_NUMBER_ITERATIONS];
	int numberOfNeuronsWithRecurrentConnectionsExcitatory[INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_NUMBER_ITERATIONS];
	int numberOfNeuronsWithRecurrentConnectionsInhibitory[INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_NUMBER_ITERATIONS];
	#endif
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PRINT_OUTPUT_VERBOSE_LOCALORNONLOCAL
	int numberConnectionsLocalOrNonLocalConnectomeExcitatory;
	int numberConnectionsLocalOrNonLocalConnectomeInhibitory;
	#endif
	//model synapse connections from soma as 4 independent half gaussians; in +x,-x,+y,-y directions 
	GaussianQuad neuronModelConnectionsLocalConnectome;
	GaussianQuad neuronModelConnectionsLocalConnectomeExcitatory;
	GaussianQuad neuronModelConnectionsLocalConnectomeInhibitory;
	GaussianQuad neuronModelConnectionsExternalConnectome;
	GaussianQuad neuronModelConnectionsExternalConnectomeExcitatory;
	GaussianQuad neuronModelConnectionsExternalConnectomeInhibitory;
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT
	GaussianQuad neuronModelConnectionsLocalConnectomeRecurrent[INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_NUMBER_ITERATIONS];
	GaussianQuad neuronModelConnectionsLocalConnectomeExcitatoryRecurrent[INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_NUMBER_ITERATIONS];
	GaussianQuad neuronModelConnectionsLocalConnectomeInhibitoryRecurrent[INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_NUMBER_ITERATIONS];
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_NEURONS
	map<string, int> neuronsWithRecurrentConnectionMap[INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_NUMBER_ITERATIONS];
	map<string, int> neuronsWithRecurrentConnectionExcitatoryMap[INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_NUMBER_ITERATIONS];
	map<string, int> neuronsWithRecurrentConnectionInhibitoryMap[INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_NUMBER_ITERATIONS];
	#endif
	#endif
};


class H01indexedCSVdatabaseCalculateNeuronLayerClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: SHAREDvectorClass SHAREDvector;
	private: H01indexedCSVdatabaseOperationsClass H01indexedCSVdatabaseOperations;

	//#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS
	public: vector<H01connectivityModelClass>* generateNumberOfConnectionsLayers();
	public: vector<vector<H01connectivityModelClass>>* generateNumberOfConnectionsLayersLayers();
	//#endif
	
	public: double getZNormalisationFactor();

	public: bool readLocalNeuronsAndConnections(const string local_connectome_folder_base, const string neuronDatasetOrListFileName, const bool neuronListIsDataset, vector<string>* neuronList, map<string, int>* neuronMap, vector<vector<string>>* localConnectomeCSVdatasetNeurons, const bool readConnections, const string neuronListConnectionsFileName, vector<vector<string>>* localConnectomeCSVdatasetConnections, const bool buildConnectionsMap, map<string, int>* connectionsMap, const bool readLayers, const bool readLayersConnections, const bool prepareRead = true);
	
		#ifdef INDEXED_CSV_DATABASE_QUERY_READ_DATASET_LOCAL_CONNECTOME_CONNECTIONS_DYNAMICALLY_GENERATE_REDUNDANT_FIELDS
		private: bool dynamicallyGenerateLocalConnectomeConnectionsRedundantFields(vector<vector<string>>* localConnectomeCSVdatasetNeurons, vector<vector<string>>* localConnectomeCSVdatasetConnections, map<string, int>* neuronMap);
		#endif	
		#ifdef INDEXED_CSV_DATABASE_NEURON_LAYERS
		#ifdef INDEXED_CSV_DATABASE_H01
		private: void calculateLocalConnectomeLayers(vector<vector<string>>* localConnectomeCSVdatasetNeurons, vector<vector<string>>* localConnectomeCSVdatasetConnections, map<string, int>* neuronMap, const bool readConnections);
			#ifdef INDEXED_CSV_DATABASE_H01
			#ifdef LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_LAYERS
			private: bool transferLocalConnectomeNeuronLayersToConnectionsLayers(vector<vector<string>>* localConnectomeCSVdatasetNeurons, vector<vector<string>>* localConnectomeCSVdatasetConnections, map<string, int>* neuronMap);
			#endif
			//#ifndef LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_LAYERS
			private: bool readCorticalLayersBoundaryKeypointTable(const string corticalLayersBoundaryKeypointTableFileName, vector<vector<vec>>* corticalLayersKeypoints);
			private: bool calculateNeuronLayers(const bool localConnectomeDatasetType, vector<vector<string>>* localConnectomeCSVdatasetNeurons, vector<vector<vec>>* corticalLayersKeypoints);
				private: int calculateNeuronLayer(const int corticalLayersNumLayers, vector<vector<vec>>* corticalLayersKeypoints, const vec* neuronPos, vec* layerSurfaceNormVector);
					private: bool isNeuronInCorticalLayer(const int layerIndex, const vec* neuronPos, vector<vec>* corticalLayerKeypoints, const bool rightOfLine, vec* layerSurfaceNormVector);
						private: bool isPointRightOrLeftOfLine(const double Ax, const double Ay, const double Bx, const double By, const double X, const double Y, const bool rightOfLine);
			//#endif
			#endif
		#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL_NORMALISE_LOCAL_CONNECTIVITY
		public: bool calculateLocalConnectomeNeuronLayerSurfaceNorms(vector<vector<string>>* localConnectomeCSVdatasetNeurons, map<string, int>* neuronMap);
			private: bool calculateNeuronLayerSurfaceNorms(vector<vector<string>>* localConnectomeCSVdatasetNeurons, vector<vector<vec>>* corticalLayersKeypoints);
				private: int calculateNeuronLayerSurfaceNorm(int corticalLayersNumLayers, vector<vector<vec>>* corticalLayersKeypoints, const vec* neuronPos, vec* layerSurfaceNormVector);
		#endif
		#endif
		#ifdef INDEXED_CSV_DATABASE_DC
		private: void calculateNeuronLayersDC(vector<vector<string>>* localConnectomeCSVdatasetNeurons);
		#endif
		#endif
};

#endif


