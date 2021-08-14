/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseTraceLocalConnectome.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: H01LocalConnectome
 * Requirements: see H01indexedCSVdatabase.hpp
 * Compilation: see H01indexedCSVdatabase.hpp
 * Usage: see H01indexedCSVdatabase.hpp
 * Description: H01 indexed CSV database trace local connectome - traces local connectome dataset (saving visualisation)
 * Input: localConnectomeNeurons.csv / localConnectomeConnectionsX.csv
 * Output Format: SVG (2D) / LDR (3D) [multiple files for video construction]
 * Comments:
 * /
 *******************************************************************************/

#include "H01indexedCSVdatabaseTraceLocalConnectome.hpp"

#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME

bool H01indexedCSVdatabaseTraceLocalConnectomeClass::traceLocalConnectomeCSVdataset(const string local_connectome_folder_base)
{
	vector<vector<string>> localConnectomeNeurons1;
	vector<vector<string>> localConnectomeConnections1;
	map<string, int> neuronMap1;
	map<string, int> connectionsMap1;
	this->traceLocalConnectomeCSVdataset(local_connectome_folder_base, READ_FILE_TRUE, LOCAL_CONNECTOME_DATASET_NEURONS_FILENAME, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, &localConnectomeNeurons1, &localConnectomeConnections1, &neuronMap1, &connectionsMap1, QUERY_POSTSYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_TRUE, APPEND_FILE_FALSE, 0, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, CONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, LAYER_ENFORCEMENT_TRUE, INDEXED_CSV_DATABASE_TRACE_START_LAYER_INDEX);

	vector<vector<string>> localConnectomeNeurons2;
	vector<vector<string>> localConnectomeConnections2;
	map<string, int> neuronMap2;
	map<string, int> connectionsMap2;	
	this->traceLocalConnectomeCSVdataset(local_connectome_folder_base, READ_FILE_TRUE, LOCAL_CONNECTOME_DATASET_NEURONS_FILENAME, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, &localConnectomeNeurons2, &localConnectomeConnections2, &neuronMap1, &connectionsMap1, QUERY_POSTSYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_TRUE, APPEND_FILE_FALSE, 0, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, CONNECTION_TYPES_DERIVED_FROM_EM_IMAGES, LAYER_ENFORCEMENT_TRUE, INDEXED_CSV_DATABASE_TRACE_START_LAYER_INDEX);
}




bool H01indexedCSVdatabaseTraceLocalConnectomeClass::traceLocalConnectomeCSVdataset(const string local_connectome_folder_base, const bool datasetRead, const string neuronDatasetFileNameRead, const string connectionDatasetFileNameRead, vector<vector<string>>* localConnectomeNeurons, vector<vector<string>>* localConnectomeConnections, map<string, int>* neuronMap, map<string, int>* connectionsMap, const bool queryPresynapticConnectionNeurons, const bool connectionDatasetWrite, const bool appendToFile, const int traceIterationIndex, const string connectionDatasetFileNameWrite, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, const bool layerIndexEnforcement, const int layerIndex)
{
	bool result = true;
		
	cout << "\nH01indexedCSVdatabaseTraceLocalConnectomeClass::traceLocalConnectomeCSVdataset: traceIterationIndex = " << traceIterationIndex << endl;
	
	if(datasetRead)
	{
		cout << "neuronDatasetFileNameRead = " << neuronDatasetFileNameRead << endl;
		cout << "connectionDatasetFileNameRead = " << connectionDatasetFileNameRead << endl;

		#ifdef INDEXED_CSV_DATABASE_QUERY_READ_CURRENT_FOLDER
		const string indexedCSVdatabaseQueryInputFolder = currentDirectory;	
		SHAREDvars.setCurrentDirectory(indexedCSVdatabaseQueryInputFolder);
		cout << "indexedCSVdatabaseQueryInputFolder = " << indexedCSVdatabaseQueryInputFolder << endl;
		#else
		#ifdef LOCAL_CONNECTOME_FOLDER_BASE_USE_RELATIVE_FOLDER
		SHAREDvars.setCurrentDirectory(currentDirectory);
		#endif
		string localConnectomeCSVdatasetFolder = local_connectome_folder_base;
		SHAREDvars.setCurrentDirectory(localConnectomeCSVdatasetFolder);
		localConnectomeCSVdatasetFolder = LOCAL_CONNECTOME_DATASET_FOLDER;
		SHAREDvars.setCurrentDirectory(localConnectomeCSVdatasetFolder);
		#endif

		cout << "connectionDatasetFileNameRead = " << connectionDatasetFileNameRead << endl;
		cout << "connectionDatasetFileNameWrite = " << connectionDatasetFileNameWrite << endl;

		int localNeuronCSVdatasetNeuronsSize = 0;
		SHAREDvars.getLinesFromFileCSV(neuronDatasetFileNameRead, localConnectomeNeurons, &localNeuronCSVdatasetNeuronsSize, CSV_DELIMITER_CHAR, true);
		
		int localConnectionCSVdatasetConnectionsSize = 0;
		SHAREDvars.getLinesFromFileCSV(connectionDatasetFileNameRead, localConnectomeConnections, &localConnectionCSVdatasetConnectionsSize, CSV_DELIMITER_CHAR, true);
		
		#ifdef LOCAL_CONNECTOME_VISUALISATION_LAYERS
		//initialise connection/neuron layer indices;
		//int corticalLayersNumKeypointsMax;	//= 28	//maximum number keypoints (number cols/2)
		const string corticalLayersBoundaryKeypointTableFileName = CORTICAL_LAYER_BOUNDARY_KEYPOINT_TABLE_FILE_NAME;
		vector<vector<vec>> corticalLayersKeypoints;
		H01indexedCSVdatabaseCalculateNeuronLayer.readCorticalLayersBoundaryKeypointTable(corticalLayersBoundaryKeypointTableFileName, &corticalLayersKeypoints);
		H01indexedCSVdatabaseCalculateNeuronLayer.calculateNeuronLayers(true, localConnectomeNeurons, &corticalLayersKeypoints);
		H01indexedCSVdatabaseCalculateNeuronLayer.calculateNeuronLayers(false, localConnectomeConnections, &corticalLayersKeypoints);
		#endif
		
		//add neurons/connections to map for efficient lookup;
		for(int i=0; i < localConnectomeNeurons->size(); i++)
		{
			vector<string>* localConnectomeNeuron = &((*localConnectomeNeurons)[i]);
			string neuronID = (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_NEURON_ID];
    		(*neuronMap)[neuronID] = i;
		}
		/*
		//connectionsMap NOT USED;
		for(int i=0; i < localConnectomeConnections->size(); i++)
		{
			vector<string>* localConnectomeConnection = &((*localConnectomeConnections)[i]);
			string connectionTargetNeuronID = (*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_ID];
			(*connectionsMap)[connectionTargetNeuronID] = i;
		}
		*/

		//initialise connection/neuron trace highlight values;
		for(int i=0; i<localConnectomeNeurons->size(); i++)
		{
			vector<string>* localConnectomeNeuron = &((*localConnectomeNeurons)[i]);
			localConnectomeNeuron->push_back(SHAREDvars.convertIntToString(INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_HIGHLIGHT_VALUE_NEURON_INACTIVE));	//add field LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_TRACE_HIGHLIGHT
		}
		for(int i=0; i<localConnectomeConnections->size(); i++)
		{
			vector<string>* localConnectomeConnection = &((*localConnectomeConnections)[i]);
			localConnectomeConnection->push_back(SHAREDvars.convertIntToString(INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_HIGHLIGHT_VALUE_CONNECTION_INACTIVE));	//add field LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_ARTIFICIAL_TRACE_HIGHLIGHT
			//localConnectomeConnection->push_back(SHAREDvars.convertIntToString(INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_HIGHLIGHT_VALUE_CONNECTION_INACTIVE));	//add field LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_ARTIFICIAL_TRACE_HIGHLIGHT
			//localConnectomeConnection->push_back(SHAREDvars.convertIntToString(INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_HIGHLIGHT_VALUE_CONNECTION_INACTIVE));	//add field LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_ARTIFICIAL_TRACE_HIGHLIGHT
		}
	}

	//perform trace: update neuron/connection trace highlights;
	for(int i=0; i<localConnectomeNeurons->size(); i++)
	{
		vector<string>* localConnectomeNeuron = &((*localConnectomeNeurons)[i]);
		
		int localConnectomeNeuronLayer = SHAREDvars.convertStringToInt((*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER]);
		int neuronTraceHighlightValue = SHAREDvars.convertStringToInt((*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_TRACE_HIGHLIGHT]);
		//cout << "neuronTraceHighlightValue = " << neuronTraceHighlightValue << endl;
		
		//if neuron on cortex entry layer;
		if(layerIndexEnforcement && (localConnectomeNeuronLayer == layerIndex))
		{
			neuronTraceHighlightValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_HIGHLIGHT_VALUE_NEURON_SOURCE;
		}
		else
		{
			//or if neuron has previously been traced;
			if(neuronTraceHighlightValue == INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_HIGHLIGHT_VALUE_NEURON_SOURCE)
			{
				neuronTraceHighlightValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_HIGHLIGHT_VALUE_NEURON_INACTIVE;
			}
			else if(neuronTraceHighlightValue == INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_HIGHLIGHT_VALUE_NEURON_TARGET)
			{
				neuronTraceHighlightValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_HIGHLIGHT_VALUE_NEURON_SOURCE;
			}
			else if(neuronTraceHighlightValue == INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_HIGHLIGHT_VALUE_NEURON_INACTIVE)
			{
			}
		}
		
		(*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_TRACE_HIGHLIGHT] = SHAREDvars.convertIntToString(neuronTraceHighlightValue);
	}

	for(int i=0; i<localConnectomeConnections->size(); i++)
	{
		vector<string>* localConnectomeConnection = &((*localConnectomeConnections)[i]);
		int connectionTraceHighlightValue = SHAREDvars.convertStringToInt((*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_ARTIFICIAL_TRACE_HIGHLIGHT]);
		connectionTraceHighlightValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_HIGHLIGHT_VALUE_CONNECTION_INACTIVE;
		(*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_ARTIFICIAL_TRACE_HIGHLIGHT] = connectionTraceHighlightValue;

		string connectionSourceNeuronID = (*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_ID];
		string connectionTargetNeuronID = (*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_ID];
		bool activateConnection = false;
		if(connectionSourceNeuronID != LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES_UNKNOWN_NEURON_ID)
		{
			int sourceNeuronIndex = (*neuronMap)[connectionSourceNeuronID];
			vector<string>* connectionSourceNeuron = &((*localConnectomeNeurons)[sourceNeuronIndex]);
			int neuronTraceHighlightValue = SHAREDvars.convertStringToInt((*connectionSourceNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_TRACE_HIGHLIGHT]);
			if(neuronTraceHighlightValue == INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_HIGHLIGHT_VALUE_NEURON_SOURCE)
			{
				activateConnection = true;
			}
		}
		if(connectionTargetNeuronID != LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES_UNKNOWN_NEURON_ID)
		{
			int targetNeuronIndex = (*neuronMap)[connectionTargetNeuronID];
			vector<string>* connectionTargetNeuron = &((*localConnectomeNeurons)[targetNeuronIndex]);
			int neuronTraceHighlightValue = SHAREDvars.convertStringToInt((*connectionTargetNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_TRACE_HIGHLIGHT]);
			if(activateConnection)
			{
				neuronTraceHighlightValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_HIGHLIGHT_VALUE_NEURON_TARGET;
			}
			else	
			{
				//support many-to-one and one-to-many connections (take no action)
				//neuronTraceHighlightValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_HIGHLIGHT_VALUE_NEURON_INACTIVE;
			}
			(*connectionTargetNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_TRACE_HIGHLIGHT] =  SHAREDvars.convertIntToString(neuronTraceHighlightValue);		
		}
		if(activateConnection)
		{			
			(*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_ARTIFICIAL_TRACE_HIGHLIGHT] = SHAREDvars.convertIntToString(INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_HIGHLIGHT_VALUE_CONNECTION_ACTIVE);
		}		
	}
	
	bool visualiseTrace = true;
	
	#ifdef LOCAL_CONNECTOME_VISUALISATION_2D_SVG
	H01indexedCSVdatabaseVisualiseLocalConnectome.visualiseLocalConnectomeCSVdataset(local_connectome_folder_base, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, true, localConnectomeNeurons, localConnectomeConnections, visualiseTrace, traceIterationIndex);
	#endif
	#ifdef LOCAL_CONNECTOME_VISUALISATION_3D_LDR
	H01indexedCSVdatabaseVisualiseLocalConnectome.visualiseLocalConnectomeCSVdataset(local_connectome_folder_base, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, false, localConnectomeNeurons, localConnectomeConnections, visualiseTrace, traceIterationIndex);
	#endif
	
	if(traceIterationIndex < INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_MAX_NUMBER_ITERATIONS)
	{
		bool layerIndexEnforcementNext = LAYER_ENFORCEMENT_FALSE;	//checkthis
		this->traceLocalConnectomeCSVdataset(local_connectome_folder_base, READ_FILE_FALSE, neuronDatasetFileNameRead, connectionDatasetFileNameRead, localConnectomeNeurons, localConnectomeConnections, neuronMap, connectionsMap, queryPresynapticConnectionNeurons, connectionDatasetWrite, appendToFile, (traceIterationIndex+1), connectionDatasetFileNameWrite, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, layerIndexEnforcementNext, layerIndex);
	}
	
	return result;
}

#endif
