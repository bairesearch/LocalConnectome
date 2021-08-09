/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseCalculateNeuronLayer.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021 Baxter AI (baxterai.com)
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

#include "H01indexedCSVdatabase.hpp"
#include "SHAREDvars.hpp"

#ifdef INDEXED_CSV_DATABASE_CALCULATE_NEURON_LAYERS

bool readCorticalLayersBoundaryKeypointTable(const string corticalLayersBoundaryKeypointTableFileName, vector<vector<vec>>* corticalLayersKeypoints);
bool calculateNeuronLayers(const bool , vector<vector<string>>* localConnectionCSVdataset, vector<vector<vec>>* corticalLayersKeypoints);
	int calculateNeuronLayer(int corticalLayersNumLayers, vector<vector<vec>>* corticalLayersKeypoints, double neuronX, double neuronY);
		bool isNeuronInCorticalLayer(int layerIndex, double neuronX, double neuronY, vector<vec>* corticalLayerKeypoints);
			bool isPointRightOfLine(double Ax, double Ay, double Bx, double By, double X, double Y);
#endif

void getLinesFromFileCSV(const string fileName, vector<vector<string>>* CSVdatasetFile, int* CSVdatasetFileSize, const char delimiter, const bool expectFirstLineHeader);
double calibrateCoordinateX(const double csvDatabaseCoordinateX);
double calibrateCoordinateY(const double csvDatabaseCoordinateY);
double calibrateCoordinateZ(const double csvDatabaseCoordinateZ);


