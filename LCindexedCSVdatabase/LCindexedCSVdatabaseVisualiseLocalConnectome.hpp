/*******************************************************************************
 *
 * File Name: LCindexedCSVdatabaseVisualiseLocalConnectome.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021-2023 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: LCLocalConnectome
 * Requirements: see LCindexedCSVdatabaseGlobalDefs.hpp
 * Compilation: see LCindexedCSVdatabaseGlobalDefs.hpp
 * Usage: see LCindexedCSVdatabaseGlobalDefs.hpp
 * Description: LC indexed CSV database visualise local connectome - visualises local connectome datasets
 * Input: localConnectomeNeurons.csv / localConnectomeConnectionsX.csv
 * Output Format: SVG (2D) / LDR (3D)
 * Comments:
 * /
 *******************************************************************************/

#ifndef HEADER_LCindexedCSVdatabaseVisualiseLocalConnectome
#define HEADER_LCindexedCSVdatabaseVisualiseLocalConnectome

#include "LCindexedCSVdatabaseGlobalDefs.hpp"
#include "LCindexedCSVdatabaseCalculateNeuronLayer.hpp"
#include "LCindexedCSVdatabaseOperations.hpp"
#include "SHAREDvars.hpp"

#ifdef INDEXED_CSV_DATABASE_VISUALISE_LOCAL_CONNECTOME

class LCindexedCSVdatabaseVisualiseLocalConnectomeClass
{
	private: LCindexedCSVdatabaseCalculateNeuronLayerClass LCindexedCSVdatabaseCalculateNeuronLayer;
	private: LCindexedCSVdatabaseOperationsClass LCindexedCSVdatabaseOperations;
	private: SHAREDvarsClass SHAREDvars;
	
	public: bool visualiseLocalConnectomeCSVdataset(const string local_connectome_folder_base);
		private: bool visualiseLocalConnectomeCSVdataset(const string local_connectome_folder_base, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages);
			public: bool visualiseLocalConnectomeCSVdataset(const string local_connectome_folder_base, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, const bool generate2Dvisualisation, vector<vector<string>>* localConnectomeCSVdatasetNeurons, vector<vector<string>>* localConnectomeCSVdatasetConnections, const bool visualiseTrace, const int traceIterationIndex);				
				private: bool visualiseLocalConnectomeGenerateContent(const string local_connectome_folder_base, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, const bool generate2Dvisualisation, vector<vector<string>>* localConnectomeCSVdatasetNeurons, vector<vector<string>>* localConnectomeCSVdatasetConnections, const string local_connectome_visualisation_filename_base, const int coloursetNumber, const bool visualiseLayers, const bool visualiseDirection, const bool visualiseFlow, const bool visualiseLayersSpecific, const int layerIndexVisualise, const bool visualiseTrace, const int traceIterationIndex);
				private: bool visualiseLocalConnectomeCSVdatasetWriteToFile(const string local_connectome_folder_base, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, const bool generate2Dvisualisation, const string local_connectome_visualisation_filename_base, string* localConnectomeVisualisationContents, const bool visualiseTrace, const int traceIterationIndex);

	private: int calculateConnectionFlowType(const double xCalibratedPre, const double yCalibratedPre, const double xCalibratedPost, const double yCalibratedPost, const double xCalibratedAverage);
	private: int calculateConnectionRadialGradientType(const double xCalibratedPre, const double xCalibratedPost);
	private: string convertDoubleToStringCalibrationPrecision(const double value);
};

#endif

#endif
