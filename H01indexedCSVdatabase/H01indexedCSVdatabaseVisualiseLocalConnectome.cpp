/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseVisualiseLocalConnectome.cpp
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

#include "H01indexedCSVdatabaseVisualiseLocalConnectome.hpp"

#ifdef INDEXED_CSV_DATABASE_VISUALISE_LOCAL_CONNECTOME

static int local_connectome_visualisation_neurons_colour_exciteneuron_layer_array[CORTICAL_LAYER_NUMBER_OF_LAYERS] = {LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITENEURON_LAYER1,LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITENEURON_LAYER2,LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITENEURON_LAYER3,LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITENEURON_LAYER4,LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITENEURON_LAYER5,LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITENEURON_LAYER6,LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITENEURON_LAYER7};
static int local_connectome_visualisation_neurons_colour_inhibitneuron_layer_array[CORTICAL_LAYER_NUMBER_OF_LAYERS] = {LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_INHIBITNEURON_LAYER1,LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_INHIBITNEURON_LAYER2,LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_INHIBITNEURON_LAYER3,LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_INHIBITNEURON_LAYER4,LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_INHIBITNEURON_LAYER5,LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_INHIBITNEURON_LAYER6,LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_INHIBITNEURON_LAYER7};

bool H01indexedCSVdatabaseVisualiseLocalConnectomeClass::visualiseLocalConnectomeCSVdataset(const string local_connectome_folder_base)
{
	bool result = true;
	
	currentDirectory = SHAREDvars.getCurrentDirectory();
	
	this->visualiseLocalConnectomeCSVdataset(local_connectome_folder_base, true);
	this->visualiseLocalConnectomeCSVdataset(local_connectome_folder_base, false);
	
	return result;
}

bool H01indexedCSVdatabaseVisualiseLocalConnectomeClass::visualiseLocalConnectomeCSVdataset(const string local_connectome_folder_base, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages)
{
	bool result = true;
	
	cout << "connectionTypesDerivedFromPresynapticNeuronsOrEMimages = " << connectionTypesDerivedFromPresynapticNeuronsOrEMimages << endl;
	
	#ifdef LOCAL_CONNECTOME_FOLDER_BASE_USE_RELATIVE_FOLDER
	SHAREDvars.setCurrentDirectory(currentDirectory);
	#endif
	string localConnectomeCSVdatabaseFolder = LOCAL_CONNECTOME_FOLDER_BASE;
	SHAREDvars.setCurrentDirectory(localConnectomeCSVdatabaseFolder);
	localConnectomeCSVdatabaseFolder = LOCAL_CONNECTOME_DATASET_FOLDER;
	SHAREDvars.setCurrentDirectory(localConnectomeCSVdatabaseFolder);

	vector<vector<string>> localConnectionCSVdatasetNeurons;
	int localConnectionCSVdatasetNeuronsSize = 0;
	SHAREDvars.getLinesFromFileCSV(LOCAL_CONNECTOME_DATASET_NEURONS_FILENAME, &localConnectionCSVdatasetNeurons, &localConnectionCSVdatasetNeuronsSize, CSV_DELIMITER_CHAR, true);
	
	vector<vector<string>> localConnectionCSVdatasetConnections;
	int localConnectionCSVdatasetConnectionsSize = 0;
	if(connectionTypesDerivedFromPresynapticNeuronsOrEMimages)
	{
		SHAREDvars.getLinesFromFileCSV(LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, &localConnectionCSVdatasetConnections, &localConnectionCSVdatasetConnectionsSize, CSV_DELIMITER_CHAR, true);
	}
	else
	{
		SHAREDvars.getLinesFromFileCSV(LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, &localConnectionCSVdatasetConnections, &localConnectionCSVdatasetConnectionsSize, CSV_DELIMITER_CHAR, true);
	}

	#ifdef LOCAL_CONNECTOME_VISUALISATION_LAYERS
	//int corticalLayersNumKeypointsMax;	//= 28	//maximum number keypoints (number cols/2)
	const string corticalLayersBoundaryKeypointTableFileName = CORTICAL_LAYER_BOUNDARY_KEYPOINT_TABLE_FILE_NAME;
	vector<vector<vec>> corticalLayersKeypoints;
	H01indexedCSVdatabaseCalculateNeuronLayer.readCorticalLayersBoundaryKeypointTable(corticalLayersBoundaryKeypointTableFileName, &corticalLayersKeypoints);
	H01indexedCSVdatabaseCalculateNeuronLayer.calculateNeuronLayers(true, &localConnectionCSVdatasetNeurons, &corticalLayersKeypoints);
	H01indexedCSVdatabaseCalculateNeuronLayer.calculateNeuronLayers(false, &localConnectionCSVdatasetConnections, &corticalLayersKeypoints);
	#endif
		
	bool visualiseTrace = false;
	int traceIterationIndexNOTUSED = INT_DEFAULT_VALUE;
	
	#ifdef LOCAL_CONNECTOME_VISUALISATION_2D_SVG
	this->visualiseLocalConnectomeCSVdataset(local_connectome_folder_base, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, true, &localConnectionCSVdatasetNeurons, &localConnectionCSVdatasetConnections, visualiseTrace, traceIterationIndexNOTUSED);
	#endif
	#ifdef LOCAL_CONNECTOME_VISUALISATION_3D_LDR
	this->visualiseLocalConnectomeCSVdataset(local_connectome_folder_base, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, false, &localConnectionCSVdatasetNeurons, &localConnectionCSVdatasetConnections, visualiseTrace, traceIterationIndexNOTUSED);
	#endif

	return result;
}

bool H01indexedCSVdatabaseVisualiseLocalConnectomeClass::visualiseLocalConnectomeCSVdataset(const string local_connectome_folder_base, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, const bool generate2Dvisualisation, vector<vector<string>>* localConnectionCSVdatasetNeurons, vector<vector<string>>* localConnectionCSVdatasetConnections, const bool visualiseTrace, const int traceIterationIndex)
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

	cout << "coloursets for which visualisations are being generated;" << endl;		//visualiseTrace coloursets not (yet) defined
	cout << "local_connectome_visualisation_filename_direction_number_coloursets = " << local_connectome_visualisation_filename_direction_number_coloursets << endl;
	if(!visualiseTrace)	//visualiseTrace coloursets not (yet) defined
	{
		cout << "local_connectome_visualisation_filename_direction_flow_number_coloursets = " << local_connectome_visualisation_filename_direction_flow_number_coloursets << endl;
		#ifdef LOCAL_CONNECTOME_VISUALISATION_LAYERS
		cout << "local_connectome_visualisation_filename_layered_direction_number_coloursets = " << local_connectome_visualisation_filename_layered_direction_number_coloursets << endl;
		cout << "local_connectome_visualisation_filename_layered_direction_flow_number_coloursets = " << local_connectome_visualisation_filename_layered_direction_flow_number_coloursets << endl;
		#endif
	}
		
	//generate visualisations;
	for(int coloursetIndex=1; coloursetIndex<=local_connectome_visualisation_filename_direction_number_coloursets; coloursetIndex++)
	{
		string local_connectome_visualisation_filename_base = string(LOCAL_CONNECTOME_VISUALISATION_FILENAME_COLOURSET_PREPEND) + local_connectome_visualisation_filename_direction + SHAREDvars.convertIntToString(coloursetIndex);
		this->visualiseLocalConnectomeGenerateContent(local_connectome_folder_base, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, generate2Dvisualisation, localConnectionCSVdatasetNeurons, localConnectionCSVdatasetConnections, local_connectome_visualisation_filename_base, coloursetIndex, false, true, false, false, INT_DEFAULT_VALUE, visualiseTrace, traceIterationIndex);
	}
	
	if(!visualiseTrace)	//visualiseTrace coloursets not (yet) defined
	{
		for(int coloursetIndex=1; coloursetIndex<=local_connectome_visualisation_filename_direction_flow_number_coloursets; coloursetIndex++)
		{
			string local_connectome_visualisation_filename_base = string(LOCAL_CONNECTOME_VISUALISATION_FILENAME_COLOURSET_PREPEND) + local_connectome_visualisation_filename_direction_flow + SHAREDvars.convertIntToString(coloursetIndex);
			this->visualiseLocalConnectomeGenerateContent(local_connectome_folder_base, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, generate2Dvisualisation, localConnectionCSVdatasetNeurons, localConnectionCSVdatasetConnections, local_connectome_visualisation_filename_base, coloursetIndex, false, true, true, false, INT_DEFAULT_VALUE, visualiseTrace, traceIterationIndex);
		}
	
		#ifdef LOCAL_CONNECTOME_VISUALISATION_LAYERS
		cout << "generate visualisations (layered);" << endl;
		for(int coloursetIndex=1; coloursetIndex<=local_connectome_visualisation_filename_layered_direction_number_coloursets; coloursetIndex++)
		{
			string local_connectome_visualisation_filename_base = string(LOCAL_CONNECTOME_VISUALISATION_FILENAME_COLOURSET_PREPEND) + local_connectome_visualisation_filename_layered_direction + SHAREDvars.convertIntToString(coloursetIndex);
			this->visualiseLocalConnectomeGenerateContent(local_connectome_folder_base, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, generate2Dvisualisation, localConnectionCSVdatasetNeurons, localConnectionCSVdatasetConnections, local_connectome_visualisation_filename_base, coloursetIndex, true, true, false, false, INT_DEFAULT_VALUE, visualiseTrace, traceIterationIndex);
		}
		for(int coloursetIndex=1; coloursetIndex<=local_connectome_visualisation_filename_layered_direction_flow_number_coloursets; coloursetIndex++)
		{
			string local_connectome_visualisation_filename_base = string(LOCAL_CONNECTOME_VISUALISATION_FILENAME_COLOURSET_PREPEND) + local_connectome_visualisation_filename_layered_direction_flow + SHAREDvars.convertIntToString(coloursetIndex);
			this->visualiseLocalConnectomeGenerateContent(local_connectome_folder_base, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, generate2Dvisualisation, localConnectionCSVdatasetNeurons, localConnectionCSVdatasetConnections, local_connectome_visualisation_filename_base, coloursetIndex, true, true, true, false, INT_DEFAULT_VALUE, visualiseTrace, traceIterationIndex);
		}

		//generate visualisations (specific layers; LX);
		cout << "generate visualisations (layered - specific layers; LX);" << endl;
		int numberLayers = CORTICAL_LAYER_NUMBER_OF_LAYERS;
		for(int layerIndex=1; layerIndex<=CORTICAL_LAYER_NUMBER_OF_LAYERS; layerIndex++)
		{
			for(int coloursetIndex=1; coloursetIndex<=local_connectome_visualisation_filename_layered_direction_number_coloursets; coloursetIndex++)
			{
				string local_connectome_visualisation_filename_base = string(LOCAL_CONNECTOME_VISUALISATION_FILENAME_COLOURSET_PREPEND) + local_connectome_visualisation_filename_layered_direction + SHAREDvars.convertIntToString(coloursetIndex) + LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_LAYERED_SPECIFIC_LAYER + SHAREDvars.convertIntToString(layerIndex);
				this->visualiseLocalConnectomeGenerateContent(local_connectome_folder_base, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, generate2Dvisualisation, localConnectionCSVdatasetNeurons, localConnectionCSVdatasetConnections, local_connectome_visualisation_filename_base, coloursetIndex, true, true, false, true, layerIndex, visualiseTrace, traceIterationIndex);
			}
			for(int coloursetIndex=1; coloursetIndex<=local_connectome_visualisation_filename_layered_direction_flow_number_coloursets; coloursetIndex++)
			{
				string local_connectome_visualisation_filename_base = string(LOCAL_CONNECTOME_VISUALISATION_FILENAME_COLOURSET_PREPEND) + local_connectome_visualisation_filename_layered_direction_flow+ SHAREDvars.convertIntToString(coloursetIndex) + LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_LAYERED_SPECIFIC_LAYER + SHAREDvars.convertIntToString(layerIndex);
				this->visualiseLocalConnectomeGenerateContent(local_connectome_folder_base, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, generate2Dvisualisation, localConnectionCSVdatasetNeurons, localConnectionCSVdatasetConnections, local_connectome_visualisation_filename_base, coloursetIndex, true, true, true, true, layerIndex, visualiseTrace, traceIterationIndex);
			}		
		}
		#endif
	}
	
	return result;
}

bool H01indexedCSVdatabaseVisualiseLocalConnectomeClass::visualiseLocalConnectomeGenerateContent(const string local_connectome_folder_base, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, const bool generate2Dvisualisation, vector<vector<string>>* localConnectionCSVdatasetNeurons, vector<vector<string>>* localConnectionCSVdatasetConnections, const string local_connectome_visualisation_filename_base, const int coloursetNumber, const bool visualiseLayers, const bool visualiseDirection, const bool visualiseFlow, const bool visualiseLayersSpecific, const int layerIndexVisualise, const bool visualiseTrace, const int traceIterationIndex)
{
	bool result = true;
		
	//neuron data;
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
	
		bool passLayerVisualisationChecks = true;

		string neuronTypeStringSVG = ""; 
		#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
		string neuronTypeStringLDR = "";
		#endif
		

		int excitationType = SHAREDvars.convertStringToInt(localConnectionCSVdatasetNeuron[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE]);
		//cout << "excitationType = " << excitationType << endl;
		if(excitationType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY)
		{
			neuronTypeStringSVG = neuronTypeStringSVG + LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOURSET_REFERENCE_EXCITATION_TYPE_EXCITATORY;
			#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
			neuronTypeStringLDR = neuronTypeStringLDR + SHAREDvars.convertIntToString(LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITATION_TYPE_EXCITATORY);
			#endif
		}
		else if(excitationType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY)
		{
			neuronTypeStringSVG = neuronTypeStringSVG + LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOURSET_REFERENCE_EXCITATION_TYPE_INHIBITORY;
			#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
			neuronTypeStringLDR = neuronTypeStringLDR + SHAREDvars.convertIntToString(LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITATION_TYPE_INHIBITORY);
			#endif
		}
		else if(excitationType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN)
		{
			cout << "visualiseLocalConnectomeGenerateContent warning; localConnectionCSVdatasetNeuron excitationType LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN; " << excitationType << endl;
			neuronTypeStringSVG = neuronTypeStringSVG + LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOURSET_REFERENCE_EXCITATION_TYPE_UNKNOWN;
			#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
			neuronTypeStringLDR = neuronTypeStringLDR + SHAREDvars.convertIntToString(LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITATION_TYPE_UNKNOWN);
			#endif
		}
		else
		{
			cerr << "visualiseLocalConnectomeGenerateContent error; localConnectionCSVdatasetNeuron excitationType ?; " << excitationType << endl;
			exit(EXIT_ERROR);
		}

		#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME
		bool traceActiveNeuron = false;
		if(visualiseTrace)
		{
			int neuronTraceHighlightValue = SHAREDvars.convertStringToInt(localConnectionCSVdatasetNeuron[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_TRACE_HIGHLIGHT]);
			if(neuronTraceHighlightValue == INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_HIGHLIGHT_VALUE_NEURON_SOURCE)
			{
				#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_COLOUR_ACTIVE_NEURONS
				neuronTypeStringSVG = string(LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_TRACE_ACTIVE) + neuronTypeStringSVG;
				#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
				neuronTypeStringLDR = SHAREDvars.convertIntToString(LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_HIGHLIGHT_VALUE_NEURON_SOURCE);
				#endif
				#endif
				#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_HIDE_INACTIVE_NEURONS
				passLayerVisualisationChecks = true;
				#endif
				traceActiveNeuron = true;
			}
			else if(neuronTraceHighlightValue == INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_HIGHLIGHT_VALUE_NEURON_TARGET)
			{
				#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_COLOUR_ACTIVE_NEURONS
				neuronTypeStringSVG = string(LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_TRACE_ACTIVE) + neuronTypeStringSVG;
				#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
				neuronTypeStringLDR = SHAREDvars.convertIntToString(LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_HIGHLIGHT_VALUE_NEURON_TARGET);
				#endif
				#endif
				#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_HIDE_INACTIVE_NEURONS
				passLayerVisualisationChecks = true;
				#endif
				traceActiveNeuron = true;
			}
			else if(neuronTraceHighlightValue == INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_HIGHLIGHT_VALUE_NEURON_INACTIVE)
			{
				#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_COLOUR_INACTIVE_NEURONS
				neuronTypeStringSVG = string(LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_TRACE_ACTIVE) + neuronTypeStringSVG;
				#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
				neuronTypeStringLDR = SHAREDvars.convertIntToString(LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_HIGHLIGHT_VALUE_NEURON_OFF);
				#endif
				#endif
				#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_HIDE_INACTIVE_NEURONS
				passLayerVisualisationChecks = false;
				#endif
			}
		}
		#endif
				
		#ifdef LOCAL_CONNECTOME_VISUALISATION_LAYERS
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
		#endif
		if(passLayerVisualisationChecks)
		{

			#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
			//special case (hard coded); colour by layer
			#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_COLOUR_ACTIVE_NEURONS
			if(!traceActiveNeuron)
			{
			#endif
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
									neuronTypeStringLDR = SHAREDvars.convertIntToString(local_connectome_visualisation_neurons_colour_exciteneuron_layer_array[layerIndex-1]);
								}
								else if(excitationType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY)
								{
									neuronTypeStringLDR = SHAREDvars.convertIntToString(local_connectome_visualisation_neurons_colour_inhibitneuron_layer_array[layerIndex-1]);
								}
							}
						}			
					}
				}
			#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_COLOUR_ACTIVE_NEURONS
			}
			#endif
			#endif

		
			neuronTypeStringSVG = neuronTypeStringSVG + LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOURSET_REFERENCE_APPEND;
			
			double xCalibrated = H01indexedCSVdatabaseOperations.calibrateCoordinateX(SHAREDvars.convertStringToDouble(localConnectionCSVdatasetNeuron[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_X]));
			double yCalibrated = H01indexedCSVdatabaseOperations.calibrateCoordinateY(SHAREDvars.convertStringToDouble(localConnectionCSVdatasetNeuron[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Y]));
			#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
			double zCalibrated = H01indexedCSVdatabaseOperations.calibrateCoordinateZ(SHAREDvars.convertStringToDouble(localConnectionCSVdatasetNeuron[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Z]));
			#endif

			string localConnectionVisualisationNeuronText = "";
			if(generate2Dvisualisation)
			{
				localConnectionVisualisationNeuronText = localConnectionVisualisationNeuronText + "     <use style='fill:url(#" + neuronTypeStringSVG + ");stroke:url(#" + neuronTypeStringSVG + ");' x='" + this->convertDoubleToStringCalibrationPrecision(xCalibrated) + "' xlink:href='#m51ca76320d' y='" + this->convertDoubleToStringCalibrationPrecision(yCalibrated) + "'/>" + STRING_NEWLINE;
				//eg <use style='fill:url(#exciteNeuronColor);stroke:url(#exciteNeuronColor);' x='2959.68672' xlink:href='#m51ca76320d' y='1597.1004'/>
			}
			#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
			else
			{
				localConnectionVisualisationNeuronText = localConnectionVisualisationNeuronText + SHAREDvars.convertIntToString(LOCAL_CONNECTOME_VISUALISATION_NEURONS_LDR_REFERENCE_TYPE) + LDR_DELIMITER + neuronTypeStringLDR + LDR_DELIMITER + this->convertDoubleToStringCalibrationPrecision(xCalibrated) + LDR_DELIMITER + this->convertDoubleToStringCalibrationPrecision(yCalibrated) + LDR_DELIMITER + this->convertDoubleToStringCalibrationPrecision(zCalibrated) + LDR_DELIMITER + LOCAL_CONNECTOME_VISUALISATION_NEURONS_LDR_MODEL_TRANSFORMATION_MATRIX + LDR_DELIMITER + LOCAL_CONNECTOME_VISUALISATION_NEURONS_LDR_MODEL_FILE_NAME + STRING_NEWLINE;
				//eg 1 2 2856.87336 1772.75136 3.80928 1.0 0.0 0.0 0.0 1.0 0.0 0.0 0.0 1.0 4-4CUBE.DAT
			}
			#endif

			//cout << "localConnectionVisualisationNeuronText = " << localConnectionVisualisationNeuronText << endl;

			localConnectomeVisualisationContentsPart4 = localConnectomeVisualisationContentsPart4 + localConnectionVisualisationNeuronText;
		
		}
	}
	
	

	//connection data;		
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

		int numberOfSynapses = SHAREDvars.convertStringToInt(localConnectionCSVdatasetConnection[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_SYN_NUM]);
		for(int s=0; s<numberOfSynapses; s++)
		{
			bool passLayerVisualisationChecks = true;

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
			
			#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME
			bool traceActiveConnection = false;
			if(visualiseTrace)
			{
				int connectionTraceHighlightValue = SHAREDvars.convertStringToInt(localConnectionCSVdatasetConnection[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_ARTIFICIAL_TRACE_HIGHLIGHT]);
				if(connectionTraceHighlightValue == INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_HIGHLIGHT_VALUE_CONNECTION_ACTIVE)
				{
					#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_COLOUR_ACTIVE_CONNECTIONS
					connectionTypeStringSVG = string(LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_TRACE_ACTIVE) + connectionTypeStringSVG;
					#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
					connectionTypeStringLDR = SHAREDvars.convertIntToString(LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_HIGHLIGHT_VALUE_NEURON_SOURCE);
					#endif
					#endif
					#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_HIDE_INACTIVE_CONNECTIONS
					passLayerVisualisationChecks = true;
					#endif
					traceActiveConnection = true;
				}
				else if(connectionTraceHighlightValue == INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_HIGHLIGHT_VALUE_CONNECTION_INACTIVE)
				{
					#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_COLOUR_ACTIVE_CONNECTIONS
					connectionTypeStringSVG = string(LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_TRACE_ACTIVE) + connectionTypeStringSVG;
					#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
					connectionTypeStringLDR = SHAREDvars.convertIntToString(LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_COLOUR_HIGHLIGHT_VALUE_CONNECTION_INACTIVE);
					#endif
					#endif
					#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_HIDE_INACTIVE_CONNECTIONS
					passLayerVisualisationChecks = false;
					#endif
					traceActiveConnection = false;
				}
			}
			#endif

			#ifdef LOCAL_CONNECTOME_VISUALISATION_LAYERS
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
			#endif
			if(passLayerVisualisationChecks)
			{

				double xCalibratedPre = H01indexedCSVdatabaseOperations.calibrateCoordinateX(SHAREDvars.convertStringToDouble(localConnectionCSVdatasetConnection[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_X]));
				double yCalibratedPre = H01indexedCSVdatabaseOperations.calibrateCoordinateY(SHAREDvars.convertStringToDouble(localConnectionCSVdatasetConnection[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_Y]));
				double zCalibratedPre = H01indexedCSVdatabaseOperations.calibrateCoordinateZ(SHAREDvars.convertStringToDouble(localConnectionCSVdatasetConnection[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_Z]));
				double xCalibratedPost = H01indexedCSVdatabaseOperations.calibrateCoordinateX(SHAREDvars.convertStringToDouble(localConnectionCSVdatasetConnection[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_X]));
				double yCalibratedPost = H01indexedCSVdatabaseOperations.calibrateCoordinateY(SHAREDvars.convertStringToDouble(localConnectionCSVdatasetConnection[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_Y]));
				double zCalibratedPost = H01indexedCSVdatabaseOperations.calibrateCoordinateZ(SHAREDvars.convertStringToDouble(localConnectionCSVdatasetConnection[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_Z]));		

				int connectionRadialGradientType = this->calculateConnectionRadialGradientType(xCalibratedPre, xCalibratedPost);	
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
					connectionFlowType = this->calculateConnectionFlowType(xCalibratedPre, yCalibratedPre, xCalibratedPost, yCalibratedPost);	
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
				#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_COLOUR_ACTIVE_CONNECTIONS
				if(!traceActiveConnection)
				{
				#endif
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
				#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_COLOUR_ACTIVE_CONNECTIONS
				}
				#endif
				#endif
				
				#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
				//special case (hard coded); colour by layer
				#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_COLOUR_ACTIVE_CONNECTIONS
				if(!traceActiveConnection)
				{
				#endif
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
										connectionTypeStringLDR = SHAREDvars.convertIntToString(local_connectome_visualisation_neurons_colour_exciteneuron_layer_array[layerIndex-1]);
									}
									else if(excitationType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY)
									{
										connectionTypeStringLDR = SHAREDvars.convertIntToString(local_connectome_visualisation_neurons_colour_inhibitneuron_layer_array[layerIndex-1]);
									}
								}
							}			
						}
					}
				#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_COLOUR_ACTIVE_CONNECTIONS
				}
				#endif
				#endif

				string localConnectionVisualisationConnectionText = "";
				if(generate2Dvisualisation)
				{
					localConnectionVisualisationConnectionText = localConnectionVisualisationConnectionText + "     <path clip-path='url(#p9a78a7c8e7)' d='M " + this->convertDoubleToStringCalibrationPrecision(xCalibratedPre) + " " + this->convertDoubleToStringCalibrationPrecision(yCalibratedPre) + "  L " + this->convertDoubleToStringCalibrationPrecision(xCalibratedPost) + " " + this->convertDoubleToStringCalibrationPrecision(yCalibratedPost) + "  ' style='fill:none;stroke:url(#" + connectionTypeStringSVG + ");stroke-opacity:0.75;stroke-width:0.25;'/>" + STRING_NEWLINE;
					//eg     <path clip-path='url(#p9a78a7c8e7)' d='M 2959.68672 1597.1004  L 2856.87336 1772.75136  ' style='fill:none;stroke:url(#exciteConnectionRadialGradient1FlowDirection1);stroke-opacity:0.75;stroke-width:0.25;'/>
				}
				#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
				else
				{
					localConnectionVisualisationConnectionText = localConnectionVisualisationConnectionText + SHAREDvars.convertIntToString(LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_LDR_REFERENCE_TYPE) + LDR_DELIMITER + connectionTypeStringLDR + LDR_DELIMITER + this->convertDoubleToStringCalibrationPrecision(xCalibratedPre) + LDR_DELIMITER + this->convertDoubleToStringCalibrationPrecision(yCalibratedPre) + LDR_DELIMITER + this->convertDoubleToStringCalibrationPrecision(zCalibratedPre) + LDR_DELIMITER + this->convertDoubleToStringCalibrationPrecision(xCalibratedPost) + LDR_DELIMITER + this->convertDoubleToStringCalibrationPrecision(yCalibratedPost) + LDR_DELIMITER + this->convertDoubleToStringCalibrationPrecision(zCalibratedPost) + STRING_NEWLINE;
					//eg 2 2 2959.68672 1597.1004 0 2856.87336 1772.75136 3.80928
				}
				#endif

				//cout << "localConnectionVisualisationConnectionText = " << localConnectionVisualisationConnectionText << endl;

				localConnectomeVisualisationContentsPart6 = localConnectomeVisualisationContentsPart6 + localConnectionVisualisationConnectionText;

			}
		}		
	}
	
	
	
	//other data;
	#ifdef LOCAL_CONNECTOME_FOLDER_BASE_USE_RELATIVE_FOLDER
	SHAREDvars.setCurrentDirectory(currentDirectory);
	#endif
	string localConnectomeVisualisationTemplatesFolder = LOCAL_CONNECTOME_FOLDER_BASE;
	SHAREDvars.setCurrentDirectory(localConnectomeVisualisationTemplatesFolder);
	localConnectomeVisualisationTemplatesFolder = LOCAL_CONNECTOME_VISUALISATION_TEMPLATES_FOLDER;
	SHAREDvars.setCurrentDirectory(localConnectomeVisualisationTemplatesFolder);	
	
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
					localConnectomeVisualisationContentsPart2FileName = localConnectomeVisualisationContentsPart2FileName + LOCAL_CONNECTOME_VISUALISATION_SVG_PART_IMPORT_FILENAME_PREPEND + SHAREDvars.convertIntToString(2) + LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_LAYERED_DIRECTION_FLOW + SHAREDvars.convertIntToString(coloursetNumber);
				}
				else
				{
					localConnectomeVisualisationContentsPart2FileName = localConnectomeVisualisationContentsPart2FileName + LOCAL_CONNECTOME_VISUALISATION_SVG_PART_IMPORT_FILENAME_PREPEND + SHAREDvars.convertIntToString(2) + LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_LAYERED_DIRECTION + SHAREDvars.convertIntToString(coloursetNumber);
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
					localConnectomeVisualisationContentsPart2FileName = localConnectomeVisualisationContentsPart2FileName + LOCAL_CONNECTOME_VISUALISATION_SVG_PART_IMPORT_FILENAME_PREPEND + SHAREDvars.convertIntToString(2) + LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_DIRECTION_FLOW + SHAREDvars.convertIntToString(coloursetNumber);
				}
				else
				{
					localConnectomeVisualisationContentsPart2FileName = localConnectomeVisualisationContentsPart2FileName + LOCAL_CONNECTOME_VISUALISATION_SVG_PART_IMPORT_FILENAME_PREPEND + SHAREDvars.convertIntToString(2) + LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_DIRECTION + SHAREDvars.convertIntToString(coloursetNumber);
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
		
		#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME
		if(visualiseTrace)
		{
			localConnectomeVisualisationContentsPart2FileName = localConnectomeVisualisationContentsPart2FileName + LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_TRACE_VISUALISATION;
		}
		#endif
		
		localConnectomeVisualisationContentsPart2FileName = localConnectomeVisualisationContentsPart2FileName + SVG_FILE_EXTENSION;
		
		//cout << "localConnectomeVisualisationContentsPart2FileName = " << localConnectomeVisualisationContentsPart2FileName << endl;
		
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
	
	this->visualiseLocalConnectomeCSVdatasetWriteToFile(local_connectome_folder_base, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, generate2Dvisualisation, local_connectome_visualisation_filename_base, &localConnectomeVisualisationContents, visualiseTrace, traceIterationIndex);

	return result;
}		

bool H01indexedCSVdatabaseVisualiseLocalConnectomeClass::visualiseLocalConnectomeCSVdatasetWriteToFile(const string local_connectome_folder_base, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, const bool generate2Dvisualisation, const string local_connectome_visualisation_filename_base, string* localConnectomeVisualisationContents, const bool visualiseTrace, const int traceIterationIndex)
{
	bool result = true;
	
	#ifdef LOCAL_CONNECTOME_FOLDER_BASE_USE_RELATIVE_FOLDER
	SHAREDvars.setCurrentDirectory(currentDirectory);
	#endif
	string localConnectomeVisualisationFolder = LOCAL_CONNECTOME_FOLDER_BASE;
	SHAREDvars.setCurrentDirectory(localConnectomeVisualisationFolder);
	localConnectomeVisualisationFolder = LOCAL_CONNECTOME_VISUALISATION_FOLDER;
	SHAREDvars.setCurrentDirectory(localConnectomeVisualisationFolder);	
	if(connectionTypesDerivedFromPresynapticNeuronsOrEMimages)
	{
		localConnectomeVisualisationFolder = LOCAL_CONNECTOME_VISUALISATION_FOLDER_CONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS;
		//SHAREDvars.setCurrentDirectory(localConnectomeVisualisationFolder);
		SHAREDvars.checkIfFolderExistsAndIfNotMakeAndSetAsCurrent(&localConnectomeVisualisationFolder);
	}
	else
	{
		localConnectomeVisualisationFolder = LOCAL_CONNECTOME_VISUALISATION_FOLDER_CONNECTION_TYPES_DERIVED_FROM_EM_IMAGES;
		//SHAREDvars.setCurrentDirectory(localConnectomeVisualisationFolder);
		SHAREDvars.checkIfFolderExistsAndIfNotMakeAndSetAsCurrent(&localConnectomeVisualisationFolder);
	}	
	if(generate2Dvisualisation)
	{
		localConnectomeVisualisationFolder = LOCAL_CONNECTOME_VISUALISATION_FOLDER_2D;
		//SHAREDvars.setCurrentDirectory(localConnectomeVisualisationFolder);
		SHAREDvars.checkIfFolderExistsAndIfNotMakeAndSetAsCurrent(&localConnectomeVisualisationFolder);
	}
	else
	{
		localConnectomeVisualisationFolder = LOCAL_CONNECTOME_VISUALISATION_FOLDER_3D;
		//SHAREDvars.setCurrentDirectory(localConnectomeVisualisationFolder);
		SHAREDvars.checkIfFolderExistsAndIfNotMakeAndSetAsCurrent(&localConnectomeVisualisationFolder);	
	}
	
	string local_connectome_visualisation_filename = "";
	local_connectome_visualisation_filename = local_connectome_visualisation_filename_base;
	
	#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME
	if(visualiseTrace)
	{
		local_connectome_visualisation_filename = local_connectome_visualisation_filename + INDEXED_CSV_DATABASE_TRACE_INDEX_PREPEND + SHAREDvars.convertIntToString(traceIterationIndex);
	}
	#endif
	
	if(generate2Dvisualisation)
	{
		local_connectome_visualisation_filename = local_connectome_visualisation_filename + SVG_FILE_EXTENSION;
	}
	#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
	else
	{
		local_connectome_visualisation_filename = local_connectome_visualisation_filename + LDR_FILE_EXTENSION;
	}
	#endif

	cout << "visualiseLocalConnectomeCSVdatasetWriteToFile{} - local_connectome_visualisation_filename = " << local_connectome_visualisation_filename << endl;

	SHAREDvars.writeStringToFile(local_connectome_visualisation_filename, localConnectomeVisualisationContents);	//inefficient?
	
	return result;
}		


int H01indexedCSVdatabaseVisualiseLocalConnectomeClass::calculateConnectionFlowType(const double xCalibratedPre, const double yCalibratedPre, const double xCalibratedPost, const double yCalibratedPost)
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

int H01indexedCSVdatabaseVisualiseLocalConnectomeClass::calculateConnectionRadialGradientType(const double xCalibratedPre, const double xCalibratedPost)
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

string H01indexedCSVdatabaseVisualiseLocalConnectomeClass::convertDoubleToStringCalibrationPrecision(const double value)
{	
	const string doubleFormat = "%0." + SHAREDvars.convertIntToString(LOCAL_CONNECTOME_VISUALISATION_CALIBRATION_DOUBLE_PRECISION) + "f";	//"%0.5f";
	string doubleString = SHAREDvars.convertDoubleToString(value, doubleFormat);
		
	//remove trailing zeros (https://stackoverflow.com/a/13709929);
	doubleString.erase(doubleString.find_last_not_of('0') + 1, CPP_STRING_FIND_RESULT_FAIL_VALUE); 
	doubleString.erase(doubleString.find_last_not_of('.') + 1, CPP_STRING_FIND_RESULT_FAIL_VALUE);
	//cout << "doubleString = " << doubleString << endl;

	return doubleString;
}

#endif
