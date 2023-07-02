/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseOperations.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021-2023 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: H01LocalConnectome
 * Requirements: see H01indexedCSVdatabaseGlobalDefs.hpp
 * Compilation: see H01indexedCSVdatabaseGlobalDefs.hpp
 * Usage: see H01indexedCSVdatabaseGlobalDefs.hpp
 * Description: H01 indexed CSV database operations
 * Comments:
 * /
 *******************************************************************************/

#ifndef HEADER_H01indexedCSVdatabaseOperations
#define HEADER_H01indexedCSVdatabaseOperations

#include "H01indexedCSVdatabaseGlobalDefs.hpp"
#include "SHAREDvars.hpp"

class H01indexedCSVdatabaseOperationsClass
{
	private: SHAREDvarsClass SHAREDvars;

	public: double calibrateCoordinateX(const double csvDatabaseCoordinateX);
	public: double calibrateCoordinateY(const double csvDatabaseCoordinateY);
	public: double calibrateCoordinateZ(const double csvDatabaseCoordinateZ);

	public: bool convertLocalConnectomeSomasCSVdataset(const string local_connectome_folder_base, const string LocalConnectomeSomasCSVdatasetFileName, const string LocalConnectomeNeuronsCSVdatasetFileName);

	public: bool readLocalConnectomeNeuronsCSVdataset(const string LocalConnectomeCSVdatasetFileName, vector<vector<string>>* localConnectomeNeuronsCSVdataset, int* localConnectomeNeuronsCSVdatasetSize, vector<string>* neuronList, map<string, int>* neuronMap);
	public: bool readLocalConnectomeConnectionsCSVdataset(const string LocalConnectomeCSVdatasetFileName, vector<vector<string>>* localConnectomeConnectionsCSVdataset, int* localConnectomeConnectionsCSVdatasetSize);
		public: bool readLocalConnectomeConnectionsCSVdataset(const string LocalConnectomeCSVdatasetFileName, vector<vector<string>>* localConnectomeConnectionsCSVdataset, int* localConnectomeConnectionsCSVdatasetSize, bool buildConnectionsMap, map<string, int>* connectionsMap);

	public: void prepareLocalConnectomeDatasetRead(const string local_connectome_folder_base);
		public: void prepareLocalConnectomeDatasetReadCustom(const string local_connectome_folder_base, const string folderName);
	public: ofstream prepareLocalConnectomeDatasetWrite(const bool write, const bool appendToFile, const string datasetFileNameWrite);
		public: void prepareLocalConnectomeDatasetWrite();
	public: void finaliseLocalConnectomeDatasetWrite(const bool write, ofstream* writeFileObject);

};

#endif
