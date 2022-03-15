/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseQuery.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021-2022 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: H01LocalConnectome
 * Requirements: requires H01 indexed CSV database to have already been generated (see INDEXED_CSV_DATABASE_CREATE: H01indexedCSVdatabaseCreate.cpp/.hpp)
 * Compilation: see H01indexedCSVdatabase.hpp
 * Usage: see H01indexedCSVdatabase.hpp
 * Description: H01 indexed CSV database query - 
 *  INDEXED_CSV_DATABASE_QUERY_EXTRACT_INCOMING_OUTGOING_CONNECTIONS: mode 1 (lookup indexed CSV database by neuron ID, and find incoming/outgoing target connections, and write them to file)
 *  INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING: mode 2 (lookup indexed CSV database by neuron ID, find incoming target connections, and generate visualisation)
 *  INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: mode 3 (automatically generate localConnectomeConnections-typesFromPresynapticNeurons.csv/localConnectomeConnections-typesFromEMimages.csv from localConnectomeNeurons.csv and indexed CSV database)
 *  INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS: mode 4 (lookup indexed CSV database by neuron ID, count/infer proportion of incoming/outgoing excitatory/inhibitory target connections to local vs distal neurons)
 *  INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: mode 5 (lookup indexed CSV database by post/pre synaptic connection neuron ID, and identify connection with pre/post synaptic X/Y coordinates (if pre/post synaptic neuron type=UNKNOWN), add pre/post synaptic neuron ID, Z coordinates, and type coordinates to connection dataset [incomplete: awaiting release of H01 Release C3 neurons dataset; will print UNKNOWN neurons (with x/y coordinates only) along with candidate neuron_ids but not reconcile them])
 *  INDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS - mode 6 (crawl indexed CSV database by pre/post synaptic connection neuron ID, and count number of unique axons/dendrites as specified by neuron ID - not explicitly connected to local connectome [incomplete])
 *  INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL - mode 7 (generate large artificial cortical model; extrapolate z region to same size as x/y [incomplete])
 * Input: 
 *  INDEXED_CSV_DATABASE_QUERY_OUTPUT_CONNECTIONS: localConnectomeNeurons.csv - id, x, y, z, type, excitation_type | somas.csv - soma_id, base_seg_id, c2_rep_strict, c2_rep_manual, c3_rep_strict, c3_rep_manual, proofread_104_rep, x, y, z, celltype, layer | localConnectomeNeuronIDlistDistinct.csv - id
 *  INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING: localConnectomeNeurons.csv - id, x, y, z, type, excitation_type | somas.csv - soma_id, base_seg_id, c2_rep_strict, c2_rep_manual, c3_rep_strict, c3_rep_manual, proofread_104_rep, x, y, z, celltype, layer | localConnectomeNeuronIDlistDistinct.csv - id
 *  INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: localConnectomeNeurons.csv - id, x, y, z, type, excitation_type | somas.csv - soma_id, base_seg_id, c2_rep_strict, c2_rep_manual, c3_rep_strict, c3_rep_manual, proofread_104_rep, x, y, z, celltype, layer | localConnectomeNeuronIDlistDistinct.csv - id
 *  INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS: localConnectomeNeurons.csv - id, x, y, z, type, excitation_type | somas.csv - soma_id, base_seg_id, c2_rep_strict, c2_rep_manual, c3_rep_strict, c3_rep_manual, proofread_104_rep, x, y, z, celltype, layer | localConnectomeNeuronIDlistDistinct.csv - id
 *  INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: localConnectomeConnections-typesFromEMimages-useAllValuesAvailableFromInBodyCellConnection.csv - pre_id, pre_x, pre_y, pre_z, pre_type, post_id, post_x, post_y, post_z, post_type, post_class_label, syn_num, excitation_type
 *  INDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS: N/A
 *  INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL: localConnectomeNeurons.csv - id, x, y, z, type, excitation_type | somas.csv - soma_id, base_seg_id, c2_rep_strict, c2_rep_manual, c3_rep_strict, c3_rep_manual, proofread_104_rep, x, y, z, celltype, layer | localConnectomeNeuronIDlistDistinct.csv - id
 * Output Format:
 *  INDEXED_CSV_DATABASE_QUERY_OUTPUT_CONNECTIONS: localConnectomeNeuronIDlistConnectionsPresynaptic.csv/localConnectomeNeuronIDlistConnectionsPostsynaptic.csv - connectionNeuronID1, connectionType1 [, locationObjectContentsXcoordinatesContent1, locationObjectContentsYcoordinatesContent1, locationObjectContentsZcoordinatesContent1], connectionNeuronID2, connectionType2 [, locationObjectContentsXcoordinatesContent2, locationObjectContentsYcoordinatesContent2, locationObjectContentsZcoordinatesContent2], etc 
 *  INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING:
 *      INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_3D_LINEAR_REGRESSION:
 *          INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING_LDR: localConnectomeIncomingAxonMapping.ldr - LDR_REFERENCE_TYPE_LINE ldrawColor plot3DpointStart.x plot3DpointStart.y plot3DpointStart.z plot3DpointEnd.x plot3DpointEnd.y plot3DpointEnd.z
 *          INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING_CSV: localConnectomeIncomingAxonMapping.csv - polyFit.connectionNeuronID, polyFit.estSynapseType, polyFit.origin.x, polyFit.origin.y, polyFit.origin.z, polyFit.axis.x, polyFit.axis.y, polyFit.axis.z
 *      INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_2D_POLY_REGRESSION:
 *          INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING_CSV: localConnectomeIncomingAxonMapping.csv - polyFit.connectionNeuronID, polyFit.estSynapseType, polyFit.a, polyFit.b, polyFit.c
 *  INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: localConnectomeConnections-typesFromPresynapticNeurons/typesFromEMimages.csv - pre_id, pre_x, pre_y, pre_z, pre_type, post_id, post_x, post_y, post_z, post_type, post_class_label, syn_num, excitation_type
 *  INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS: N/A
 *  INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: localConnectomeConnections-typesFromEMimages.csv - pre_id, pre_x, pre_y, pre_z, pre_type, post_id, post_x, post_y, post_z, post_type, post_class_label, syn_num, excitation_type
 *  INDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS: N/A
 *  INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL: localConnectomeConnectionsLargeModel-typesFromPresynapticNeurons/typesFromEMimages.csv - pre_id, pre_x, pre_y, pre_z, pre_type, post_id, post_x, post_y, post_z, post_type, post_class_label, syn_num, excitation_type; localConnectomeNeuronsLargeModel.csv - id, x, y, z, type, excitation_type
 * Comments:
 * /
 *******************************************************************************/

#include "H01indexedCSVdatabaseQuery.hpp"
#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING
#include "H01indexedCSVdatabaseAlgorithms.hpp"
#endif

#ifdef INDEXED_CSV_DATABASE_QUERY

void initialiseGaussianQuad(GaussianQuad* gaussianQuad)
{
	gaussianQuad->numberConnections = 0;
	initialiseQuad(&(gaussianQuad->mean));
	initialiseQuad(&(gaussianQuad->variance));
	initialiseQuad(&(gaussianQuad->count));
}
void initialiseQuad(Quad* q)
{
	q->xPos = 0.0;
	q->yPos = 0.0;
	q->xNeg = 0.0;
	q->yNeg = 0.0;
}

#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING
map<long,vector<H01indexedCSVdatabaseQueryObject>> neuronConnectionMap;	//map<targetNeuronIDindex,vector<H01indexedCSVdatabaseQueryObject [with neuronIDindex]>> 
vector<H01indexedCSVdatabaseAlgorithmsFit> neuronConnectionPolyFitVector;

H01indexedCSVdatabaseQueryObject::H01indexedCSVdatabaseQueryObject(void)
{
	neuronID = 0;	//sourceNeuronID
	type = 0;
	locationObjectContentsXcoordinates = 0;
	locationObjectContentsYcoordinates = 0;
	locationObjectContentsZcoordinates = 0;
	/*
	neuronIDstring = "";
	typeString = "";
	locationObjectContentsXcoordinatesContent = "";
	locationObjectContentsYcoordinatesContent = "";
	locationObjectContentsZcoordinatesContent = "";
	*/
}
H01indexedCSVdatabaseQueryObject::~H01indexedCSVdatabaseQueryObject(void)
{
}
#endif

	
#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS
H01connectivityModelClass::H01connectivityModelClass(void)
{
	numberOfLocalConnectomeNeurons = 0;
	numberOfLocalConnectomeNeuronsExcitatory = 0;
	numberOfLocalConnectomeNeuronsInhibitory = 0;
	initialiseGaussianQuad(&neuronModelConnectionsLocalConnectome);
	initialiseGaussianQuad(&neuronModelConnectionsExternalConnectome);
	initialiseGaussianQuad(&neuronModelConnectionsLocalConnectomeExcitatory);
	initialiseGaussianQuad(&neuronModelConnectionsExternalConnectomeExcitatory);
	initialiseGaussianQuad(&neuronModelConnectionsLocalConnectomeInhibitory);
	initialiseGaussianQuad(&neuronModelConnectionsExternalConnectomeInhibitory);
}
H01connectivityModelClass::~H01connectivityModelClass(void)
{
}
vector<H01connectivityModelClass>* H01indexedCSVdatabaseQueryClass::generateNumberOfConnectionsLayers()
{
	vector<H01connectivityModelClass>* numberConnectionsLayers = new vector<H01connectivityModelClass>(CORTICAL_LAYER_NUMBER_OF_LAYERS+1);
	return numberConnectionsLayers;
}
vector<vector<H01connectivityModelClass>>* H01indexedCSVdatabaseQueryClass::generateNumberOfConnectionsLayersLayers()
{
	vector<vector<H01connectivityModelClass>>* numberConnectionsLayersLayers = new vector<vector<H01connectivityModelClass>>(CORTICAL_LAYER_NUMBER_OF_LAYERS+1, vector<H01connectivityModelClass>(CORTICAL_LAYER_NUMBER_OF_LAYERS+1));
	return numberConnectionsLayersLayers;
}
	
void H01indexedCSVdatabaseQueryClass::incrementNumberOfConnections(GaussianQuad* gaussianQuad, const vec* posSource, const vec* posTarget, const vec* sourceNeuronCorticalFlowVector)
{
	gaussianQuad->numberConnections++;

	#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL	
	incrementGaussianQuad(gaussianQuad, posSource, posTarget, sourceNeuronCorticalFlowVector);
	#endif
}
#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL	
void H01indexedCSVdatabaseQueryClass::incrementGaussianQuad(GaussianQuad* gaussianQuad, const vec* posSource, const vec* posTarget, const vec* sourceNeuronCorticalFlowVector)
{
	if(posTarget != NULL)
	{//verify that targetNeuronID centroid/soma is also in local connectome;
		
		#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL_NORMALISE_LOCAL_CONNECTIVITY
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
void H01indexedCSVdatabaseQueryClass::averageGaussianQuad(GaussianQuad* gaussianQuad)
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

#endif

bool H01indexedCSVdatabaseQueryClass::queryIndexedCSVdatabase(const int queryMode, const string indexed_csv_database_folder, const string local_connectome_folder_base)
{
	bool result = true;

	currentDirectory = SHAREDvars.getCurrentDirectory();
	
	bool na = false;
	
	#ifdef INDEXED_CSV_DATABASE_QUERY_EXTRACT_INCOMING_OUTGOING_CONNECTIONS
	if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_EXTRACT_INCOMING_OUTGOING_CONNECTIONS)
	{
		this->queryIndexedCSVdatabaseByNeuronDatasetOrListFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST_DISTINCT_FILE_NAME, INDEXED_CSV_DATABASE_QUERY_PREFER_DATASET_LOCAL_CONNECTOME_NEURONS_VALUE, QUERY_PRESYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_TRUE, APPEND_FILE_FALSE, INDEXED_CSV_DATABASE_QUERY_EXTRACT_INCOMING_OUTGOING_CONNECTIONS_NEURON_LIST_CONNECTIONS_PRESYNAPTIC_FILE_NAME, na);	//extract incoming connections
		this->queryIndexedCSVdatabaseByNeuronDatasetOrListFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST_DISTINCT_FILE_NAME, INDEXED_CSV_DATABASE_QUERY_PREFER_DATASET_LOCAL_CONNECTOME_NEURONS_VALUE, QUERY_POSTSYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_TRUE, APPEND_FILE_FALSE, INDEXED_CSV_DATABASE_QUERY_EXTRACT_INCOMING_OUTGOING_CONNECTIONS_NEURON_LIST_CONNECTIONS_POSTSYNAPTIC_FILE_NAME, na);	//extract outgoing connections
		/*
		//depreciated;
		this->queryIndexedCSVdatabaseByNeuronDatasetOrListFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST1_FILE_NAME, INDEXED_CSV_DATABASE_QUERY_PREFER_DATASET_LOCAL_CONNECTOME_NEURONS_VALUE, QUERY_PRESYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_TRUE, APPEND_FILE_FALSE, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST1_CONNECTIONS_PRESYNAPTIC_FILE_NAME, na);
		this->queryIndexedCSVdatabaseByNeuronDatasetOrListFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST1_FILE_NAME, INDEXED_CSV_DATABASE_QUERY_PREFER_DATASET_LOCAL_CONNECTOME_NEURONS_VALUE, QUERY_POSTSYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_TRUE, APPEND_FILE_FALSE, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST1_CONNECTIONS_POSTSYNAPTIC_FILE_NAME, na);
		this->queryIndexedCSVdatabaseByNeuronDatasetOrListFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST2_FILE_NAME, INDEXED_CSV_DATABASE_QUERY_PREFER_DATASET_LOCAL_CONNECTOME_NEURONS_VALUE, QUERY_PRESYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_TRUE, APPEND_FILE_FALSE, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST2_CONNECTIONS_PRESYNAPTIC_FILE_NAME, na);
		this->queryIndexedCSVdatabaseByNeuronDatasetOrListFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST2_FILE_NAME, INDEXED_CSV_DATABASE_QUERY_PREFER_DATASET_LOCAL_CONNECTOME_NEURONS_VALUE, QUERY_POSTSYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_TRUE, APPEND_FILE_FALSE, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST2_CONNECTIONS_POSTSYNAPTIC_FILE_NAME, na);
		*/
	}
	#endif
	#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING
	else if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING)
	{		
		this->queryIndexedCSVdatabaseByNeuronDatasetOrListFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST_DISTINCT_FILE_NAME, INDEXED_CSV_DATABASE_QUERY_PREFER_DATASET_LOCAL_CONNECTOME_NEURONS_VALUE, QUERY_PRESYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_TRUE, APPEND_FILE_FALSE, INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_FILE_NAME, na);	//map incoming connections	
	}
	#endif	
	#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET
	else if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET)
	{			
		#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_ORIG
		this->queryIndexedCSVdatabaseByNeuronDatasetOrListFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, LOCAL_CONNECTOME_DATASET_NEURONS_FILENAME, true, na, WRITE_FILE_TRUE, APPEND_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, CONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS);
		this->queryIndexedCSVdatabaseByNeuronDatasetOrListFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, LOCAL_CONNECTOME_DATASET_NEURONS_FILENAME, true, na, WRITE_FILE_TRUE, APPEND_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, CONNECTION_TYPES_DERIVED_FROM_EM_IMAGES);		
		#else
		this->queryIndexedCSVdatabaseByNeuronDatasetOrListFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, LOCAL_CONNECTOME_DATASET_NEURONS_FILENAME, true, QUERY_POSTSYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_TRUE, APPEND_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, CONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS);
		this->queryIndexedCSVdatabaseByNeuronDatasetOrListFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, LOCAL_CONNECTOME_DATASET_NEURONS_FILENAME, true, QUERY_POSTSYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_TRUE, APPEND_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, CONNECTION_TYPES_DERIVED_FROM_EM_IMAGES);
		#endif
	}	
	#endif
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS
	else if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS)
	{
		string neuronListConnectionsFileNameNA = "";
		#ifdef DEBUG_INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PRINT_OUTPUT_VERBOSE_LOCALORNONLOCAL
		cout << "DEBUG_INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PRINT_OUTPUT_VERBOSE_LOCALORNONLOCAL print format: neuronID num_excitatory_connections num_inhibitory_connections" << endl;
		#endif
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_PRESYNAPTIC_NEURONS
		cout << "CONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS:" << endl;
		vector<H01connectivityModelClass>* connectivityModelLayersIncoming = generateNumberOfConnectionsLayers();
		vector<H01connectivityModelClass>* connectivityModelLayersOutgoing = generateNumberOfConnectionsLayers();
		cout << "QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS: QUERY_PRESYNAPTIC_CONNECTION_NEURONS (QUERY_BY_POSTSYNAPTIC_CONNECTION_NEURONS) - count incoming connections" << endl;
		this->queryIndexedCSVdatabaseByNeuronDatasetOrListFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST_DISTINCT_FILE_NAME, INDEXED_CSV_DATABASE_QUERY_PREFER_DATASET_LOCAL_CONNECTOME_NEURONS_VALUE, QUERY_PRESYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_FALSE, APPEND_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, CONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, connectivityModelLayersIncoming);	//count incoming connections
		cout << "QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS: QUERY_POSTSYNAPTIC_CONNECTION_NEURONS (QUERY_BY_PRESYNAPTIC_CONNECTION_NEURONS) - count outgoing connections" << endl;		
		this->queryIndexedCSVdatabaseByNeuronDatasetOrListFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST_DISTINCT_FILE_NAME, INDEXED_CSV_DATABASE_QUERY_PREFER_DATASET_LOCAL_CONNECTOME_NEURONS_VALUE, QUERY_POSTSYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_FALSE, APPEND_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, CONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, connectivityModelLayersOutgoing);	//count outgoing connections
		#endif
		cout << "CONNECTION_TYPES_DERIVED_FROM_EM_IMAGES:" << endl;
		connectivityModelLayersIncoming = generateNumberOfConnectionsLayers();
		connectivityModelLayersOutgoing = generateNumberOfConnectionsLayers();
		cout << "QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS: QUERY_PRESYNAPTIC_CONNECTION_NEURONS (QUERY_BY_POSTSYNAPTIC_CONNECTION_NEURONS) - count incoming connections" << endl;
		this->queryIndexedCSVdatabaseByNeuronDatasetOrListFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST_DISTINCT_FILE_NAME, INDEXED_CSV_DATABASE_QUERY_PREFER_DATASET_LOCAL_CONNECTOME_NEURONS_VALUE, QUERY_PRESYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_FALSE, APPEND_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, CONNECTION_TYPES_DERIVED_FROM_EM_IMAGES, connectivityModelLayersIncoming);	//count incoming connections
		cout << "QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS: QUERY_POSTSYNAPTIC_CONNECTION_NEURONS (QUERY_BY_PRESYNAPTIC_CONNECTION_NEURONS) - count outgoing connections" << endl;		
		this->queryIndexedCSVdatabaseByNeuronDatasetOrListFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST_DISTINCT_FILE_NAME, INDEXED_CSV_DATABASE_QUERY_PREFER_DATASET_LOCAL_CONNECTOME_NEURONS_VALUE, QUERY_POSTSYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_FALSE, APPEND_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, CONNECTION_TYPES_DERIVED_FROM_EM_IMAGES, connectivityModelLayersOutgoing);	//count outgoing connections
	}
	#endif
	#ifdef INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET
	else if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET)
	{	
		cout << "warning: query mode 5 (INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET) is awaiting release of H01 Release C3 neurons dataset; will print UNKNOWN neurons (with x/y coordinates only) along with candidate neuron_ids but not reconcile them" << endl;
		
		//only need to perform operation on !connectionTypesDerivedFromPresynapticNeuronsOrEMimages
		vector<vector<string>> localConnectomeCSVdatasetConnections;
		this->queryIndexedCSVdatabaseByConnectionDatasetFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, READ_FILE_TRUE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES_USEALLVALUESAVAILABLEFROMINBODYCELLCONNECTION, &localConnectomeCSVdatasetConnections, QUERY_PRESYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_FALSE, APPEND_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, CONNECTION_TYPES_DERIVED_FROM_EM_IMAGES);
		this->queryIndexedCSVdatabaseByConnectionDatasetFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, READ_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES_USEALLVALUESAVAILABLEFROMINBODYCELLCONNECTION, &localConnectomeCSVdatasetConnections, QUERY_POSTSYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_TRUE, APPEND_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, CONNECTION_TYPES_DERIVED_FROM_EM_IMAGES);
		/*
		this->queryIndexedCSVdatabaseByConnectionDatasetFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, READ_FILE_TRUE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, QUERY_PRESYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_TRUE, APPEND_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, CONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS);
		this->queryIndexedCSVdatabaseByConnectionDatasetFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, READ_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, QUERY_POSTSYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_TRUE, APPEND_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, CONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS);
		*/		
	}	
	#endif
	#ifdef INDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS
	else if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS)
	{
		cout << "executing INDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS: it will take a long time (approximately 30 minutes) to crawl the indexed CSV database" << endl;
		cout << "INDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS: QUERY_PRESYNAPTIC_CONNECTION_NEURONS (QUERY_BY_PRESYNAPTIC_CONNECTION_NEURONS) - count incoming connections" << endl;
		this->crawlIndexedCSVdatabase(queryMode, indexed_csv_database_folder, QUERY_PRESYNAPTIC_CONNECTION_NEURONS);	//count unique incoming neurons (axons) not explicitly in local connectome 
		cout << "INDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS: QUERY_POSTSYNAPTIC_CONNECTION_NEURONS (QUERY_BY_POSTSYNAPTIC_CONNECTION_NEURONS) - count outgoing connections" << endl;		
		this->crawlIndexedCSVdatabase(queryMode, indexed_csv_database_folder, QUERY_POSTSYNAPTIC_CONNECTION_NEURONS);	//count unique outgoing neurons (dendrites) not explicitly in local connectome
	}
	#endif
	#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL
	else if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL)
	{
		string neuronListConnectionsFileNameNA = "";
		#ifdef DEBUG_INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PRINT_OUTPUT_VERBOSE_LOCALORNONLOCAL
		cout << "DEBUG_INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PRINT_OUTPUT_VERBOSE_LOCALORNONLOCAL print format: neuronID num_excitatory_connections num_inhibitory_connections" << endl;
		#endif
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_PRESYNAPTIC_NEURONS
		cout << "CONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS:" << endl;
		vector<H01connectivityModelClass>* connectivityModelLayersIncoming = generateNumberOfConnectionsLayers();
		vector<H01connectivityModelClass>* connectivityModelLayersOutgoing = generateNumberOfConnectionsLayers();
		cout << "INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL: QUERY_PRESYNAPTIC_CONNECTION_NEURONS (QUERY_BY_POSTSYNAPTIC_CONNECTION_NEURONS) - count incoming connections" << endl;
		this->queryIndexedCSVdatabaseByNeuronDatasetOrListFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST_DISTINCT_FILE_NAME, INDEXED_CSV_DATABASE_QUERY_PREFER_DATASET_LOCAL_CONNECTOME_NEURONS_VALUE, QUERY_PRESYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_FALSE, APPEND_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, CONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, connectivityModelLayersIncoming);	//count incoming connections
		cout << "INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL: QUERY_POSTSYNAPTIC_CONNECTION_NEURONS (QUERY_BY_PRESYNAPTIC_CONNECTION_NEURONS) - count outgoing connections" << endl;		
		this->queryIndexedCSVdatabaseByNeuronDatasetOrListFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST_DISTINCT_FILE_NAME, INDEXED_CSV_DATABASE_QUERY_PREFER_DATASET_LOCAL_CONNECTOME_NEURONS_VALUE, QUERY_POSTSYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_FALSE, APPEND_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, CONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, connectivityModelLayersOutgoing);	//count outgoing connections
		cout << "INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL" << endl;
		this->generateLargeModelNeuronsAndConnectionsWrapper(queryMode, indexed_csv_database_folder, local_connectome_folder_base, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST_DISTINCT_FILE_NAME, INDEXED_CSV_DATABASE_QUERY_PREFER_DATASET_LOCAL_CONNECTOME_NEURONS_VALUE, WRITE_FILE_TRUE, APPEND_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, CONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, connectivityModelLayersIncoming, connectivityModelLayersOutgoing);	//generate large model
		#endif
		cout << "CONNECTION_TYPES_DERIVED_FROM_EM_IMAGES:" << endl;
		connectivityModelLayersIncoming = generateNumberOfConnectionsLayers();
		connectivityModelLayersOutgoing = generateNumberOfConnectionsLayers();
		cout << "INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL: QUERY_PRESYNAPTIC_CONNECTION_NEURONS (QUERY_BY_POSTSYNAPTIC_CONNECTION_NEURONS) - count incoming connections" << endl;
		this->queryIndexedCSVdatabaseByNeuronDatasetOrListFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST_DISTINCT_FILE_NAME, INDEXED_CSV_DATABASE_QUERY_PREFER_DATASET_LOCAL_CONNECTOME_NEURONS_VALUE, QUERY_PRESYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_FALSE, APPEND_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, CONNECTION_TYPES_DERIVED_FROM_EM_IMAGES, connectivityModelLayersIncoming);	//count incoming connections
		cout << "INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL: QUERY_POSTSYNAPTIC_CONNECTION_NEURONS (QUERY_BY_PRESYNAPTIC_CONNECTION_NEURONS) - count outgoing connections" << endl;		
		this->queryIndexedCSVdatabaseByNeuronDatasetOrListFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST_DISTINCT_FILE_NAME, INDEXED_CSV_DATABASE_QUERY_PREFER_DATASET_LOCAL_CONNECTOME_NEURONS_VALUE, QUERY_POSTSYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_FALSE, APPEND_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, CONNECTION_TYPES_DERIVED_FROM_EM_IMAGES, connectivityModelLayersOutgoing);	//count outgoing connections
		cout << "INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL" << endl;
		this->generateLargeModelNeuronsAndConnectionsWrapper(queryMode, indexed_csv_database_folder, local_connectome_folder_base, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST_DISTINCT_FILE_NAME, INDEXED_CSV_DATABASE_QUERY_PREFER_DATASET_LOCAL_CONNECTOME_NEURONS_VALUE, WRITE_FILE_TRUE, APPEND_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, CONNECTION_TYPES_DERIVED_FROM_EM_IMAGES, connectivityModelLayersIncoming, connectivityModelLayersOutgoing);	//generate large model
	}
	#endif
	else
	{
		cerr << "H01indexedCSVdatabaseQueryClass::queryIndexedCSVdatabase error: queryMode unknown: " << queryMode << endl;
		exit(EXIT_ERROR);
	}
	
	return result;
}

#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL
bool H01indexedCSVdatabaseQueryClass::generateLargeModelNeuronsAndConnectionsWrapper(const int queryMode, const string indexed_csv_database_folder, const string local_connectome_folder_base, const string neuronDatasetOrListFileName, const bool neuronListIsDataset, const bool write, const bool appendToFile, const string neuronListConnectionsFileName, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, vector<H01connectivityModelClass>* numberOfConnectionsLayersIncoming, vector<H01connectivityModelClass>* numberOfConnectionsLayersOutgoing)
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
	if(!H01indexedCSVdatabaseCalculateNeuronLayer.readLocalNeuronsAndConnections(local_connectome_folder_base, neuronDatasetOrListFileName, neuronListIsDataset, &neuronList, &neuronMap, &localConnectomeCSVdatasetNeurons, readConnections, neuronListConnectionsFileName, &localConnectomeCSVdatasetConnections, buildConnectionsMap, &connectionsMap, readLayers, readLayersConnections))
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
	string writeFileString;
	ofstream writeFileObject;
	
	ios_base::openmode writeMode;
	if(appendToFile)
	{
		writeMode = ofstream::app;
	}
	else
	{
		writeMode = ofstream::out; 
	}
	if(write)
	{	
		#ifdef INDEXED_CSV_DATABASE_QUERY_OUTPUT
		#ifdef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_FILE_OBJECT
		SHAREDvars.setCurrentDirectory(indexedCSVdatabaseQueryOutputFolder);
		writeFileObject.open(connectionsLargeModelFileName.c_str(), writeMode);
		#else
		writeFileString.reserve(2000000000);	//max expected file/string size = indexed csv database query output size (neuron_id ~11 bytes + , + type ~1 byte = ~13 bytes) * avg num connections per neuron ID (~2000) * number of neuron IDs in list (~30000) = 13*2000*30000 = ~1GB of RAM if string
		//string writeFileString = "";	//inefficient
		#endif
		#endif
	}

	if(write)
	{	
		#ifdef INDEXED_CSV_DATABASE_QUERY_OUTPUT
		#ifdef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_FILE_OBJECT
		writeFileObject.close();
		#else
		//cout << "writeFileString = " << writeFileString << endl;
		SHAREDvars.setCurrentDirectory(indexedCSVdatabaseQueryOutputFolder);
		//SHAREDvars.writeStringToFile(connectionsLargeModelFileName, &writeFileString);	//inefficient?
		ofstream out(connectionsLargeModelFileName, writeMode);
    	out << writeFileString;
    	out.close();
		#endif
		#endif
	}
	*/
}
#endif

bool H01indexedCSVdatabaseQueryClass::queryIndexedCSVdatabaseByNeuronDatasetOrListFile(const int queryMode, const string indexed_csv_database_folder, const string local_connectome_folder_base, const string neuronDatasetOrListFileName, const bool neuronListIsDataset, const bool queryPresynapticConnectionNeurons, const bool write, const bool appendToFile, const string neuronListConnectionsFileName, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, vector<H01connectivityModelClass>* connectivityModelLayers)
{
	bool result = true;
		
	//read connections?;
	bool readConnections = false;
	bool buildConnectionsMap = false;
	#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_VERIFICATION
	if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET)
	{
		//verify local connectome connections dataset generation vs existing (dev/ODS generated) connections datasets
		readConnections = true;	//vector<vector<string>>*localConnectomeCSVdatasetConnections is required by INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_VERIFICATION		
		buildConnectionsMap = true;	
	}
	#endif
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL
	if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS)
	{
		readConnections = true;	//vector<vector<string>>*localConnectomeCSVdatasetConnections is required by INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL
	}	
	#endif
	if(readConnections)
	{
		if(write)
		{
			cerr << "H01indexedCSVdatabaseQueryClass::queryIndexedCSVdatabaseByNeuronDatasetOrListFile: readConnections does not support write" << endl;
			exit(EXIT_ERROR);
		}	
	}

	//read layers?;
	bool readLayers = false;
	bool readLayersConnections = false;
	#ifdef INDEXED_CSV_DATABASE_QUERY_LAYERS
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS
	if((queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS) || (queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL))	//constraint only added for efficiency (only calculate layers when necessary)
	{	
		readLayers = true;
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL
		readLayersConnections = true;
		#endif
	}
	#endif
	#endif
			
	vector<string> neuronList;
	map<string, int> neuronMap;
	vector<vector<string>> localConnectomeCSVdatasetNeurons;	
	vector<vector<string>> localConnectomeCSVdatasetConnections;
	map<string, int> connectionsMap;	
	if(!H01indexedCSVdatabaseCalculateNeuronLayer.readLocalNeuronsAndConnections(local_connectome_folder_base, neuronDatasetOrListFileName, neuronListIsDataset, &neuronList, &neuronMap, &localConnectomeCSVdatasetNeurons, readConnections, neuronListConnectionsFileName, &localConnectomeCSVdatasetConnections, buildConnectionsMap, &connectionsMap, readLayers, readLayersConnections))
	{
		result = false;
	}

	#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL_NORMALISE_LOCAL_CONNECTIVITY
	if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL)
	{	
		H01indexedCSVdatabaseCalculateNeuronLayer.calculateLocalConnectomeNeuronLayerSurfaceNorms(&localConnectomeCSVdatasetNeurons, &neuronMap);
	}
	#endif
			
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL
	if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS)	//added 05 Mar 2022
	{
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL_ONLY_COUNT_OUTGOING
		if(!queryPresynapticConnectionNeurons)	//only count connections from preSynaptic to postSynaptic neuron
		{
		#endif
			cout << "\n\nINDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL:" << endl;
			countConnectionsLocal(queryMode, &neuronList, &neuronMap, &localConnectomeCSVdatasetNeurons, &localConnectomeCSVdatasetConnections, queryPresynapticConnectionNeurons, connectionTypesDerivedFromPresynapticNeuronsOrEMimages);
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL_ONLY_COUNT_OUTGOING
		}
		#endif
	}
	#endif
		
	const string csvDelimiter = CSV_DELIMITER;
	#ifdef INDEXED_CSV_DATABASE_QUERY_WRITE_CURRENT_FOLDER
	const string indexedCSVdatabaseQueryOutputFolder = currentDirectory;	
	#else
	const string indexedCSVdatabaseQueryOutputFolder = INDEXED_CSV_DATABASE_QUERY_OUTPUT_FOLDER;
	#endif
	
	string writeFileString;
	ofstream writeFileObject;
	
	ios_base::openmode writeMode;
	if(appendToFile)
	{
		writeMode = ofstream::app;
	}
	else
	{
		writeMode = ofstream::out; 
	}
	if(write)
	{	
		#ifdef INDEXED_CSV_DATABASE_QUERY_OUTPUT
		#ifdef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_FILE_OBJECT
		SHAREDvars.setCurrentDirectory(indexedCSVdatabaseQueryOutputFolder);
		writeFileObject.open(neuronListConnectionsFileName.c_str(), writeMode);
		#else
		writeFileString.reserve(2000000000);	//max expected file/string size = indexed csv database query output size (neuron_id ~11 bytes + , + type ~1 byte = ~13 bytes) * avg num connections per neuron ID (~2000) * number of neuron IDs in list (~30000) = 13*2000*30000 = ~1GB of RAM if string
		//string writeFileString = "";	//inefficient
		#endif
		#endif
	}
	
	#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_ORIG
	if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET)
	{
		queryIndexedCSVdatabaseByNeuronList(queryMode, indexed_csv_database_folder, &neuronList, &neuronMap, &localConnectomeCSVdatasetNeurons, &localConnectomeCSVdatasetConnections, &connectionsMap, true, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, &writeFileObject, &writeFileString, appendToFile, connectivityModelLayers);
		queryIndexedCSVdatabaseByNeuronList(queryMode, indexed_csv_database_folder, &neuronList, &neuronMap, &localConnectomeCSVdatasetNeurons, &localConnectomeCSVdatasetConnections, &connectionsMap, false, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, &writeFileObject, &writeFileString, appendToFile, connectivityModelLayers);	
	}
	else
	{
	#endif
		bool queryByPresynapticConnectionNeurons = !queryPresynapticConnectionNeurons;
		queryIndexedCSVdatabaseByNeuronList(queryMode, indexed_csv_database_folder, &neuronList, &neuronMap, &localConnectomeCSVdatasetNeurons, &localConnectomeCSVdatasetConnections, &connectionsMap, queryByPresynapticConnectionNeurons, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, &writeFileObject, &writeFileString, appendToFile, connectivityModelLayers);
	#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_ORIG
	}
	#endif
	
	#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING	
	if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING)
	{
    	map<long,vector<H01indexedCSVdatabaseQueryObject>>::iterator it;
		for(it = neuronConnectionMap.begin(); it != neuronConnectionMap.end(); it++)
		{
			long connectionNeuronID = it->first;
			vector<H01indexedCSVdatabaseQueryObject>* neuronVector = &(it->second);
			#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_2D_POLY_REGRESSION
			vector<double> x;
			vector<double> y;
			//vector<double> z;
			#endif
			#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_3D_LINEAR_REGRESSION
			vector<Eigen::Vector3d> xyz;
			#endif

			if(neuronVector->size() >= INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_MIN_NUM_POINTS_REQUIRED)
			{
				#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_EST_TYPE_BY_AVERAGE_SYNAPSE_TYPE
				double totalSynapseType = 0.0;
				#endif

				for(int i=0; i<neuronVector->size(); i++)
				{
					H01indexedCSVdatabaseQueryObject* queryObject = &((*neuronVector)[i]);
					cout << "x = " << queryObject->locationObjectContentsXcoordinates << ", y = " << queryObject->locationObjectContentsYcoordinates << ", z = " << queryObject->locationObjectContentsZcoordinates << endl;
					double xCoord = (double)(queryObject->locationObjectContentsXcoordinates) * AVRO_JSON_DATABASE_COORDINATES_CALIBRATION_X;
					double yCoord = (double)(queryObject->locationObjectContentsYcoordinates) * AVRO_JSON_DATABASE_COORDINATES_CALIBRATION_Y;
					double zCoord = (double)(queryObject->locationObjectContentsZcoordinates) * AVRO_JSON_DATABASE_COORDINATES_CALIBRATION_Z;
					#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_2D_POLY_REGRESSION
					x.push_back(xCoord);
					y.push_back(yCoord);
					//z.push_back(zCoord);	//not yet modelled/plotted
					#endif
					#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_3D_LINEAR_REGRESSION
					std::vector<double> vec = {xCoord, yCoord, zCoord};
					Eigen::Vector3d vec3D(vec.data());
					//Eigen::Vector3d vec3D(xCoord, yCoord, zCoord);
					xyz.push_back(vec3D);
					#endif
					#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_EST_TYPE_BY_AVERAGE_SYNAPSE_TYPE
					totalSynapseType = totalSynapseType + queryObject->type;
					#endif
				}

				int estSynapseType = 0;
				#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_EST_TYPE_BY_AVERAGE_SYNAPSE_TYPE
				double averageSynapseType = totalSynapseType/(neuronVector->size());
				estSynapseType = round(averageSynapseType);	//1: inhibitory, 2: exitatory
				#endif

				#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_CONNECTION_TYPE_REQUIRED
				if(estSynapseType == INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_CONNECTION_TYPE)
				{
				#endif

					#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_2D_POLY_REGRESSION
					PolynomialRegression<double> polyreg;
					vector<double> coeffs;
					const int order = 2;
					polyreg.fitIt(x, y, order, coeffs);
					#endif
					#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_3D_LINEAR_REGRESSION
					std::pair<Eigen::Vector3d, Eigen::Vector3d> line3D = best_line_from_points<Eigen::Vector3d>(xyz);
					#endif

					#ifdef INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING
					H01indexedCSVdatabaseAlgorithmsFit polyFit;
					polyFit.connectionNeuronID = connectionNeuronID;
					#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_EST_TYPE_BY_AVERAGE_SYNAPSE_TYPE
					polyFit.estSynapseType = estSynapseType;
					#endif
					#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_2D_POLY_REGRESSION
					polyFit.a = coeffs[0];
					polyFit.b = coeffs[1];
					polyFit.c = coeffs[2];
					#endif
					#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_3D_LINEAR_REGRESSION
					polyFit.origin.x = line3D.first[0];
					polyFit.origin.y = line3D.first[1];
					polyFit.origin.z = line3D.first[2];
					polyFit.axis.x = line3D.second[0];
					polyFit.axis.y = line3D.second[1];
					polyFit.axis.z = line3D.second[2];
					#endif
					neuronConnectionPolyFitVector.push_back(polyFit);
					const string doubleFormat = "%0.9f";
					#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_2D_POLY_REGRESSION
					string neuronConnectionString = SHAREDvars.convertLongToString(polyFit.connectionNeuronID) + csvDelimiter + SHAREDvars.convertIntToString(polyFit.estSynapseType) + csvDelimiter + SHAREDvars.convertDoubleToString(polyFit.a, doubleFormat) + csvDelimiter + SHAREDvars.convertDoubleToString(polyFit.b, doubleFormat) + csvDelimiter + SHAREDvars.convertDoubleToString(polyFit.c, doubleFormat) + STRING_NEWLINE;
					#endif
					#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_3D_LINEAR_REGRESSION
					#ifdef INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING_LDR
					//2 <colour> x1 y1 z1 x2 y2 z2 - https://www.ldraw.org/article/218.html
					string ldrDelimiter = LDR_DELIMITER;
					int ldrawColor = 0;
					if(polyFit.estSynapseType == AVRO_JSON_DATABASE_EXCITATORY_SYNAPSE_TYPE)
					{
						ldrawColor = INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_CONNECTION_TYPE_EXCITATORY_COLOUR;
					}
					else if(polyFit.estSynapseType == AVRO_JSON_DATABASE_INHIBITORY_SYNAPSE_TYPE)
					{
						ldrawColor = INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_CONNECTION_TYPE_INHIBITORY_COLOUR;
					}
					else
					{
						cerr << "bool queryIndexedCSVdatabaseByNeuronDatasetOrListFile unknown polyFit.estSynapseType; " << polyFit.estSynapseType << endl;
						exit(EXIT_ERROR);
					}
					vec plot3DpointStart;
					plot3DpointStart.x = polyFit.origin.x - polyFit.axis.x * (AVRO_JSON_DATABASE_COORDINATES_RANGE_X/2);
					plot3DpointStart.y = polyFit.origin.y - polyFit.axis.y * (AVRO_JSON_DATABASE_COORDINATES_RANGE_Y/2);
					plot3DpointStart.z = polyFit.origin.z - polyFit.axis.z * (AVRO_JSON_DATABASE_COORDINATES_RANGE_Z/2);
					vec plot3DpointEnd;
					plot3DpointEnd.x = polyFit.origin.x + polyFit.axis.x * (AVRO_JSON_DATABASE_COORDINATES_RANGE_X/2);
					plot3DpointEnd.y = polyFit.origin.y + polyFit.axis.y * (AVRO_JSON_DATABASE_COORDINATES_RANGE_Y/2);
					plot3DpointEnd.z = polyFit.origin.z + polyFit.axis.z * (AVRO_JSON_DATABASE_COORDINATES_RANGE_Z/2);
					string neuronConnectionString = SHAREDvars.convertIntToString(LDR_REFERENCE_TYPE_LINE) + ldrDelimiter + SHAREDvars.convertIntToString(ldrawColor) + ldrDelimiter + SHAREDvars.convertDoubleToString(plot3DpointStart.x, doubleFormat) + ldrDelimiter + SHAREDvars.convertDoubleToString(plot3DpointStart.y, doubleFormat) + ldrDelimiter + SHAREDvars.convertDoubleToString(plot3DpointStart.z, doubleFormat) + ldrDelimiter + SHAREDvars.convertDoubleToString(plot3DpointEnd.x, doubleFormat) + ldrDelimiter + SHAREDvars.convertDoubleToString(plot3DpointEnd.y, doubleFormat) + ldrDelimiter + SHAREDvars.convertDoubleToString(plot3DpointEnd.z, doubleFormat) + STRING_NEWLINE;	
					#else
					string neuronConnectionString = SHAREDvars.convertLongToString(polyFit.connectionNeuronID) + csvDelimiter + SHAREDvars.convertIntToString(polyFit.estSynapseType) + csvDelimiter + SHAREDvars.convertDoubleToString(polyFit.origin.x, doubleFormat) + csvDelimiter + SHAREDvars.convertDoubleToString(polyFit.origin.y, doubleFormat) + csvDelimiter + SHAREDvars.convertDoubleToString(polyFit.origin.z, doubleFormat) + csvDelimiter + SHAREDvars.convertDoubleToString(polyFit.axis.x, doubleFormat) + csvDelimiter + SHAREDvars.convertDoubleToString(polyFit.axis.y, doubleFormat) + csvDelimiter + SHAREDvars.convertDoubleToString(polyFit.axis.z, doubleFormat) + STRING_NEWLINE;
					#endif
					#endif
					#ifdef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_FILE_OBJECT
					SHAREDvars.writeStringToFileObject(neuronConnectionString, &writeFileObject);
					#else
					this->appendStringBufferToPreallocatedString(writeFileString, neuronConnectionString);
					//writeFileString = writeFileString + neuronConnectionString	//inefficient
					#endif
					#else
					cout << "INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING: queryIndexedCSVdatabaseByNeuronDatasetOrListFile(): connectionNeuronID = " << connectionNeuronID << ", estSynapseType = " << estSynapseType << ", poly fit; a = " << coeffs[0] << ", b = " << coeffs[1] << ", c = " << coeffs[2] << endl;			
					#endif

				#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_CONNECTION_TYPE_REQUIRED
				}
				#endif
			}
		}
	}
	#endif

	if(write)
	{	
		#ifdef INDEXED_CSV_DATABASE_QUERY_OUTPUT
		#ifdef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_FILE_OBJECT
		writeFileObject.close();
		#else
		//cout << "writeFileString = " << writeFileString << endl;
		SHAREDvars.setCurrentDirectory(indexedCSVdatabaseQueryOutputFolder);
		//SHAREDvars.writeStringToFile(neuronListConnectionsFileName, &writeFileString);	//inefficient?
		ofstream out(neuronListConnectionsFileName, writeMode);
    	out << writeFileString;
    	out.close();
		#endif
		#endif
	}
	
	return result;
}

bool H01indexedCSVdatabaseQueryClass::queryIndexedCSVdatabaseByNeuronList(const int queryMode, const string indexed_csv_database_folder, vector<string>* neuronList, map<string, int>* neuronMap, vector<vector<string>>* localConnectomeCSVdatasetNeurons, vector<vector<string>>* localConnectomeCSVdatasetConnections, map<string, int>* connectionsMap, const bool queryByPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, ofstream* writeFileObject, string* writeFileString, const bool appendToFile, vector<H01connectivityModelClass>* connectivityModelLayers)
{
	bool result = true;
	
	
	#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET
	if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET)
	{
		if(!appendToFile)
		{
			#ifdef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_FILE_OBJECT
			string neuronConnectionsFileHeader = string(LOCAL_CONNECTOME_DATASET_CONNECTIONS_HEADER) + STRING_NEWLINE;
			SHAREDvars.writeStringToFileObject(neuronConnectionsFileHeader, writeFileObject);
			#else
			this->appendStringBufferToPreallocatedString(writeFileString, neuronConnectionsFileHeader);
			//writeFileString = writeFileString + neuronConnectionString	//inefficient
			#endif	
		}
	}
	#endif
		
	const string csvDelimiter = CSV_DELIMITER;
	
	for(int neuronIndex=0; neuronIndex<neuronList->size(); neuronIndex++)
	{
		string neuronID = (*neuronList)[neuronIndex];
		vector<vector<string>> neuronConnectionList;
										
		if(queryIndexedCSVdatabaseByNeuronID(indexed_csv_database_folder, neuronID, queryByPresynapticConnectionNeurons, &neuronConnectionList))
		{	
			
			#ifdef INDEXED_CSV_DATABASE_QUERY_READ_DATASET_LOCAL_CONNECTOME_NEURONS
			vector<string>* localConnectomeNeuron = &((*localConnectomeCSVdatasetNeurons)[neuronIndex]);
			#endif

			#ifdef DEBUG_INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PRINT_OUTPUT_VERBOSE_LOCALORNONLOCAL
			int numberConnectionsLocalConnectomeExcitatoryCurrent = 0;	
			int numberConnectionsLocalConnectomeInhibitoryCurrent = 0;
			#endif

			#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS
			#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_NUMBER_EXCITATORY_INHIBITORY_NEURONS
			if((queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS) || (queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL))
			{
				#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
				int localConnectomeNeuronLayer = SHAREDvars.convertStringToInt((*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER]);
				#endif

				(*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].numberOfLocalConnectomeNeurons++;	//(*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].numberOfLocalConnectomeNeurons = (*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].numberOfLocalConnectomeNeurons+1;
				#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
				(*connectivityModelLayers)[localConnectomeNeuronLayer].numberOfLocalConnectomeNeurons++;
				#endif

				//int excitationType;
				string sourceNeuronType = "";
				sourceNeuronType = (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE];

				//count outgoing or incoming connections (depending on queryByPresynapticConnectionNeurons or !queryByPresynapticConnectionNeurons):
				if(sourceNeuronType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_PYRAMIDAL)
				{
					(*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].numberOfLocalConnectomeNeuronsExcitatory++;
					#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
					(*connectivityModelLayers)[localConnectomeNeuronLayer].numberOfLocalConnectomeNeuronsExcitatory++;
					#endif
					//excitationType = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY;
				}
				else if(sourceNeuronType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_INTERNEURON)
				{
					(*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].numberOfLocalConnectomeNeuronsInhibitory++;
					#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
					(*connectivityModelLayers)[localConnectomeNeuronLayer].numberOfLocalConnectomeNeuronsInhibitory++;
					#endif
					//excitationType = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY;
				}
				else
				{
					//excitationType = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN;
					cerr << "H01indexedCSVdatabaseQueryClass::queryIndexedCSVdatabaseByNeuronList warning, source neuron type unknown, sourceNeuronType = " << sourceNeuronType << endl;
				}
			}
			#endif
			#endif
										
			for(int connectionIndex=0; connectionIndex<neuronConnectionList.size(); connectionIndex++)
			{
				vector<string> csvLineVector = neuronConnectionList[connectionIndex];

				string sourceNeuronID = "";
				string targetNeuronID = "";
				if(queryByPresynapticConnectionNeurons)
				{
					sourceNeuronID = csvLineVector[INDEXED_CSV_DATABASE_PRESYNAPTIC_SITE_NEURON_ID_FIELD_INDEX];
					targetNeuronID = csvLineVector[INDEXED_CSV_DATABASE_POSTSYNAPTIC_SITE_NEURON_ID_FIELD_INDEX];
				}
				else
				{
					sourceNeuronID = csvLineVector[INDEXED_CSV_DATABASE_POSTSYNAPTIC_SITE_NEURON_ID_FIELD_INDEX];
					targetNeuronID = csvLineVector[INDEXED_CSV_DATABASE_PRESYNAPTIC_SITE_NEURON_ID_FIELD_INDEX];	
				}
				if(sourceNeuronID != neuronID)
				{
					cerr << "H01indexedCSVdatabaseQueryClass::queryIndexedCSVdatabaseByNeuronList error: (sourceNeuronID != neuronID), sourceNeuronID = " << sourceNeuronID << ", neuronID = " << neuronID << endl;
					exit(EXIT_ERROR);
				}
				string connectionType = csvLineVector[INDEXED_CSV_DATABASE_SYNAPSE_TYPE_FIELD_INDEX];	//connection/synapse type as automatically derived from EM images (exitatory/inhibitory)
				int connectionTypeInt = SHAREDvars.convertStringToInt(connectionType);
				//cout << "connectionType = " << connectionType << endl;
				//cout << "connectionTypeInt = " << connectionTypeInt << endl;
				string locationObjectContentsXcoordinatesContent = csvLineVector[INDEXED_CSV_DATABASE_SYNAPSE_LOCATION_COORDINATE_X_FIELD_INDEX];	//connection/synapse location
				string locationObjectContentsYcoordinatesContent = csvLineVector[INDEXED_CSV_DATABASE_SYNAPSE_LOCATION_COORDINATE_Y_FIELD_INDEX];	//connection/synapse location
				string locationObjectContentsZcoordinatesContent = csvLineVector[INDEXED_CSV_DATABASE_SYNAPSE_LOCATION_COORDINATE_Z_FIELD_INDEX];	//connection/synapse location
				//cout << locationObjectContentsXcoordinatesContent << " " << locationObjectContentsYcoordinatesContent << " " << locationObjectContentsZcoordinatesContent << endl;

				#ifdef INDEXED_CSV_DATABASE_QUERY_CONFIDENCE_THRESHOLD_REQUIRED
				//cout << "csvLineVector[INDEXED_CSV_DATABASE_SYNAPSE_CONFIDENCE_FIELD_INDEX] = " << csvLineVector[INDEXED_CSV_DATABASE_SYNAPSE_CONFIDENCE_FIELD_INDEX] << endl;
				double connectionConfidence = SHAREDvars.convertStringToDouble(csvLineVector[INDEXED_CSV_DATABASE_SYNAPSE_CONFIDENCE_FIELD_INDEX]);	//connection/synapse confidence
				if(connectionConfidence > INDEXED_CSV_DATABASE_QUERY_CONFIDENCE_THRESHOLD)
				{
				#endif

					#ifdef INDEXED_CSV_DATABASE_QUERY_EXTRACT_INCOMING_OUTGOING_CONNECTIONS
					if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_EXTRACT_INCOMING_OUTGOING_CONNECTIONS)
					{
						string neuronConnectionString = targetNeuronID + csvDelimiter + connectionType;
						#ifdef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_FILE_OBJECT
						SHAREDvars.writeStringToFileObject(neuronConnectionString, writeFileObject);
						#else
						this->appendStringBufferToPreallocatedString(writeFileString, neuronConnectionString);
						//writeFileString = writeFileString + neuronConnectionString	//inefficient
						#endif
						//append csvDelimiter or STRING_NEWLINE:
						if(connectionIndex == neuronConnectionList.size()-1)
						{
							#ifdef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_FILE_OBJECT
							SHAREDvars.writeStringToFileObject(STRING_NEWLINE, writeFileObject);
							#else
							this->appendStringBufferToPreallocatedString(writeFileString, STRING_NEWLINE);
							//writeFileString = writeFileString + STRING_NEWLINE;	//inefficient
							#endif
						}
						else
						{
							#ifdef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_FILE_OBJECT
							SHAREDvars.writeStringToFileObject(csvDelimiter, writeFileObject);
							#else
							this->appendStringBufferToPreallocatedString(writeFileString, csvDelimiter);
							//writeFileString = writeFileString + csvDelimiter;	//inefficient
							#endif
						}
					}
					#endif		


					#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING	
					if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING)
					{
						string neuronConnectionString = targetNeuronID + csvDelimiter + connectionType + csvDelimiter + locationObjectContentsXcoordinatesContent + csvDelimiter + locationObjectContentsYcoordinatesContent + csvDelimiter + locationObjectContentsZcoordinatesContent;		
						#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_NON_LOCAL_NEURONS_REQUIRED
						//if(!std::count(neuronList.begin(), neuronList.end(), connectionNeuronID))	//verify that connectionNeuronID is not in neuronList
						if(neuronMap->count(targetNeuronID) == 0)	//verify that targetNeuronID is not in neuronList
						{
						#endif
							long targetNeuronIDindex = SHAREDvars.convertStringToLong(targetNeuronID);
							long sourceNeuronIDIndex = SHAREDvars.convertStringToLong(sourceNeuronID);

							//H01indexedCSVdatabaseQueryObject* queryObject = new H01indexedCSVdatabaseQueryObject();
							H01indexedCSVdatabaseQueryObject queryObject;
							queryObject.neuronID = sourceNeuronIDIndex;
							queryObject.type = SHAREDvars.convertStringToInt(connectionType);
							queryObject.locationObjectContentsXcoordinates = SHAREDvars.convertStringToInt(locationObjectContentsXcoordinatesContent);
							queryObject.locationObjectContentsYcoordinates = SHAREDvars.convertStringToInt(locationObjectContentsYcoordinatesContent);
							queryObject.locationObjectContentsZcoordinates = SHAREDvars.convertStringToInt(locationObjectContentsZcoordinatesContent);
							vector<H01indexedCSVdatabaseQueryObject>* neuronIDvector;
							if(neuronConnectionMap.count(targetNeuronIDindex) != 0)
							{
								neuronIDvector = &(neuronConnectionMap.at(targetNeuronIDindex));
							}
							else
							{
								vector<H01indexedCSVdatabaseQueryObject> innerVectorNewObject; 
								neuronIDvector = &innerVectorNewObject;
								neuronConnectionMap.insert(std::pair<long, vector<H01indexedCSVdatabaseQueryObject>>(targetNeuronIDindex, innerVectorNewObject)); 
							}
							neuronIDvector->push_back(queryObject); 
						#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_NON_LOCAL_NEURONS_REQUIRED
						}
						#endif
					}
					#endif


					#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET
					if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET)
					{
						#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_RESTRICT_PREPOSTSYNAPTIC_CLASS_LABELS
						string classLabelContentsSmall1 = csvLineVector[INDEXED_CSV_DATABASE_PRESYNAPTIC_SITE_CLASSLABEL_FIELD_INDEX];
						string classLabelContentsSmall2 = csvLineVector[INDEXED_CSV_DATABASE_POSTSYNAPTIC_SITE_CLASSLABEL_FIELD_INDEX];
						if(SHAREDvars.textInTextArray(classLabelContentsSmall1, indexed_csv_database_presynaptic_site_classlabel_valid, INDEXED_CSV_DATABASE_PRESYNAPTIC_SITE_CLASSLABEL_VALID_NUM) && SHAREDvars.textInTextArray(classLabelContentsSmall2, indexed_csv_database_postsynaptic_site_classlabel_valid, INDEXED_CSV_DATABASE_POSTSYNAPTIC_SITE_CLASSLABEL_VALID_NUM))
						{
						#endif
							#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_LOCAL_NEURONS_REQUIRED		
							//verify that targetNeuronID centroid/soma is also in local connectome;
							if(neuronMap->count(targetNeuronID) != 0)	//verify that targetNeuronID is in neuronList
							{
							#endif
								string neuronConnectionString = "";

								int neuronIndexTarget = (*neuronMap)[targetNeuronID];
								vector<string>* localConnectomeNeuronTarget = &((*localConnectomeCSVdatasetNeurons)[neuronIndexTarget]);

								int excitationType;
								if(connectionTypesDerivedFromPresynapticNeuronsOrEMimages)
								{
									string presynapticNeuronType = "";
									if(queryByPresynapticConnectionNeurons)
									{
										presynapticNeuronType = (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE];
									}
									else
									{
										presynapticNeuronType = (*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE];
									}
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
										excitationType = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN;
										cerr << "H01indexedCSVdatabaseQueryClass::queryIndexedCSVdatabaseByNeuronList warning, source neuron type unknown, presynapticNeuronType = " << presynapticNeuronType << endl;
									}
								}
								else
								{
									if(connectionTypeInt == AVRO_JSON_DATABASE_EXCITATORY_SYNAPSE_TYPE)
									{
										excitationType = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY;
									}
									else if(connectionTypeInt == AVRO_JSON_DATABASE_INHIBITORY_SYNAPSE_TYPE)
									{
										excitationType = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY;
									}
									else
									{
										excitationType = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN;
										cerr << "H01indexedCSVdatabaseQueryClass::queryIndexedCSVdatabaseByNeuronList warning, connection type unknown, connectionType = " << connectionType << endl;
									}
								}

								int numberOfSynapses = 1;
								#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_VERIFICATION
								string pre_id; 
								string post_id;
								#endif
								//pre_id, pre_x, pre_y, pre_z, pre_type, post_id, post_x, post_y, post_z, post_type, post_class_label, syn_num, excitation_type
								string postClassLabel = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_CLASS_LABEL_UNKNOWN;
								#ifdef INDEXED_CSV_DATABASE_CREATE_CLASS_LABELS_SAVE_VERBATIM
								postClassLabel = csvLineVector[INDEXED_CSV_DATABASE_POSTSYNAPTIC_SITE_CLASSLABEL_FIELD_INDEX];
								#endif
								if(queryByPresynapticConnectionNeurons)
								{
									#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_VERIFICATION
									pre_id = sourceNeuronID;
									post_id = targetNeuronID;
									#endif
									neuronConnectionString = 
									sourceNeuronID + CSV_DELIMITER + (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_X] + CSV_DELIMITER + (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Y] + CSV_DELIMITER + (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Z] + CSV_DELIMITER + (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE] + CSV_DELIMITER + 
									targetNeuronID + CSV_DELIMITER + (*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_X] + CSV_DELIMITER + (*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Y] + CSV_DELIMITER + (*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Z] + CSV_DELIMITER + (*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE] + CSV_DELIMITER + 
									postClassLabel + CSV_DELIMITER + SHAREDvars.convertIntToString(numberOfSynapses) + CSV_DELIMITER + SHAREDvars.convertIntToString(excitationType) + STRING_NEWLINE;
								}
								else
								{
									#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_VERIFICATION
									pre_id = targetNeuronID;
									post_id = sourceNeuronID;
									#endif
									neuronConnectionString = 
									targetNeuronID + CSV_DELIMITER + (*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_X] + CSV_DELIMITER + (*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Y] + CSV_DELIMITER + (*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Z] + CSV_DELIMITER + (*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE] + CSV_DELIMITER + 
									sourceNeuronID + CSV_DELIMITER + (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_X] + CSV_DELIMITER + (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Y] + CSV_DELIMITER + (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Z] + CSV_DELIMITER + (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE] + CSV_DELIMITER + 
									postClassLabel + CSV_DELIMITER + SHAREDvars.convertIntToString(numberOfSynapses) + CSV_DELIMITER + SHAREDvars.convertIntToString(excitationType) + STRING_NEWLINE;		
								}

								#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_VERIFICATION
								string connectionsMapKey = pre_id + post_id;
								//cout << "connectionsMapKey to find = " << connectionsMapKey << endl;
								if(connectionsMap.count(connectionsMapKey) == 0)	//verify that connectionsMapKey is found in existing (dev/ODS generated) connections dataset
								{
									cout << "INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_VERIFICATION warning: generated connection not found in existing (dev/ODS generated) connections dataset; pre_id post_id = " << pre_id << " " << post_id << endl;
								}
								else
								{
									//cout << "INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_VERIFICATION: generated connection found in existing (dev/ODS generated) connections dataset; pre_id post_id = " << pre_id << " " << post_id << endl;
								}
								#endif

								#ifdef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_FILE_OBJECT
								SHAREDvars.writeStringToFileObject(neuronConnectionString, writeFileObject);
								#else
								this->appendStringBufferToPreallocatedString(writeFileString, neuronConnectionString);
								//writeFileString = writeFileString + neuronConnectionString	//inefficient
								#endif

							#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_LOCAL_NEURONS_REQUIRED		
							}
							#endif
						#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_RESTRICT_PREPOSTSYNAPTIC_CLASS_LABELS
						}
						#endif
					}
					#endif	

					#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS
					#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PROPORTION_LOCAL_VS_NONLOCAL_CONNECTIONS
					if((queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS) || (queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL))
					{
						#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
						int localConnectomeNeuronLayer = SHAREDvars.convertStringToInt((*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER]);
						#endif

						vector<string>* localConnectomeNeuronTarget = NULL;
						if(neuronMap->count(targetNeuronID) != 0)	//verify that targetNeuronID is in neuronList
						{
							int neuronIndexTarget = (*neuronMap)[targetNeuronID];
							localConnectomeNeuronTarget = &((*localConnectomeCSVdatasetNeurons)[neuronIndexTarget]);
						}
												
						int excitationType = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN;
						#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_PRESYNAPTIC_NEURONS
						if(connectionTypesDerivedFromPresynapticNeuronsOrEMimages)
						{
							string presynapticNeuronType = LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_UNKNOWN;
							if(queryByPresynapticConnectionNeurons)
							{
								presynapticNeuronType = (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE];
							}
							else
							{
								//verify that targetNeuronID centroid/soma is also in local connectome;
								if(localConnectomeNeuronTarget != NULL)	//verify that targetNeuronID is in neuronList
								{
									presynapticNeuronType = (*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE];
								}
								else
								{
									presynapticNeuronType = LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_UNKNOWN;	//excitationType will be LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN if presynaptic neuron is not in local connectome
								}
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
								excitationType = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN;	//excitationType will be LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN if presynaptic neuron is not in local connectome 
							}
						}
						else
						{
						#endif
							if(connectionTypeInt == AVRO_JSON_DATABASE_EXCITATORY_SYNAPSE_TYPE)
							{
								excitationType = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY;
							}
							else if(connectionTypeInt == AVRO_JSON_DATABASE_INHIBITORY_SYNAPSE_TYPE)
							{
								excitationType = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY;
							}
							else
							{
								excitationType = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN;
								cerr << "H01indexedCSVdatabaseQueryClass::queryIndexedCSVdatabaseByNeuronList warning, connection type unknown, connectionType = " << connectionType << endl;
							}
						#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_PRESYNAPTIC_NEURONS
						}
						#endif
	
						vec posSource;
						vec posTarget; 
						vec* posTargetPointer = &posTarget;
						#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL
						posSource.x = SHAREDvars.convertStringToInt((*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_X]);
						posSource.y = SHAREDvars.convertStringToInt((*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Y]);
						posSource.z = SHAREDvars.convertStringToInt((*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Z]);
						if(localConnectomeNeuronTarget != NULL)
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
						#endif
						vec sourceNeuronCorticalFlowVector;
						#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL_NORMALISE_LOCAL_CONNECTIVITY
						if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL)
						{
							sourceNeuronCorticalFlowVector.x = SHAREDvars.convertStringToInt((*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER_SURFACE_NORM_X]);
							sourceNeuronCorticalFlowVector.y = SHAREDvars.convertStringToInt((*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER_SURFACE_NORM_Y]);
						}
						#endif

						bool preAndPostSynapticNeuronAreInLocalConnectome = false;	
						if(neuronMap->count(targetNeuronID) != 0)	//verify that targetNeuronID is in neuronList
						{
							preAndPostSynapticNeuronAreInLocalConnectome = true;	//sourceNeuronID and targetNeuronID are in local connectome 
						}
						if(preAndPostSynapticNeuronAreInLocalConnectome)
						{
							incrementNumberOfConnections(&(*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].neuronModelConnectionsLocalConnectome, &posSource, posTargetPointer, &sourceNeuronCorticalFlowVector);
							#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
							incrementNumberOfConnections(&(*connectivityModelLayers)[localConnectomeNeuronLayer].neuronModelConnectionsLocalConnectome, &posSource, posTargetPointer, &sourceNeuronCorticalFlowVector);
							#endif							
							if(excitationType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY)
							{
								incrementNumberOfConnections(&(*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].neuronModelConnectionsLocalConnectomeExcitatory, &posSource, posTargetPointer, &sourceNeuronCorticalFlowVector);
								#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
								incrementNumberOfConnections(&(*connectivityModelLayers)[localConnectomeNeuronLayer].neuronModelConnectionsLocalConnectomeExcitatory, &posSource, posTargetPointer, &sourceNeuronCorticalFlowVector);
								#endif
								#ifdef DEBUG_INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PRINT_OUTPUT_VERBOSE_LOCALORNONLOCAL
								numberConnectionsLocalConnectomeExcitatoryCurrent++;
								#endif
							}
							else if(excitationType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY)
							{
								incrementNumberOfConnections(&(*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].neuronModelConnectionsLocalConnectomeInhibitory, &posSource, posTargetPointer, &sourceNeuronCorticalFlowVector);
								#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
								incrementNumberOfConnections(&(*connectivityModelLayers)[localConnectomeNeuronLayer].neuronModelConnectionsLocalConnectomeInhibitory, &posSource, posTargetPointer, &sourceNeuronCorticalFlowVector);
								#endif
								#ifdef DEBUG_INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PRINT_OUTPUT_VERBOSE_LOCALORNONLOCAL
								numberConnectionsLocalConnectomeInhibitoryCurrent++;
								#endif
							}
							else
							{
								cerr << "H01indexedCSVdatabaseQueryClass::queryIndexedCSVdatabaseByNeuronList warning, connection type unknown, connectionType = " << connectionType << endl;
							}
						}
						else
						{
							incrementNumberOfConnections(&(*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].neuronModelConnectionsExternalConnectome, &posSource, posTargetPointer, &sourceNeuronCorticalFlowVector);
							#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
							incrementNumberOfConnections(&(*connectivityModelLayers)[localConnectomeNeuronLayer].neuronModelConnectionsExternalConnectome, &posSource, posTargetPointer, &sourceNeuronCorticalFlowVector);
							#endif							
							if(excitationType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY)
							{
								incrementNumberOfConnections(&(*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].neuronModelConnectionsExternalConnectomeExcitatory, &posSource, posTargetPointer, &sourceNeuronCorticalFlowVector);
								#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
								incrementNumberOfConnections(&(*connectivityModelLayers)[localConnectomeNeuronLayer].neuronModelConnectionsExternalConnectomeExcitatory, &posSource, posTargetPointer, &sourceNeuronCorticalFlowVector);
								#endif
								#ifdef DEBUG_INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PRINT_OUTPUT_VERBOSE_LOCALORNONLOCAL
								numberConnectionsLocalConnectomeExcitatoryCurrent++;
								#endif
							}
							else if(excitationType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY)
							{
								incrementNumberOfConnections(&(*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].neuronModelConnectionsExternalConnectomeInhibitory, &posSource, posTargetPointer, &sourceNeuronCorticalFlowVector);
								#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
								incrementNumberOfConnections(&(*connectivityModelLayers)[localConnectomeNeuronLayer].neuronModelConnectionsExternalConnectomeInhibitory, &posSource, posTargetPointer, &sourceNeuronCorticalFlowVector);
								#endif
								#ifdef DEBUG_INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PRINT_OUTPUT_VERBOSE_LOCALORNONLOCAL
								numberConnectionsLocalConnectomeInhibitoryCurrent++;
								#endif
							}
							else
							{
								#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_PRESYNAPTIC_NEURONS
								//excitationType will be LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN if presynaptic neuron is not in local connectome (so numberConnectionsExternalConnectomeExcitatory+numberConnectionsExternalConnectomeInhibitory != numberConnectionsExternalConnectome)
								#else
								cerr << "H01indexedCSVdatabaseQueryClass::queryIndexedCSVdatabaseByNeuronList warning, connection type unknown, connectionType = " << connectionType << endl;
								exit(EXIT_ERROR);
								#endif
							}
						}
					}
					#endif
					#endif
				#ifdef INDEXED_CSV_DATABASE_QUERY_CONFIDENCE_THRESHOLD_REQUIRED
				}
				#endif
			}

			if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_EXTRACT_INCOMING_OUTGOING_CONNECTIONS)
			{
				//notify user that program is still running
				cout << "neuronIndex = " << neuronIndex << ", neuronID = " << neuronID << ", neuronConnectionList.size() = " << neuronConnectionList.size() << endl;
			}
			
			#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS
			if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS)
			{
				#ifdef DEBUG_INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PRINT_OUTPUT_VERBOSE_LOCALORNONLOCAL
				cout << neuronID << " " << numberConnectionsLocalConnectomeExcitatoryCurrent << " " << numberConnectionsLocalConnectomeInhibitoryCurrent << endl;
				#endif
			}
			#endif	
		}
		else
		{
			#ifdef INDEXED_CSV_DATABASE_QUERY_EXTRACT_INCOMING_OUTGOING_CONNECTIONS
			if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_EXTRACT_INCOMING_OUTGOING_CONNECTIONS)
			{
				//add a new line such that database lengths are matched
				#ifdef INDEXED_CSV_DATABASE_QUERY_OUTPUT
				#ifdef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_FILE_OBJECT
				SHAREDvars.writeStringToFileObject(STRING_NEWLINE, writeFileObject);
				#else
				this->appendStringBufferToPreallocatedString(writeFileString, STRING_NEWLINE);
				//writeFileString = writeFileString + STRING_NEWLINE;	//inefficient
				#endif			
				#endif
			}
			#endif	
			#ifdef LOCAL_CONNECTOME_OFFICAL_RELEASE_C3_SOMAS_EXPECT_SOME_MISSING_FROM_C3_CONNECTIONS_DATABASE
	
			#endif
		}
	}

	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS
	if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS)
	{
		printNumberOfConnectionsLayers(queryByPresynapticConnectionNeurons, connectivityModelLayers, true, true, true);
	}
	#endif

	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS
	if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL)
	{	
		#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL
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

#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL
void H01indexedCSVdatabaseQueryClass::generateLargeModelNeuronsAndConnectionsLayers(constEffective vector<H01connectivityModelClass>* connectivityModelLayersIncomingLayers, constEffective vector<H01connectivityModelClass>* connectivityModelLayersOutgoingLayers, const bool countLocalConnectomeNeurons, const bool countInternalConnectomeConnections, const bool countExternalConnectomeConnections)
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
			
		generateLargeModelNeuronsAndConnections(connectivityModelLayersIncoming, connectivityModelLayersOutgoing, countLocalConnectomeNeurons, countInternalConnectomeConnections, countExternalConnectomeConnections);

	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
	}
	#endif
}

void H01indexedCSVdatabaseQueryClass::generateLargeModelNeuronsAndConnections(const H01connectivityModelClass* connectivityModelLayersIncoming, const H01connectivityModelClass* connectivityModelLayersOutgoing, const bool countLocalConnectomeNeurons, const bool countInternalConnectomeConnections, const bool countExternalConnectomeConnections)
{
	/*
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_NUMBER_EXCITATORY_INHIBITORY_NEURONS
	if(countLocalConnectomeNeurons)
	{
		cout << "\tnumberOfLocalConnectomeNeurons = " << connectivityModel->numberOfLocalConnectomeNeurons << endl;
		cout << "\tnumberOfLocalConnectomeNeuronsExcitatory = " << connectivityModel->numberOfLocalConnectomeNeuronsExcitatory << endl;
		cout << "\tnumberOfLocalConnectomeNeuronsInhibitory = " << connectivityModel->numberOfLocalConnectomeNeuronsInhibitory << endl;
	}
	#endif	
	double normalisationFactorZ = 0.0;
	double numberConnectionsLocalConnectomeNormalisedZ = 0.0;
	double numberConnectionsLocalConnectomeExcitatoryNormalisedZ = 0.0;
	double numberConnectionsLocalConnectomeInhibitoryNormalisedZ = 0.0;
	if(countInternalConnectomeConnections)
	{
		cout << "\tnumberConnectionsLocalConnectome = " << connectivityModel->neuronModelConnectionsLocalConnectome->numberConnections << endl;
		cout << "\tnumberConnectionsLocalConnectomeExcitatory = " << connectivityModel->neuronModelConnectionsLocalConnectomeExcitatory->numberConnections << endl;
		cout << "\tnumberConnectionsLocalConnectomeInhibitory = " << connectivityModel->neuronModelConnectionsLocalConnectomeInhibitory->numberConnections << endl;
		normalisationFactorZ = getZNormalisationFactor();
		numberConnectionsLocalConnectomeNormalisedZ = double(connectivityModel->neuronModelConnectionsLocalConnectome->numberConnections) * normalisationFactorZ;
		numberConnectionsLocalConnectomeExcitatoryNormalisedZ = double(connectivityModel->neuronModelConnectionsLocalConnectomeExcitatory->numberConnections) * normalisationFactorZ;
		numberConnectionsLocalConnectomeInhibitoryNormalisedZ = double(connectivityModel->neuronModelConnectionsLocalConnectomeInhibitory->numberConnections) * normalisationFactorZ;
		cout << "\tnormalisationFactorZ = " << normalisationFactorZ << endl;
		cout << "\tnumberConnectionsLocalConnectomeNormalisedZ = " << numberConnectionsLocalConnectomeNormalisedZ << endl;
		cout << "\tnumberConnectionsLocalConnectomeExcitatoryNormalisedZ = " << numberConnectionsLocalConnectomeExcitatoryNormalisedZ << endl;
		cout << "\tnumberConnectionsLocalConnectomeInhibitoryNormalisedZ = " << numberConnectionsLocalConnectomeInhibitoryNormalisedZ << endl;
	}
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PROPORTION_LOCAL_VS_NONLOCAL_CONNECTIONS
	if(countExternalConnectomeConnections)
	{
		cout << "\tnumberConnectionsExternalConnectome = " << connectivityModel->neuronModelConnectionsExternalConnectome->numberConnections << endl;
		cout << "\tnumberConnectionsExternalConnectomeExcitatory = " << connectivityModel->neuronModelConnectionsExternalConnectomeExcitatory->numberConnections << endl;
		cout << "\tnumberConnectionsExternalConnectomeInhibitory = " << connectivityModel->neuronModelConnectionsExternalConnectomeInhibitory->numberConnections << endl;
		double fractionOfConnectionsLocalConnectome = numberConnectionsLocalConnectomeNormalisedZ/double(numberConnectionsLocalConnectomeNormalisedZ+connectivityModel->neuronModelConnectionsExternalConnectome->numberConnections);
		double fractionOfConnectionsLocalConnectomeExcitatory = numberConnectionsLocalConnectomeExcitatoryNormalisedZ/double(numberConnectionsLocalConnectomeExcitatoryNormalisedZ+connectivityModel->neuronModelConnectionsExternalConnectomeExcitatory->numberConnections);
		double fractionOfConnectionsLocalConnectomeInhibitory = numberConnectionsLocalConnectomeInhibitoryNormalisedZ/double(numberConnectionsLocalConnectomeInhibitoryNormalisedZ+connectivityModel->neuronModelConnectionsExternalConnectomeInhibitory->numberConnections);
		//double fractionOfConnectionsLocalConnectome = numberConnectionsLocalConnectome/(numberConnectionsLocalConnectome+numberConnectionsExternalConnectome);
		cout << "\tfractionOfConnectionsLocalConnectome = " << fractionOfConnectionsLocalConnectome << endl;
		cout << "\tfractionOfConnectionsLocalConnectomeExcitatory = " << fractionOfConnectionsLocalConnectomeExcitatory << endl;
		cout << "\tfractionOfConnectionsLocalConnectomeInhibitory = " << fractionOfConnectionsLocalConnectomeInhibitory << endl;
	}
	#endif

	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_NUMBER_EXCITATORY_INHIBITORY_NEURONS		
	if(countLocalConnectomeNeurons)
	{
		if(countInternalConnectomeConnections)
		{
			double numberConnectionsLocalConnectomePerNeuron = connectivityModel->neuronModelConnectionsLocalConnectome->numberConnections/double(connectivityModel->numberOfLocalConnectomeNeurons);
			double numberConnectionsLocalConnectomePerNeuronExcitatory = connectivityModel->neuronModelConnectionsLocalConnectomeExcitatory->numberConnections/double(connectivityModel->numberOfLocalConnectomeNeuronsExcitatory);
			double numberConnectionsLocalConnectomePerNeuronInhibitory = connectivityModel->neuronModelConnectionsLocalConnectomeInhibitory->numberConnections/double(connectivityModel->numberOfLocalConnectomeNeuronsInhibitory);
			double numberConnectionsLocalConnectomePerNeuronNormalisedZ = numberConnectionsLocalConnectomeNormalisedZ/double(connectivityModel->numberOfLocalConnectomeNeurons);
			double numberConnectionsLocalConnectomePerNeuronExcitatoryNormalisedZ = numberConnectionsLocalConnectomeExcitatoryNormalisedZ/double(connectivityModel->numberOfLocalConnectomeNeuronsExcitatory);
			double numberConnectionsLocalConnectomePerNeuronInhibitoryNormalisedZ = numberConnectionsLocalConnectomeInhibitoryNormalisedZ/double(connectivityModel->numberOfLocalConnectomeNeuronsInhibitory);
			cout << "\tnumberConnectionsLocalConnectomePerNeuron = " << numberConnectionsLocalConnectomePerNeuron << endl;
			cout << "\tnumberConnectionsLocalConnectomePerNeuronExcitatory = " << numberConnectionsLocalConnectomePerNeuronExcitatory << endl;
			cout << "\tnumberConnectionsLocalConnectomePerNeuronInhibitory = " << numberConnectionsLocalConnectomePerNeuronInhibitory << endl;
			cout << "\tnumberConnectionsLocalConnectomePerNeuronNormalisedZ = " << numberConnectionsLocalConnectomePerNeuronNormalisedZ << endl;
			cout << "\tnumberConnectionsLocalConnectomePerNeuronExcitatoryNormalisedZ = " << numberConnectionsLocalConnectomePerNeuronExcitatoryNormalisedZ << endl;
			cout << "\tnumberConnectionsLocalConnectomePerNeuronInhibitoryNormalisedZ = " << numberConnectionsLocalConnectomePerNeuronInhibitoryNormalisedZ << endl;		
		}
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PROPORTION_LOCAL_VS_NONLOCAL_CONNECTIONS
		if(countExternalConnectomeConnections)
		{
			double numberConnectionsExternalConnectomePerNeuron = connectivityModel->neuronModelConnectionsExternalConnectome->numberConnections/double(connectivityModel->numberOfLocalConnectomeNeurons);
			double numberConnectionsExternalConnectomePerNeuronExcitatory = connectivityModel->neuronModelConnectionsExternalConnectomeExcitatory->numberConnections/double(connectivityModel->numberOfLocalConnectomeNeuronsExcitatory);
			double numberConnectionsExternalConnectomePerNeuronInhibitory = connectivityModel->neuronModelConnectionsExternalConnectomeInhibitory->numberConnections/double(connectivityModel->numberOfLocalConnectomeNeuronsInhibitory);
			cout << "\tnumberConnectionsExternalConnectomePerNeuron = " << numberConnectionsExternalConnectomePerNeuron << endl;
			cout << "\tnumberConnectionsExternalConnectomePerNeuronExcitatory = " << numberConnectionsExternalConnectomePerNeuronExcitatory << endl;
			cout << "\tnumberConnectionsExternalConnectomePerNeuronInhibitory = " << numberConnectionsExternalConnectomePerNeuronInhibitory << endl;
		}
		#endif
	}
	#endif
	*/
}
#endif

#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS
double H01indexedCSVdatabaseQueryClass::getZNormalisationFactor()
{
	double normalisationFactorZ = double(AVRO_JSON_DATABASE_COORDINATES_RANGE_X*AVRO_JSON_DATABASE_COORDINATES_CALIBRATION_X + AVRO_JSON_DATABASE_COORDINATES_RANGE_Y*AVRO_JSON_DATABASE_COORDINATES_CALIBRATION_Y)/2.0 / (AVRO_JSON_DATABASE_COORDINATES_RANGE_Z*AVRO_JSON_DATABASE_COORDINATES_CALIBRATION_Z);
	return normalisationFactorZ;
}
	
void H01indexedCSVdatabaseQueryClass::printNumberOfConnectionsLayers(const bool queryByPresynapticConnectionNeurons, constEffective vector<H01connectivityModelClass>* connectivityModelLayers, const bool countLocalConnectomeNeurons, const bool countInternalConnectomeConnections, const bool countExternalConnectomeConnections)
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
			
		printNumberOfConnections(queryByPresynapticConnectionNeurons, connectivityModel, countLocalConnectomeNeurons, countInternalConnectomeConnections, countExternalConnectomeConnections);

	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
	}
	#endif
}

void H01indexedCSVdatabaseQueryClass::printNumberOfConnections(const bool queryByPresynapticConnectionNeurons, const H01connectivityModelClass* connectivityModel, const bool countLocalConnectomeNeurons, const bool countInternalConnectomeConnections, const bool countExternalConnectomeConnections)
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
	cout << "\tINDEXED_CSV_DATABASE_QUERY_COUNT_PROPORTION_LOCAL_VS_NONLOCAL_CONNECTIONS: queryByPresynapticConnectionNeurons (F:incoming/T:outgoing) = " << SHAREDvars.convertBoolToString(queryByPresynapticConnectionNeurons) << endl;
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_NUMBER_EXCITATORY_INHIBITORY_NEURONS
	if(countLocalConnectomeNeurons)
	{
		cout << "\tnumberOfLocalConnectomeNeurons = " << connectivityModel->numberOfLocalConnectomeNeurons << endl;
		cout << "\tnumberOfLocalConnectomeNeuronsExcitatory = " << connectivityModel->numberOfLocalConnectomeNeuronsExcitatory << endl;
		cout << "\tnumberOfLocalConnectomeNeuronsInhibitory = " << connectivityModel->numberOfLocalConnectomeNeuronsInhibitory << endl;
	}
	#endif	
	double normalisationFactorZ = 0.0;
	double numberConnectionsLocalConnectomeNormalisedZ = 0.0;
	double numberConnectionsLocalConnectomeExcitatoryNormalisedZ = 0.0;
	double numberConnectionsLocalConnectomeInhibitoryNormalisedZ = 0.0;
	if(countInternalConnectomeConnections)
	{
		cout << "\tnumberConnectionsLocalConnectome = " << connectivityModel->neuronModelConnectionsLocalConnectome.numberConnections << endl;
		cout << "\tnumberConnectionsLocalConnectomeExcitatory = " << connectivityModel->neuronModelConnectionsLocalConnectomeExcitatory.numberConnections << endl;
		cout << "\tnumberConnectionsLocalConnectomeInhibitory = " << connectivityModel->neuronModelConnectionsLocalConnectomeInhibitory.numberConnections << endl;
		normalisationFactorZ = getZNormalisationFactor();
		numberConnectionsLocalConnectomeNormalisedZ = double(connectivityModel->neuronModelConnectionsLocalConnectome.numberConnections) * normalisationFactorZ;
		numberConnectionsLocalConnectomeExcitatoryNormalisedZ = double(connectivityModel->neuronModelConnectionsLocalConnectomeExcitatory.numberConnections) * normalisationFactorZ;
		numberConnectionsLocalConnectomeInhibitoryNormalisedZ = double(connectivityModel->neuronModelConnectionsLocalConnectomeInhibitory.numberConnections) * normalisationFactorZ;
		cout << "\tnormalisationFactorZ = " << normalisationFactorZ << endl;
		cout << "\tnumberConnectionsLocalConnectomeNormalisedZ = " << numberConnectionsLocalConnectomeNormalisedZ << endl;
		cout << "\tnumberConnectionsLocalConnectomeExcitatoryNormalisedZ = " << numberConnectionsLocalConnectomeExcitatoryNormalisedZ << endl;
		cout << "\tnumberConnectionsLocalConnectomeInhibitoryNormalisedZ = " << numberConnectionsLocalConnectomeInhibitoryNormalisedZ << endl;
	}
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PROPORTION_LOCAL_VS_NONLOCAL_CONNECTIONS
	if(countExternalConnectomeConnections)
	{
		cout << "\tnumberConnectionsExternalConnectome = " << connectivityModel->neuronModelConnectionsExternalConnectome.numberConnections << endl;
		cout << "\tnumberConnectionsExternalConnectomeExcitatory = " << connectivityModel->neuronModelConnectionsExternalConnectomeExcitatory.numberConnections << endl;
		cout << "\tnumberConnectionsExternalConnectomeInhibitory = " << connectivityModel->neuronModelConnectionsExternalConnectomeInhibitory.numberConnections << endl;
		double fractionOfConnectionsLocalConnectome = numberConnectionsLocalConnectomeNormalisedZ/double(numberConnectionsLocalConnectomeNormalisedZ+connectivityModel->neuronModelConnectionsExternalConnectome.numberConnections);
		double fractionOfConnectionsLocalConnectomeExcitatory = numberConnectionsLocalConnectomeExcitatoryNormalisedZ/double(numberConnectionsLocalConnectomeExcitatoryNormalisedZ+connectivityModel->neuronModelConnectionsExternalConnectomeExcitatory.numberConnections);
		double fractionOfConnectionsLocalConnectomeInhibitory = numberConnectionsLocalConnectomeInhibitoryNormalisedZ/double(numberConnectionsLocalConnectomeInhibitoryNormalisedZ+connectivityModel->neuronModelConnectionsExternalConnectomeInhibitory.numberConnections);
		//double fractionOfConnectionsLocalConnectome = numberConnectionsLocalConnectome/(numberConnectionsLocalConnectome+numberConnectionsExternalConnectome);
		cout << "\tfractionOfConnectionsLocalConnectome = " << fractionOfConnectionsLocalConnectome << endl;
		cout << "\tfractionOfConnectionsLocalConnectomeExcitatory = " << fractionOfConnectionsLocalConnectomeExcitatory << endl;
		cout << "\tfractionOfConnectionsLocalConnectomeInhibitory = " << fractionOfConnectionsLocalConnectomeInhibitory << endl;
	}
	#endif

	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_NUMBER_EXCITATORY_INHIBITORY_NEURONS		
	if(countLocalConnectomeNeurons)
	{
		if(countInternalConnectomeConnections)
		{
			double numberConnectionsLocalConnectomePerNeuron = connectivityModel->neuronModelConnectionsLocalConnectome.numberConnections/double(connectivityModel->numberOfLocalConnectomeNeurons);
			double numberConnectionsLocalConnectomePerNeuronExcitatory = connectivityModel->neuronModelConnectionsLocalConnectomeExcitatory.numberConnections/double(connectivityModel->numberOfLocalConnectomeNeuronsExcitatory);
			double numberConnectionsLocalConnectomePerNeuronInhibitory = connectivityModel->neuronModelConnectionsLocalConnectomeInhibitory.numberConnections/double(connectivityModel->numberOfLocalConnectomeNeuronsInhibitory);
			double numberConnectionsLocalConnectomePerNeuronNormalisedZ = numberConnectionsLocalConnectomeNormalisedZ/double(connectivityModel->numberOfLocalConnectomeNeurons);
			double numberConnectionsLocalConnectomePerNeuronExcitatoryNormalisedZ = numberConnectionsLocalConnectomeExcitatoryNormalisedZ/double(connectivityModel->numberOfLocalConnectomeNeuronsExcitatory);
			double numberConnectionsLocalConnectomePerNeuronInhibitoryNormalisedZ = numberConnectionsLocalConnectomeInhibitoryNormalisedZ/double(connectivityModel->numberOfLocalConnectomeNeuronsInhibitory);
			cout << "\tnumberConnectionsLocalConnectomePerNeuron = " << numberConnectionsLocalConnectomePerNeuron << endl;
			cout << "\tnumberConnectionsLocalConnectomePerNeuronExcitatory = " << numberConnectionsLocalConnectomePerNeuronExcitatory << endl;
			cout << "\tnumberConnectionsLocalConnectomePerNeuronInhibitory = " << numberConnectionsLocalConnectomePerNeuronInhibitory << endl;
			cout << "\tnumberConnectionsLocalConnectomePerNeuronNormalisedZ = " << numberConnectionsLocalConnectomePerNeuronNormalisedZ << endl;
			cout << "\tnumberConnectionsLocalConnectomePerNeuronExcitatoryNormalisedZ = " << numberConnectionsLocalConnectomePerNeuronExcitatoryNormalisedZ << endl;
			cout << "\tnumberConnectionsLocalConnectomePerNeuronInhibitoryNormalisedZ = " << numberConnectionsLocalConnectomePerNeuronInhibitoryNormalisedZ << endl;		
		}
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PROPORTION_LOCAL_VS_NONLOCAL_CONNECTIONS
		if(countExternalConnectomeConnections)
		{
			double numberConnectionsExternalConnectomePerNeuron = connectivityModel->neuronModelConnectionsExternalConnectome.numberConnections/double(connectivityModel->numberOfLocalConnectomeNeurons);
			double numberConnectionsExternalConnectomePerNeuronExcitatory = connectivityModel->neuronModelConnectionsExternalConnectomeExcitatory.numberConnections/double(connectivityModel->numberOfLocalConnectomeNeuronsExcitatory);
			double numberConnectionsExternalConnectomePerNeuronInhibitory = connectivityModel->neuronModelConnectionsExternalConnectomeInhibitory.numberConnections/double(connectivityModel->numberOfLocalConnectomeNeuronsInhibitory);
			cout << "\tnumberConnectionsExternalConnectomePerNeuron = " << numberConnectionsExternalConnectomePerNeuron << endl;
			cout << "\tnumberConnectionsExternalConnectomePerNeuronExcitatory = " << numberConnectionsExternalConnectomePerNeuronExcitatory << endl;
			cout << "\tnumberConnectionsExternalConnectomePerNeuronInhibitory = " << numberConnectionsExternalConnectomePerNeuronInhibitory << endl;
		}
		#endif
	}
	#endif
}


#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL
//count connections for all local connectome neuron 1 Layer X to local connectome neuron 2 layer Y;
bool H01indexedCSVdatabaseQueryClass::countConnectionsLocal(const int queryMode, vector<string>* neuronList, map<string, int>* neuronMap, vector<vector<string>>* localConnectomeCSVdatasetNeurons, vector<vector<string>>* localConnectomeCSVdatasetConnections, const bool queryPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages)
{
	bool result = true;

	bool queryByPresynapticConnectionNeurons = !queryPresynapticConnectionNeurons;

	//vector<H01connectivityModelClass>* connectivityModelLayersLayers = generateNumberOfConnectionsLayersLayers();
	
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL_BY_LAYER
	int numberLayers = CORTICAL_LAYER_NUMBER_OF_LAYERS;
	#endif
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL_BY_LAYER
	for(int sourceLayerIndex=0; sourceLayerIndex<=CORTICAL_LAYER_NUMBER_OF_LAYERS; sourceLayerIndex++)
	{
	#else
	int sourceLayerIndex = 0;	//all layers
	#endif
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL_BY_LAYER
		for(int targetLayerIndex=0; targetLayerIndex<=CORTICAL_LAYER_NUMBER_OF_LAYERS; targetLayerIndex++)
		{
		#else
		int targetLayerIndex = 0;	//all layers
		#endif
			//H01connectivityModelClass* connectivityModel = connectivityModelLayersLayers[sourceLayerIndex][targetLayerIndex];
			
			#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL_BY_EXCITATION_TYPE
			int sourceExcitationTypeIndex = LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN;
			for(int sourceExcitationTypeIndex=LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN; sourceExcitationTypeIndex<=LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY; sourceExcitationTypeIndex++)
			{
			#else
			int sourceExcitationTypeIndex = LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN;	//all excitation types
			#endif
				#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL_BY_EXCITATION_TYPE
				int targetExcitationTypeIndex = LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN;
				for(int targetExcitationTypeIndex=LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN; targetExcitationTypeIndex<=LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY; targetExcitationTypeIndex++)
				{
				#else
				int targetExcitationTypeIndex = LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN;	//all excitation types
				#endif
						
					cout << "" << endl;
					#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL_BY_LAYER	
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
					#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL_BY_EXCITATION_TYPE
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
					#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL_NUMBER_EXCITATORY_INHIBITORY_NEURONS
					map<string, int> independentSourceNeuronMap;
					#endif
					/*						
					int numberConnectionsLocalConnectome = 0;
					int numberConnectionsLocalConnectomeExcitatory = 0;
					int numberConnectionsLocalConnectomeInhibitory = 0;
					int numberConnectionsExternalConnectome = 0;	//NOTUSED
					int numberConnectionsExternalConnectomeExcitatory = 0;	//NOTUSED
					int numberConnectionsExternalConnectomeInhibitory = 0;	//NOTUSED
					#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL_NUMBER_EXCITATORY_INHIBITORY_NEURONS
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
						int excitationType;
						#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL_NUMBER_EXCITATORY_INHIBITORY_NEURONS
						string sourceNeuronType = "";
						#endif
						if(queryByPresynapticConnectionNeurons)
						{
							#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL_NUMBER_EXCITATORY_INHIBITORY_NEURONS
							sourceNeuronType = (localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_TYPE];
							#endif
							sourceNeuronID = (localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_ID];
							targetNeuronID = (localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_ID];
						}
						else
						{
							#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL_NUMBER_EXCITATORY_INHIBITORY_NEURONS
							sourceNeuronType = (localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_TYPE];
							#endif
							sourceNeuronID = (localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_ID];
							targetNeuronID = (localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_ID];
						}

						int neuronIndexSource = (*neuronMap)[sourceNeuronID];
						vector<string>* localConnectomeNeuronSource = &((*localConnectomeCSVdatasetNeurons)[neuronIndexSource]);
						int neuronIndexTarget = (*neuronMap)[targetNeuronID];
						vector<string>* localConnectomeNeuronTarget = &((*localConnectomeCSVdatasetNeurons)[neuronIndexTarget]);							
						
						#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL_NUMBER_EXCITATORY_INHIBITORY_CONNECTIONS
						#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_PRESYNAPTIC_NEURONS
						if(connectionTypesDerivedFromPresynapticNeuronsOrEMimages)
						{
							string presynapticNeuronType = LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_UNKNOWN;
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
								cerr << "H01indexedCSVdatabaseQueryClass::countConnectionsLocal error: presynapticNeuronType unknown" << endl;
								exit(EXIT_ERROR);
							}
						}
						else
						{
						#endif
							excitationType = SHAREDvars.convertStringToInt((localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE]);
						#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_PRESYNAPTIC_NEURONS
						}
						#endif
						#endif

						bool layerRequirements = true;	
						#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL_BY_LAYER	
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
						#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL_BY_EXCITATION_TYPE
						string sourceNeuronExcitationType = (*localConnectomeNeuronSource)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE];
						string targetNeuronExcitationType = (*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE];	
						int sourceNeuronExcitationTypeIndex = LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN;
						int targetNeuronExcitationTypeIndex = LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN;	
						if(sourceNeuronExcitationType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_PYRAMIDAL)
						{
							sourceNeuronExcitationTypeIndex = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY;
						}
						else if(sourceNeuronExcitationType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_INTERNEURON)
						{
							sourceNeuronExcitationTypeIndex = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY;
						}
						else
						{
							cerr << "H01indexedCSVdatabaseQueryClass::countConnectionsLocal error: sourceNeuronExcitationType unknown" << endl;
							exit(EXIT_ERROR);
						}
						if(targetNeuronExcitationType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_PYRAMIDAL)
						{
							targetNeuronExcitationTypeIndex = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY;
						}
						else if(targetNeuronExcitationType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_INTERNEURON)
						{
							targetNeuronExcitationTypeIndex = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY;
						}
						else
						{
							cerr << "H01indexedCSVdatabaseQueryClass::countConnectionsLocal error: targetNeuronExcitationType unknown" << endl;
							exit(EXIT_ERROR);
						}
						//cout << "sourceNeuronExcitationTypeIndex = " << sourceNeuronExcitationTypeIndex << endl;
						//cout << "targetNeuronExcitationTypeIndex = " << targetNeuronExcitationTypeIndex << endl;
						if(sourceExcitationTypeIndex == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN)
						{
						}
						else if(sourceExcitationTypeIndex == sourceNeuronExcitationTypeIndex)
						{
						}
						else
						{
							excitationTypeRequirements = false;
						}
						if(targetExcitationTypeIndex == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN)
						{
						}
						else if(targetExcitationTypeIndex == targetNeuronExcitationTypeIndex)
						{
						}
						else
						{
							excitationTypeRequirements = false;
						}
						#endif
	

						vec posSource;
						vec posTarget; 
						#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL
						posSource.x = SHAREDvars.convertStringToInt((*localConnectomeNeuronSource)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_X]);
						posSource.y = SHAREDvars.convertStringToInt((*localConnectomeNeuronSource)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Y]);
						posSource.z = SHAREDvars.convertStringToInt((*localConnectomeNeuronSource)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Z]);
						posTarget.x = SHAREDvars.convertStringToInt((*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_X]);
						posTarget.y = SHAREDvars.convertStringToInt((*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_X]);
						posTarget.z = SHAREDvars.convertStringToInt((*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_X]);						
						#endif
						vec sourceNeuronCorticalFlowVector;
						#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL_NORMALISE_LOCAL_CONNECTIVITY
						if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL)
						{
							sourceNeuronCorticalFlowVector.x = SHAREDvars.convertStringToInt((*localConnectomeNeuronSource)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER_SURFACE_NORM_X]);
							sourceNeuronCorticalFlowVector.y = SHAREDvars.convertStringToInt((*localConnectomeNeuronSource)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER_SURFACE_NORM_Y]);
						}
						#endif
									
						if(layerRequirements && excitationTypeRequirements)
						{
							//only count local connectome connections (not external connections);
							incrementNumberOfConnections(&(connectivityModel.neuronModelConnectionsLocalConnectome), &posSource, &posTarget, &sourceNeuronCorticalFlowVector);
							#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL_NUMBER_EXCITATORY_INHIBITORY_CONNECTIONS
							if(excitationType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY)
							{
								incrementNumberOfConnections(&(connectivityModel.neuronModelConnectionsLocalConnectomeExcitatory), &posSource, &posTarget, &sourceNeuronCorticalFlowVector);
							}
							else if(excitationType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY)
							{
								incrementNumberOfConnections(&(connectivityModel.neuronModelConnectionsLocalConnectomeInhibitory), &posSource, &posTarget, &sourceNeuronCorticalFlowVector);
							}
							#endif
							#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL_NUMBER_EXCITATORY_INHIBITORY_NEURONS
							//count unique neurons
							if(independentSourceNeuronMap.count(sourceNeuronID) == 0)	//verify that sourceNeuronID is not in independentSourceNeuronMap
							{
								independentSourceNeuronMap[sourceNeuronID] = (*neuronMap)[sourceNeuronID];
								connectivityModel.numberOfLocalConnectomeNeurons++;
								if(sourceNeuronType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_TYPE_PYRAMIDAL)
								{
									connectivityModel.numberOfLocalConnectomeNeuronsExcitatory++;
								}
								else if(sourceNeuronType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_TYPE_INTERNEURON)
								{
									connectivityModel.numberOfLocalConnectomeNeuronsInhibitory++;
								}
							}
							#endif
						}
					}
					
					#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL	
					//not currently required;
					averageGaussianQuad(&(connectivityModel.neuronModelConnectionsLocalConnectome));
					averageGaussianQuad(&(connectivityModel.neuronModelConnectionsLocalConnectomeExcitatory));
					averageGaussianQuad(&(connectivityModel.neuronModelConnectionsLocalConnectomeInhibitory));
					#endif
					
					bool countNumberInhibitoryExcitatoryNeurons = false;
					#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL_NUMBER_EXCITATORY_INHIBITORY_NEURONS
					countNumberInhibitoryExcitatoryNeurons = true;
					#endif
		
					cout << "\tnumberConnectionsLocalConnectome = " << connectivityModel.neuronModelConnectionsLocalConnectome.numberConnections << endl;
					#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL_NUMBER_EXCITATORY_INHIBITORY_CONNECTIONS
					cout << "\t\tnumberConnectionsLocalConnectomeExcitatory = " << connectivityModel.neuronModelConnectionsLocalConnectomeExcitatory.numberConnections << endl;
					cout << "\t\tnumberConnectionsLocalConnectomeInhibitory = " << connectivityModel.neuronModelConnectionsLocalConnectomeInhibitory.numberConnections << endl;
					#endif		
					#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL_NUMBER_EXCITATORY_INHIBITORY_NEURONS			
					cout << "\t\tnumberOfLocalConnectomeNeurons = " << connectivityModel.numberOfLocalConnectomeNeurons << endl;
					cout << "\t\tnumberOfLocalConnectomeNeuronsExcitatory = " << connectivityModel.numberOfLocalConnectomeNeuronsExcitatory << endl;
					cout << "\t\tnumberOfLocalConnectomeNeuronsInhibitory = " << connectivityModel.numberOfLocalConnectomeNeuronsInhibitory << endl;
					#endif		
					//printNumberOfConnections(queryByPresynapticConnectionNeurons, &connectivityModel, countNumberInhibitoryExcitatoryNeurons, true, false);
				
				#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL_BY_EXCITATION_TYPE
				}
				#endif
			#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL_BY_EXCITATION_TYPE
			}
			#endif
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL_BY_LAYER
		}
		#endif
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_LOCAL_BY_LAYER
	}
	#endif
}
#endif	
		
#endif

#ifdef INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET
bool H01indexedCSVdatabaseQueryClass::queryIndexedCSVdatabaseByConnectionDatasetFile(const int queryMode, const string indexed_csv_database_folder, const string local_connectome_folder_base, const bool connectionDatasetRead, const string connectionDatasetFileNameRead, vector<vector<string>>* localConnectomeCSVdatasetConnections, const bool queryPresynapticConnectionNeurons, const bool connectionDatasetWrite, const bool appendToFile, const string connectionDatasetFileNameWrite, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages)
{
	bool result = true;
	
	vector<string> neuronList;

	int neuronListFileLength = 0;
	
	vector<vector<string>> localConnectomeNeuronsNOTUSED;
	
	//query mode 5 (lookup indexed CSV database by post/pre synaptic connection neuron ID..)
	
	if(connectionDatasetRead)
	{
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

		int localConnectionCSVdatasetConnectionsSize = 0;
		H01indexedCSVdatabaseOperations.readLocalConnectomeConnectionsCSVdataset(connectionDatasetFileNameRead, localConnectomeCSVdatasetConnections, &localConnectionCSVdatasetConnectionsSize);
	}
	
	for(int i=0; i<localConnectomeCSVdatasetConnections->size(); i++)
	{
		vector<string>* localConnectomeConnection = &((*localConnectomeCSVdatasetConnections)[i]);
		string neuronID = "";
		if(queryPresynapticConnectionNeurons)
		{
			neuronID = (*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_ID];
		}
		else
		{
			neuronID = (*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_ID];
		}
		//cout << "neuronID = " << neuronID << endl;
		neuronList.push_back(neuronID);
	}
	
	const string csvDelimiter = CSV_DELIMITER;

	#ifdef INDEXED_CSV_DATABASE_QUERY_WRITE_CURRENT_FOLDER
	const string indexedCSVdatabaseQueryOutputFolder = currentDirectory;	
	#else
	const string indexedCSVdatabaseQueryOutputFolder = INDEXED_CSV_DATABASE_QUERY_OUTPUT_FOLDER;
	#endif
	
	string writeFileString;
	ofstream writeFileObject;
	
	ios_base::openmode writeMode;
	if(appendToFile)
	{
		writeMode = ofstream::app;
	}
	else
	{
		writeMode = ofstream::out; 
	}
	if(connectionDatasetWrite)
	{	
		#ifdef INDEXED_CSV_DATABASE_QUERY_OUTPUT
		#ifdef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_FILE_OBJECT
		SHAREDvars.setCurrentDirectory(indexedCSVdatabaseQueryOutputFolder);
		writeFileObject.open(connectionDatasetFileNameWrite.c_str(), writeMode);
		#else
		writeFileString.reserve(5000000);	// max expected file/string size = ~5MB (see localConnectomeConnections.csv)
		//string writeFileString = "";	//inefficient
		#endif
		#endif
	}
	
	bool queryByPresynapticConnectionNeurons = !queryPresynapticConnectionNeurons;
	queryIndexedCSVdatabaseByConnectionNeuronList(queryMode, indexed_csv_database_folder, &neuronList, localConnectomeCSVdatasetConnections, queryByPresynapticConnectionNeurons, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, connectionDatasetWrite, &writeFileObject, &writeFileString);
	
	if(connectionDatasetWrite)
	{	
		#ifdef INDEXED_CSV_DATABASE_QUERY_OUTPUT
		#ifdef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_FILE_OBJECT
		writeFileObject.close();
		#else
		//cout << "writeFileString = " << writeFileString << endl;
		SHAREDvars.setCurrentDirectory(indexedCSVdatabaseQueryOutputFolder);
		//SHAREDvars.writeStringToFile(connectionDatasetFileNameWrite, &writeFileString);	//inefficient?
		ofstream out(connectionDatasetFileNameWrite, writeMode);
    	out << writeFileString;
    	out.close();
		#endif
		#endif
	}
	
	return result;
}

bool H01indexedCSVdatabaseQueryClass::queryIndexedCSVdatabaseByConnectionNeuronList(const int queryMode, const string indexed_csv_database_folder, vector<string>* neuronList, vector<vector<string>>* localConnectomeCSVdatasetConnections, const bool queryByPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, const bool connectionDatasetWrite, ofstream* writeFileObject, string* writeFileString)
{
	bool result = true;
	
	if(connectionDatasetWrite)
	{
		#ifdef INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET
		#ifdef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_FILE_OBJECT
		string neuronConnectionsFileHeader = string(LOCAL_CONNECTOME_DATASET_CONNECTIONS_HEADER) + STRING_NEWLINE;
		SHAREDvars.writeStringToFileObject(neuronConnectionsFileHeader, writeFileObject);
		#else
		this->appendStringBufferToPreallocatedString(writeFileString, neuronConnectionsFileHeader);
		//writeFileString = writeFileString + neuronConnectionString	//inefficient
		#endif	
		#endif
	}
		
	const string csvDelimiter = CSV_DELIMITER;
	
	for(int neuronIndex=0; neuronIndex<neuronList->size(); neuronIndex++)
	{
		string neuronID = (*neuronList)[neuronIndex];
		vector<vector<string>> neuronConnectionList;
		
		#ifdef INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET
		bool knownNeuronID = true;
		if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET)
		{	
			if(neuronID == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES_UNKNOWN_NEURON_ID)
			{
				knownNeuronID = false;
			}
		}
		if(knownNeuronID)
		{
		#endif								
			this->queryIndexedCSVdatabaseByNeuronID(indexed_csv_database_folder, neuronID, queryByPresynapticConnectionNeurons, &neuronConnectionList);
		#ifdef INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET
		}
		else
		{
			if(connectionDatasetWrite)
			{
				//cerr << "H01indexedCSVdatabaseQueryClass::queryIndexedCSVdatabaseByConnectionNeuronList error: connectionDatasetWrite && !knownNeuronID" << endl;
			}
		}
		#endif
	
		#ifdef INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET
		vector<string>* localConnectomeConnection = &((*localConnectomeCSVdatasetConnections)[neuronIndex]);
		#endif
	
		#ifdef INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET
		long connectionsSourceNeuronID;
		long connectionsTargetNeuronID;
		string connectionsSourceNeuronType = "";
		string connectionsTargetNeuronType = "";
		vecInt connectionsSourceNeuronCoordinates;
		vecInt connectionsTargetNeuronCoordinates;
		int connectionNumberOfSynapses;
		int connectionExcitationType;
		if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET)
		{	
			if(queryByPresynapticConnectionNeurons)
			{
				connectionsSourceNeuronID = SHAREDvars.convertStringToLong((*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_ID]);
				connectionsTargetNeuronID = SHAREDvars.convertStringToLong((*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_ID]);
				connectionsSourceNeuronType = (*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_TYPE];
				connectionsTargetNeuronType = (*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_TYPE];
				connectionsSourceNeuronCoordinates.x = SHAREDvars.convertStringToInt((*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_X]);
				connectionsSourceNeuronCoordinates.y = SHAREDvars.convertStringToInt((*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_Y]);
				connectionsSourceNeuronCoordinates.z = SHAREDvars.convertStringToInt((*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_Z]);
				connectionsTargetNeuronCoordinates.x = SHAREDvars.convertStringToInt((*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_X]);
				connectionsTargetNeuronCoordinates.y = SHAREDvars.convertStringToInt((*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_Y]);
				connectionsTargetNeuronCoordinates.z = SHAREDvars.convertStringToInt((*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_Z]);
			}
			else
			{
				connectionsSourceNeuronID = SHAREDvars.convertStringToLong((*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_ID]);
				connectionsTargetNeuronID = SHAREDvars.convertStringToLong((*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_ID]);	
				connectionsSourceNeuronType = (*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_TYPE];
				connectionsTargetNeuronType = (*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_TYPE];
				connectionsSourceNeuronCoordinates.x = SHAREDvars.convertStringToInt((*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_X]);
				connectionsSourceNeuronCoordinates.y = SHAREDvars.convertStringToInt((*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_Y]);
				connectionsSourceNeuronCoordinates.z = SHAREDvars.convertStringToInt((*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_Z]);
				connectionsTargetNeuronCoordinates.x = SHAREDvars.convertStringToInt((*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_X]);
				connectionsTargetNeuronCoordinates.y = SHAREDvars.convertStringToInt((*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_Y]);
				connectionsTargetNeuronCoordinates.z = SHAREDvars.convertStringToInt((*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_Z]);
			}
			connectionNumberOfSynapses = SHAREDvars.convertStringToInt((*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_SYN_NUM]);	//=1
			connectionExcitationType = SHAREDvars.convertStringToInt((*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE]);
		}
		#endif
			
		for(int connectionIndex=0; connectionIndex<neuronConnectionList.size(); connectionIndex++)
		{
			vector<string> csvLineVector = neuronConnectionList[connectionIndex];

			string sourceNeuronID = "";
			string targetNeuronID = "";
			if(queryByPresynapticConnectionNeurons)
			{
				sourceNeuronID = csvLineVector[INDEXED_CSV_DATABASE_PRESYNAPTIC_SITE_NEURON_ID_FIELD_INDEX];
				targetNeuronID = csvLineVector[INDEXED_CSV_DATABASE_POSTSYNAPTIC_SITE_NEURON_ID_FIELD_INDEX];
			}
			else
			{
				sourceNeuronID = csvLineVector[INDEXED_CSV_DATABASE_POSTSYNAPTIC_SITE_NEURON_ID_FIELD_INDEX];
				targetNeuronID = csvLineVector[INDEXED_CSV_DATABASE_PRESYNAPTIC_SITE_NEURON_ID_FIELD_INDEX];	
			}
			if(sourceNeuronID != neuronID)
			{
				cerr << "H01indexedCSVdatabaseQueryClass::queryIndexedCSVdatabaseByNeuronList error: (sourceNeuronID != neuronID), sourceNeuronID = " << sourceNeuronID << ", neuronID = " << neuronID << endl;
				exit(EXIT_ERROR);
			}
				
				
			#ifdef INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET
			if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET)
			{	
				//query mode 5 (..identify connection with pre/post synaptic X/Y coordinates (if pre/post synaptic neuron type=UNKNOWN), add pre/post synaptic neuron ID, Z coordinates, and type coordinates to connection dataset)

				if(connectionsSourceNeuronID != SHAREDvars.convertStringToLong(neuronID))
				{
					cerr << "H01indexedCSVdatabaseQueryClass::queryIndexedCSVdatabaseByNeuronList error: (connectionsSourceNeuronID != neuronID), connectionsSourceNeuronID = " << connectionsSourceNeuronID << ", neuronID = " << neuronID << endl;
					exit(EXIT_ERROR);
				}

				if(connectionsTargetNeuronType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_TYPE_UNKNOWN)
				{
					cout << "INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: identified target neuron of type=UNNKOWN" << endl;

					long targetNeuronIDlong = SHAREDvars.convertStringToLong(targetNeuronID);
					/*
					//incomplete: awaiting release of H01 Release C3 neurons dataset; will print UNKNOWN neurons (with x/y coordinates only) along with candidate neuron_ids but not reconcile them
					cout << "INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: identified target neuron of type=UNNKOWN in " << LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES_USEALLVALUESAVAILABLEFROMINBODYCELLCONNECTION << " (only x/y coordinates available), candidate targetNeuronID = " << targetNeuronIDlong << endl;
					//look up targetNeuronIDlong in H01 Release C3 neurons dataset
					C3neuronsDatasetNeuron = findNeuronInC3neuronsDataset(targetNeuronIDlong)
					
					//complete target neuron parameters
					if((connectionsTargetNeuronCoordinates.x == C3neuronsDatasetNeuron.x) & (connectionsTargetNeuronCoordinates.y == C3neuronsDatasetNeuron.y))
					{
						connectionsTargetNeuronID = C3neuronsDatasetNeuron.neuronID
						connectionsTargetNeuronType = C3neuronsDatasetNeuron.type 
						connectionsTargetNeuronCoordinates.z = C3neuronsDatasetNeuron.z
					}
					*/	
					
				}
			}
			#endif
		}

		#ifdef INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET
		if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET)
		{	
			//resave connection data with updated target neuron parameters
			//pre_id, pre_x, pre_y, pre_z, pre_type, post_id, post_x, post_y, post_z, post_type, post_class_label, syn_num, excitation_type
			string neuronConnectionString = "";
			if(queryByPresynapticConnectionNeurons)
			{
				neuronConnectionString = 
				SHAREDvars.convertLongToString(connectionsSourceNeuronID) + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionsSourceNeuronCoordinates.x) + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionsSourceNeuronCoordinates.y) + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionsSourceNeuronCoordinates.z) + CSV_DELIMITER + connectionsSourceNeuronType + CSV_DELIMITER + 
				SHAREDvars.convertLongToString(connectionsTargetNeuronID) + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionsTargetNeuronCoordinates.x) + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionsTargetNeuronCoordinates.y) + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionsTargetNeuronCoordinates.z) + CSV_DELIMITER + connectionsTargetNeuronType + CSV_DELIMITER + 
				LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_CLASS_LABEL_UNKNOWN + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionNumberOfSynapses) + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionExcitationType) + STRING_NEWLINE;
			}
			else
			{
				neuronConnectionString = 
				SHAREDvars.convertLongToString(connectionsTargetNeuronID) + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionsTargetNeuronCoordinates.x) + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionsTargetNeuronCoordinates.y) + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionsTargetNeuronCoordinates.z) + CSV_DELIMITER + connectionsTargetNeuronType + CSV_DELIMITER + 
				SHAREDvars.convertLongToString(connectionsSourceNeuronID) + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionsSourceNeuronCoordinates.x) + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionsSourceNeuronCoordinates.y) + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionsSourceNeuronCoordinates.z) + CSV_DELIMITER + connectionsSourceNeuronType + CSV_DELIMITER + 
				LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_CLASS_LABEL_UNKNOWN + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionNumberOfSynapses) + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionExcitationType) + STRING_NEWLINE;
			}

			if(connectionDatasetWrite)
			{
				#ifdef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_FILE_OBJECT
				SHAREDvars.writeStringToFileObject(neuronConnectionString, writeFileObject);
				#else
				this->appendStringBufferToPreallocatedString(writeFileString, neuronConnectionString);
				//writeFileString = writeFileString + neuronConnectionString	//inefficient
				#endif	
			}
		}
		#endif
				
	}
		
	return result;
}

#endif


bool H01indexedCSVdatabaseQueryClass::queryIndexedCSVdatabaseByNeuronID(const string indexed_csv_database_folder, const string neuronID, const bool queryByPresynapticConnectionNeurons, vector<vector<string>>* neuronConnectionList)
{
	bool result = false;
	
	#ifdef INDEXED_CSV_DATABASE_STORE_MULTIPLE_PREPOSTSYNAPTIC_NEURONID_SYNAPSES_PER_FILE
	int fileNameNeuronIDmaxNumCharacters = INDEXED_CSV_DATABASE_FILENAME_NEURONID_MAX_NUM_CHARACTERS;
	#else
	int fileNameNeuronIDmaxNumCharacters = neuronID.length();	//not used
	#endif
	
	int numberLevels = INDEXED_CSV_DATABASE_NUMBER_LEVELS;	//eg 2
	int numberCharactersPerFolderLevel = INDEXED_CSV_DATABASE_NUMBER_CHARACTERS_PER_FOLDER_LEVEL;	//eg 4
		
	const string indexedCSVdatabaseFolder = indexed_csv_database_folder;
	const string indexedCSVdatabaseFileNameCsvExtension = CSV_FILE_EXTENSION;
	
	string indexedCSVdatabaseFileNameCsvPrefix = "";
	if(queryByPresynapticConnectionNeurons)
	{
		indexedCSVdatabaseFileNameCsvPrefix = INDEXED_CSV_DATABASE_FILENAME_PREFIX_PRESYNAPTIC_CSV;		//look for the postsynaptic neuron/connection id (!queryPresynapticConnectionNeurons) within the database file indexed by presynaptic neuron id (queryByPresynapticConnectionNeurons)
	}
	else
	{
		indexedCSVdatabaseFileNameCsvPrefix = INDEXED_CSV_DATABASE_FILENAME_PREFIX_POSTSYNAPTIC_CSV;	//look for the presynaptic neuron/connection id (queryPresynapticConnectionNeurons) within the database file indexed by postsynaptic neuron id (!queryByPresynapticConnectionNeurons)
	}

	int numberConnectionsFound = 0;
		
	const string csvDelimiter = CSV_DELIMITER;
		
	SHAREDvars.setCurrentDirectory(indexedCSVdatabaseFolder);
	for(int r=0; r<numberLevels; r++)
	{
		string neuronIDlevelName = neuronID.substr(r*numberCharactersPerFolderLevel, numberCharactersPerFolderLevel);
		if(r == numberLevels-1)
		{
			//last level (create file rather than folder)
			#ifdef INDEXED_CSV_DATABASE_STORE_MULTIPLE_PREPOSTSYNAPTIC_NEURONID_SYNAPSES_PER_FILE
			string fileNameNeuronIDidentifier = neuronID.substr(0, fileNameNeuronIDmaxNumCharacters);
			#else
			string fileNameNeuronIDidentifier = neuronID;
			#endif

			string csvFileName = indexedCSVdatabaseFileNameCsvPrefix + fileNameNeuronIDidentifier + indexedCSVdatabaseFileNameCsvExtension;
			
			vector<string> csvFileContents;
			int csvFileLength = 0;
			SHAREDvars.getLinesFromFile(csvFileName, &csvFileContents, &csvFileLength);

			for(int l=0; l<csvFileContents.size(); l++)
			{				
				string csvLineText = csvFileContents[l];

				string sourceNeuronID = "";
				if(queryByPresynapticConnectionNeurons)
				{
					//get INDEXED_CSV_DATABASE_PRESYNAPTIC_SITE_NEURON_ID_FIELD_INDEX (optimised): 
					int sourceNeuronIDindexEnd = csvLineText.find(csvDelimiter);
					sourceNeuronID = csvLineText.substr(0, sourceNeuronIDindexEnd);
					//sourceNeuronID = csvLineVector[INDEXED_CSV_DATABASE_PRESYNAPTIC_SITE_NEURON_ID_FIELD_INDEX];	//inefficient;
				}
				else
				{
					//get INDEXED_CSV_DATABASE_POSTSYNAPTIC_SITE_NEURON_ID_FIELD_INDEX (optimised):
					int sourceNeuronIDindexStart = csvLineText.find(csvDelimiter)+1;
					int sourceNeuronIDindexEnd = csvLineText.find(csvDelimiter, sourceNeuronIDindexStart);
					sourceNeuronID = csvLineText.substr(sourceNeuronIDindexStart, sourceNeuronIDindexEnd-sourceNeuronIDindexStart);
					//sourceNeuronID = csvLineVector[INDEXED_CSV_DATABASE_POSTSYNAPTIC_SITE_NEURON_ID_FIELD_INDEX];	//inefficient;
				}
								
				if(sourceNeuronID == neuronID)
				{ 
					numberConnectionsFound++;
					result = true;
					//cout << "sourceNeuronID = " << sourceNeuronID << endl;

					vector<string> csvLineVector;
					this->convertCSVlineToVector(&csvLineText, &csvLineVector);
				
					neuronConnectionList->push_back(csvLineVector);
				}
			}
		}
		else
		{
			//open folder
			if(SHAREDvars.directoryExists(&neuronIDlevelName))
			{
				SHAREDvars.setCurrentDirectory(&neuronIDlevelName);
			}
			else
			{
				#ifdef LOCAL_CONNECTOME_OFFICAL_RELEASE_C3_SOMAS_EXPECT_SOME_MISSING_FROM_C3_CONNECTIONS_DATABASE
				//ignore missing entry
				r = numberLevels;
				#else
				cerr << "queryIndexedCSVdatabaseByNeuronID error: csv index folder does not exist: " << neuronIDlevelName << ", neuronID = " << neuronID << endl;
				exit(EXIT_ERROR);
				#endif
			}
		}
	}

	return result;
}

bool H01indexedCSVdatabaseQueryClass::convertCSVlineToVector(const string* csvLineText, vector<string>* csvLineVector)
{	
	bool result = true;
	
	string delimiter = CSV_DELIMITER;
	size_t last = 0; 
	size_t next = 0; 
	while((next = csvLineText->find(delimiter, last)) != string::npos) 
	{   
		string csvLineFieldEntry = csvLineText->substr(last, next-last); 
		//cout << "csvLineFieldEntry = " << csvLineFieldEntry << endl;
		csvLineVector->push_back(csvLineFieldEntry);
		last = next + 1;
	} 
	string csvLineFieldEntry = csvLineText->substr(last);
	//cout << "csvLineFieldEntry = " << csvLineFieldEntry << endl;
	csvLineVector->push_back(csvLineFieldEntry);
	
	return result;
}






#ifdef INDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS
bool H01indexedCSVdatabaseQueryClass::crawlIndexedCSVdatabase(const int queryMode, const string indexed_csv_database_folder, const bool queryPresynapticConnectionNeurons)
{
	bool result = true;
		
	map<long,long> uniqueNeuronIDmap;	
	//consider adding multimap<long,long> neuronIDmap to store every connection from/to same neuron_id in same key

	bool queryByPresynapticConnectionNeurons = queryPresynapticConnectionNeurons;	//note this is not inverted

	#ifdef INDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS_COUNT_NUMBER_INCOMING_OUTGOING_EXCITATORY_INHIBITORY_SYNAPSES
	long numberConnectionsExcitatory = 0; 
	long numberConnectionsInhibitory = 0;
	#endif
	
	//requires INDEXED_CSV_DATABASE_STORE_MULTIPLE_PREPOSTSYNAPTIC_NEURONID_SYNAPSES_PER_FILE
	int neuronIDminCharactersInt = pow(10, INDEXED_CSV_DATABASE_FILENAME_NEURONID_MAX_NUM_CHARACTERS-1);	//eg 100000	//neuron_ids always start with 1 (never 0)
	int neuronIDmaxCharactersInt = pow(10, INDEXED_CSV_DATABASE_FILENAME_NEURONID_MAX_NUM_CHARACTERS)-1;	//eg 999999
	cout << endl;
	for(long i=neuronIDminCharactersInt; i<=neuronIDmaxCharactersInt; i++)
	{
		string neuronIDstart = SHAREDvars.convertIntToString(i);
		cout << "neuronIDstart = " << neuronIDstart << "\r";
		crawlIndexedCSVdatabase(queryMode, indexed_csv_database_folder, neuronIDstart, queryByPresynapticConnectionNeurons, &uniqueNeuronIDmap, &numberConnectionsExcitatory, &numberConnectionsInhibitory);
	}
	cout << endl;
			
	cout << "\nINDEXED_CSV_DATABASE_QUERY_COUNT_NUMBER_UNIQUE_AXONS_DENDRITES: queryPresynapticConnectionNeurons (T:incoming/F:outgoing) = " << SHAREDvars.convertBoolToString(queryByPresynapticConnectionNeurons) << endl;
	#ifdef INDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS_COUNT_NUMBER_INCOMING_OUTGOING_EXCITATORY_INHIBITORY_SYNAPSES
	cout << "numberConnections = " << numberConnectionsExcitatory+numberConnectionsInhibitory << endl;
	cout << "numberConnectionsExcitatory = " << numberConnectionsExcitatory << endl;
	cout << "numberConnectionsInhibitory = " << numberConnectionsInhibitory << endl;
	#endif	
	cout << "Number of unique neurons (F:axons/T:dendrites) is " << uniqueNeuronIDmap.size() << endl;
  
	return result;
}
bool H01indexedCSVdatabaseQueryClass::crawlIndexedCSVdatabase(const int queryMode, const string indexed_csv_database_folder, const string neuronIDstart, const bool queryByPresynapticConnectionNeurons, map<long,long>* uniqueNeuronIDmap, long* numberConnectionsExcitatory, long* numberConnectionsInhibitory)
{
	bool result = true;
	
	#ifdef INDEXED_CSV_DATABASE_STORE_MULTIPLE_PREPOSTSYNAPTIC_NEURONID_SYNAPSES_PER_FILE
	int fileNameNeuronIDmaxNumCharacters = INDEXED_CSV_DATABASE_FILENAME_NEURONID_MAX_NUM_CHARACTERS;
	#else
	int fileNameNeuronIDmaxNumCharacters = neuronIDstart.length();	//not used
	#endif
	
	int numberLevels = INDEXED_CSV_DATABASE_NUMBER_LEVELS;	//eg 2
	int numberCharactersPerFolderLevel = INDEXED_CSV_DATABASE_NUMBER_CHARACTERS_PER_FOLDER_LEVEL;	//eg 4
		
	const string indexedCSVdatabaseFolder = indexed_csv_database_folder;
	const string indexedCSVdatabaseFileNameCsvExtension = CSV_FILE_EXTENSION;
	
	string indexedCSVdatabaseFileNameCsvPrefix = "";
	if(queryByPresynapticConnectionNeurons)
	{
		indexedCSVdatabaseFileNameCsvPrefix = INDEXED_CSV_DATABASE_FILENAME_PREFIX_PRESYNAPTIC_CSV;		//look for the presynaptic neuron/connection id within the database file indexed by presynaptic neuron id (queryByPresynapticConnectionNeurons)
	}
	else
	{
		indexedCSVdatabaseFileNameCsvPrefix = INDEXED_CSV_DATABASE_FILENAME_PREFIX_POSTSYNAPTIC_CSV;	//look for the postsynaptic neuron/connection id within the database file indexed by postsynaptic neuron id (!queryByPresynapticConnectionNeurons)
	}
	
	const string csvDelimiter = CSV_DELIMITER;
		
	SHAREDvars.setCurrentDirectory(indexedCSVdatabaseFolder);
	for(int r=0; r<numberLevels; r++)
	{
		string neuronIDlevelName = neuronIDstart.substr(r*numberCharactersPerFolderLevel, numberCharactersPerFolderLevel);
		if(r == numberLevels-1)
		{
			//last level (create file rather than folder)
			#ifdef INDEXED_CSV_DATABASE_STORE_MULTIPLE_PREPOSTSYNAPTIC_NEURONID_SYNAPSES_PER_FILE
			string fileNameNeuronIDidentifier = neuronIDstart.substr(0, fileNameNeuronIDmaxNumCharacters);
			#else
			string fileNameNeuronIDidentifier = neuronIDstart;
			#endif

			string csvFileName = indexedCSVdatabaseFileNameCsvPrefix + fileNameNeuronIDidentifier + indexedCSVdatabaseFileNameCsvExtension;
			
			vector<string> csvFileContents;
			int csvFileLength = 0;
			if(SHAREDvars.getLinesFromFile(csvFileName, &csvFileContents, &csvFileLength))
			{
				for(int l=0; l<csvFileContents.size(); l++)
				{				
					string csvLineText = csvFileContents[l];
																	
					string sourceNeuronID = "";
					if(queryByPresynapticConnectionNeurons)
					{
						//get INDEXED_CSV_DATABASE_PRESYNAPTIC_SITE_NEURON_ID_FIELD_INDEX (optimised): 
						int sourceNeuronIDindexEnd = csvLineText.find(csvDelimiter);
						sourceNeuronID = csvLineText.substr(0, sourceNeuronIDindexEnd);
						//sourceNeuronID = csvLineVector[INDEXED_CSV_DATABASE_PRESYNAPTIC_SITE_NEURON_ID_FIELD_INDEX];	//inefficient;
					}
					else
					{
						//get INDEXED_CSV_DATABASE_POSTSYNAPTIC_SITE_NEURON_ID_FIELD_INDEX (optimised):
						int sourceNeuronIDindexStart = csvLineText.find(csvDelimiter)+1;
						int sourceNeuronIDindexEnd = csvLineText.find(csvDelimiter, sourceNeuronIDindexStart);
						sourceNeuronID = csvLineText.substr(sourceNeuronIDindexStart, sourceNeuronIDindexEnd-sourceNeuronIDindexStart);
						//sourceNeuronID = csvLineVector[INDEXED_CSV_DATABASE_POSTSYNAPTIC_SITE_NEURON_ID_FIELD_INDEX];	//inefficient;
					}

					#ifdef INDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS_COUNT_NUMBER_INCOMING_OUTGOING_EXCITATORY_INHIBITORY_SYNAPSES
					vector<string> csvLineVector;
					this->convertCSVlineToVector(&csvLineText, &csvLineVector);
					string connectionType = csvLineVector[INDEXED_CSV_DATABASE_SYNAPSE_TYPE_FIELD_INDEX];	//connection/synapse type as automatically derived from EM images (exitatory/inhibitory)
					int connectionTypeInt = SHAREDvars.convertStringToInt(connectionType);
					if(connectionTypeInt == AVRO_JSON_DATABASE_EXCITATORY_SYNAPSE_TYPE)
					{
						(*numberConnectionsExcitatory) = (*numberConnectionsExcitatory) + 1;
					}
					else if(connectionTypeInt == AVRO_JSON_DATABASE_INHIBITORY_SYNAPSE_TYPE)
					{
						(*numberConnectionsInhibitory) = (*numberConnectionsInhibitory) + 1;
					}
					#endif
					
					long sourceNeuronIDlong = SHAREDvars.convertStringToLong(sourceNeuronID);
					(*uniqueNeuronIDmap)[sourceNeuronIDlong] = sourceNeuronIDlong;
				}
			}
			else
			{
				//file not found (no such pre/postSynapticNeuronID exists in indexed CSV database)
			}
		}
		else
		{
			//open folder
			if(SHAREDvars.directoryExists(&neuronIDlevelName))
			{
				SHAREDvars.setCurrentDirectory(&neuronIDlevelName);
			}
			else
			{
				cout << "queryIndexedCSVdatabaseByNeuronID note: csv index folder does not exist (neuron id sequence may be missing from database): " << neuronIDlevelName << ", neuronIDstart = " << neuronIDstart << endl;
				r = numberLevels;
			}
		}
	}
	
	return result;
}
#endif

#ifndef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_FILE_OBJECT
void H01indexedCSVdatabaseQueryClass::appendStringBufferToPreallocatedString(string* s, string stringToAppend)
{
	s->append(fileName.c_str(), stringToAppend->length());
}
#endif


#endif

