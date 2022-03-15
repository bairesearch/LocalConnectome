/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseCalculateNeuronLayer.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021-2022 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: H01LocalConnectome
 * Requirements: see H01indexedCSVdatabase.hpp
 * Compilation: see H01indexedCSVdatabase.hpp
 * Usage: see H01indexedCSVdatabase.hpp
 * Description: H01 indexed CSV database calculate neuron layer
 * Input: corticalLayersBoundaryKeypoints.csv
 * Output Format: vector<vector<vec>> corticalLayersKeypoints
 * Comments:
 * /
 *******************************************************************************/

#ifndef HEADER_H01indexedCSVdatabaseCalculateNeuronLayer
#define HEADER_H01indexedCSVdatabaseCalculateNeuronLayer

#include "H01indexedCSVdatabase.hpp"
#include "H01indexedCSVdatabaseOperations.hpp"
#include "SHAREDvars.hpp"
#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL
#include "SHAREDvector.hpp"
#endif


class H01indexedCSVdatabaseCalculateNeuronLayerClass
{
	private: SHAREDvarsClass SHAREDvars;
	#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL
	private: SHAREDvectorClass SHAREDvector;
	#endif
	private: H01indexedCSVdatabaseOperationsClass H01indexedCSVdatabaseOperations;

	public: bool readLocalNeuronsAndConnections(const string local_connectome_folder_base, const string neuronDatasetOrListFileName, const bool neuronListIsDataset, vector<string>* neuronList, map<string, int>* neuronMap, vector<vector<string>>* localConnectomeCSVdatasetNeurons, const bool readConnections, const string neuronListConnectionsFileName, vector<vector<string>>* localConnectomeCSVdatasetConnections, const bool buildConnectionsMap, map<string, int>* connectionsMap, const bool readLayers, const bool readLayersConnections);
		
		#ifdef INDEXED_CSV_DATABASE_CALCULATE_NEURON_LAYERS
		public: bool calculateLocalConnectomeLayers(vector<vector<string>>* localConnectomeCSVdatasetNeurons, vector<vector<string>>* localConnectomeCSVdatasetConnections, map<string, int>* neuronMap, const bool readConnections);
			#ifdef LOCAL_CONNECTOME_OFFICAL_RELEASE_C3_SOMAS_LAYERS
			private: bool transferLocalConnectomeNeuronLayersToConnectionsLayers(vector<vector<string>>* localConnectomeCSVdatasetNeurons, vector<vector<string>>* localConnectionCSVdataset, map<string, int>* neuronMap);
			#endif
			//#ifndef LOCAL_CONNECTOME_OFFICAL_RELEASE_C3_SOMAS_LAYERS
			private: bool readCorticalLayersBoundaryKeypointTable(const string corticalLayersBoundaryKeypointTableFileName, vector<vector<vec>>* corticalLayersKeypoints);
			private: bool calculateNeuronLayers(const bool localConnectomeDatasetType, vector<vector<string>>* localConnectionCSVdataset, vector<vector<vec>>* corticalLayersKeypoints);
				private: int calculateNeuronLayer(const int corticalLayersNumLayers, vector<vector<vec>>* corticalLayersKeypoints, const vec* neuronPos, vec* layerSurfaceNormVector);
					private: bool isNeuronInCorticalLayer(const int layerIndex, const vec* neuronPos, vector<vec>* corticalLayerKeypoints, const bool rightOfLine, vec* layerSurfaceNormVector);
						private: bool isPointRightOrLeftOfLine(const double Ax, const double Ay, const double Bx, const double By, const double X, const double Y, const bool rightOfLine);
			//#endif
		#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL_NORMALISE_LOCAL_CONNECTIVITY
		public: bool calculateLocalConnectomeNeuronLayerSurfaceNorms(vector<vector<string>>* localConnectomeCSVdatasetNeurons, map<string, int>* neuronMap);
			private: bool calculateNeuronLayerSurfaceNorms(vector<vector<string>>* localConnectionCSVdataset, vector<vector<vec>>* corticalLayersKeypoints);
				private: int calculateNeuronLayerSurfaceNorm(int corticalLayersNumLayers, vector<vector<vec>>* corticalLayersKeypoints, const vec* neuronPos, vec* layerSurfaceNormVector);
		#endif
		#endif
};

#endif


