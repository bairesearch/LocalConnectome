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

#ifndef HEADER_H01indexedCSVdatabaseCalculateNeuronLayer
#define HEADER_H01indexedCSVdatabaseCalculateNeuronLayer

#include "H01indexedCSVdatabase.hpp"
#include "H01indexedCSVdatabaseOperations.hpp"
#include "SHAREDvars.hpp"


class H01indexedCSVdatabaseCalculateNeuronLayerClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: H01indexedCSVdatabaseOperationsClass H01indexedCSVdatabaseOperations;

#ifdef INDEXED_CSV_DATABASE_CALCULATE_NEURON_LAYERS
	public: bool readCorticalLayersBoundaryKeypointTable(const string corticalLayersBoundaryKeypointTableFileName, vector<vector<vec>>* corticalLayersKeypoints);
	public: bool calculateNeuronLayers(const bool localConnectomeDatasetType, vector<vector<string>>* localConnectionCSVdataset, vector<vector<vec>>* corticalLayersKeypoints);
		private: int calculateNeuronLayer(int corticalLayersNumLayers, vector<vector<vec>>* corticalLayersKeypoints, double neuronX, double neuronY);
			private: bool isNeuronInCorticalLayer(int layerIndex, double neuronX, double neuronY, vector<vec>* corticalLayerKeypoints);
				private: bool isPointRightOfLine(double Ax, double Ay, double Bx, double By, double X, double Y);
#endif
};

#endif


