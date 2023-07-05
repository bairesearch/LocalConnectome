/*******************************************************************************
 *
 * File Name: LCindexedCSVdatabaseTraceLocalConnectome.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021-2023 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: LCLocalConnectome
 * Requirements: see LCindexedCSVdatabaseGlobalDefs.hpp
 * Compilation: see LCindexedCSVdatabaseGlobalDefs.hpp
 * Usage: see LCindexedCSVdatabaseGlobalDefs.hpp
 * Description: LC indexed CSV database trace local connectome - traces local connectome dataset (saving visualisation)
 * Input: localConnectomeNeurons.csv / localConnectomeConnectionsX.csv
 * Output Format: SVG (2D) / LDR (3D) [multiple files for video construction]
 * Comments:
 * /
 *******************************************************************************/


#ifndef HEADER_LCindexedCSVdatabaseTraceLocalConnectome
#define HEADER_LCindexedCSVdatabaseTraceLocalConnectome

#include "LCindexedCSVdatabaseGlobalDefs.hpp"
#include "LCindexedCSVdatabaseVisualiseLocalConnectome.hpp"
#include "LCindexedCSVdatabaseCalculateNeuronLayer.hpp"
#include "LCindexedCSVdatabaseOperations.hpp"
#include "SHAREDvars.hpp"

#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME

class LCindexedCSVdatabaseTraceLocalConnectomeClass
{
	private: LCindexedCSVdatabaseVisualiseLocalConnectomeClass LCindexedCSVdatabaseVisualiseLocalConnectome;
	private: LCindexedCSVdatabaseCalculateNeuronLayerClass LCindexedCSVdatabaseCalculateNeuronLayer;
	private: LCindexedCSVdatabaseOperationsClass LCindexedCSVdatabaseOperations;
	private: SHAREDvarsClass SHAREDvars;
	
	public: bool traceLocalConnectomeCSVdataset(const string local_connectome_folder_base);
		private: bool traceLocalConnectomeCSVdatasetFile(const string local_connectome_folder_base, const bool datasetRead, const string neuronDatasetFileNameRead, const string connectionDatasetFileNameRead, const bool queryPresynapticConnectionNeurons, const bool connectionDatasetWrite, const bool appendToFile, const string connectionDatasetFileNameWrite, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages);
			private: bool traceLocalConnectomeCSVdataset(const string local_connectome_folder_base, vector<vector<string>>* localConnectomeCSVdatasetNeurons, vector<vector<string>>* localConnectomeCSVdatasetConnections, map<string, int>* neuronMap, map<string, int>* connectionsMap, const bool queryPresynapticConnectionNeurons, const bool connectionDatasetWrite, const bool appendToFile, const int traceIterationIndex, const string connectionDatasetFileNameWrite, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, const bool layerIndexEnforcement, const int layerIndex);
				private: bool traceLocalConnectomeCSVdatasetHighlight(
					const bool drawConnectionExcitatory, const bool drawConnectionInhibitory, const bool highlightConnectionExcitatory, const bool highlightConnectionInhibitory, const bool markConnectionExcitatory, const bool markConnectionInhibitory, 
					const bool drawTargetExcitatory, const bool drawTargetInhibitory, const bool highlightTargetExcitatory, const bool highlightTargetInhibitory, const bool markTargetExcitatory, const bool markTargetInhibitory, 
					const bool drawTargetOfExcitatoryConnection, const bool drawTargetOfInhibitoryConnection, const bool highlightTargetOfExcitatoryConnection, const bool highlightTargetOfInhibitoryConnection, const bool markTargetOfExcitatoryConnection, const bool markTargetOfInhibitoryConnection, const bool markInhibitedNeurons, const bool markExcitedNeurons, 
					const string local_connectome_folder_base, vector<vector<string>>* localConnectomeCSVdatasetNeurons, vector<vector<string>>* localConnectomeCSVdatasetConnections, map<string, int>* neuronMap, map<string, int>* connectionsMap, const bool queryPresynapticConnectionNeurons, const bool connectionDatasetWrite, const bool appendToFile, const int traceIterationIndex, const int traceIterationIndexSub, const string connectionDatasetFileNameWrite, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, const bool layerIndexEnforcement, const int layerIndex);

};

#endif

#endif

