/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseTraceLocalConnectome.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: H01LocalConnectome
 * Requirements: see H01indexedCSVdatabase.hpp
 * Compilation: see H01indexedCSVdatabase.hpp
 * Usage: see H01indexedCSVdatabase.hpp
 * Description: H01 indexed CSV database trace local connectome - traces local connectome dataset (saving visualisation)
 * Input: localConnectomeNeurons.csv / localConnectomeConnectionsX.csv
 * Output Format: SVG (2D) / LDR (3D) [multiple files for video construction]
 * Comments:
 * /
 *******************************************************************************/


#ifndef HEADER_H01indexedCSVdatabaseTraceLocalConnectome
#define HEADER_H01indexedCSVdatabaseTraceLocalConnectome

#include "H01indexedCSVdatabase.hpp"
#include "H01indexedCSVdatabaseVisualiseLocalConnectome.hpp"
#include "H01indexedCSVdatabaseCalculateNeuronLayer.hpp"
#include "H01indexedCSVdatabaseOperations.hpp"
#include "SHAREDvars.hpp"

#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME

class H01indexedCSVdatabaseTraceLocalConnectomeClass
{
	private: H01indexedCSVdatabaseVisualiseLocalConnectomeClass H01indexedCSVdatabaseVisualiseLocalConnectome;
	private: H01indexedCSVdatabaseCalculateNeuronLayerClass H01indexedCSVdatabaseCalculateNeuronLayer;
	private: H01indexedCSVdatabaseOperationsClass H01indexedCSVdatabaseOperations;
	private: SHAREDvarsClass SHAREDvars;
	
	public: bool traceLocalConnectomeCSVdataset(const string local_connectome_folder_base);
		private: bool traceLocalConnectomeCSVdatasetFile(const string local_connectome_folder_base, const bool datasetRead, const string neuronDatasetFileNameRead, const string connectionDatasetFileNameRead, const bool queryPresynapticConnectionNeurons, const bool connectionDatasetWrite, const bool appendToFile, const int traceIterationIndex, const string connectionDatasetFileNameWrite, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, const bool layerIndexEnforcement, const int layerIndex);
			private: bool traceLocalConnectomeCSVdataset(const string local_connectome_folder_base, vector<vector<string>>* localConnectomeNeurons, vector<vector<string>>* localConnectomeConnections, map<string, int>* neuronMap, map<string, int>* connectionsMap, const bool queryPresynapticConnectionNeurons, const bool connectionDatasetWrite, const bool appendToFile, const int traceIterationIndex, const string connectionDatasetFileNameWrite, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, const bool layerIndexEnforcement, const int layerIndex);
				private: bool traceLocalConnectomeCSVdatasetHighlight(
					const bool drawConnectionExcitatory, const bool drawConnectionInhibitory, const bool highlightConnectionExcitatory, const bool highlightConnectionInhibitory, const bool markConnectionExcitatory, const bool markConnectionInhibitory, 
					const bool drawTargetExcitatory, const bool drawTargetInhibitory, const bool highlightTargetExcitatory, const bool highlightTargetInhibitory, const bool markTargetExcitatory, const bool markTargetInhibitory, 
					const bool drawTargetOfExcitatoryConnection, const bool drawTargetOfInhibitoryConnection, const bool highlightTargetOfExcitatoryConnection, const bool highlightTargetOfInhibitoryConnection, const bool markTargetOfExcitatoryConnection, const bool markTargetOfInhibitoryConnection,
					const string local_connectome_folder_base, vector<vector<string>>* localConnectomeNeurons, vector<vector<string>>* localConnectomeConnections, map<string, int>* neuronMap, map<string, int>* connectionsMap, const bool queryPresynapticConnectionNeurons, const bool connectionDatasetWrite, const bool appendToFile, const int traceIterationIndex, const int traceIterationIndexSub, const string connectionDatasetFileNameWrite, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, const bool layerIndexEnforcement, const int layerIndex);

};

#endif

#endif

