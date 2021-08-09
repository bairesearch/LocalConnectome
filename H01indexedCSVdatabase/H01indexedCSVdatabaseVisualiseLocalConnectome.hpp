/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseVisualiseLocalConnectome.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: H01LocalConnectome
 * Requirements: see H01indexedCSVdatabase.hpp
 * Compilation: see H01indexedCSVdatabase.hpp
 * Usage: see H01indexedCSVdatabase.hpp
 * Description: H01 indexed CSV database visualise local connectome
 * Input: localConnectomeNeurons.csv / localConnectomeConnectionsX.csv
 * Output Format: SVG (2D) / LDR (3D)
 * Comments:
 * /
 *******************************************************************************/

#include "H01indexedCSVdatabase.hpp"

#ifdef INDEXED_CSV_DATABASE_VISUALISE_LOCAL_CONNECTOME

bool visualiseLocalConnectomeCSVdataset();
	bool visualiseLocalConnectomeCSVdataset(const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages);
		bool visualiseLocalConnectomeCSVdataset(const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, const bool generate2Dvisualisation, vector<vector<string>>* localConnectionCSVdatasetNeurons, vector<vector<string>>* localConnectionCSVdatasetConnections);
			bool visualiseLocalConnectomeGenerateContent(const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, const bool generate2Dvisualisation, vector<vector<string>>* localConnectionCSVdatasetNeurons, vector<vector<string>>* localConnectionCSVdatasetConnections, const string local_connectome_visualisation_filename_base, const int coloursetNumber, const bool visualiseLayers, const bool visualiseDirection, const bool visualiseFlow, const bool visualiseLayersSpecific, const int layerIndexVisualise);
			bool visualiseLocalConnectomeCSVdatasetWriteToFile(const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, const bool generate2Dvisualisation, const string local_connectome_visualisation_filename_base, string* localConnectomeVisualisationContents);

int calculateConnectionFlowType(const double xCalibratedPre, const double yCalibratedPre, const double xCalibratedPost, const double yCalibratedPost);
int calculateConnectionRadialGradientType(const double xCalibratedPre, const double xCalibratedPost);
string convertDoubleToString(const double value);

#endif


