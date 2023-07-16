/*******************************************************************************
 *
 * File Name: LCindexedCSVdatabaseOperations.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021-2023 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: LCLocalConnectome
 * Requirements: see LCindexedCSVdatabaseGlobalDefs.hpp
 * Compilation: see LCindexedCSVdatabaseGlobalDefs.hpp
 * Usage: see LCindexedCSVdatabaseGlobalDefs.hpp
 * Description: LC indexed CSV database operations
 * Comments:
 * /
 *******************************************************************************/

#ifndef HEADER_LCindexedCSVdatabaseOperations
#define HEADER_LCindexedCSVdatabaseOperations

#include "LCindexedCSVdatabaseGlobalDefs.hpp"
#include "SHAREDvars.hpp"

class LCindexedCSVdatabaseOperationsClass
{
	private: SHAREDvarsClass SHAREDvars;

	public: double calibrateCoordinateX(const double csvDatabaseCoordinateX);
	public: double calibrateCoordinateY(const double csvDatabaseCoordinateY);
	public: double calibrateCoordinateZ(const double csvDatabaseCoordinateZ);

	#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_NEURONS_DATASET
	public: bool convertLocalConnectomeSomasCSVdataset(const string local_connectome_folder_base, const string LocalConnectomeSomasCSVdatasetFileName, const string LocalConnectomeNeuronsCSVdatasetFileName);
	#endif
	
	public: bool readLocalConnectomeNeuronsCSVdataset(const string LocalConnectomeCSVdatasetFileName, vector<vector<string>>* localConnectomeNeuronsCSVdataset, int* localConnectomeNeuronsCSVdatasetSize, vector<string>* neuronList, map<string, int>* neuronMap);
	public: bool readLocalConnectomeConnectionsCSVdataset(const string LocalConnectomeCSVdatasetFileName, vector<vector<string>>* localConnectomeConnectionsCSVdataset, int* localConnectomeConnectionsCSVdatasetSize);
		public: bool readLocalConnectomeConnectionsCSVdataset(const string LocalConnectomeCSVdatasetFileName, vector<vector<string>>* localConnectomeConnectionsCSVdataset, int* localConnectomeConnectionsCSVdatasetSize, bool buildConnectionsMap, map<string, int>* connectionsMap);

	public: void prepareLocalConnectomeDatasetRead(const string local_connectome_folder_base);
		public: void prepareLocalConnectomeDatasetReadCustom(const string local_connectome_folder_base, const string folderName);
	public: ofstream prepareLocalConnectomeDatasetWrite(const bool write, const bool appendToFile, const string datasetFileNameWrite);
		public: void prepareLocalConnectomeDatasetWrite();
	public: void finaliseLocalConnectomeDatasetWrite(const bool write, ofstream* writeFileObject);

	#ifdef INDEXED_CSV_DATABASE_PREPROCESS_ADC_MIRRORTWINS
	public: bool readPreprocessMirrorTwinsCSVdataset(map<string, int>* mirrorTwinsMap, map<string, int>* neuronMap);
	#endif

};

#endif
