/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseOperations.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: H01LocalConnectome
 * Requirements: see H01indexedCSVdatabase.hpp
 * Compilation: see H01indexedCSVdatabase.hpp
 * Usage: see H01indexedCSVdatabase.hpp
 * Description: H01 indexed CSV database operations
 * Comments:
 * /
 *******************************************************************************/

#ifndef HEADER_H01indexedCSVdatabaseOperations
#define HEADER_H01indexedCSVdatabaseOperations

#include "H01indexedCSVdatabase.hpp"
#include "SHAREDvars.hpp"

class H01indexedCSVdatabaseOperationsClass
{
	private: SHAREDvarsClass SHAREDvars;

	public: double calibrateCoordinateX(const double csvDatabaseCoordinateX);
	public: double calibrateCoordinateY(const double csvDatabaseCoordinateY);
	public: double calibrateCoordinateZ(const double csvDatabaseCoordinateZ);
	
	public: bool readLocalConnectomeNeuronsCSVdataset(const string LocalConnectomeCSVdatasetFileName, vector<vector<string>>* localConnectomeNeuronsCSVdataset, int* localConnectomeNeuronsCSVdatasetSize, vector<string>* neuronList, map<string, int>* neuronMap);
	public: bool readLocalConnectomeConnectionsCSVdataset(const string LocalConnectomeCSVdatasetFileName, vector<vector<string>>* localConnectomeConnectionsCSVdataset, int* localConnectomeConnectionsCSVdatasetSize);


};

#endif
