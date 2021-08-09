/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseVisualiseLocalConnectome.cpp
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

#include "H01indexedCSVdatabaseVisualiseLocalConnectome.hpp"
#include "H01indexedCSVdatabaseCalculateNeuronLayer.hpp"
#include "SHAREDvars.hpp"

#ifdef INDEXED_CSV_DATABASE_VISUALISE_LOCAL_CONNECTOME

SHAREDvarsClass SHAREDvars;

string currentDirectory;

static int local_connectome_visualisation_neurons_colour_exciteneuron_layer_array[CORTICAL_LAYER_NUMBER_OF_LAYERS] = {LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITENEURON_LAYER1,LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITENEURON_LAYER2,LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITENEURON_LAYER3,LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITENEURON_LAYER4,LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITENEURON_LAYER5,LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITENEURON_LAYER6,LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITENEURON_LAYER7};
static int local_connectome_visualisation_neurons_colour_inhibitneuron_layer_array[CORTICAL_LAYER_NUMBER_OF_LAYERS] = {LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_INHIBITNEURON_LAYER1,LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_INHIBITNEURON_LAYER2,LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_INHIBITNEURON_LAYER3,LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_INHIBITNEURON_LAYER4,LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_INHIBITNEURON_LAYER5,LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_INHIBITNEURON_LAYER6,LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_INHIBITNEURON_LAYER7};

bool visualiseLocalConnectomeCSVdataset()
{
	bool result = true;
	
	currentDirectory = SHAREDvars.getCurrentDirectory();
	
	visualiseLocalConnectomeCSVdataset(true);
	visualiseLocalConnectomeCSVdataset(false);
	
	return result;
}

bool visualiseLocalConnectomeCSVdataset(const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages)
{
	bool result = true;
	
	cout << "connectionTypesDerivedFromPresynapticNeuronsOrEMimages = " << connectionTypesDerivedFromPresynapticNeuronsOrEMimages << endl;
	
	#ifdef INDEXED_CSV_DATABASE_VISUALISE_LOCAL_CONNECTOME_USE_RELATIVE_FOLDER
	SHAREDvars.setCurrentDirectory(currentDirectory);
	#endif
	string localConnectomeCSVdatabaseFolder = LOCAL_CONNECTOME_FOLDER_BASE;
	SHAREDvars.setCurrentDirectory(localConnectomeCSVdatabaseFolder);
	localConnectomeCSVdatabaseFolder = LOCAL_CONNECTOME_DATASET_FOLDER;
	SHAREDvars.setCurrentDirectory(localConnectomeCSVdatabaseFolder);

	vector<vector<string>> localConnectionCSVdatasetNeurons;
	int localConnectionCSVdatasetNeuronsSize = 0;
	getLinesFromFileCSV(LOCAL_CONNECTOME_DATASET_NEURONS_FILENAME, &localConnectionCSVdatasetNeurons, &localConnectionCSVdatasetNeuronsSize, CSV_DELIMITER_CHAR, true);
	
	vector<vector<string>> localConnectionCSVdatasetConnections;
	int localConnectionCSVdatasetConnectionsSize = 0;
	if(connectionTypesDerivedFromPresynapticNeuronsOrEMimages)
	{
		getLinesFromFileCSV(LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, &localConnectionCSVdatasetConnections, &localConnectionCSVdatasetConnectionsSize, CSV_DELIMITER_CHAR, true);
	}
	else
	{
		getLinesFromFileCSV(LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, &localConnectionCSVdatasetConnections, &localConnectionCSVdatasetConnectionsSize, CSV_DELIMITER_CHAR, true);
	}

	#ifdef LOCAL_CONNECTOME_VISUALISATION_LAYERS
	//int corticalLayersNumKeypointsMax;	//= 28	//maximum number keypoints (number cols/2)
	const string corticalLayersBoundaryKeypointTableFileName = CORTICAL_LAYER_BOUNDARY_KEYPOINT_TABLE_FILE_NAME;
	vector<vector<vec>> corticalLayersKeypoints;
	readCorticalLayersBoundaryKeypointTable(corticalLayersBoundaryKeypointTableFileName, &corticalLayersKeypoints);
	calculateNeuronLayers(true, &localConnectionCSVdatasetNeurons, &corticalLayersKeypoints);
	calculateNeuronLayers(false, &localConnectionCSVdatasetConnections, &corticalLayersKeypoints);
	#endif
		
	#ifdef LOCAL_CONNECTOME_VISUALISATION_2D_SVG
	visualiseLocalConnectomeCSVdataset(connectionTypesDerivedFromPresynapticNeuronsOrEMimages, true, &localConnectionCSVdatasetNeurons, &localConnectionCSVdatasetConnections);
	#endif
	#ifdef LOCAL_CONNECTOME_VISUALISATION_3D_LDR
	visualiseLocalConnectomeCSVdataset(connectionTypesDerivedFromPresynapticNeuronsOrEMimages, false, &localConnectionCSVdatasetNeurons, &localConnectionCSVdatasetConnections);
	#endif

	return result;
}

bool visualiseLocalConnectomeCSVdataset(const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, const bool generate2Dvisualisation, vector<vector<string>>* localConnectionCSVdatasetNeurons, vector<vector<string>>* localConnectionCSVdatasetConnections)
{
	bool result = true;
	
	cout << "generate2Dvisualisation = " << generate2Dvisualisation << endl;
	
	string local_connectome_visualisation_filename_direction;
	string local_connectome_visualisation_filename_direction_flow;
	#ifdef LOCAL_CONNECTOME_VISUALISATION_LAYERS
	string local_connectome_visualisation_filename_layered_direction;
	string local_connectome_visualisation_filename_layered_direction_flow;
	#endif
	int local_connectome_visualisation_filename_direction_number_coloursets;
	int local_connectome_visualisation_filename_direction_flow_number_coloursets;
	#ifdef LOCAL_CONNECTOME_VISUALISATION_LAYERS
	int local_connectome_visualisation_filename_layered_direction_number_coloursets;
	int local_connectome_visualisation_filename_layered_direction_flow_number_coloursets;
	#endif
	
	if(generate2Dvisualisation)
	{
		local_connectome_visualisation_filename_direction = LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_DIRECTION;
		local_connectome_visualisation_filename_direction_flow = LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_DIRECTION_FLOW;
		#ifdef LOCAL_CONNECTOME_VISUALISATION_LAYERS
		local_connectome_visualisation_filename_layered_direction = LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_LAYERED_DIRECTION;
		local_connectome_visualisation_filename_layered_direction_flow = LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_LAYERED_DIRECTION_FLOW;
		#endif
		local_connectome_visualisation_filename_direction_number_coloursets = LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_DIRECTION_NUMBER_COLOURSETS;
		local_connectome_visualisation_filename_direction_flow_number_coloursets = LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_DIRECTION_FLOW_NUMBER_COLOURSETS;
		#ifdef LOCAL_CONNECTOME_VISUALISATION_LAYERS
		local_connectome_visualisation_filename_layered_direction_number_coloursets = LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_LAYERED_DIRECTION_NUMER_COLOURSETS;
		local_connectome_visualisation_filename_layered_direction_flow_number_coloursets = LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_LAYERED_DIRECTION_FLOW_NUMBER_COLOURSETS;
		#endif
	}
	else
	{
		local_connectome_visualisation_filename_direction = LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_DIRECTION;
		local_connectome_visualisation_filename_direction_flow = LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_DIRECTION_FLOW;
		#ifdef LOCAL_CONNECTOME_VISUALISATION_LAYERS
		local_connectome_visualisation_filename_layered_direction = LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_LAYERED_DIRECTION;
		local_connectome_visualisation_filename_layered_direction_flow = LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_LAYERED_DIRECTION_FLOW;
		#endif
		local_connectome_visualisation_filename_direction_number_coloursets = LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_DIRECTION_NUMBER_COLOURSETS;
		local_connectome_visualisation_filename_direction_flow_number_coloursets = LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_DIRECTION_FLOW_NUMBER_COLOURSETS;
		#ifdef LOCAL_CONNECTOME_VISUALISATION_LAYERS
		local_connectome_visualisation_filename_layered_direction_number_coloursets = LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_LAYERED_DIRECTION_NUMER_COLOURSETS;
		local_connectome_visualisation_filename_layered_direction_flow_number_coloursets = LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_LAYERED_DIRECTION_FLOW_NUMBER_COLOURSETS;
		#endif
	}
	
	//generate visualisations;
	cout << "generate visualisations;" << endl;
	cout << "local_connectome_visualisation_filename_direction_number_coloursets = " << local_connectome_visualisation_filename_direction_number_coloursets << endl;
	for(int coloursetIndex=1; coloursetIndex<=local_connectome_visualisation_filename_direction_number_coloursets; coloursetIndex++)
	{
		string local_connectome_visualisation_filename_base = string(LOCAL_CONNECTOME_VISUALISATION_FILENAME_COLOURSET_PREPEND) + local_connectome_visualisation_filename_direction + SHAREDvars.convertIntToString(coloursetIndex);
		visualiseLocalConnectomeGenerateContent(connectionTypesDerivedFromPresynapticNeuronsOrEMimages, generate2Dvisualisation, localConnectionCSVdatasetNeurons, localConnectionCSVdatasetConnections, local_connectome_visualisation_filename_base, coloursetIndex, false, true, false, false, INT_DEFAULT_VALUE);
	}
	cout << "local_connectome_visualisation_filename_direction_flow_number_coloursets = " << local_connectome_visualisation_filename_direction_flow_number_coloursets << endl;
	for(int coloursetIndex=1; coloursetIndex<=local_connectome_visualisation_filename_direction_flow_number_coloursets; coloursetIndex++)
	{
		string local_connectome_visualisation_filename_base = string(LOCAL_CONNECTOME_VISUALISATION_FILENAME_COLOURSET_PREPEND) + local_connectome_visualisation_filename_direction_flow + SHAREDvars.convertIntToString(coloursetIndex);
		visualiseLocalConnectomeGenerateContent(connectionTypesDerivedFromPresynapticNeuronsOrEMimages, generate2Dvisualisation, localConnectionCSVdatasetNeurons, localConnectionCSVdatasetConnections, local_connectome_visualisation_filename_base, coloursetIndex, false, true, true, false, INT_DEFAULT_VALUE);
	}
	
	#ifdef LOCAL_CONNECTOME_VISUALISATION_LAYERS
	for(int coloursetIndex=1; coloursetIndex<=local_connectome_visualisation_filename_layered_direction_number_coloursets; coloursetIndex++)
	{
		string local_connectome_visualisation_filename_base = string(LOCAL_CONNECTOME_VISUALISATION_FILENAME_COLOURSET_PREPEND) + local_connectome_visualisation_filename_layered_direction + SHAREDvars.convertIntToString(coloursetIndex);
		visualiseLocalConnectomeGenerateContent(connectionTypesDerivedFromPresynapticNeuronsOrEMimages, generate2Dvisualisation, localConnectionCSVdatasetNeurons, localConnectionCSVdatasetConnections, local_connectome_visualisation_filename_base, coloursetIndex, true, true, false, false, INT_DEFAULT_VALUE);
	}
	for(int coloursetIndex=1; coloursetIndex<=local_connectome_visualisation_filename_layered_direction_flow_number_coloursets; coloursetIndex++)
	{
		string local_connectome_visualisation_filename_base = string(LOCAL_CONNECTOME_VISUALISATION_FILENAME_COLOURSET_PREPEND) + local_connectome_visualisation_filename_layered_direction_flow + SHAREDvars.convertIntToString(coloursetIndex);
		visualiseLocalConnectomeGenerateContent(connectionTypesDerivedFromPresynapticNeuronsOrEMimages, generate2Dvisualisation, localConnectionCSVdatasetNeurons, localConnectionCSVdatasetConnections, local_connectome_visualisation_filename_base, coloursetIndex, true, true, true, false, INT_DEFAULT_VALUE);
	}
			
	//generate visualisations (specific layers; LX);
	cout << "generate visualisations (specific layers; LX);" << endl;
	int numberLayers = CORTICAL_LAYER_NUMBER_OF_LAYERS;
	for(int layerIndex=1; layerIndex<=CORTICAL_LAYER_NUMBER_OF_LAYERS; layerIndex++)
	{
		for(int coloursetIndex=1; coloursetIndex<=local_connectome_visualisation_filename_layered_direction_number_coloursets; coloursetIndex++)
		{
			string local_connectome_visualisation_filename_base = string(LOCAL_CONNECTOME_VISUALISATION_FILENAME_COLOURSET_PREPEND) + local_connectome_visualisation_filename_layered_direction + SHAREDvars.convertIntToString(coloursetIndex) + LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_LAYERED_SPECIFIC_LAYER + SHAREDvars.convertIntToString(layerIndex);
			visualiseLocalConnectomeGenerateContent(connectionTypesDerivedFromPresynapticNeuronsOrEMimages, generate2Dvisualisation, localConnectionCSVdatasetNeurons, localConnectionCSVdatasetConnections, local_connectome_visualisation_filename_base, coloursetIndex, true, true, false, true, layerIndex);
		}
		for(int coloursetIndex=1; coloursetIndex<=local_connectome_visualisation_filename_layered_direction_flow_number_coloursets; coloursetIndex++)
		{
			string local_connectome_visualisation_filename_base = string(LOCAL_CONNECTOME_VISUALISATION_FILENAME_COLOURSET_PREPEND) + local_connectome_visualisation_filename_layered_direction_flow+ SHAREDvars.convertIntToString(coloursetIndex) + LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_LAYERED_SPECIFIC_LAYER + SHAREDvars.convertIntToString(layerIndex);
			visualiseLocalConnectomeGenerateContent(connectionTypesDerivedFromPresynapticNeuronsOrEMimages, generate2Dvisualisation, localConnectionCSVdatasetNeurons, localConnectionCSVdatasetConnections, local_connectome_visualisation_filename_base, coloursetIndex, true, true, true, true, layerIndex);
		}		
	}
	#endif
	
	return result;
}

bool visualiseLocalConnectomeGenerateContent(const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, const bool generate2Dvisualisation, vector<vector<string>>* localConnectionCSVdatasetNeurons, vector<vector<string>>* localConnectionCSVdatasetConnections, const string local_connectome_visualisation_filename_base, const int coloursetNumber, const bool visualiseLayers, const bool visualiseDirection, const bool visualiseFlow, const bool visualiseLayersSpecific, const int layerIndexVisualise)
{
	bool result = true;
		
	//neuron data
	string localConnectomeVisualisationContentsPart4 = "";
	if(generate2Dvisualisation)
	{	
		localConnectomeVisualisationContentsPart4 = localConnectomeVisualisationContentsPart4 + LOCAL_CONNECTOME_VISUALISATION_NEURONS_SVG_HEADER_TEXT + STRING_NEWLINE;
	}
	#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
	else
	{
		localConnectomeVisualisationContentsPart4 = localConnectomeVisualisationContentsPart4 + LOCAL_CONNECTOME_VISUALISATION_NEURONS_LDR_HEADER_TEXT + STRING_NEWLINE;
	}
	#endif
	for(int i=0;i<localConnectionCSVdatasetNeurons->size(); i++)
	{
		vector<string> localConnectionCSVdatasetNeuron = (*localConnectionCSVdatasetNeurons)[i];
		//cout << "localConnectionCSVdatasetNeurons: i = " << i << endl;
		
		int excitationType = SHAREDvars.convertStringToInt(localConnectionCSVdatasetNeuron[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE]);
		//cout << "excitationType = " << excitationType << endl;
		string neuronTypeStringSVG = ""; 
		#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
		string neuronTypeStringLDR = "";
		#endif
		if(excitationType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY)
		{
			neuronTypeStringSVG = LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOURSET_REFERENCE_EXCITATION_TYPE_EXCITATORY;
			#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
			neuronTypeStringLDR = SHAREDvars.convertIntToString(LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITATION_TYPE_EXCITATORY);
			#endif
		}
		else if(excitationType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY)
		{
			neuronTypeStringSVG = LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOURSET_REFERENCE_EXCITATION_TYPE_INHIBITORY;
			#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
			neuronTypeStringLDR = SHAREDvars.convertIntToString(LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITATION_TYPE_INHIBITORY);
			#endif
		}
		else if(excitationType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN)
		{
			cout << "visualiseLocalConnectomeGenerateContent warning; localConnectionCSVdatasetNeuron excitationType LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN; " << excitationType << endl;
			neuronTypeStringSVG = LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOURSET_REFERENCE_EXCITATION_TYPE_UNKNOWN;
			#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
			neuronTypeStringLDR = SHAREDvars.convertIntToString(LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITATION_TYPE_UNKNOWN);
			#endif
		}
		else
		{
			cerr << "visualiseLocalConnectomeGenerateContent error; localConnectionCSVdatasetNeuron excitationType ?; " << excitationType << endl;
			exit(EXIT_ERROR);
		}
		
		#ifdef LOCAL_CONNECTOME_VISUALISATION_LAYERS
		bool passLayerVisualisationChecks = true;
		int layerIndex = INT_DEFAULT_VALUE;
		if(visualiseLayers)
		{
			layerIndex = SHAREDvars.convertStringToInt(localConnectionCSVdatasetNeuron[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER]);
			neuronTypeStringSVG = neuronTypeStringSVG + LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOURSET_REFERENCE_LAYER + SHAREDvars.convertIntToString(layerIndex);
			
			if(visualiseLayersSpecific)
			{
				passLayerVisualisationChecks = false;
				if(layerIndex == layerIndexVisualise)
				{
					passLayerVisualisationChecks = true;	
				}
			}
		}
		if(passLayerVisualisationChecks)
		{
		#endif

			#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
			//special case (hard coded); colour by layer
			if(!generate2Dvisualisation)
			{	
				if(!visualiseFlow)
				{
					if(visualiseLayers)
					{
						if(coloursetNumber == 1)
						{
							if(excitationType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY)
							{
								neuronTypeStringLDR = SHAREDvars.convertIntToString(local_connectome_visualisation_neurons_colour_exciteneuron_layer_array[layerIndex]);
							}
							else if(excitationType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY)
							{
								neuronTypeStringLDR = SHAREDvars.convertIntToString(local_connectome_visualisation_neurons_colour_inhibitneuron_layer_array[layerIndex]);
							}
						}
					}			
				}
			}
			#endif
		
			neuronTypeStringSVG = neuronTypeStringSVG + LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOURSET_REFERENCE_APPEND;
			
			double xCalibrated = calibrateCoordinateX(SHAREDvars.convertStringToDouble(localConnectionCSVdatasetNeuron[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_X]));
			double yCalibrated = calibrateCoordinateY(SHAREDvars.convertStringToDouble(localConnectionCSVdatasetNeuron[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Y]));
			#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
			double zCalibrated = calibrateCoordinateZ(SHAREDvars.convertStringToDouble(localConnectionCSVdatasetNeuron[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Z]));
			#endif

			string localConnectionVisualisationNeuronText = "";
			if(generate2Dvisualisation)
			{
				localConnectionVisualisationNeuronText = localConnectionVisualisationNeuronText + "     <use style='fill:url(#" + neuronTypeStringSVG + ");stroke:url(#" + neuronTypeStringSVG + ");' x='" + convertDoubleToString(xCalibrated) + "' xlink:href='#m51ca76320d' y='" + convertDoubleToString(yCalibrated) + "'/>" + STRING_NEWLINE;
				//eg <use style='fill:url(#exciteNeuronColor);stroke:url(#exciteNeuronColor);' x='2959.68672' xlink:href='#m51ca76320d' y='1597.1004'/>
			}
			#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
			else
			{
				localConnectionVisualisationNeuronText = localConnectionVisualisationNeuronText + SHAREDvars.convertIntToString(LOCAL_CONNECTOME_VISUALISATION_NEURONS_LDR_REFERENCE_TYPE) + LDR_DELIMITER + neuronTypeStringLDR + LDR_DELIMITER + convertDoubleToString(xCalibrated) + LDR_DELIMITER + convertDoubleToString(yCalibrated) + LDR_DELIMITER + convertDoubleToString(zCalibrated) + LDR_DELIMITER + LOCAL_CONNECTOME_VISUALISATION_NEURONS_LDR_MODEL_TRANSFORMATION_MATRIX + LDR_DELIMITER + LOCAL_CONNECTOME_VISUALISATION_NEURONS_LDR_MODEL_FILE_NAME + STRING_NEWLINE;
				//eg 1 2 2856.87336 1772.75136 3.80928 1.0 0.0 0.0 0.0 1.0 0.0 0.0 0.0 1.0 4-4CUBE.DAT
			}
			#endif

			//cout << "localConnectionVisualisationNeuronText = " << localConnectionVisualisationNeuronText << endl;

			localConnectomeVisualisationContentsPart4 = localConnectomeVisualisationContentsPart4 + localConnectionVisualisationNeuronText;
		
		#ifdef LOCAL_CONNECTOME_VISUALISATION_LAYERS
		}
		#endif
	}
	
	

	//connection data			
	string localConnectomeVisualisationContentsPart6 = "";
	if(generate2Dvisualisation)
	{	
		localConnectomeVisualisationContentsPart6 = localConnectomeVisualisationContentsPart6 + LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_SVG_HEADER_TEXT + STRING_NEWLINE;
	}
	#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
	else
	{
		localConnectomeVisualisationContentsPart6 = localConnectomeVisualisationContentsPart6 + LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_LDR_HEADER_TEXT + STRING_NEWLINE;
	}
	#endif
	for(int i=0;i<localConnectionCSVdatasetConnections->size(); i++)
	{
		//cout << "localConnectionCSVdatasetConnections: i = " << i << endl;

		vector<string> localConnectionCSVdatasetConnection = (*localConnectionCSVdatasetConnections)[i];
			
		int excitationType = SHAREDvars.convertStringToInt(localConnectionCSVdatasetConnection[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE]);
		string connectionTypeStringSVG = ""; 
		string connectionTypeStringLDR = "";
		if(excitationType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY)
		{
			connectionTypeStringSVG = connectionTypeStringSVG + LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_COLOURSET_REFERENCE_EXCITATION_TYPE_EXCITATORY;
			#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
			connectionTypeStringLDR = SHAREDvars.convertIntToString(LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_COLOUR_EXCITATION_TYPE_EXCITATORY);	
			#endif
		}
		else if(excitationType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY)
		{
			connectionTypeStringSVG = connectionTypeStringSVG + LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_COLOURSET_REFERENCE_EXCITATION_TYPE_INHIBITORY;
			#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
			connectionTypeStringLDR = SHAREDvars.convertIntToString(LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_COLOUR_EXCITATION_TYPE_INHIBITORY);
			#endif
		}
		else
		{
			cerr << "visualiseLocalConnectomeGenerateContent error; localConnectionCSVdatasetConnection excitationType unknown = " << excitationType << endl;
		}

		#ifdef LOCAL_CONNECTOME_VISUALISATION_LAYERS
		bool passLayerVisualisationChecks = true;
		int layerIndex = INT_DEFAULT_VALUE;
		if(visualiseLayers)
		{
			layerIndex = SHAREDvars.convertStringToInt(localConnectionCSVdatasetConnection[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_ARTIFICIAL_LAYER]);
			connectionTypeStringSVG = connectionTypeStringSVG + LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_COLOURSET_REFERENCE_LAYER + SHAREDvars.convertIntToString(layerIndex);
						
			if(visualiseLayersSpecific)
			{
				passLayerVisualisationChecks = false;
				if(layerIndex == layerIndexVisualise)
				{
					passLayerVisualisationChecks = true;	
				}
			}
		}
		if(passLayerVisualisationChecks)
		{
		#endif

			double xCalibratedPre = calibrateCoordinateX(SHAREDvars.convertStringToDouble(localConnectionCSVdatasetConnection[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_X]));
			double yCalibratedPre = calibrateCoordinateY(SHAREDvars.convertStringToDouble(localConnectionCSVdatasetConnection[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_Y]));
			double zCalibratedPre = calibrateCoordinateZ(SHAREDvars.convertStringToDouble(localConnectionCSVdatasetConnection[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_Z]));
			double xCalibratedPost = calibrateCoordinateX(SHAREDvars.convertStringToDouble(localConnectionCSVdatasetConnection[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_X]));
			double yCalibratedPost = calibrateCoordinateY(SHAREDvars.convertStringToDouble(localConnectionCSVdatasetConnection[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_Y]));
			double zCalibratedPost = calibrateCoordinateZ(SHAREDvars.convertStringToDouble(localConnectionCSVdatasetConnection[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_Z]));		

			int connectionRadialGradientType = calculateConnectionRadialGradientType(xCalibratedPre, xCalibratedPost);	
			if(connectionRadialGradientType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_RADIALGRADIENT_TYPE_POSITIVE)
			{
				connectionTypeStringSVG = connectionTypeStringSVG + LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_COLOURSET_REFERENCE_RADIALGRADIENT_TYPE_POSITIVE;
			}
			else if(connectionRadialGradientType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_RADIALGRADIENT_TYPE_NEGATIVE)
			{
				connectionTypeStringSVG = connectionTypeStringSVG + LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_COLOURSET_REFERENCE_RADIALGRADIENT_TYPE_NEGATIVE;
			}
			
			int connectionFlowType = INT_DEFAULT_VALUE;
			if(visualiseFlow)
			{
				connectionFlowType = calculateConnectionFlowType(xCalibratedPre, yCalibratedPre, xCalibratedPost, yCalibratedPost);	
				if(connectionFlowType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_FLOW_TYPE_POSITIVE)
				{
					connectionTypeStringSVG = connectionTypeStringSVG + LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_COLOURSET_REFERENCE_FLOW_TYPE_POSITIVE;
				}
				else if(connectionFlowType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_FLOW_TYPE_NEGATIVE)
				{
					connectionTypeStringSVG = connectionTypeStringSVG + LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_COLOURSET_REFERENCE_FLOW_TYPE_NEGATIVE;
				}
			}
	
			connectionTypeStringSVG = connectionTypeStringSVG + LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_COLOURSET_REFERENCE_APPEND;

			#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
			if(!generate2Dvisualisation)
			{	
				//LDR format doesn't support (line) radial colour gradients; so connection direction will be ignored (not rendered)
				if(visualiseFlow)
				{
					if(excitationType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY)
					{
						if(connectionFlowType)
						{
							connectionTypeStringLDR = SHAREDvars.convertIntToString(LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_COLOUR_EXCITATION_TYPE_EXCITATORY_FLOW_TYPE_POSITIVE);
						}
						else
						{
							connectionTypeStringLDR = SHAREDvars.convertIntToString(LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_COLOUR_EXCITATION_TYPE_EXCITATORY_FLOW_TYPE_NEGATIVE);
						}
					}
					else if(excitationType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY)
					{
						if(connectionFlowType)
						{
							connectionTypeStringLDR = SHAREDvars.convertIntToString(LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_COLOUR_EXCITATION_TYPE_INHIBITORY_FLOW_TYPE_POSITIVE);
						}
						else
						{
							connectionTypeStringLDR = SHAREDvars.convertIntToString(LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_COLOUR_EXCITATION_TYPE_INHIBITORY_FLOW_TYPE_NEGATIVE);
						}
					}
					//cout << "connectionTypeStringLDR = " << connectionTypeStringLDR << endl;
				}
			}
			#endif
			#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
			//special case (hard coded); colour by layer
			if(!generate2Dvisualisation)
			{	
				if(!visualiseFlow)
				{
					if(visualiseLayers)
					{
						if(coloursetNumber == 1)
						{
							if(excitationType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY)
							{
								connectionTypeStringLDR = SHAREDvars.convertIntToString(local_connectome_visualisation_neurons_colour_exciteneuron_layer_array[layerIndex]);
							}
							else if(excitationType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY)
							{
								connectionTypeStringLDR = SHAREDvars.convertIntToString(local_connectome_visualisation_neurons_colour_inhibitneuron_layer_array[layerIndex]);
							}
						}
					}			
				}
			}
			#endif

			string localConnectionVisualisationConnectionText = "";
			if(generate2Dvisualisation)
			{
				localConnectionVisualisationConnectionText = localConnectionVisualisationConnectionText + "     <path clip-path='url(#p9a78a7c8e7)' d='M " + convertDoubleToString(xCalibratedPre) + " " + convertDoubleToString(yCalibratedPre) + "  L " + convertDoubleToString(xCalibratedPost) + " " + convertDoubleToString(yCalibratedPost) + "  ' style='fill:none;stroke:url(#" + connectionTypeStringSVG + ");stroke-opacity:0.75;stroke-width:0.25;'/>" + STRING_NEWLINE;
				//eg     <path clip-path='url(#p9a78a7c8e7)' d='M 2959.68672 1597.1004  L 2856.87336 1772.75136  ' style='fill:none;stroke:url(#exciteConnectionRadialGradient1FlowDirection1);stroke-opacity:0.75;stroke-width:0.25;'/>
			}
			#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
			else
			{
				localConnectionVisualisationConnectionText = localConnectionVisualisationConnectionText + SHAREDvars.convertIntToString(LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_LDR_REFERENCE_TYPE) + LDR_DELIMITER + connectionTypeStringLDR + LDR_DELIMITER + convertDoubleToString(xCalibratedPre) + LDR_DELIMITER + convertDoubleToString(yCalibratedPre) + LDR_DELIMITER + convertDoubleToString(zCalibratedPre) + LDR_DELIMITER + convertDoubleToString(xCalibratedPost) + LDR_DELIMITER + convertDoubleToString(yCalibratedPost) + LDR_DELIMITER + convertDoubleToString(zCalibratedPost) + STRING_NEWLINE;
				//eg 2 2 2959.68672 1597.1004 0 2856.87336 1772.75136 3.80928
			}
			#endif
			
			//cout << "localConnectionVisualisationConnectionText = " << localConnectionVisualisationConnectionText << endl;

			localConnectomeVisualisationContentsPart6 = localConnectomeVisualisationContentsPart6 + localConnectionVisualisationConnectionText;

		#ifdef LOCAL_CONNECTOME_VISUALISATION_LAYERS
		}
		#endif
				
	}
	
	
	
	//other data
	SHAREDvars.setCurrentDirectory(currentDirectory);	//assume LOCAL_CONNECTOME_VISUALISATION_SVG/LDR_PART_IMPORT_FILENAME_PREPEND content resides in current folder
	string localConnectomeVisualisationContents = "";
	if(generate2Dvisualisation)
	{		
		string localConnectomeVisualisationContentsPart1FileName = string(LOCAL_CONNECTOME_VISUALISATION_SVG_PART_IMPORT_FILENAME_PREPEND) + SHAREDvars.convertIntToString(1) + SVG_FILE_EXTENSION; 
		string localConnectomeVisualisationContentsPart3FileName = string(LOCAL_CONNECTOME_VISUALISATION_SVG_PART_IMPORT_FILENAME_PREPEND) + SHAREDvars.convertIntToString(3) + SVG_FILE_EXTENSION; 
		string localConnectomeVisualisationContentsPart5FileName = string(LOCAL_CONNECTOME_VISUALISATION_SVG_PART_IMPORT_FILENAME_PREPEND) + SHAREDvars.convertIntToString(5) + SVG_FILE_EXTENSION; 
		string localConnectomeVisualisationContentsPart7FileName = string(LOCAL_CONNECTOME_VISUALISATION_SVG_PART_IMPORT_FILENAME_PREPEND) + SHAREDvars.convertIntToString(7) + SVG_FILE_EXTENSION; 
		string localConnectomeVisualisationContentsPart1 = SHAREDvars.getFileContents(localConnectomeVisualisationContentsPart1FileName);
		string localConnectomeVisualisationContentsPart3 = SHAREDvars.getFileContents(localConnectomeVisualisationContentsPart3FileName);
		string localConnectomeVisualisationContentsPart5 = SHAREDvars.getFileContents(localConnectomeVisualisationContentsPart5FileName);
		string localConnectomeVisualisationContentsPart7 = SHAREDvars.getFileContents(localConnectomeVisualisationContentsPart7FileName);
		
		//colour set data
		string localConnectomeVisualisationContentsPart2FileName = "";
		#ifdef LOCAL_CONNECTOME_VISUALISATION_LAYERS
		if(visualiseLayers)
		{
			if(visualiseDirection)
			{
				if(visualiseFlow)
				{
					localConnectomeVisualisationContentsPart2FileName = localConnectomeVisualisationContentsPart2FileName + LOCAL_CONNECTOME_VISUALISATION_SVG_PART_IMPORT_FILENAME_PREPEND + SHAREDvars.convertIntToString(2) + LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_LAYERED_DIRECTION_FLOW + SHAREDvars.convertIntToString(coloursetNumber) + SVG_FILE_EXTENSION;
				}
				else
				{
					localConnectomeVisualisationContentsPart2FileName = localConnectomeVisualisationContentsPart2FileName + LOCAL_CONNECTOME_VISUALISATION_SVG_PART_IMPORT_FILENAME_PREPEND + SHAREDvars.convertIntToString(2) + LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_LAYERED_DIRECTION + SHAREDvars.convertIntToString(coloursetNumber) + SVG_FILE_EXTENSION;
				}
			}
			else
			{
				cerr << "visualiseDirection generation currently required (information can be disgarded by colourset)" << endl;
				exit(EXIT_ERROR);
			}
		}
		else
		{
		#endif
			if(visualiseDirection)
			{
				if(visualiseFlow)
				{
					localConnectomeVisualisationContentsPart2FileName = localConnectomeVisualisationContentsPart2FileName + LOCAL_CONNECTOME_VISUALISATION_SVG_PART_IMPORT_FILENAME_PREPEND + SHAREDvars.convertIntToString(2) + LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_DIRECTION_FLOW + SHAREDvars.convertIntToString(coloursetNumber) + SVG_FILE_EXTENSION;
				}
				else
				{
					localConnectomeVisualisationContentsPart2FileName = localConnectomeVisualisationContentsPart2FileName + LOCAL_CONNECTOME_VISUALISATION_SVG_PART_IMPORT_FILENAME_PREPEND + SHAREDvars.convertIntToString(2) + LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_DIRECTION + SHAREDvars.convertIntToString(coloursetNumber) + SVG_FILE_EXTENSION;
				}
			}
			else
			{
				cerr << "visualiseDirection generation currently required (information can be disgarded by colourset)" << endl;
				exit(EXIT_ERROR);
			}
		#ifdef LOCAL_CONNECTOME_VISUALISATION_LAYERS
		}
		#endif
		cout << "localConnectomeVisualisationContentsPart2FileName = " << localConnectomeVisualisationContentsPart2FileName << endl;
		
		string localConnectomeVisualisationContentsPart2 = SHAREDvars.getFileContents(localConnectomeVisualisationContentsPart2FileName);

		localConnectomeVisualisationContents = localConnectomeVisualisationContents + localConnectomeVisualisationContentsPart1 + localConnectomeVisualisationContentsPart2 + localConnectomeVisualisationContentsPart3 + localConnectomeVisualisationContentsPart4 + localConnectomeVisualisationContentsPart5 + localConnectomeVisualisationContentsPart6 + localConnectomeVisualisationContentsPart7;		
	}
	#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
	else
	{
		string localConnectomeVisualisationContentsPart1FileName = LOCAL_CONNECTOME_VISUALISATION_LDR_PART_IMPORT_FILENAME_PREPEND + SHAREDvars.convertIntToString(1) + LDR_FILE_EXTENSION; 
		string localConnectomeVisualisationContentsPart1 = SHAREDvars.getFileContents(localConnectomeVisualisationContentsPart1FileName);
		
		localConnectomeVisualisationContents = localConnectomeVisualisationContents + localConnectomeVisualisationContentsPart1 + localConnectomeVisualisationContentsPart4 + localConnectomeVisualisationContentsPart6;	
	}
	#endif
	
	visualiseLocalConnectomeCSVdatasetWriteToFile(connectionTypesDerivedFromPresynapticNeuronsOrEMimages, generate2Dvisualisation, local_connectome_visualisation_filename_base, &localConnectomeVisualisationContents);

	return result;
}		

bool visualiseLocalConnectomeCSVdatasetWriteToFile(const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, const bool generate2Dvisualisation, const string local_connectome_visualisation_filename_base, string* localConnectomeVisualisationContents)
{
	bool result = true;
	
	#ifdef INDEXED_CSV_DATABASE_VISUALISE_LOCAL_CONNECTOME_USE_RELATIVE_FOLDER
	SHAREDvars.setCurrentDirectory(currentDirectory);
	#endif
	string localConnectomeVisualisationFolder = LOCAL_CONNECTOME_FOLDER_BASE;
	SHAREDvars.setCurrentDirectory(localConnectomeVisualisationFolder);

	localConnectomeVisualisationFolder = LOCAL_CONNECTOME_VISUALISATION_FOLDER;
	SHAREDvars.setCurrentDirectory(localConnectomeVisualisationFolder);	
		
	if(connectionTypesDerivedFromPresynapticNeuronsOrEMimages)
	{
		localConnectomeVisualisationFolder = LOCAL_CONNECTOME_VISUALISATION_FOLDER_CONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS;
		SHAREDvars.setCurrentDirectory(localConnectomeVisualisationFolder);	
	}
	else
	{
		localConnectomeVisualisationFolder = LOCAL_CONNECTOME_VISUALISATION_FOLDER_CONNECTION_TYPES_DERIVED_FROM_EM_IMAGES;
		SHAREDvars.setCurrentDirectory(localConnectomeVisualisationFolder);	
	}
		
	if(generate2Dvisualisation)
	{
		localConnectomeVisualisationFolder = LOCAL_CONNECTOME_VISUALISATION_FOLDER_2D;
		SHAREDvars.setCurrentDirectory(localConnectomeVisualisationFolder);	
	}
	else
	{
		localConnectomeVisualisationFolder = LOCAL_CONNECTOME_VISUALISATION_FOLDER_3D;
		SHAREDvars.setCurrentDirectory(localConnectomeVisualisationFolder);	
	}
	
	
	string local_connectome_visualisation_filename = "";
	if(generate2Dvisualisation)
	{
		local_connectome_visualisation_filename = local_connectome_visualisation_filename_base + SVG_FILE_EXTENSION;
	}
	else
	{
		local_connectome_visualisation_filename = local_connectome_visualisation_filename_base + LDR_FILE_EXTENSION;
	}

	SHAREDvars.writeStringToFile(local_connectome_visualisation_filename, localConnectomeVisualisationContents);	//inefficient?
	
	return result;
}		


int calculateConnectionFlowType(const double xCalibratedPre, const double yCalibratedPre, const double xCalibratedPost, const double yCalibratedPost)
{
	double connectionFlowVectorX = xCalibratedPost - xCalibratedPre;
	double connectionFlowVectorY = yCalibratedPost - yCalibratedPre;
	double dotProduct = connectionFlowVectorX*LOCAL_CONNECTOME_VISUALISATION_POSITIVE_FLOW_VECTOR_X + connectionFlowVectorY*LOCAL_CONNECTOME_VISUALISATION_POSITIVE_FLOW_VECTOR_Y;
	int connectionFlowType;
	if(dotProduct > 0)
	{
		connectionFlowType = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_FLOW_TYPE_POSITIVE;
	}
	else
	{
		connectionFlowType = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_FLOW_TYPE_NEGATIVE;
	}
	return connectionFlowType;
}

int calculateConnectionRadialGradientType(const double xCalibratedPre, const double xCalibratedPost)
{
	int connectionRadialGradientType;
	if(xCalibratedPre > xCalibratedPost)
	{
		connectionRadialGradientType = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_RADIALGRADIENT_TYPE_NEGATIVE;
	}
	else
	{
		connectionRadialGradientType = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_RADIALGRADIENT_TYPE_POSITIVE;
	}
	return connectionRadialGradientType;
}

string convertDoubleToString(const double value)
{	
	const string doubleFormat = "%0." + SHAREDvars.convertIntToString(LOCAL_CONNECTOME_VISUALISATION_CALIBRATION_DOUBLE_PRECISION) + "f";	//"%0.5f";
	string doubleString = SHAREDvars.convertDoubleToString(value, doubleFormat);
	return doubleString;
}

#endif
