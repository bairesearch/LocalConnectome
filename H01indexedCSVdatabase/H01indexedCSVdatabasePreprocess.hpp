
/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabasePreprocess.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021-2023 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: H01LocalConnectome
 * Requirements: see H01indexedCSVdatabaseGlobalDefs.hpp
 * Compilation: see H01indexedCSVdatabaseGlobalDefs.hpp
 * Usage: see H01indexedCSVdatabaseGlobalDefs.hpp
 * Description: H01 indexed CSV database preprocess - 
 * /
 *******************************************************************************/

#ifndef HEADER_H01indexedCSVdatabasePreprocess
#define HEADER_H01indexedCSVdatabasePreprocess

#include "H01indexedCSVdatabaseGlobalDefs.hpp"
#include "H01indexedCSVdatabaseCalculateNeuronLayer.hpp"	//now used universally for local connectome i/o wrapper - not just by INDEXED_CSV_DATABASE_QUERY_LAYERS
#include "H01indexedCSVdatabaseOperations.hpp"
#include "SHAREDvars.hpp"

#ifdef INDEXED_CSV_DATABASE_PREPROCESS

typedef struct { string x, y, z; } vecString;

class H01indexedCSVdatabasePreprocessClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: H01indexedCSVdatabaseOperationsClass H01indexedCSVdatabaseOperations;
	private: H01indexedCSVdatabaseCalculateNeuronLayerClass H01indexedCSVdatabaseCalculateNeuronLayer;
	
	public: bool preprocess(const int preprocessMode, const string local_connectome_folder_base);
		#ifdef INDEXED_CSV_DATABASE_ADC
		private: void generateLocalConnectomeNeuronsDatasetFromADCneuronsFile(const string local_connectome_folder_base, const string neuronDatasetFileNameWrite);
			private: vector<string> parseADCcoordinatesPositionVectorString(const string positionVectorString);
		private: void generateLocalConnectomeConnectionsDatasetFromADCconnectionsFile(const string local_connectome_folder_base, const string connectionDatasetFileNameWrite, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages);
		#endif
		#ifdef INDEXED_CSV_DATABASE_LDC
		#ifdef INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_NEURONS_DATASET
		private: void generateLocalConnectomeNeuronsDatasetFromSkeletons(const string local_connectome_folder_base, const string neuronDatasetFileNameWrite);
			private: vector<string> getNeuronIDlist(const string local_connectome_folder_base, const bool writeToFile);
			private: vector<string> getNeuronTypeList(const string local_connectome_folder_base, vector<string>* neuronIDlist);
			private: vector<vecString> getNeuronPosList(const string local_connectome_folder_base, vector<string>* neuronIDlist);
		#endif
		#ifdef INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET
		private: void generateLocalConnectomeConnectionsDatasetFromMatrix(const string local_connectome_folder_base, const string connectionDatasetFileNameWrite);
			private: void convertConnectionsListMatrixFile(const string local_connectome_folder_base, const string matrixFileName, const string matrixFilePreClassLabel, const string matrixFilePostClassLabel, ofstream* writeFileObject, map<string, string>* neuronRegionTypeMap, map<string, string>* neuronExcitationTypeMap);
		#endif
		#endif
		private: int getNeuronExcitationTypeFromType(const string neuronType, map<string, string>* neuronExcitationTypeMap);
		private: map<string, string> calculateNeuronExcitationTypeMap(const string local_connectome_folder_base, const string neuronTypesFileName);
		#ifdef INDEXED_CSV_DATABASE_LDC
		private: map<string, string> calculateNeuronRegionTypeMap(const string local_connectome_folder_base);
		#endif
		#ifdef INDEXED_CSV_DATABASE_ADC
		private: map<string, string> calculateNeuronCoordinatesTypeMap(const string local_connectome_folder_base);
		private: map<string, string> calculateNeuronNeurotransmitterTypeMap(const string local_connectome_folder_base);
		#endif
			private: map<string, string> calculateNeuronTypeMap(const string local_connectome_folder_base, const string neuronTypesFileName, const int neuronTypeIndex, const int neuronValueIndex, const char delimiter);
				private: void calculateNeuronTypeMap(const string local_connectome_folder_base, const string neuronTypesFileName, const int neuronTypeIndex, const int neuronValueIndex, const char delimiter, map<string, string>* neuronTypeMap);

};

#endif

#endif

