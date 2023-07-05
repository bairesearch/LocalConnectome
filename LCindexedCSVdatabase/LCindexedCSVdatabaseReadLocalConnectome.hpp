
/*******************************************************************************
 *
 * File Name: LCindexedCSVdatabaseReadLocalConnectome.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021-2023 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: LCLocalConnectome
 * Requirements: see LCindexedCSVdatabaseGlobalDefs.hpp
 * Compilation: see LCindexedCSVdatabaseGlobalDefs.hpp
 * Usage: see LCindexedCSVdatabaseGlobalDefs.hpp
 * Description: LC indexed CSV database read local connectome - 
 * /
 *******************************************************************************/

#ifndef HEADER_LCindexedCSVdatabaseReadLocalConnectome
#define HEADER_LCindexedCSVdatabaseReadLocalConnectome

#include "LCindexedCSVdatabaseGlobalDefs.hpp"
#include "LCindexedCSVdatabaseCalculateNeuronLayer.hpp"	//now used universally for local connectome i/o wrapper - not just by INDEXED_CSV_DATABASE_QUERY_LAYERS
#include "LCindexedCSVdatabaseOperations.hpp"
#include "SHAREDvars.hpp"

#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME

class LCindexedCSVdatabaseReadLocalConnectomeClass
{
	private: SHAREDvarsClass SHAREDvars;
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL
	private: SHAREDvectorClass SHAREDvector;
	#endif
	private: LCindexedCSVdatabaseOperationsClass LCindexedCSVdatabaseOperations;
	private: LCindexedCSVdatabaseCalculateNeuronLayerClass LCindexedCSVdatabaseCalculateNeuronLayer;
	
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS
	private: void incrementNumberOfConnections(GaussianQuad* gaussianQuad, const vec* posSource, const vec* posTarget, const vec* sourceNeuronCorticalFlowVector);
	#endif
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL
		private: void incrementGaussianQuad(GaussianQuad* gaussianQuad, const vec* posSource, const vec* posTarget, const vec* sourceNeuronCorticalFlowVector);
	private: void averageGaussianQuad(GaussianQuad* gaussianQuad);
	#endif
	
	public: bool readLocalConnectome(const int readMode, const string local_connectome_folder_base);
		private: bool readLocalConnectomeByNeuronDatasetOrListFile(const int readMode, const string local_connectome_folder_base, const string neuronDatasetOrListFileName, const bool neuronListIsDataset, const bool queryPresynapticConnectionNeurons, const string neuronListConnectionsFileName, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, vector<H01connectivityModelClass>* connectivityModelLayers);
		#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL
		private: bool generateLargeModelNeuronsAndConnectionsWrapper(const int readMode, const string local_connectome_folder_base, const string neuronDatasetOrListFileName, const bool neuronListIsDataset, const bool write, const bool appendToFile, const string neuronListConnectionsFileName, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, vector<H01connectivityModelClass>* numberOfConnectionsLayersIncoming, vector<H01connectivityModelClass>* numberOfConnectionsLayersOutgoing);
		#endif
			#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS
			private: bool countConnectionsLocalMatrix(const int readMode, vector<string>* neuronList, map<string, int>* neuronMap, vector<vector<string>>* localConnectomeCSVdatasetNeurons, vector<vector<string>>* localConnectomeCSVdatasetConnections, const bool queryPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages);
			private: void countConnectionsLocal(const int readMode, vector<string>* neuronList, map<string, int>* neuronMap, map<string, int>* connectionsMap, vector<vector<string>>* localConnectomeCSVdatasetNeurons, vector<vector<string>>* localConnectomeCSVdatasetConnections, const bool queryPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, vector<H01connectivityModelClass>* connectivityModelLayers);
				#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_NUMBER_EXCITATORY_INHIBITORY_NEURONS
				private: void addConnectionNumberInhibitoryExcitatoryNeurons(const bool excitationType, const int localConnectomeNeuronLayer, vector<H01connectivityModelClass>* connectivityModelLayers);
				#endif
				private: void addConnection(const bool excitationTypeConnection, const int localConnectomeNeuronLayer, vector<H01connectivityModelClass>* connectivityModelLayers, const vec* posSource, const vec* posTarget, const vec* sourceNeuronCorticalFlowVector);
				#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT
				private: void addRecurrentNeuron(const int r, const bool excitationTypeConnection, const int localConnectomeNeuronLayer, vector<H01connectivityModelClass>* connectivityModelLayers, string sourceNeuronID);
					private: void addRecurrentConnectionToNeuronMap(map<string, int>* neuronsWithRecurrentConnectionMap, const string sourceNeuronID);
				private: void addRecurrentConnection(const int r, const bool excitationTypeConnection, const int localConnectomeNeuronLayer, vector<H01connectivityModelClass>* connectivityModelLayers);
				#endif
			private: void printNumberOfConnectionsLayersLocal(const bool queryByPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, constEffective vector<H01connectivityModelClass>* connectivityModelLayers);
				private: void printNumberOfConnectionsLocal(const int layerIndex, const bool queryByPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, const H01connectivityModelClass* connectivityModel);		
			#endif
			#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL
			private: void generateLargeModelNeuronsAndConnectionsLayers(constEffective vector<H01connectivityModelClass>* connectivityModelLayersIncomingLayers, constEffective vector<H01connectivityModelClass>* connectivityModelLayersOutgoingLayers);
				private: void generateLargeModelNeuronsAndConnections(const H01connectivityModelClass* connectivityModelLayersIncoming, const H01connectivityModelClass* connectivityModelLayersOutgoing);
			#endif
		private: int calculateConnectionExcitationType(const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, vector<string>* localConnectomeConnection, vector<string>* localConnectomeNeuronSource);

};

#endif

#endif

