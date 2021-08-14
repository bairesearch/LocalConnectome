/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseVisualiseLocalConnectome.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: H01LocalConnectome
 * Requirements: see H01indexedCSVdatabase.hpp
 * Compilation: see H01indexedCSVdatabase.hpp
 * Usage: see H01indexedCSVdatabase.hpp
 * Description: H01 indexed CSV database visualise local connectome - visualises local connectome datasets
 * Input: localConnectomeNeurons.csv / localConnectomeConnectionsX.csv
 * Output Format: SVG (2D) / LDR (3D)
 * Comments:
 * /
 *******************************************************************************/

#ifndef HEADER_H01indexedCSVdatabaseVisualiseLocalConnectome
#define HEADER_H01indexedCSVdatabaseVisualiseLocalConnectome

#include "H01indexedCSVdatabase.hpp"
#include "H01indexedCSVdatabaseCalculateNeuronLayer.hpp"
#include "H01indexedCSVdatabaseOperations.hpp"
#include "SHAREDvars.hpp"

#ifdef INDEXED_CSV_DATABASE_VISUALISE_LOCAL_CONNECTOME

class H01indexedCSVdatabaseVisualiseLocalConnectomeClass
{
	private: H01indexedCSVdatabaseCalculateNeuronLayerClass H01indexedCSVdatabaseCalculateNeuronLayer;
	private: H01indexedCSVdatabaseOperationsClass H01indexedCSVdatabaseOperations;
	private: SHAREDvarsClass SHAREDvars;
	
	public: bool visualiseLocalConnectomeCSVdataset(const string local_connectome_folder_base);
		private: bool visualiseLocalConnectomeCSVdataset(const string local_connectome_folder_base, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages);
			public: bool visualiseLocalConnectomeCSVdataset(const string local_connectome_folder_base, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, const bool generate2Dvisualisation, vector<vector<string>>* localConnectionCSVdatasetNeurons, vector<vector<string>>* localConnectionCSVdatasetConnections, const bool visualiseTrace, const int traceIterationIndex);				
				private: bool visualiseLocalConnectomeGenerateContent(const string local_connectome_folder_base, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, const bool generate2Dvisualisation, vector<vector<string>>* localConnectionCSVdatasetNeurons, vector<vector<string>>* localConnectionCSVdatasetConnections, const string local_connectome_visualisation_filename_base, const int coloursetNumber, const bool visualiseLayers, const bool visualiseDirection, const bool visualiseFlow, const bool visualiseLayersSpecific, const int layerIndexVisualise, const bool visualiseTrace, const int traceIterationIndex);
				private: bool visualiseLocalConnectomeCSVdatasetWriteToFile(const string local_connectome_folder_base, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, const bool generate2Dvisualisation, const string local_connectome_visualisation_filename_base, string* localConnectomeVisualisationContents, const bool visualiseTrace, const int traceIterationIndex);

	private: int calculateConnectionFlowType(const double xCalibratedPre, const double yCalibratedPre, const double xCalibratedPost, const double yCalibratedPost);
	private: int calculateConnectionRadialGradientType(const double xCalibratedPre, const double xCalibratedPost);
	private: string convertDoubleToStringCalibrationPrecision(const double value);
};

#endif

#endif
