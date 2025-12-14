/*******************************************************************************
 *
 * File Name: LCindexedCSVdatabaseReadLocalConnectome.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021-2023 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: LCLocalConnectome
 * Requirements: see LCindexedCSVdatabaseGlobalDefs.hpp
 * Compilation: see LCindexedCSVdatabaseGlobalDefs.hpp
 * Usage: see LCindexedCSVdatabaseGlobalDefs.hpp
 * Description: LC indexed CSV database read local connectome - 
 * /
 *******************************************************************************/


#include "LCindexedCSVdatabaseReadLocalConnectome.hpp"


#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS
void LCindexedCSVdatabaseReadLocalConnectomeClass::incrementNumberOfConnections(GaussianQuad* gaussianQuad, const vec* posSource, const vec* posTarget, const vec* sourceNeuronCorticalFlowVector)
{
	gaussianQuad->numberConnections++;

	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL	
	incrementGaussianQuad(gaussianQuad, posSource, posTarget, sourceNeuronCorticalFlowVector);
	#endif
}
#endif

#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL	
void LCindexedCSVdatabaseReadLocalConnectomeClass::incrementGaussianQuad(GaussianQuad* gaussianQuad, const vec* posSource, const vec* posTarget, const vec* sourceNeuronCorticalFlowVector)
{
	if(posTarget != NULL)
	{//verify that targetNeuronID centroid/soma is also in local connectome;
		
		#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL_NORMALISE_LOCAL_CONNECTIVITY
		//normalise x/y comparison wrt sourceNeuronCorticalFlowVector
		vec connectionDirection;
		SHAREDvector.subtractVectors(&connectionDirection, posTarget, posSource);
		vec posTargetNormalised;
		double angleRadians = SHAREDvector.calculateAngleSigned(sourceNeuronCorticalFlowVector, &connectionDirection);	//check if can use interior angle [need to ensure vectors are going in same direction]
		//cout << "angleRadians = " << angleRadians << endl;
		SHAREDvector.calculatePointGivenAngleAndMagnitude(&posTargetNormalised, angleRadians, SHAREDvector.findMagnitudeOfVector(&connectionDirection));
		vec* posTargetNormalisedPointer = &posTargetNormalised;
		#else
		vec* posTargetNormalisedPointer = posTarget;
		#endif

		bool verticalCorticalFlowPositive = false;	//positive/negative
		bool lateralCorticalFlowPositive = false;	//positive/negative		
		double mean = SHAREDvector.calculateTheDistanceBetweenTwoPointsXYOnly(posSource, &posTargetNormalised);
		double variance = pow(mean, 2.0);	
		if(posTargetNormalisedPointer->x > posSource->x)
		{
			lateralCorticalFlowPositive = true;
		}
		if(posTargetNormalisedPointer->y > posSource->y)
		{
			verticalCorticalFlowPositive = true;
		}		
		
		if(lateralCorticalFlowPositive)
		{
			gaussianQuad->mean.xPos = gaussianQuad->mean.xPos + mean;
			gaussianQuad->variance.xPos = gaussianQuad->variance.xPos + variance;
			gaussianQuad->count.xPos++;
		}	
		else
		{
			gaussianQuad->mean.xNeg = gaussianQuad->mean.xNeg + mean;
			gaussianQuad->variance.xNeg = gaussianQuad->variance.xNeg + variance;
			gaussianQuad->count.xNeg++;
		}
		if(verticalCorticalFlowPositive)
		{
			gaussianQuad->mean.yPos = gaussianQuad->mean.yPos + mean;
			gaussianQuad->variance.yPos = gaussianQuad->variance.yPos + variance;
			gaussianQuad->count.yPos++;
		}
		else
		{
			gaussianQuad->mean.yNeg = gaussianQuad->mean.yNeg + mean;
			gaussianQuad->variance.yNeg = gaussianQuad->variance.yNeg + variance;
			gaussianQuad->count.yNeg++;
		}
	}
}
void LCindexedCSVdatabaseReadLocalConnectomeClass::averageGaussianQuad(GaussianQuad* gaussianQuad)
{
	gaussianQuad->mean.xPos = gaussianQuad->mean.xPos / gaussianQuad->count.xPos;
	gaussianQuad->variance.xPos = gaussianQuad->variance.xPos / gaussianQuad->count.xPos;

	gaussianQuad->mean.yNeg = gaussianQuad->mean.yNeg / gaussianQuad->count.yNeg;
	gaussianQuad->variance.yNeg = gaussianQuad->variance.yNeg / gaussianQuad->count.yNeg;

	gaussianQuad->mean.yPos = gaussianQuad->mean.yPos / gaussianQuad->count.yPos;
	gaussianQuad->variance.yPos = gaussianQuad->variance.yPos / gaussianQuad->count.yPos;

	gaussianQuad->mean.yNeg = gaussianQuad->mean.yNeg / gaussianQuad->count.yNeg;
	gaussianQuad->variance.yNeg = gaussianQuad->variance.yNeg / gaussianQuad->count.yNeg;
}
#endif


bool LCindexedCSVdatabaseReadLocalConnectomeClass::readLocalConnectome(const int readMode, const string local_connectome_folder_base)
{
	bool result = true;

	currentDirectory = SHAREDvars.getCurrentDirectory();
	
	bool na = false;
	
	bool neuronListIsDataset = true;	//depreciated !neuronListIsDataset are not supported by LCindexedCSVdatabaseReadLocalConnectome
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS
	if(readMode == READ_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS)
	{
		string neuronListConnectionsFileNameNA = "";
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PRINT_OUTPUT_VERBOSE_LOCALORNONLOCAL
		cout << "INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PRINT_OUTPUT_VERBOSE_LOCALORNONLOCAL print format: neuronID num_excitatory_connections num_inhibitory_connections" << endl;
		#endif
		vector<H01connectivityModelClass>* connectivityModelLayersIncoming = NULL;
		vector<H01connectivityModelClass>* connectivityModelLayersOutgoing = NULL;
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_PRESYNAPTIC_NEURONS
		cout << "\n\nCONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS:" << endl;
		connectivityModelLayersIncoming = LCindexedCSVdatabaseCalculateNeuronLayer.generateNumberOfConnectionsLayers();
		connectivityModelLayersOutgoing = LCindexedCSVdatabaseCalculateNeuronLayer.generateNumberOfConnectionsLayers();
		//cout << "\nQUERY_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS: QUERY_PRESYNAPTIC_CONNECTION_NEURONS (QUERY_BY_POSTSYNAPTIC_CONNECTION_NEURONS) - count incoming connections" << endl;
		//this->readLocalConnectomeByNeuronDatasetOrListFile(readMode, local_connectome_folder_base, LOCAL_CONNECTOME_DATASET_NEURONS_FILENAME, neuronListIsDataset, QUERY_PRESYNAPTIC_CONNECTION_NEURONS, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, CONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, connectivityModelLayersIncoming);	//count incoming connections
		cout << "\nQUERY_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS: QUERY_POSTSYNAPTIC_CONNECTION_NEURONS (QUERY_BY_PRESYNAPTIC_CONNECTION_NEURONS) - count outgoing connections" << endl;		
		this->readLocalConnectomeByNeuronDatasetOrListFile(readMode, local_connectome_folder_base, LOCAL_CONNECTOME_DATASET_NEURONS_FILENAME, neuronListIsDataset, QUERY_POSTSYNAPTIC_CONNECTION_NEURONS, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, CONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, connectivityModelLayersOutgoing);	//count outgoing connections
		#endif
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_EM_IMAGES
		cout << "\n\nCONNECTION_TYPES_DERIVED_FROM_EM_IMAGES:" << endl;
		connectivityModelLayersIncoming = LCindexedCSVdatabaseCalculateNeuronLayer.generateNumberOfConnectionsLayers();
		connectivityModelLayersOutgoing = LCindexedCSVdatabaseCalculateNeuronLayer.generateNumberOfConnectionsLayers();
		//cout << "\nQUERY_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS: QUERY_PRESYNAPTIC_CONNECTION_NEURONS (QUERY_BY_POSTSYNAPTIC_CONNECTION_NEURONS) - count incoming connections" << endl;
		//this->readLocalConnectomeByNeuronDatasetOrListFile(readMode, local_connectome_folder_base, LOCAL_CONNECTOME_DATASET_NEURONS_FILENAME, neuronListIsDataset, QUERY_PRESYNAPTIC_CONNECTION_NEURONS, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, CONNECTION_TYPES_DERIVED_FROM_EM_IMAGES, connectivityModelLayersIncoming);	//count incoming connections
		cout << "\nQUERY_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS: QUERY_POSTSYNAPTIC_CONNECTION_NEURONS (QUERY_BY_PRESYNAPTIC_CONNECTION_NEURONS) - count outgoing connections" << endl;		
		this->readLocalConnectomeByNeuronDatasetOrListFile(readMode, local_connectome_folder_base, LOCAL_CONNECTOME_DATASET_NEURONS_FILENAME, neuronListIsDataset, QUERY_POSTSYNAPTIC_CONNECTION_NEURONS, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, CONNECTION_TYPES_DERIVED_FROM_EM_IMAGES, connectivityModelLayersOutgoing);	//count outgoing connections
		#endif
	}
	#endif
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL
	else if(readMode == READ_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL)
	{
		string neuronListConnectionsFileNameNA = "";
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PRINT_OUTPUT_VERBOSE_LOCALORNONLOCAL
		cout << "INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PRINT_OUTPUT_VERBOSE_LOCALORNONLOCAL print format: neuronID num_excitatory_connections num_inhibitory_connections" << endl;
		#endif
		vector<H01connectivityModelClass>* connectivityModelLayersIncoming = NULL;
		vector<H01connectivityModelClass>* connectivityModelLayersOutgoing = NULL;
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_PRESYNAPTIC_NEURONS
		cout << "\n\nCONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS:" << endl;
		connectivityModelLayersIncoming = LCindexedCSVdatabaseCalculateNeuronLayer.generateNumberOfConnectionsLayers();
		connectivityModelLayersOutgoing = LCindexedCSVdatabaseCalculateNeuronLayer.generateNumberOfConnectionsLayers();
		//cout << "\nINDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL: QUERY_PRESYNAPTIC_CONNECTION_NEURONS (QUERY_BY_POSTSYNAPTIC_CONNECTION_NEURONS) - count incoming connections" << endl;
		//this->readLocalConnectomeByNeuronDatasetOrListFile(readMode, local_connectome_folder_base, LOCAL_CONNECTOME_DATASET_NEURONS_FILENAME, neuronListIsDataset, QUERY_PRESYNAPTIC_CONNECTION_NEURONS, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, CONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, connectivityModelLayersIncoming);	//count incoming connections
		cout << "\nINDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL: QUERY_POSTSYNAPTIC_CONNECTION_NEURONS (QUERY_BY_PRESYNAPTIC_CONNECTION_NEURONS) - count outgoing connections" << endl;		
		this->readLocalConnectomeByNeuronDatasetOrListFile(readMode, local_connectome_folder_base, LOCAL_CONNECTOME_DATASET_NEURONS_FILENAME, neuronListIsDataset, QUERY_POSTSYNAPTIC_CONNECTION_NEURONS, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, CONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, connectivityModelLayersOutgoing);	//count outgoing connections
		cout << "\nINDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL" << endl;
		this->generateLargeModelNeuronsAndConnectionsWrapper(readMode, local_connectome_folder_base, LOCAL_CONNECTOME_DATASET_NEURONS_FILENAME, neuronListIsDataset, WRITE_FILE_TRUE, APPEND_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, CONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, connectivityModelLayersIncoming, connectivityModelLayersOutgoing);	//generate large model
		#endif
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_EM_IMAGES
		cout << "\n\nCONNECTION_TYPES_DERIVED_FROM_EM_IMAGES:" << endl;
		connectivityModelLayersIncoming = LCindexedCSVdatabaseCalculateNeuronLayer.generateNumberOfConnectionsLayers();
		connectivityModelLayersOutgoing = LCindexedCSVdatabaseCalculateNeuronLayer.generateNumberOfConnectionsLayers();
		//cout << "\nINDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL: QUERY_PRESYNAPTIC_CONNECTION_NEURONS (QUERY_BY_POSTSYNAPTIC_CONNECTION_NEURONS) - count incoming connections" << endl;
		//this->readLocalConnectomeByNeuronDatasetOrListFile(readMode, local_connectome_folder_base, LOCAL_CONNECTOME_DATASET_NEURONS_FILENAME, neuronListIsDataset, QUERY_PRESYNAPTIC_CONNECTION_NEURONS, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, CONNECTION_TYPES_DERIVED_FROM_EM_IMAGES, connectivityModelLayersIncoming);	//count incoming connections
		cout << "\nINDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL: QUERY_POSTSYNAPTIC_CONNECTION_NEURONS (QUERY_BY_PRESYNAPTIC_CONNECTION_NEURONS) - count outgoing connections" << endl;		
		this->readLocalConnectomeByNeuronDatasetOrListFile(readMode, local_connectome_folder_base, LOCAL_CONNECTOME_DATASET_NEURONS_FILENAME, neuronListIsDataset, QUERY_POSTSYNAPTIC_CONNECTION_NEURONS, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, CONNECTION_TYPES_DERIVED_FROM_EM_IMAGES, connectivityModelLayersOutgoing);	//count outgoing connections
		cout << "\nINDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL" << endl;
		this->generateLargeModelNeuronsAndConnectionsWrapper(readMode, local_connectome_folder_base, LOCAL_CONNECTOME_DATASET_NEURONS_FILENAME, neuronListIsDataset, WRITE_FILE_TRUE, APPEND_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, CONNECTION_TYPES_DERIVED_FROM_EM_IMAGES, connectivityModelLayersIncoming, connectivityModelLayersOutgoing);	//generate large model
		#endif
	}
	#endif
	else
	{
		cerr << "LCindexedCSVdatabaseReadLocalConnectomeClass::queryIndexedCSVdatabase error: readMode unknown: " << readMode << endl;
		exit(EXIT_ERROR);
	}
	
	return result;
}

bool LCindexedCSVdatabaseReadLocalConnectomeClass::readLocalConnectomeByNeuronDatasetOrListFile(const int readMode, const string local_connectome_folder_base, const string neuronDatasetOrListFileName, const bool neuronListIsDataset, const bool queryPresynapticConnectionNeurons, const string neuronListConnectionsFileName, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, vector<H01connectivityModelClass>* connectivityModelLayers)
{
	bool result = true;
		
	//read connections?;
	bool readConnections = false;
	bool buildConnectionsMap = false;
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS
	if(readMode == READ_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS)
	{
		readConnections = true;	//vector<vector<string>>*localConnectomeCSVdatasetConnections is required by INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS
		buildConnectionsMap = true;
	}	
	#endif

	//read layers?;
	bool readLayers = false;
	bool readLayersConnections = false;
	#ifdef INDEXED_CSV_DATABASE_QUERY_LAYERS
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS
	if((readMode == READ_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS) || (readMode == READ_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL))	//constraint only added for efficiency (only calculate layers when necessary)
	{	
		readLayers = true;
	}
	#endif
	#endif
				
	vector<string> neuronList;
	map<string, int> neuronMap;
	vector<vector<string>> localConnectomeCSVdatasetNeurons;	
	vector<vector<string>> localConnectomeCSVdatasetConnections;
	map<string, int> connectionsMap;	
	if(!LCindexedCSVdatabaseCalculateNeuronLayer.readLocalNeuronsAndConnections(local_connectome_folder_base, neuronDatasetOrListFileName, neuronListIsDataset, &neuronList, &neuronMap, &localConnectomeCSVdatasetNeurons, readConnections, neuronListConnectionsFileName, &localConnectomeCSVdatasetConnections, buildConnectionsMap, &connectionsMap, readLayers, readLayersConnections))
	{
		result = false;
	}

	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL_NORMALISE_LOCAL_CONNECTIVITY
	if(readMode == READ_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL)
	{	
		LCindexedCSVdatabaseCalculateNeuronLayer.calculateLocalConnectomeNeuronLayerSurfaceNorms(&localConnectomeCSVdatasetNeurons, &neuronMap);
	}
	#endif
	
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS
	if(readMode == READ_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS)	//added 05 Mar 2022
	{
		#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_ONLY_COUNT_OUTGOING
		if(!queryPresynapticConnectionNeurons)	//only count connections from preSynaptic to postSynaptic neuron
		{
		#endif
			cout << "\n\nINDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS:" << endl;
			countConnectionsLocalMatrix(readMode, &neuronList, &neuronMap, &localConnectomeCSVdatasetNeurons, &localConnectomeCSVdatasetConnections, queryPresynapticConnectionNeurons, connectionTypesDerivedFromPresynapticNeuronsOrEMimages);
		#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_ONLY_COUNT_OUTGOING
		}
		#endif
	}
	if((readMode == READ_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS) || (readMode == READ_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL))
	{
		countConnectionsLocal(readMode, &neuronList, &neuronMap, &connectionsMap, &localConnectomeCSVdatasetNeurons, &localConnectomeCSVdatasetConnections, queryPresynapticConnectionNeurons, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, connectivityModelLayers);
	}
	if(readMode == READ_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS)
	{
		bool queryByPresynapticConnectionNeurons = !queryPresynapticConnectionNeurons;
		printNumberOfConnectionsLayersLocal(queryByPresynapticConnectionNeurons, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, connectivityModelLayers);
	}
	#endif
	
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL
	if(readMode == READ_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL)
	{	
		#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
		int maxLayer = CORTICAL_LAYER_NUMBER_OF_LAYERS;
		#else
		int maxLayer = LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL;
		#endif
		for(int layerIndex=0; layerIndex<=maxLayer; layerIndex++)
		{
			averageGaussianQuad(&(*connectivityModelLayers)[layerIndex].neuronModelConnectionsLocalConnectome);
			averageGaussianQuad(&(*connectivityModelLayers)[layerIndex].neuronModelConnectionsLocalConnectomeExcitatory);
			averageGaussianQuad(&(*connectivityModelLayers)[layerIndex].neuronModelConnectionsLocalConnectomeInhibitory);
			averageGaussianQuad(&(*connectivityModelLayers)[layerIndex].neuronModelConnectionsExternalConnectome);
			averageGaussianQuad(&(*connectivityModelLayers)[layerIndex].neuronModelConnectionsExternalConnectomeExcitatory);
			averageGaussianQuad(&(*connectivityModelLayers)[layerIndex].neuronModelConnectionsExternalConnectomeInhibitory);
		}
		#endif
	}
	#endif
	
	return result;
}
		

#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL
bool LCindexedCSVdatabaseReadLocalConnectomeClass::generateLargeModelNeuronsAndConnectionsWrapper(const int readMode, const string local_connectome_folder_base, const string neuronDatasetOrListFileName, const bool neuronListIsDataset, const bool write, const bool appendToFile, const string neuronListConnectionsFileName, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, vector<H01connectivityModelClass>* numberOfConnectionsLayersIncoming, vector<H01connectivityModelClass>* numberOfConnectionsLayersOutgoing)
{
	bool result = true;
	
	bool readConnections = true;
	bool buildConnectionsMap = true;	//CHECKTHIS - is this required?
	bool readLayers = true;
	bool readLayersConnections = false;	//CHECKTHIS
		
	vector<string> neuronList;
	map<string, int> neuronMap;
	vector<vector<string>> localConnectomeCSVdatasetNeurons;	
	vector<vector<string>> localConnectomeCSVdatasetConnections;
	map<string, int> connectionsMap;	
	if(!LCindexedCSVdatabaseCalculateNeuronLayer.readLocalNeuronsAndConnections(local_connectome_folder_base, neuronDatasetOrListFileName, neuronListIsDataset, &neuronList, &neuronMap, &localConnectomeCSVdatasetNeurons, readConnections, neuronListConnectionsFileName, &localConnectomeCSVdatasetConnections, buildConnectionsMap, &connectionsMap, readLayers, readLayersConnections))
	{
		result = false;
	}
		
	const string csvDelimiter = CSV_DELIMITER;
	#ifdef INDEXED_CSV_DATABASE_QUERY_WRITE_CURRENT_FOLDER
	const string indexedCSVdatabaseQueryOutputFolder = currentDirectory;	
	#else
	const string indexedCSVdatabaseQueryOutputFolder = INDEXED_CSV_DATABASE_QUERY_OUTPUT_FOLDER;
	#endif
	
	string neuronsLargeModelFileName = LOCAL_CONNECTOME_DATASET_LARGE_MODEL_NEURONS_FILENAME;
	string connectionsLargeModelFileName = "";
	if(connectionTypesDerivedFromPresynapticNeuronsOrEMimages)
	{
		connectionsLargeModelFileName = LOCAL_CONNECTOME_DATASET_LARGE_MODEL_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS;
	}
	else
	{
		connectionsLargeModelFileName = LOCAL_CONNECTOME_DATASET_LARGE_MODEL_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES;
	}
	
	
	/*
	ofstream writeFileObject = LCindexedCSVdatabaseOperations.prepareLocalConnectomeDatasetWrite(write, appendToFile, connectionDatasetFileNameWrite);

	LCindexedCSVdatabaseOperations.finaliseLocalConnectomeDatasetWrite(write, &writeFileObject);
	*/
	
	return result;
}
#endif

#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS
//count connections for all local connectome neuron 1 Layer X to local connectome neuron 2 layer Y;
bool LCindexedCSVdatabaseReadLocalConnectomeClass::countConnectionsLocalMatrix(const int readMode, vector<string>* neuronList, map<string, int>* neuronMap, vector<vector<string>>* localConnectomeCSVdatasetNeurons, vector<vector<string>>* localConnectomeCSVdatasetConnections, const bool queryPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages)
{
	bool result = true;

	bool queryByPresynapticConnectionNeurons = !queryPresynapticConnectionNeurons;

	//vector<H01connectivityModelClass>* connectivityModelLayersLayers = LCindexedCSVdatabaseCalculateNeuronLayer.generateNumberOfConnectionsLayersLayers();
	
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_BY_LAYER
	int numberLayers = CORTICAL_LAYER_NUMBER_OF_LAYERS;
	#endif
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_BY_LAYER
	for(int sourceLayerIndex=0; sourceLayerIndex<=CORTICAL_LAYER_NUMBER_OF_LAYERS; sourceLayerIndex++)
	{
	#else
	int sourceLayerIndex = 0;	//all layers
	#endif
		#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_BY_LAYER
		for(int targetLayerIndex=0; targetLayerIndex<=CORTICAL_LAYER_NUMBER_OF_LAYERS; targetLayerIndex++)
		{
		#else
		int targetLayerIndex = 0;	//all layers
		#endif
			//H01connectivityModelClass* connectivityModel = connectivityModelLayersLayers[sourceLayerIndex][targetLayerIndex];
			
			#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_BY_EXCITATION_TYPE
			for(int sourceExcitationTypeIndex=0; sourceExcitationTypeIndex<=LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY; sourceExcitationTypeIndex++)
			{
			#else
			int sourceExcitationTypeIndex = LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN;	//all excitation types
			#endif
				#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_BY_EXCITATION_TYPE
				for(int targetExcitationTypeIndex=0; targetExcitationTypeIndex<=LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY; targetExcitationTypeIndex++)
				{
				#else
				int targetExcitationTypeIndex = LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN;	//all excitation types
				#endif
						
					cout << "" << endl;
					#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_BY_LAYER	
					if(sourceLayerIndex == 0)
					{
						cout << "source All layers: " << endl;
					}
					else
					{
						cout << "source sourceLayerIndex = " << sourceLayerIndex << endl;
					}
					if(targetLayerIndex == 0)
					{
						cout << "target All layers: " << endl;
					}
					else
					{
						cout << "target targetLayerIndex = " << targetLayerIndex << endl;
					}
					#endif
					#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_BY_EXCITATION_TYPE
					if(sourceExcitationTypeIndex == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN)
					{
						cout << "source All excitationTypes: " << endl;
					}
					else
					{
						cout << "source sourceExcitationTypeIndex = " << sourceExcitationTypeIndex << endl;
					}
					if(targetExcitationTypeIndex == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN)
					{
						cout << "target All excitationTypes: " << endl;
					}
					else
					{
						cout << "target targetExcitationTypeIndex = " << targetExcitationTypeIndex << endl;
					}
					#endif

					H01connectivityModelClass connectivityModel;
					#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_NUMBER_EXCITATORY_INHIBITORY_NEURONS
					map<string, int> independentSourceNeuronMap;
					#endif
					/*						
					int numberConnectionsLocalConnectome = 0;
					int numberConnectionsLocalConnectomeExcitatory = 0;
					int numberConnectionsLocalConnectomeInhibitory = 0;
					int numberConnectionsExternalConnectome = 0;	//NOTUSED
					int numberConnectionsExternalConnectomeExcitatory = 0;	//NOTUSED
					int numberConnectionsExternalConnectomeInhibitory = 0;	//NOTUSED
					#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_NUMBER_EXCITATORY_INHIBITORY_NEURONS
					int numberOfLocalConnectomeNeurons = 0;
					int numberOfLocalConnectomeNeuronsExcitatory = 0;
					int numberOfLocalConnectomeNeuronsInhibitory = 0;
					#endif
					*/
					
					for(int i=0;i<localConnectomeCSVdatasetConnections->size(); i++)
					{
						//cout << "localConnectomeCSVdatasetConnections: i = " << i << endl;
						vector<string> localConnectomeConnection = (*localConnectomeCSVdatasetConnections)[i];

						string sourceNeuronID = "";
						string targetNeuronID = "";
						/*
						#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_NUMBER_EXCITATORY_INHIBITORY_NEURONS
						string sourceNeuronType = "";
						#endif
						*/
						if(queryByPresynapticConnectionNeurons)
						{
							/*
							#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_NUMBER_EXCITATORY_INHIBITORY_NEURONS
							sourceNeuronType = (localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_TYPE];
							#endif
							*/
							sourceNeuronID = (localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_ID];
							targetNeuronID = (localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_ID];
						}
						else
						{
							/*
							#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_NUMBER_EXCITATORY_INHIBITORY_NEURONS
							sourceNeuronType = (localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_TYPE];
							#endif
							*/
							sourceNeuronID = (localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_ID];
							targetNeuronID = (localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_ID];
						}

						int neuronIndexSource = (*neuronMap)[sourceNeuronID];
						vector<string>* localConnectomeNeuronSource = &((*localConnectomeCSVdatasetNeurons)[neuronIndexSource]);
						int neuronIndexTarget = (*neuronMap)[targetNeuronID];
						vector<string>* localConnectomeNeuronTarget = &((*localConnectomeCSVdatasetNeurons)[neuronIndexTarget]);							
						
						#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_NUMBER_EXCITATORY_INHIBITORY_CONNECTIONS
						int excitationType = calculateConnectionExcitationType(connectionTypesDerivedFromPresynapticNeuronsOrEMimages, &localConnectomeConnection, localConnectomeNeuronSource);
						#endif

						bool layerRequirements = true;	
						#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_BY_LAYER	
						int sourceNeuronLayerIndex = SHAREDvars.convertStringToInt((*localConnectomeNeuronSource)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER]);
						int targetNeuronLayerIndex = SHAREDvars.convertStringToInt((*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER]);		
						if(sourceLayerIndex == 0)
						{
						}
						else if(sourceLayerIndex == sourceNeuronLayerIndex)
						{
						}
						else
						{
							layerRequirements = false;
						}
						if(targetLayerIndex == 0)
						{
						}
						else if(targetLayerIndex == targetNeuronLayerIndex)
						{
						}
						else
						{
							layerRequirements = false;
						}
						#endif
						
						bool excitationTypeRequirements = true;
						#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_BY_EXCITATION_TYPE
						#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_PRESYNAPTIC_NEURONS_LOOKUP
						int sourceNeuronExcitationType = SHAREDvars.convertStringToInt((*localConnectomeNeuronSource)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE]);
						int targetNeuronExcitationType = SHAREDvars.convertStringToInt((*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE]);	
						#else
						string sourceNeuronType = (*localConnectomeNeuronSource)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE];
						string targetNeuronType = (*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE];	
						int sourceNeuronExcitationType = LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN;
						int targetNeuronExcitationType = LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN;	
						if(sourceNeuronType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_PYRAMIDAL)
						{
							sourceNeuronExcitationType = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY;
						}
						else if(sourceNeuronType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_INTERNEURON)
						{
							sourceNeuronExcitationType = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY;
						}
						else
						{
							cerr << "LCindexedCSVdatabaseReadLocalConnectomeClass::countConnectionsLocalMatrix error: sourceNeuronExcitationType unknown" << endl;
							exit(EXIT_ERROR);
						}
						if(targetNeuronType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_PYRAMIDAL)
						{
							targetNeuronExcitationType = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY;
						}
						else if(targetNeuronType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_INTERNEURON)
						{
							targetNeuronExcitationType = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY;
						}
						else
						{
							cerr << "LCindexedCSVdatabaseReadLocalConnectomeClass::countConnectionsLocalMatrix error: targetNeuronExcitationType unknown" << endl;
							exit(EXIT_ERROR);
						}
						#endif
						//cout << "sourceNeuronExcitationType = " << sourceNeuronExcitationType << endl;
						//cout << "targetNeuronExcitationType = " << targetNeuronExcitationType << endl;
						if(sourceExcitationTypeIndex == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN)
						{
						}
						else if(sourceExcitationTypeIndex == sourceNeuronExcitationType)
						{
						}
						else
						{
							excitationTypeRequirements = false;
						}
						if(targetExcitationTypeIndex == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN)
						{
						}
						else if(targetExcitationTypeIndex == targetNeuronExcitationType)
						{
						}
						else
						{
							excitationTypeRequirements = false;
						}
						#endif
	

						vec posSource;
						vec posTarget; 
						#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL
						posSource.x = SHAREDvars.convertStringToInt((*localConnectomeNeuronSource)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_X]);
						posSource.y = SHAREDvars.convertStringToInt((*localConnectomeNeuronSource)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Y]);
						posSource.z = SHAREDvars.convertStringToInt((*localConnectomeNeuronSource)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Z]);
						posTarget.x = SHAREDvars.convertStringToInt((*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_X]);
						posTarget.y = SHAREDvars.convertStringToInt((*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_X]);
						posTarget.z = SHAREDvars.convertStringToInt((*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_X]);						
						#endif
						vec sourceNeuronCorticalFlowVector;
						#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL_NORMALISE_LOCAL_CONNECTIVITY
						if(readMode == READ_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL)
						{
							sourceNeuronCorticalFlowVector.x = SHAREDvars.convertStringToInt((*localConnectomeNeuronSource)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER_SURFACE_NORM_X]);
							sourceNeuronCorticalFlowVector.y = SHAREDvars.convertStringToInt((*localConnectomeNeuronSource)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER_SURFACE_NORM_Y]);
						}
						#endif
									
						if(layerRequirements && excitationTypeRequirements)
						{
							//only count local connectome connections (not external connections);
							incrementNumberOfConnections(&(connectivityModel.neuronModelConnectionsLocalConnectome), &posSource, &posTarget, &sourceNeuronCorticalFlowVector);
							#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_NUMBER_EXCITATORY_INHIBITORY_CONNECTIONS
							if(excitationType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY)
							{
								incrementNumberOfConnections(&(connectivityModel.neuronModelConnectionsLocalConnectomeExcitatory), &posSource, &posTarget, &sourceNeuronCorticalFlowVector);
							}
							else if(excitationType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY)
							{
								incrementNumberOfConnections(&(connectivityModel.neuronModelConnectionsLocalConnectomeInhibitory), &posSource, &posTarget, &sourceNeuronCorticalFlowVector);
							}
							#endif
							#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_NUMBER_EXCITATORY_INHIBITORY_NEURONS
							//count unique neurons
							if(independentSourceNeuronMap.count(sourceNeuronID) == 0)	//verify that sourceNeuronID is not in independentSourceNeuronMap
							{
								independentSourceNeuronMap[sourceNeuronID] = (*neuronMap)[sourceNeuronID];
								connectivityModel.numberOfLocalConnectomeNeurons++;
								if(sourceNeuronExcitationType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY)
								{
									connectivityModel.numberOfLocalConnectomeNeuronsExcitatory++;
								}
								else if(sourceNeuronExcitationType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY)
								{
									connectivityModel.numberOfLocalConnectomeNeuronsInhibitory++;
								}
							}
							#endif
						}
					}
					
					#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL	
					//not currently required;
					averageGaussianQuad(&(connectivityModel.neuronModelConnectionsLocalConnectome));
					averageGaussianQuad(&(connectivityModel.neuronModelConnectionsLocalConnectomeExcitatory));
					averageGaussianQuad(&(connectivityModel.neuronModelConnectionsLocalConnectomeInhibitory));
					#endif
					
					bool countNumberInhibitoryExcitatoryNeurons = false;
					#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_NUMBER_EXCITATORY_INHIBITORY_NEURONS
					countNumberInhibitoryExcitatoryNeurons = true;
					#endif
		
					cout << "\t numberConnectionsLocalConnectome = " << connectivityModel.neuronModelConnectionsLocalConnectome.numberConnections << endl;
					#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_NUMBER_EXCITATORY_INHIBITORY_CONNECTIONS
					cout << "\t\t numberConnectionsLocalConnectomeExcitatory = " << connectivityModel.neuronModelConnectionsLocalConnectomeExcitatory.numberConnections << endl;
					cout << "\t\t numberConnectionsLocalConnectomeInhibitory = " << connectivityModel.neuronModelConnectionsLocalConnectomeInhibitory.numberConnections << endl;
					#endif		
					#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_NUMBER_EXCITATORY_INHIBITORY_NEURONS			
					cout << "\t\t numberOfLocalConnectomeNeurons = " << connectivityModel.numberOfLocalConnectomeNeurons << endl;
					cout << "\t\t numberOfLocalConnectomeNeuronsExcitatory = " << connectivityModel.numberOfLocalConnectomeNeuronsExcitatory << endl;
					cout << "\t\t numberOfLocalConnectomeNeuronsInhibitory = " << connectivityModel.numberOfLocalConnectomeNeuronsInhibitory << endl;
					#endif		
					//printNumberOfConnectionsLocal(queryByPresynapticConnectionNeurons, &connectivityModel, countNumberInhibitoryExcitatoryNeurons, true, false);
				
				#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_BY_EXCITATION_TYPE
				}
				#endif
			#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_BY_EXCITATION_TYPE
			}
			#endif
		#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_BY_LAYER
		}
		#endif
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_BY_LAYER
	}
	#endif
	
	return result;
}

void LCindexedCSVdatabaseReadLocalConnectomeClass::countConnectionsLocal(const int readMode, vector<string>* neuronList, map<string, int>* neuronMap, map<string, int>* connectionsMap, vector<vector<string>>* localConnectomeCSVdatasetNeurons, vector<vector<string>>* localConnectomeCSVdatasetConnections, const bool queryPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, vector<H01connectivityModelClass>* connectivityModelLayers)
{
	bool queryByPresynapticConnectionNeurons = !queryPresynapticConnectionNeurons;
	//if queryByPresynapticConnectionNeurons, calculate excitation type based on localConnectomeNeuronSource (else based on localConnectomeNeuronTarget)

	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_NUMBER_EXCITATORY_INHIBITORY_NEURONS
	for(int i=0; i<localConnectomeCSVdatasetNeurons->size(); i++)
	{
		vector<string>* localConnectomeNeuron = &((*localConnectomeCSVdatasetNeurons)[i]);
		int localConnectomeNeuronLayer = SHAREDvars.convertStringToInt((*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER]);
		//cout << "neuronTraceValue = " << neuronTraceValue << endl;
	
		//if draw but not trace inhibitory connections need to make sure then need to deactivate previous inhibitory neurons
		string neuronType = (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE];
		int neuronExcitationType = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN;
		if(neuronType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_PYRAMIDAL)
		{
			neuronExcitationType = LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY;
		}
		else if(neuronType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_INTERNEURON)
		{
			neuronExcitationType = LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY;
		}
	
		addConnectionNumberInhibitoryExcitatoryNeurons(neuronExcitationType, localConnectomeNeuronLayer, connectivityModelLayers);
	}
	#endif
	
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_CONTRALATERAL
	int numberOfContralateralConnections = 0;
	int numberOfContralateralContralateralConnections = 0;
	int numberOfContralateralContralateralConnectionsRecurrent = 0;
	int numberOfContralateralContralateralConnectionsNonrecurrent = 0;
	multimap<string, string> connectionsMultimap;
	for(int i=0;i<localConnectomeCSVdatasetConnections->size(); i++)
	{
		vector<string> localConnectomeConnection = (*localConnectomeCSVdatasetConnections)[i];
		string connectionsMapKey = (localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_ID];
		string connectionsMapValue = (localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_ID];
		connectionsMultimap.insert(std::pair<string,string>(connectionsMapKey, connectionsMapValue));
	}
	double xCalibratedAverage = 0.0;
	for(int i=0;i<localConnectomeCSVdatasetNeurons->size(); i++)
	{
		vector<string> localConnectionCSVdatasetNeuron = (*localConnectomeCSVdatasetNeurons)[i];
		double xCalibrated = LCindexedCSVdatabaseOperations.calibrateCoordinateX(SHAREDvars.convertStringToDouble(localConnectionCSVdatasetNeuron[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_X]));
		xCalibratedAverage = xCalibratedAverage + xCalibrated;
	}
	xCalibratedAverage = xCalibratedAverage/localConnectomeCSVdatasetNeurons->size();
	#endif
	
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_MIRROR_TWINS
	map<string, int> mirrorTwinsMap;
	LCindexedCSVdatabaseOperations.readPreprocessMirrorTwinsCSVdataset(&mirrorTwinsMap, neuronMap);
	#endif
	
	for(int i=0;i<localConnectomeCSVdatasetConnections->size(); i++)
	{
		//cout << "localConnectomeCSVdatasetConnections: i = " << i << endl;
		vector<string> localConnectomeConnection = (*localConnectomeCSVdatasetConnections)[i];

		string sourceNeuronID = "";
		string targetNeuronID = "";
		string sourceNeuronType = "";
		if(queryByPresynapticConnectionNeurons)
		{
			sourceNeuronType = (localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_TYPE];
			sourceNeuronID = (localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_ID];
			targetNeuronID = (localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_ID];
		}
		else
		{
			sourceNeuronType = (localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_TYPE];
			sourceNeuronID = (localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_ID];
			targetNeuronID = (localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_ID];
		}
		
		int neuronIndexSource = (*neuronMap)[sourceNeuronID];
		vector<string>* localConnectomeNeuronSource = &((*localConnectomeCSVdatasetNeurons)[neuronIndexSource]);
		int neuronIndexTarget = (*neuronMap)[targetNeuronID];
		vector<string>* localConnectomeNeuronTarget = &((*localConnectomeCSVdatasetNeurons)[neuronIndexTarget]);

		int excitationType = calculateConnectionExcitationType(connectionTypesDerivedFromPresynapticNeuronsOrEMimages, &localConnectomeConnection, localConnectomeNeuronSource);

		int localConnectomeNeuronLayer;
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
		localConnectomeNeuronLayer = SHAREDvars.convertStringToInt((*localConnectomeNeuronSource)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER]);
		//cout << "localConnectomeNeuronLayer = " << localConnectomeNeuronLayer << endl;
		#endif

		#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_CONTRALATERAL
		if(!queryPresynapticConnectionNeurons)
		{
			bool connectionContralateral = LCindexedCSVdatabaseOperations.isConnectionContralateral(&localConnectomeConnection, xCalibratedAverage);
			if(connectionContralateral)
			{
				numberOfContralateralConnections++;
				string connectionsMapKey = targetNeuronID;
				auto range = connectionsMultimap.equal_range(connectionsMapKey);
				bool hasContralateralContralateralConnections = false;
				bool hasContralateralContralateralConnectionsNonrecurrent = false;
				bool hasContralateralContralateralConnectionsRecurrent = false;
				if(range.first != connectionsMultimap.end())
				{
					for(auto it = range.first; it != range.second; ++it) 
					{
						string targetNeuronID2 = it->second;
						string connectionConnectionsMapKey2 = targetNeuronID + targetNeuronID2;
						if(it->first != targetNeuronID)
						{
							cerr << "LCindexedCSVdatabaseReadLocalConnectomeClass::countConnectionsLocal error: (it->first != targetNeuronID)" << endl;
							cerr << "it->first = " << it->first << endl;
							cerr << "targetNeuronID = " << targetNeuronID << endl;
							cerr << "it->second  = " << it->second << endl;
							exit(EXIT_ERROR);
						}
						if(connectionsMap->count(connectionConnectionsMapKey2) == 0)
						{
							cerr << "LCindexedCSVdatabaseReadLocalConnectomeClass::countConnectionsLocal error: (connectionsMap->count(connectionConnectionsMapKey2) == 0)" << endl;
							exit(EXIT_ERROR);
						}
						int i2 = (*connectionsMap)[connectionConnectionsMapKey2];
						vector<string> localConnectomeConnection2 = (*localConnectomeCSVdatasetConnections)[i2];
						bool connectionContralateral2 = LCindexedCSVdatabaseOperations.isConnectionContralateral(&localConnectomeConnection2, xCalibratedAverage);
    					if(connectionContralateral2)
						{
							hasContralateralContralateralConnections = true;
							if(targetNeuronID2 == sourceNeuronID)	//recurrent
							{
								hasContralateralContralateralConnectionsRecurrent = true;
							}
							else
							{
								hasContralateralContralateralConnectionsNonrecurrent = true;
							}
						}
					}
				}
				if(hasContralateralContralateralConnections)
				{
					numberOfContralateralContralateralConnections++;
					if(hasContralateralContralateralConnectionsRecurrent)
					{
						numberOfContralateralContralateralConnectionsRecurrent++;
					}
					else
					{
						if(hasContralateralContralateralConnectionsNonrecurrent)
						{
							numberOfContralateralContralateralConnectionsNonrecurrent++;
						}
					}
				}
			}
		}
		#endif
		
		#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT
		for(int r=0; r<INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_NUMBER_ITERATIONS; r++)
		{ 
			if(r == 0)
			{
				bool foundRecursiveConnection = false;
				string recursiveConnectionConnectionsMapKey;
				if(queryByPresynapticConnectionNeurons)
				{
					recursiveConnectionConnectionsMapKey = targetNeuronID + targetNeuronID;	//CHECKTHIS
				}
				else
				{
					recursiveConnectionConnectionsMapKey = sourceNeuronID + sourceNeuronID;
				}
				if(connectionsMap->count(recursiveConnectionConnectionsMapKey) != 0)	//verify that connectionsMapKey is found
				{
					foundRecursiveConnection = true;
				}
				#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_MIRROR_TWINS
				string mirrorTwinsMapKey = recursiveConnectionConnectionsMapKey;
				if(mirrorTwinsMap.count(mirrorTwinsMapKey) == 0)	//verify that mirrorTwinsMapKey is found
				{
					foundRecursiveConnection = false;
				}
				#endif
				if(foundRecursiveConnection)
				{	
					addRecurrentConnection(r, excitationType, localConnectomeNeuronLayer, connectivityModelLayers);
					#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_NEURONS
					addRecurrentNeuron(r, excitationType, localConnectomeNeuronLayer, connectivityModelLayers, sourceNeuronID);
					#endif
				}			
			}
			#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_MIRROR_TWINS
			else if(r == 1 || r==2)
			#else
			else if(r == 1)
			#endif
			{
				bool foundRecurrentConnection = false;
				string recurrentConnectionConnectionsMapKey;
				bool recurrentConnectionConnectionsMapKeyDirection = queryByPresynapticConnectionNeurons;
				#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_MIRROR_TWINS
				if(r==2)
				{
					recurrentConnectionConnectionsMapKeyDirection = !recurrentConnectionConnectionsMapKeyDirection;	//just verify that a single connection exists between the two mirror pair neurons
				}
				#endif
				if(recurrentConnectionConnectionsMapKeyDirection)
				{
					recurrentConnectionConnectionsMapKey = targetNeuronID + sourceNeuronID;	//find recurrent connection part 2 from target back to source
				}
				else
				{
					recurrentConnectionConnectionsMapKey = sourceNeuronID + targetNeuronID;	//find recurrent connection part 1 from source to target	//CHECKTHIS - connectionsMap is always defined from pre_id to post_id
				}
				if(connectionsMap->count(recurrentConnectionConnectionsMapKey) != 0)	//verify that connectionsMapKey is found
				{
					foundRecurrentConnection = true;
				}
				#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_MIRROR_TWINS
				string mirrorTwinsMapKey = recurrentConnectionConnectionsMapKey;
				if(mirrorTwinsMap.count(mirrorTwinsMapKey) == 0)	//verify that mirrorTwinsMapKey is found
				{
					foundRecurrentConnection = false;
				}
				#endif
				if(foundRecurrentConnection)
				{	
					addRecurrentConnection(r, excitationType, localConnectomeNeuronLayer, connectivityModelLayers);
					#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_NEURONS
					addRecurrentNeuron(r, excitationType, localConnectomeNeuronLayer, connectivityModelLayers, sourceNeuronID);
					#endif
				}
			}
		}
		#endif
		
		vec posSource;
		vec posTarget; 
		vec sourceNeuronCorticalFlowVector;
		vec* posTargetPointer = &posTarget;
		
		#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL			
		if(readMode == READ_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL)
		{
			posSource.x = SHAREDvars.convertStringToInt((*localConnectomeNeuronSource)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_X]);
			posSource.y = SHAREDvars.convertStringToInt((*localConnectomeNeuronSource)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Y]);
			posSource.z = SHAREDvars.convertStringToInt((*localConnectomeNeuronSource)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Z]);
			bool targetNeuronIsInLocalConnectome = true;
			if(targetNeuronIsInLocalConnectome)
			{//verify that targetNeuronID centroid/soma is also in local connectome;
				posTarget.x = SHAREDvars.convertStringToInt((*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_X]);
				posTarget.y = SHAREDvars.convertStringToInt((*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Y]);
				posTarget.z = SHAREDvars.convertStringToInt((*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Z]);
				#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PRINT_DISTANCES
				//double distance = SHAREDvector.calculateTheDistanceBetweenTwoPointsXYOnly(&posSource, &posTarget); 	//or calculateTheDistanceBetweenTwoPoints
				vec vectorSourceToTarget;
				vectorSourceToTarget.x = posTarget.x - posSource.x;
				vectorSourceToTarget.y = posTarget.y - posSource.y;
				cout << "vectorSourceToTarget.x = " << vectorSourceToTarget.x << endl;
				cout << "vectorSourceToTarget.y = " << vectorSourceToTarget.y << endl;
				#endif	
			}
			else
			{
				posTargetPointer = NULL;	//tell incrementNumberOfConnections that targetNeuronID centroid/soma is not in local connectome
			}	
		
			#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL_NORMALISE_LOCAL_CONNECTIVITY
			sourceNeuronCorticalFlowVector.x = SHAREDvars.convertStringToInt((*localConnectomeNeuronSource)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER_SURFACE_NORM_X]);
			sourceNeuronCorticalFlowVector.y = SHAREDvars.convertStringToInt((*localConnectomeNeuronSource)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER_SURFACE_NORM_Y]);
			#endif
		}
		#endif
	
		addConnection(excitationType, localConnectomeNeuronLayer, connectivityModelLayers, &posSource, posTargetPointer, &sourceNeuronCorticalFlowVector);
	}
	
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_CONTRALATERAL
	cout << "\nINDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_CONTRALATERAL:" << endl;
	cout << "numberOfContralateralConnections = " << numberOfContralateralConnections << endl;
	cout << "numberOfContralateralContralateralConnections = " << numberOfContralateralContralateralConnections << endl;
	cout << "numberOfContralateralContralateralConnectionsRecurrent = " << numberOfContralateralContralateralConnectionsRecurrent << endl;
	cout << "numberOfContralateralContralateralConnectionsNonrecurrent = " << numberOfContralateralContralateralConnectionsNonrecurrent << endl;
	#endif
	
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_NEURONS
	for(int layerIndex=0; layerIndex<=CORTICAL_LAYER_NUMBER_OF_LAYERS; layerIndex++)
	{
		for(int r=0; r<INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_NUMBER_ITERATIONS; r++)
		{
			(*connectivityModelLayers)[layerIndex].numberOfNeuronsWithRecurrentConnections[r] = (*connectivityModelLayers)[layerIndex].neuronsWithRecurrentConnectionMap[r].size();
			(*connectivityModelLayers)[layerIndex].numberOfNeuronsWithRecurrentConnectionsExcitatory[r] = (*connectivityModelLayers)[layerIndex].neuronsWithRecurrentConnectionExcitatoryMap[r].size();
			(*connectivityModelLayers)[layerIndex].numberOfNeuronsWithRecurrentConnectionsInhibitory[r] = (*connectivityModelLayers)[layerIndex].neuronsWithRecurrentConnectionInhibitoryMap[r].size();
		}
	}
	#endif
}

#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_NUMBER_EXCITATORY_INHIBITORY_NEURONS
void LCindexedCSVdatabaseReadLocalConnectomeClass::addConnectionNumberInhibitoryExcitatoryNeurons(const bool excitationType, const int localConnectomeNeuronLayer, vector<H01connectivityModelClass>* connectivityModelLayers)
{
	(*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].numberOfLocalConnectomeNeurons++;	//(*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].numberOfLocalConnectomeNeurons = (*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].numberOfLocalConnectomeNeurons+1;
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
	(*connectivityModelLayers)[localConnectomeNeuronLayer].numberOfLocalConnectomeNeurons++;
	#endif
			
	//count outgoing or incoming connections (depending on queryByPresynapticConnectionNeurons or !queryByPresynapticConnectionNeurons):
	if(excitationType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY)
	{
		(*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].numberOfLocalConnectomeNeuronsExcitatory++;
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
		(*connectivityModelLayers)[localConnectomeNeuronLayer].numberOfLocalConnectomeNeuronsExcitatory++;
		#endif
	}
	else if(excitationType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY)
	{
		(*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].numberOfLocalConnectomeNeuronsInhibitory++;
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
		(*connectivityModelLayers)[localConnectomeNeuronLayer].numberOfLocalConnectomeNeuronsInhibitory++;
		#endif
	}
	else
	{
		//excitationType = LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN;
		cerr << "LCindexedCSVdatabaseQueryClass::addConnectionNumberInhibitoryExcitatoryNeurons warning, excitationType unknown, excitationType = " << excitationType << endl;
	}
}
#endif

void LCindexedCSVdatabaseReadLocalConnectomeClass::addConnection(const bool excitationTypeConnection, const int localConnectomeNeuronLayer, vector<H01connectivityModelClass>* connectivityModelLayers, const vec* posSource, const vec* posTarget, const vec* sourceNeuronCorticalFlowVector)
{	
	incrementNumberOfConnections(&(*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].neuronModelConnectionsLocalConnectome, posSource, posTarget, sourceNeuronCorticalFlowVector);
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
	incrementNumberOfConnections(&(*connectivityModelLayers)[localConnectomeNeuronLayer].neuronModelConnectionsLocalConnectome, posSource, posTarget, sourceNeuronCorticalFlowVector);
	#endif	

	if(excitationTypeConnection == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY)
	{
		incrementNumberOfConnections(&(*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].neuronModelConnectionsLocalConnectomeExcitatory, posSource, posTarget, sourceNeuronCorticalFlowVector);
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
		incrementNumberOfConnections(&(*connectivityModelLayers)[localConnectomeNeuronLayer].neuronModelConnectionsLocalConnectomeExcitatory, posSource, posTarget, sourceNeuronCorticalFlowVector);
		#endif
	}
	else if(excitationTypeConnection == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY)
	{
		incrementNumberOfConnections(&(*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].neuronModelConnectionsLocalConnectomeInhibitory, posSource, posTarget, sourceNeuronCorticalFlowVector);
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
		incrementNumberOfConnections(&(*connectivityModelLayers)[localConnectomeNeuronLayer].neuronModelConnectionsLocalConnectomeInhibitory, posSource, posTarget, sourceNeuronCorticalFlowVector);
		#endif
	}
	else
	{
		cerr << "LCindexedCSVdatabaseQueryClass::countConnections warning (preAndPostSynapticNeuronAreInLocalConnectome), excitationType type unknown, excitationTypeConnection = " << excitationTypeConnection << endl;
	}
}

#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT
void LCindexedCSVdatabaseReadLocalConnectomeClass::addRecurrentConnection(const int r, const bool excitationTypeConnection, const int localConnectomeNeuronLayer, vector<H01connectivityModelClass>* connectivityModelLayers)
{	
	vec posSource;	//not used for recurrent connections
	vec posTarget; 	//not used for recurrent connections
	vec sourceNeuronCorticalFlowVector;	//not used for recurrent connections

	incrementNumberOfConnections(&(*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].neuronModelConnectionsLocalConnectomeRecurrent[r], &posSource, &posTarget, &sourceNeuronCorticalFlowVector);
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
	incrementNumberOfConnections(&(*connectivityModelLayers)[localConnectomeNeuronLayer].neuronModelConnectionsLocalConnectomeRecurrent[r], &posSource, &posTarget, &sourceNeuronCorticalFlowVector);
	#endif
	if(excitationTypeConnection)
	{
		incrementNumberOfConnections(&(*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].neuronModelConnectionsLocalConnectomeExcitatoryRecurrent[r], &posSource, &posTarget, &sourceNeuronCorticalFlowVector);
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
		incrementNumberOfConnections(&(*connectivityModelLayers)[localConnectomeNeuronLayer].neuronModelConnectionsLocalConnectomeExcitatoryRecurrent[r], &posSource, &posTarget, &sourceNeuronCorticalFlowVector);
		#endif
	}
	else
	{
		incrementNumberOfConnections(&(*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].neuronModelConnectionsLocalConnectomeInhibitoryRecurrent[r], &posSource, &posTarget, &sourceNeuronCorticalFlowVector);	
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
		incrementNumberOfConnections(&(*connectivityModelLayers)[localConnectomeNeuronLayer].neuronModelConnectionsLocalConnectomeInhibitoryRecurrent[r], &posSource, &posTarget, &sourceNeuronCorticalFlowVector);
		#endif
	}
}

#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_NEURONS
void LCindexedCSVdatabaseReadLocalConnectomeClass::addRecurrentNeuron(const int r, const bool excitationTypeConnection, const int localConnectomeNeuronLayer, vector<H01connectivityModelClass>* connectivityModelLayers, string sourceNeuronID)
{
	addRecurrentConnectionToNeuronMap(&(*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].neuronsWithRecurrentConnectionMap[r], sourceNeuronID);
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
	addRecurrentConnectionToNeuronMap(&(*connectivityModelLayers)[localConnectomeNeuronLayer].neuronsWithRecurrentConnectionMap[r], sourceNeuronID);
	#endif
	if(excitationTypeConnection)
	{
		addRecurrentConnectionToNeuronMap(&(*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].neuronsWithRecurrentConnectionExcitatoryMap[r], sourceNeuronID);
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
		addRecurrentConnectionToNeuronMap(&(*connectivityModelLayers)[localConnectomeNeuronLayer].neuronsWithRecurrentConnectionExcitatoryMap[r], sourceNeuronID);
		#endif
	}
	else
	{
		addRecurrentConnectionToNeuronMap(&(*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].neuronsWithRecurrentConnectionInhibitoryMap[r], sourceNeuronID);	
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
		addRecurrentConnectionToNeuronMap(&(*connectivityModelLayers)[localConnectomeNeuronLayer].neuronsWithRecurrentConnectionInhibitoryMap[r], sourceNeuronID);
		#endif
	}
}	

void LCindexedCSVdatabaseReadLocalConnectomeClass::addRecurrentConnectionToNeuronMap(map<string, int>* neuronsWithRecurrentConnectionMap, const string sourceNeuronID)
{
	int numberOfRecurrentConnections;
	if(neuronsWithRecurrentConnectionMap->count(sourceNeuronID) != 0)
	{
		numberOfRecurrentConnections = (*neuronsWithRecurrentConnectionMap)[sourceNeuronID] + 1;
	}
	else
	{
		numberOfRecurrentConnections = 1;
	}
	(*neuronsWithRecurrentConnectionMap)[sourceNeuronID] = numberOfRecurrentConnections;
}	
#endif
#endif



void LCindexedCSVdatabaseReadLocalConnectomeClass::printNumberOfConnectionsLayersLocal(const bool queryByPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, constEffective vector<H01connectivityModelClass>* connectivityModelLayers)
{
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
	int numberLayers = CORTICAL_LAYER_NUMBER_OF_LAYERS;
	for(int layerIndex=0; layerIndex<=CORTICAL_LAYER_NUMBER_OF_LAYERS; layerIndex++)
	{
	#else
	int layerIndex = 0;	//all layers
	#endif

		H01connectivityModelClass* connectivityModel = &((*connectivityModelLayers)[layerIndex]);

		if(layerIndex == 0)
		{
			cout << "\nAll layers: " << endl;
		}
		else
		{
			cout << "\nlayerIndex = " << layerIndex << endl;
		}
			
		printNumberOfConnectionsLocal(layerIndex, queryByPresynapticConnectionNeurons, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, connectivityModel);

	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
	}
	#endif
}
void LCindexedCSVdatabaseReadLocalConnectomeClass::printNumberOfConnectionsLocal(const int layerIndex, const bool queryByPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, const H01connectivityModelClass* connectivityModel)
{
	/*
	This analysis aims to map the distribution of synaptic connections of local connectome neurons to local vs non-local neurons.
		This requires analysing the connections from the local connectome neurons to the non-local neurons in the C3 synaptic connections database.
		For each of the neuron centroids in the local connectome, estimate the number of incoming/outgoing synapses;
			a) to/from nearby neurons whose somas/centroids reside in/outside of the H01 dataset, versus those;
			b) to/from distant neurons (eg via long range axonal tracts/highways). 
		It is estimated that;
			a) should approximately equal the number of connections in the local connectome * normalisationFactor, where normalisationFactor = ((xRange+yRange)/2)/zRange, based on;
				i) an extrapolation of the z local connectome field to a similar distance as the local x/y coordinate range; and in which it is assumed;
				ii) the existent x/y coordinates range is sufficiently high (~1mm) to capture the full/majority expanse of a typical pyramidal/interneuron dendritic/axonal tree along these axes. 
			b) is simply the total number of synapses of a given local connectome neuron minus a) 
		There may be alternate/preestablished techniques for inferring the ratio of proximal/local versus distal connectivity in the H01 connectome.
		Rationale summary: for neurons whose centroids reside in the local connectome; a large number of their synapses in the local connectome (H01 release) will not be identified as being connected to neurons which reside in the local vicinity because the Z slice is so small (compared to the X/Y slices, which capture the typical width of a pyramidal dendritic/axonal tree) 
	*/
	
	cout << "\n printNumberOfConnectionsLocal: queryByPresynapticConnectionNeurons (F:incoming/T:outgoing) = " << SHAREDvars.convertBoolToString(queryByPresynapticConnectionNeurons) << endl;
	
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_NORMALISE_Z
	double normalisationFactorZ = LCindexedCSVdatabaseCalculateNeuronLayer.getZNormalisationFactor();
	#endif
	
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_NUMBER_EXCITATORY_INHIBITORY_NEURONS
	{
		cout << "INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_NUMBER_EXCITATORY_INHIBITORY_NEURONS:" << endl;
		cout << "countLocalConnectomeNeurons:" << endl;
		cout << "\t numberOfLocalConnectomeNeurons = " << connectivityModel->numberOfLocalConnectomeNeurons << endl;
		cout << "\t numberOfLocalConnectomeNeuronsExcitatory = " << connectivityModel->numberOfLocalConnectomeNeuronsExcitatory << endl;
		cout << "\t numberOfLocalConnectomeNeuronsInhibitory = " << connectivityModel->numberOfLocalConnectomeNeuronsInhibitory << endl;
		cout << "\t fractionOfLocalConnectomeNeuronsExcitatory = " << double(connectivityModel->numberOfLocalConnectomeNeuronsExcitatory) / double(connectivityModel->numberOfLocalConnectomeNeurons) << endl;
		cout << "\t fractionOfLocalConnectomeNeuronsInhibitory = " << double(connectivityModel->numberOfLocalConnectomeNeuronsInhibitory) / double(connectivityModel->numberOfLocalConnectomeNeurons) << endl;
	}
	#endif	
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_NEURONS
	{
		for(int r=0; r<INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_NUMBER_ITERATIONS; r++)
		{
			cout << "INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_NEURONS (r = " << r << "):" << endl;
			cout << "\t numberOfNeuronsWithRecurrentConnections = " << connectivityModel->numberOfNeuronsWithRecurrentConnections[r] << endl;
			cout << "\t numberOfNeuronsWithRecurrentConnectionsExcitatory = " << connectivityModel->numberOfNeuronsWithRecurrentConnectionsExcitatory[r] << endl;
			cout << "\t numberOfNeuronsWithRecurrentConnectionsInhibitory = " << connectivityModel->numberOfNeuronsWithRecurrentConnectionsInhibitory[r] << endl;	
			#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_NORMALISE_Z
			double numberOfNeuronsWithRecurrentConnectionsNormalisedZ = double(connectivityModel->numberOfNeuronsWithRecurrentConnections[r]) * normalisationFactorZ;
			double numberOfNeuronsWithRecurrentConnectionsExcitatoryNormalisedZ = double(connectivityModel->numberOfNeuronsWithRecurrentConnectionsExcitatory[r]) * normalisationFactorZ;
			double numberOfNeuronsWithRecurrentConnectionsInhibitoryNormalisedZ = double(connectivityModel->numberOfNeuronsWithRecurrentConnectionsInhibitory[r]) * normalisationFactorZ;
			cout << "\t numberOfNeuronsWithRecurrentConnectionsNormalisedZ = " << numberOfNeuronsWithRecurrentConnectionsNormalisedZ << endl;
			cout << "\t numberOfNeuronsWithRecurrentConnectionsExcitatoryNormalisedZ = " << numberOfNeuronsWithRecurrentConnectionsExcitatoryNormalisedZ << endl;
			cout << "\t numberOfNeuronsWithRecurrentConnectionsInhibitoryNormalisedZ = " << numberOfNeuronsWithRecurrentConnectionsInhibitoryNormalisedZ << endl;
			#endif
		}
	}
	#endif	
		
	cout << "INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS:" << endl;
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_NORMALISE_Z
	double numberConnectionsLocalConnectomeNormalisedZ = 0.0;
	double numberConnectionsLocalConnectomeExcitatoryNormalisedZ = 0.0;
	double numberConnectionsLocalConnectomeInhibitoryNormalisedZ = 0.0;
	#endif
	{
		cout << "countInternalConnectomeConnections:" << endl;
		cout << "\t numberConnectionsLocalConnectome = " << connectivityModel->neuronModelConnectionsLocalConnectome.numberConnections << endl;
		cout << "\t numberConnectionsLocalConnectomeExcitatory = " << connectivityModel->neuronModelConnectionsLocalConnectomeExcitatory.numberConnections << endl;
		cout << "\t numberConnectionsLocalConnectomeInhibitory = " << connectivityModel->neuronModelConnectionsLocalConnectomeInhibitory.numberConnections << endl;
		cout << "\t\t fractionOfConnectionsLocalConnectomeExcitatory = " << double(connectivityModel->neuronModelConnectionsLocalConnectomeExcitatory.numberConnections) / double(connectivityModel->neuronModelConnectionsLocalConnectome.numberConnections) << endl;
		cout << "\t\t fractionOfConnectionsLocalConnectomeInhibitory = " << double(connectivityModel->neuronModelConnectionsLocalConnectomeInhibitory.numberConnections) / double(connectivityModel->neuronModelConnectionsLocalConnectome.numberConnections) << endl;
		#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_NORMALISE_Z
		numberConnectionsLocalConnectomeNormalisedZ = double(connectivityModel->neuronModelConnectionsLocalConnectome.numberConnections) * normalisationFactorZ;
		numberConnectionsLocalConnectomeExcitatoryNormalisedZ = double(connectivityModel->neuronModelConnectionsLocalConnectomeExcitatory.numberConnections) * normalisationFactorZ;
		numberConnectionsLocalConnectomeInhibitoryNormalisedZ = double(connectivityModel->neuronModelConnectionsLocalConnectomeInhibitory.numberConnections) * normalisationFactorZ;
		cout << "\t normalisationFactorZ = " << normalisationFactorZ << endl;
		cout << "\t numberConnectionsLocalConnectomeNormalisedZ = " << numberConnectionsLocalConnectomeNormalisedZ << endl;
		cout << "\t numberConnectionsLocalConnectomeExcitatoryNormalisedZ = " << numberConnectionsLocalConnectomeExcitatoryNormalisedZ << endl;
		cout << "\t numberConnectionsLocalConnectomeInhibitoryNormalisedZ = " << numberConnectionsLocalConnectomeInhibitoryNormalisedZ << endl;
		#endif
	}
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PER_NEURON		
	{
		cout << "INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PER_NEURON:" << endl;
		{
			cout << "countInternalConnectomeConnections per neuron:" << endl;
			double numberConnectionsLocalConnectomePerNeuron = connectivityModel->neuronModelConnectionsLocalConnectome.numberConnections/double(connectivityModel->numberOfLocalConnectomeNeurons);
			double numberConnectionsLocalConnectomePerNeuronExcitatory = connectivityModel->neuronModelConnectionsLocalConnectomeExcitatory.numberConnections/double(connectivityModel->numberOfLocalConnectomeNeuronsExcitatory);
			double numberConnectionsLocalConnectomePerNeuronInhibitory = connectivityModel->neuronModelConnectionsLocalConnectomeInhibitory.numberConnections/double(connectivityModel->numberOfLocalConnectomeNeuronsInhibitory);
			cout << "\t numberConnectionsLocalConnectomePerNeuron = " << numberConnectionsLocalConnectomePerNeuron << endl;
			cout << "\t numberConnectionsLocalConnectomePerNeuronExcitatory = " << numberConnectionsLocalConnectomePerNeuronExcitatory << endl;
			cout << "\t numberConnectionsLocalConnectomePerNeuronInhibitory = " << numberConnectionsLocalConnectomePerNeuronInhibitory << endl;
			#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_NORMALISE_Z
			double numberConnectionsLocalConnectomePerNeuronNormalisedZ = numberConnectionsLocalConnectomeNormalisedZ/double(connectivityModel->numberOfLocalConnectomeNeurons);
			double numberConnectionsLocalConnectomePerNeuronExcitatoryNormalisedZ = numberConnectionsLocalConnectomeExcitatoryNormalisedZ/double(connectivityModel->numberOfLocalConnectomeNeuronsExcitatory);
			double numberConnectionsLocalConnectomePerNeuronInhibitoryNormalisedZ = numberConnectionsLocalConnectomeInhibitoryNormalisedZ/double(connectivityModel->numberOfLocalConnectomeNeuronsInhibitory);
			cout << "\t numberConnectionsLocalConnectomePerNeuronNormalisedZ = " << numberConnectionsLocalConnectomePerNeuronNormalisedZ << endl;
			cout << "\t numberConnectionsLocalConnectomePerNeuronExcitatoryNormalisedZ = " << numberConnectionsLocalConnectomePerNeuronExcitatoryNormalisedZ << endl;
			cout << "\t numberConnectionsLocalConnectomePerNeuronInhibitoryNormalisedZ = " << numberConnectionsLocalConnectomePerNeuronInhibitoryNormalisedZ << endl;
			#endif
		}
	}
	#endif
		
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT
	{
		for(int r=0; r<INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_NUMBER_ITERATIONS; r++)
		{
			cout << "INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT (r = " << r << "):" << endl;
			cout << "\t numberConnectionsLocalConnectomeRecurrent = " << connectivityModel->neuronModelConnectionsLocalConnectomeRecurrent[r].numberConnections << endl;
			cout << "\t numberConnectionsLocalConnectomeExcitatoryRecurrent = " << connectivityModel->neuronModelConnectionsLocalConnectomeExcitatoryRecurrent[r].numberConnections << endl;
			cout << "\t numberConnectionsLocalConnectomeInhibitoryRecurrent = " << connectivityModel->neuronModelConnectionsLocalConnectomeInhibitoryRecurrent[r].numberConnections << endl;
			cout << "\t\t fractionOfConnectionsLocalConnectomeExcitatoryRecurrent = " << double(connectivityModel->neuronModelConnectionsLocalConnectomeExcitatoryRecurrent[r].numberConnections) / double(connectivityModel->neuronModelConnectionsLocalConnectomeRecurrent[r].numberConnections) << endl;
			cout << "\t\t fractionOfConnectionsLocalConnectomeInhibitoryRecurrent = " << double(connectivityModel->neuronModelConnectionsLocalConnectomeInhibitoryRecurrent[r].numberConnections) / double(connectivityModel->neuronModelConnectionsLocalConnectomeRecurrent[r].numberConnections) << endl;
			#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_NORMALISE_Z
			double numberConnectionsLocalConnectomeRecurrentNormalisedZ = double(connectivityModel->neuronModelConnectionsLocalConnectomeRecurrent[r].numberConnections) * normalisationFactorZ;
			double numberConnectionsLocalConnectomeExcitatoryRecurrentNormalisedZ = double(connectivityModel->neuronModelConnectionsLocalConnectomeExcitatoryRecurrent[r].numberConnections) * normalisationFactorZ;
			double numberConnectionsLocalConnectomeInhibitoryRecurrentNormalisedZ = double(connectivityModel->neuronModelConnectionsLocalConnectomeInhibitoryRecurrent[r].numberConnections) * normalisationFactorZ;
			cout << "\t numberConnectionsLocalConnectomeRecurrentNormalisedZ = " << numberConnectionsLocalConnectomeRecurrentNormalisedZ << endl;
			cout << "\t numberConnectionsLocalConnectomeExcitatoryRecurrentNormalisedZ = " << numberConnectionsLocalConnectomeExcitatoryRecurrentNormalisedZ << endl;
			cout << "\t numberConnectionsLocalConnectomeInhibitoryRecurrentNormalisedZ = " << numberConnectionsLocalConnectomeInhibitoryRecurrentNormalisedZ << endl;
			#endif
		}
	}
	#endif
}

#endif


#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL
void LCindexedCSVdatabaseReadLocalConnectomeClass::generateLargeModelNeuronsAndConnectionsLayers(constEffective vector<H01connectivityModelClass>* connectivityModelLayersIncomingLayers, constEffective vector<H01connectivityModelClass>* connectivityModelLayersOutgoingLayers)
{
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
	int numberLayers = CORTICAL_LAYER_NUMBER_OF_LAYERS;
	for(int layerIndex=0; layerIndex<=CORTICAL_LAYER_NUMBER_OF_LAYERS; layerIndex++)
	{
	#else
	int layerIndex = 0;	//all layers
	#endif

		H01connectivityModelClass* connectivityModelLayersIncoming = &((*connectivityModelLayersIncomingLayers)[layerIndex]);
		H01connectivityModelClass* connectivityModelLayersOutgoing = &((*connectivityModelLayersOutgoingLayers)[layerIndex]);

		if(layerIndex == 0)
		{
			cout << "\nAll layers: " << endl;
		}
		else
		{
			cout << "\nlayerIndex = " << layerIndex << endl;
		}
			
		generateLargeModelNeuronsAndConnections(connectivityModelLayersIncoming, connectivityModelLayersOutgoing);

	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
	}
	#endif
}

void LCindexedCSVdatabaseReadLocalConnectomeClass::generateLargeModelNeuronsAndConnections(const H01connectivityModelClass* connectivityModelLayersIncoming, const H01connectivityModelClass* connectivityModelLayersOutgoing)
{
	/*
	see printNumberOfConnectionsLayers
	*/
}
#endif

int LCindexedCSVdatabaseReadLocalConnectomeClass::calculateConnectionExcitationType(const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, vector<string>* localConnectomeConnection, vector<string>* localConnectomeNeuronSource)
{
	int excitationType;
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_PRESYNAPTIC_NEURONS
	if(connectionTypesDerivedFromPresynapticNeuronsOrEMimages)
	{
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_PRESYNAPTIC_NEURONS_LOOKUP
		excitationType = SHAREDvars.convertStringToInt((*localConnectomeNeuronSource)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE]);
		#else
		excitationType = LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_UNKNOWN;
		if(queryByPresynapticConnectionNeurons)
		{
			presynapticNeuronType = (*localConnectomeNeuronSource)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE];
		}
		else
		{
			presynapticNeuronType = (*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE];
		}
		//cout << "presynapticNeuronType = " << presynapticNeuronType << endl;

		if(presynapticNeuronType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_PYRAMIDAL)
		{
			excitationType = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY;
		}
		else if(presynapticNeuronType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_INTERNEURON)
		{
			excitationType = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY;
		}
		else
		{
			cerr << "LCindexedCSVdatabaseReadLocalConnectomeClass::calculateConnectionExcitationType error: presynapticNeuronType unknown" << endl;
			exit(EXIT_ERROR);
		}
		#endif
	}
	else
	{
	#endif
		excitationType = SHAREDvars.convertStringToInt((*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE]);
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_PRESYNAPTIC_NEURONS
	}
	#endif
	return excitationType;
}
