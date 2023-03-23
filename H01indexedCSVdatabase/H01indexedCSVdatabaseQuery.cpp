/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseQuery.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021-2023 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: H01LocalConnectome
 * Requirements: requires H01 indexed CSV database to have already been generated (see INDEXED_CSV_DATABASE_CREATE: H01indexedCSVdatabaseCreate.cpp/.hpp)
 * Compilation: see H01indexedCSVdatabaseGlobalDefs.hpp
 * Usage: see H01indexedCSVdatabaseGlobalDefs.hpp
 * Description: H01 indexed CSV database query
 * Comments:
 * /
 *******************************************************************************/

#include "H01indexedCSVdatabaseQuery.hpp"
#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING
#include "H01indexedCSVdatabaseAlgorithms.hpp"
#endif

#ifdef INDEXED_CSV_DATABASE_QUERY

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
void H01indexedCSVdatabaseQueryClass::incrementNumberOfConnections(GaussianQuad* gaussianQuad, const vec* posSource, const vec* posTarget, const vec* sourceNeuronCorticalFlowVector)
{
	gaussianQuad->numberConnections++;
}
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
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PRINT_OUTPUT_VERBOSE_LOCALORNONLOCAL
		cout << "INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PRINT_OUTPUT_VERBOSE_LOCALORNONLOCAL print format: neuronID num_excitatory_connections num_inhibitory_connections" << endl;
		#endif
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_PRESYNAPTIC_NEURONS
		cout << "\n\nCONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS:" << endl;
		vector<H01connectivityModelClass>* connectivityModelLayersIncoming = H01indexedCSVdatabaseCalculateNeuronLayer.generateNumberOfConnectionsLayers();
		vector<H01connectivityModelClass>* connectivityModelLayersOutgoing = H01indexedCSVdatabaseCalculateNeuronLayer.generateNumberOfConnectionsLayers();
		cout << "\nQUERY_MODE_INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS: QUERY_PRESYNAPTIC_CONNECTION_NEURONS (QUERY_BY_POSTSYNAPTIC_CONNECTION_NEURONS) - count incoming connections" << endl;
		this->queryIndexedCSVdatabaseByNeuronDatasetOrListFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST_DISTINCT_FILE_NAME, INDEXED_CSV_DATABASE_QUERY_PREFER_DATASET_LOCAL_CONNECTOME_NEURONS_VALUE, QUERY_PRESYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_FALSE, APPEND_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, CONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, connectivityModelLayersIncoming);	//count incoming connections
		cout << "\nQUERY_MODE_INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS: QUERY_POSTSYNAPTIC_CONNECTION_NEURONS (QUERY_BY_PRESYNAPTIC_CONNECTION_NEURONS) - count outgoing connections" << endl;		
		this->queryIndexedCSVdatabaseByNeuronDatasetOrListFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST_DISTINCT_FILE_NAME, INDEXED_CSV_DATABASE_QUERY_PREFER_DATASET_LOCAL_CONNECTOME_NEURONS_VALUE, QUERY_POSTSYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_FALSE, APPEND_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, CONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, connectivityModelLayersOutgoing);	//count outgoing connections
		#endif
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_EM_IMAGES
		cout << "\n\nCONNECTION_TYPES_DERIVED_FROM_EM_IMAGES:" << endl;
		connectivityModelLayersIncoming = H01indexedCSVdatabaseCalculateNeuronLayer.generateNumberOfConnectionsLayers();
		connectivityModelLayersOutgoing = H01indexedCSVdatabaseCalculateNeuronLayer.generateNumberOfConnectionsLayers();
		cout << "\nQUERY_MODE_INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS: QUERY_PRESYNAPTIC_CONNECTION_NEURONS (QUERY_BY_POSTSYNAPTIC_CONNECTION_NEURONS) - count incoming connections" << endl;
		this->queryIndexedCSVdatabaseByNeuronDatasetOrListFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST_DISTINCT_FILE_NAME, INDEXED_CSV_DATABASE_QUERY_PREFER_DATASET_LOCAL_CONNECTOME_NEURONS_VALUE, QUERY_PRESYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_FALSE, APPEND_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, CONNECTION_TYPES_DERIVED_FROM_EM_IMAGES, connectivityModelLayersIncoming);	//count incoming connections
		cout << "\nQUERY_MODE_INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS: QUERY_POSTSYNAPTIC_CONNECTION_NEURONS (QUERY_BY_PRESYNAPTIC_CONNECTION_NEURONS) - count outgoing connections" << endl;		
		this->queryIndexedCSVdatabaseByNeuronDatasetOrListFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST_DISTINCT_FILE_NAME, INDEXED_CSV_DATABASE_QUERY_PREFER_DATASET_LOCAL_CONNECTOME_NEURONS_VALUE, QUERY_POSTSYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_FALSE, APPEND_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, CONNECTION_TYPES_DERIVED_FROM_EM_IMAGES, connectivityModelLayersOutgoing);	//count outgoing connections
		#endif
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
		cout << "\nINDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS: QUERY_PRESYNAPTIC_CONNECTION_NEURONS (QUERY_BY_PRESYNAPTIC_CONNECTION_NEURONS) - count incoming connections" << endl;
		this->crawlIndexedCSVdatabase(queryMode, indexed_csv_database_folder, QUERY_PRESYNAPTIC_CONNECTION_NEURONS);	//count unique incoming neurons (axons) not explicitly in local connectome 
		cout << "\nINDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS: QUERY_POSTSYNAPTIC_CONNECTION_NEURONS (QUERY_BY_POSTSYNAPTIC_CONNECTION_NEURONS) - count outgoing connections" << endl;		
		this->crawlIndexedCSVdatabase(queryMode, indexed_csv_database_folder, QUERY_POSTSYNAPTIC_CONNECTION_NEURONS);	//count unique outgoing neurons (dendrites) not explicitly in local connectome
	}
	#endif
	else
	{
		cerr << "H01indexedCSVdatabaseQueryClass::queryIndexedCSVdatabase error: queryMode unknown: " << queryMode << endl;
		exit(EXIT_ERROR);
	}
	
	return result;
}
		
		
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
	if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS)	//constraint only added for efficiency (only calculate layers when necessary)
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
	if(!H01indexedCSVdatabaseCalculateNeuronLayer.readLocalNeuronsAndConnections(local_connectome_folder_base, neuronDatasetOrListFileName, neuronListIsDataset, &neuronList, &neuronMap, &localConnectomeCSVdatasetNeurons, readConnections, neuronListConnectionsFileName, &localConnectomeCSVdatasetConnections, buildConnectionsMap, &connectionsMap, readLayers, readLayersConnections))
	{
		result = false;
	}
	
	ofstream writeFileObject = H01indexedCSVdatabaseOperations.prepareLocalConnectomeDatasetWrite(write, appendToFile, neuronListConnectionsFileName);

	#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_ORIG
	if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET)
	{
		queryIndexedCSVdatabaseByNeuronList(queryMode, indexed_csv_database_folder, &neuronList, &neuronMap, &localConnectomeCSVdatasetNeurons, &localConnectomeCSVdatasetConnections, &connectionsMap, true, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, &writeFileObject, appendToFile, connectivityModelLayers);
		queryIndexedCSVdatabaseByNeuronList(queryMode, indexed_csv_database_folder, &neuronList, &neuronMap, &localConnectomeCSVdatasetNeurons, &localConnectomeCSVdatasetConnections, &connectionsMap, false, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, &writeFileObject, appendToFile, connectivityModelLayers);	
	}
	else
	{
	#endif
		bool queryByPresynapticConnectionNeurons = !queryPresynapticConnectionNeurons;
		queryIndexedCSVdatabaseByNeuronList(queryMode, indexed_csv_database_folder, &neuronList, &neuronMap, &localConnectomeCSVdatasetNeurons, &localConnectomeCSVdatasetConnections, &connectionsMap, queryByPresynapticConnectionNeurons, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, &writeFileObject, appendToFile, connectivityModelLayers);
	#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_ORIG
	}
	#endif
	
	#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING	
	if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING)
	{
		performIncomingAxonMapping(&writeFileObject);
	}
	#endif

	H01indexedCSVdatabaseOperations.finaliseLocalConnectomeDatasetWrite(write, &writeFileObject);
	
	return result;
}

#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING	
void H01indexedCSVdatabaseQueryClass::performIncomingAxonMapping(ofstream* writeFileObject)
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
				SHAREDvars.writeStringToFileObject(neuronConnectionString, writeFileObject);
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
	
bool H01indexedCSVdatabaseQueryClass::queryIndexedCSVdatabaseByNeuronList(const int queryMode, const string indexed_csv_database_folder, vector<string>* neuronList, map<string, int>* neuronMap, vector<vector<string>>* localConnectomeCSVdatasetNeurons, vector<vector<string>>* localConnectomeCSVdatasetConnections, map<string, int>* connectionsMap, const bool queryByPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, ofstream* writeFileObject, const bool appendToFile, vector<H01connectivityModelClass>* connectivityModelLayers)
{
	bool result = true;
	
	#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET
	if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET)
	{
		if(!appendToFile)
		{
			string neuronConnectionsFileHeader = string(LOCAL_CONNECTOME_DATASET_CONNECTIONS_HEADER) + STRING_NEWLINE;
			SHAREDvars.writeStringToFileObject(neuronConnectionsFileHeader, writeFileObject);
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

			#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS
			#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_NUMBER_EXCITATORY_INHIBITORY_NEURONS
			if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS)
			{
				countConnectionsNumberInhibitoryExcitatoryNeurons(localConnectomeNeuron, connectivityModelLayers);
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
						SHAREDvars.writeStringToFileObject(neuronConnectionString, writeFileObject);
						//append csvDelimiter or STRING_NEWLINE:
						if(connectionIndex == neuronConnectionList.size()-1)
						{
							SHAREDvars.writeStringToFileObject(STRING_NEWLINE, writeFileObject);
						}
						else
						{
							SHAREDvars.writeStringToFileObject(csvDelimiter, writeFileObject);
						}
					}
					#endif		

					#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING	
					if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING)
					{
						performIncomingAxonMappingRead(neuronMap, sourceNeuronID, targetNeuronID, connectionType, locationObjectContentsXcoordinatesContent, locationObjectContentsYcoordinatesContent, locationObjectContentsZcoordinatesContent);
					}	
					#endif

					#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET
					if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET)
					{
						generateLocalConnectomeConnectionsDataset(csvLineVector, neuronMap, localConnectomeCSVdatasetNeurons, localConnectomeNeuron, sourceNeuronID, targetNeuronID, connectionTypeInt, queryByPresynapticConnectionNeurons, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, writeFileObject);
					}
					#endif	

					#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS
					if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS)
					{
						countConnections(queryMode, neuronMap, localConnectomeCSVdatasetNeurons, localConnectomeNeuron, connectivityModelLayers, sourceNeuronID, targetNeuronID, connectionTypeInt, queryByPresynapticConnectionNeurons, connectionTypesDerivedFromPresynapticNeuronsOrEMimages);
					}
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
		}
		else
		{
			#ifdef INDEXED_CSV_DATABASE_QUERY_EXTRACT_INCOMING_OUTGOING_CONNECTIONS
			if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_EXTRACT_INCOMING_OUTGOING_CONNECTIONS)
			{
				//add a new line such that database lengths are matched
				#ifdef INDEXED_CSV_DATABASE_QUERY_OUTPUT
				SHAREDvars.writeStringToFileObject(STRING_NEWLINE, writeFileObject);			
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
		printNumberOfConnectionsLayers(queryByPresynapticConnectionNeurons, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, connectivityModelLayers, true, true, true);
	}
	#endif
									
	return result;
}

#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS
#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_NUMBER_EXCITATORY_INHIBITORY_NEURONS
void H01indexedCSVdatabaseQueryClass::countConnectionsNumberInhibitoryExcitatoryNeurons(vector<string>* localConnectomeNeuron, vector<H01connectivityModelClass>* connectivityModelLayers)
{
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
	int localConnectomeNeuronLayer = SHAREDvars.convertStringToInt((*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER]);
	#endif

	string sourceNeuronType = (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE];

	int excitationType = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN;
	if(sourceNeuronType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_PYRAMIDAL)
	{
		excitationType = LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY;
	}
	else if(sourceNeuronType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_INTERNEURON)
	{
		excitationType = LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY;
	}
	else
	{
		excitationType = LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN;	//excitationType will be LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN if presynaptic neuron is not in local connectome 
	}

	addConnectionNumberInhibitoryExcitatoryNeurons(excitationType, localConnectomeNeuronLayer, connectivityModelLayers);
}

void H01indexedCSVdatabaseQueryClass::addConnectionNumberInhibitoryExcitatoryNeurons(const bool excitationType, const int localConnectomeNeuronLayer, vector<H01connectivityModelClass>* connectivityModelLayers)
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
		cerr << "H01indexedCSVdatabaseQueryClass::addConnectionNumberInhibitoryExcitatoryNeurons warning, excitationType unknown, excitationType = " << excitationType << endl;
	}
}

#endif
#endif

#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING	
void H01indexedCSVdatabaseQueryClass::performIncomingAxonMappingRead(map<string, int>* neuronMap, const string sourceNeuronID, const string targetNeuronID, const string connectionType, const string locationObjectContentsXcoordinatesContent, const string locationObjectContentsYcoordinatesContent, const string locationObjectContentsZcoordinatesContent)
{
	const string csvDelimiter = CSV_DELIMITER;

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
void H01indexedCSVdatabaseQueryClass::generateLocalConnectomeConnectionsDataset(vector<string> csvLineVector, map<string, int>* neuronMap, vector<vector<string>>* localConnectomeCSVdatasetNeurons, vector<string>* localConnectomeNeuron, const string sourceNeuronID, const string targetNeuronID, const int connectionTypeInt, const bool queryByPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, ofstream* writeFileObject)
{
	#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_RESTRICT_PREPOSTSYNAPTIC_CLASS_LABELS
	string classLabelContentsSmall1 = csvLineVector[INDEXED_CSV_DATABASE_PRESYNAPTIC_SITE_CLASSLABEL_FIELD_INDEX];
	string classLabelContentsSmall2 = csvLineVector[INDEXED_CSV_DATABASE_POSTSYNAPTIC_SITE_CLASSLABEL_FIELD_INDEX];
	if(SHAREDvars.textInTextArray(classLabelContentsSmall1, indexed_csv_database_presynaptic_site_classlabel_valid, INDEXED_CSV_DATABASE_PRESYNAPTIC_SITE_CLASSLABEL_VALID_NUM) && SHAREDvars.textInTextArray(classLabelContentsSmall2, indexed_csv_database_postsynaptic_site_classlabel_valid, INDEXED_CSV_DATABASE_POSTSYNAPTIC_SITE_CLASSLABEL_VALID_NUM))
	{
	#endif
		#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_LOCAL_NEURONS_REQUIRED		
		//verify that targetNeuronID centroid/soma is also in local connectome;
		bool targetNeuronIsInLocalConnectome = false;
		if(neuronMap->count(targetNeuronID) != 0)	//verify that targetNeuronID is in neuronList
		{
			targetNeuronIsInLocalConnectome = true;
		}
		if(targetNeuronIsInLocalConnectome)
		{
		#endif
			string neuronConnectionString = "";

			int neuronIndexTarget = (*neuronMap)[targetNeuronID];
			vector<string>* localConnectomeNeuronTarget = &((*localConnectomeCSVdatasetNeurons)[neuronIndexTarget]);

			string presynapticNeuronType = "";
			vector<string>* localConnectomeNeuronQuery;
			if(connectionTypesDerivedFromPresynapticNeuronsOrEMimages)
			{
				if(queryByPresynapticConnectionNeurons)
				{
					presynapticNeuronType = (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE];
					localConnectomeNeuronQuery = localConnectomeNeuron;
				}
				else
				{
					presynapticNeuronType = (*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE];
					localConnectomeNeuronQuery = localConnectomeNeuronTarget;
				}
			}
			int excitationType = calculateConnectionExcitationType2(connectionTypeInt, localConnectomeNeuronQuery, presynapticNeuronType, queryByPresynapticConnectionNeurons, connectionTypesDerivedFromPresynapticNeuronsOrEMimages);
		
			int numberOfSynapses = 1;
			#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_VERIFICATION
			string pre_id; 
			string post_id;
			#endif
			//pre_id, pre_x, pre_y, pre_z, pre_type, post_id, post_x, post_y, post_z, post_type, post_class_label, syn_num, excitation_type
			string postClassLabel = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_CLASS_LABEL_UNKNOWN;
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

			SHAREDvars.writeStringToFileObject(neuronConnectionString, writeFileObject);

		#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_LOCAL_NEURONS_REQUIRED		
		}
		#endif
	#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_RESTRICT_PREPOSTSYNAPTIC_CLASS_LABELS
	}
	#endif
}
#endif



#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS
void H01indexedCSVdatabaseQueryClass::countConnections(const int queryMode, map<string, int>* neuronMap, vector<vector<string>>* localConnectomeCSVdatasetNeurons, vector<string>* localConnectomeNeuron, vector<H01connectivityModelClass>* connectivityModelLayers, const string sourceNeuronID, const string targetNeuronID, const int connectionTypeInt, const bool queryByPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages)
{
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
	int localConnectomeNeuronLayer = SHAREDvars.convertStringToInt((*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER]);
	#endif

	bool targetNeuronIsInLocalConnectome = false;
	vector<string>* localConnectomeNeuronTarget = NULL;
	if(neuronMap->count(targetNeuronID) != 0)	//verify that targetNeuronID is in neuronList
	{
		targetNeuronIsInLocalConnectome = true;
		int neuronIndexTarget = (*neuronMap)[targetNeuronID];
		localConnectomeNeuronTarget = &((*localConnectomeCSVdatasetNeurons)[neuronIndexTarget]);
	}

	int excitationType = calculateConnectionExcitationType1(connectionTypeInt, localConnectomeNeuron, localConnectomeNeuronTarget, queryByPresynapticConnectionNeurons, connectionTypesDerivedFromPresynapticNeuronsOrEMimages);

	vec posSource;
	vec posTarget; 
	vec sourceNeuronCorticalFlowVector;
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL
	#endif

	bool preAndPostSynapticNeuronAreInLocalConnectome = false;	
	if(neuronMap->count(targetNeuronID) != 0)	//verify that targetNeuronID is in neuronList
	{
		preAndPostSynapticNeuronAreInLocalConnectome = true;	//sourceNeuronID and targetNeuronID are in local connectome 
	}
	addConnection(preAndPostSynapticNeuronAreInLocalConnectome, excitationType, localConnectomeNeuronLayer, connectivityModelLayers, &posSource, &posTarget, &sourceNeuronCorticalFlowVector);
}

void H01indexedCSVdatabaseQueryClass::addConnection(const bool preAndPostSynapticNeuronAreInLocalConnectome, const bool excitationTypeConnection, const int localConnectomeNeuronLayer, vector<H01connectivityModelClass>* connectivityModelLayers, const vec* posSource, const vec* posTarget, const vec* sourceNeuronCorticalFlowVector)
{	
	if(preAndPostSynapticNeuronAreInLocalConnectome)
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
			#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PRINT_OUTPUT_VERBOSE_LOCALORNONLOCAL
			(*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].numberConnectionsLocalOrNonLocalConnectomeExcitatory++;
			#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
			(*connectivityModelLayers)[localConnectomeNeuronLayer].numberConnectionsLocalOrNonLocalConnectomeExcitatory++;
			#endif
			#endif
		}
		else if(excitationTypeConnection == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY)
		{
			incrementNumberOfConnections(&(*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].neuronModelConnectionsLocalConnectomeInhibitory, posSource, posTarget, sourceNeuronCorticalFlowVector);
			#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
			incrementNumberOfConnections(&(*connectivityModelLayers)[localConnectomeNeuronLayer].neuronModelConnectionsLocalConnectomeInhibitory, posSource, posTarget, sourceNeuronCorticalFlowVector);
			#endif
			#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PRINT_OUTPUT_VERBOSE_LOCALORNONLOCAL
			(*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].numberConnectionsLocalOrNonLocalConnectomeInhibitory++;
			#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
			(*connectivityModelLayers)[localConnectomeNeuronLayer].numberConnectionsLocalOrNonLocalConnectomeInhibitory++;
			#endif
			#endif
		}
		else
		{
			cerr << "H01indexedCSVdatabaseQueryClass::addConnection warning (preAndPostSynapticNeuronAreInLocalConnectome), excitationTypeConnection type unknown, excitationTypeConnection = " << excitationTypeConnection << endl;
		}
	}
	else
	{
		incrementNumberOfConnections(&(*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].neuronModelConnectionsExternalConnectome, posSource, posTarget, sourceNeuronCorticalFlowVector);
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
		incrementNumberOfConnections(&(*connectivityModelLayers)[localConnectomeNeuronLayer].neuronModelConnectionsExternalConnectome, posSource, posTarget, sourceNeuronCorticalFlowVector);
		#endif

		if(excitationTypeConnection == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY)
		{
			incrementNumberOfConnections(&(*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].neuronModelConnectionsExternalConnectomeExcitatory, posSource, posTarget, sourceNeuronCorticalFlowVector);
			#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
			incrementNumberOfConnections(&(*connectivityModelLayers)[localConnectomeNeuronLayer].neuronModelConnectionsExternalConnectomeExcitatory, posSource, posTarget, sourceNeuronCorticalFlowVector);
			#endif
			#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PRINT_OUTPUT_VERBOSE_LOCALORNONLOCAL
			(*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].numberConnectionsLocalOrNonLocalConnectomeExcitatory++;
			#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
			(*connectivityModelLayers)[localConnectomeNeuronLayer].numberConnectionsLocalOrNonLocalConnectomeExcitatory++;
			#endif
			#endif
		}
		else if(excitationTypeConnection == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY)
		{
			incrementNumberOfConnections(&(*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].neuronModelConnectionsExternalConnectomeInhibitory, posSource, posTarget, sourceNeuronCorticalFlowVector);
			#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
			incrementNumberOfConnections(&(*connectivityModelLayers)[localConnectomeNeuronLayer].neuronModelConnectionsExternalConnectomeInhibitory, posSource, posTarget, sourceNeuronCorticalFlowVector);
			#endif
			#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PRINT_OUTPUT_VERBOSE_LOCALORNONLOCAL
			(*connectivityModelLayers)[LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL].numberConnectionsLocalOrNonLocalConnectomeInhibitory++;
			#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
			(*connectivityModelLayers)[localConnectomeNeuronLayer].numberConnectionsLocalOrNonLocalConnectomeInhibitory++;
			#endif
			#endif
		}
		else
		{
			#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_PRESYNAPTIC_NEURONS
			//excitationTypeConnection will be LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN if presynaptic neuron is not in local connectome (so numberConnectionsExternalConnectomeExcitatory+numberConnectionsExternalConnectomeInhibitory != numberConnectionsExternalConnectome)
			#else
			cerr << "H01indexedCSVdatabaseQueryClass::addConnection warning (!preAndPostSynapticNeuronAreInLocalConnectome), excitationTypeConnection type unknown, excitationTypeConnection = " << excitationTypeConnection << endl;
			exit(EXIT_ERROR);
			#endif
		}
	}
}
#endif

int H01indexedCSVdatabaseQueryClass::calculateConnectionExcitationType1(const int connectionTypeInt, vector<string>* localConnectomeNeuronSource, vector<string>* localConnectomeNeuronTarget, const bool queryByPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages)
{	
	string presynapticNeuronType;
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_PRESYNAPTIC_NEURONS
	#ifndef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_PRESYNAPTIC_NEURONS_LOOKUP
	if(connectionTypesDerivedFromPresynapticNeuronsOrEMimages)
	{
		presynapticNeuronType = LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_UNKNOWN;
		if(queryByPresynapticConnectionNeurons)
		{
			//verify that sourceNeuronID centroid/soma is in local connectome (eg check required for countRecurrentConnections);
			if(localConnectomeNeuronSource != NULL)	//verify that sourceNeuronID is in neuronList	//sourceNeuronIsInLocalConnectome
			{
				presynapticNeuronType = (*localConnectomeNeuronSource)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE];
			}
			else
			{
				presynapticNeuronType = LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_UNKNOWN;
			}
		}
		else
		{
			//verify that targetNeuronID centroid/soma is also in local connectome;
			if(localConnectomeNeuronTarget != NULL)	//verify that targetNeuronID is in neuronList	//targetNeuronIsInLocalConnectome
			{
				presynapticNeuronType = (*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE];
			}
			else
			{
				presynapticNeuronType = LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_UNKNOWN;	//excitationType will be LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN if presynaptic neuron is not in local connectome
			}
		}
	}
	#endif
	#endif
	int excitationType = calculateConnectionExcitationType2(connectionTypeInt, localConnectomeNeuronSource, presynapticNeuronType, queryByPresynapticConnectionNeurons, connectionTypesDerivedFromPresynapticNeuronsOrEMimages);
	return excitationType;
}

int H01indexedCSVdatabaseQueryClass::calculateConnectionExcitationType2(const int connectionTypeInt, vector<string>* localConnectomeNeuronSource, const string presynapticNeuronType, const bool queryByPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages)
{	
	int excitationType = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN;
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_PRESYNAPTIC_NEURONS
	if(connectionTypesDerivedFromPresynapticNeuronsOrEMimages)
	{
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_PRESYNAPTIC_NEURONS_LOOKUP
		excitationType = SHAREDvars.convertStringToInt((*localConnectomeNeuronSource)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE]);
		#else
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
		#endif
	}
	else
	{
	#endif
		//connectionTypeInt is indexedCVSdatabases connection type (not used by local connectome)
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
			cerr << "H01indexedCSVdatabaseQueryClass::calculateConnectionExcitationType2 warning, connection type unknown, connectionType = " << connectionTypeInt << endl;
		}
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_PRESYNAPTIC_NEURONS
	}
	#endif
	return excitationType;
}

		

#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS

void H01indexedCSVdatabaseQueryClass::printNumberOfConnectionsLayers(const bool queryByPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, constEffective vector<H01connectivityModelClass>* connectivityModelLayers, const bool countLocalConnectomeNeurons, const bool countInternalConnectomeConnections, const bool countExternalConnectomeConnections)
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
			
		printNumberOfConnections(layerIndex, queryByPresynapticConnectionNeurons, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, connectivityModel, countLocalConnectomeNeurons, countInternalConnectomeConnections, countExternalConnectomeConnections);

	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
	}
	#endif
}

void H01indexedCSVdatabaseQueryClass::printNumberOfConnections(const int layerIndex, const bool queryByPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, const H01connectivityModelClass* connectivityModel, const bool countLocalConnectomeNeurons, const bool countInternalConnectomeConnections, const bool countExternalConnectomeConnections)
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
	
	cout << "\n printNumberOfConnections: queryByPresynapticConnectionNeurons (F:incoming/T:outgoing) = " << SHAREDvars.convertBoolToString(queryByPresynapticConnectionNeurons) << endl;

	double normalisationFactorZ = H01indexedCSVdatabaseCalculateNeuronLayer.getZNormalisationFactor();
	
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
		
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PRINT_OUTPUT_VERBOSE_LOCALORNONLOCAL
	{
		cout << "INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PRINT_OUTPUT_VERBOSE_LOCALORNONLOCAL:" << endl;
		cout << "\t numberConnectionsLocalOrNonLocalConnectomeExcitatory = " << connectivityModel->numberConnectionsLocalOrNonLocalConnectomeExcitatory << endl;
		cout << "\t numberConnectionsLocalOrNonLocalConnectomeInhibitory = " << connectivityModel->numberConnectionsLocalOrNonLocalConnectomeInhibitory << endl;	
		//cout << neuronID << " " << connectivityModel->numberConnectionsLocalOrNonLocalConnectomeExcitatory << " " << connectivityModel->numberConnectionsLocalOrNonLocalConnectomeInhibitory << endl;
	}
	#endif	
	
	cout << "INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS:" << endl;
	double numberConnectionsLocalConnectomeNormalisedZ = 0.0;
	double numberConnectionsLocalConnectomeExcitatoryNormalisedZ = 0.0;
	double numberConnectionsLocalConnectomeInhibitoryNormalisedZ = 0.0;
	if(countInternalConnectomeConnections)
	{
		cout << "countInternalConnectomeConnections:" << endl;
		cout << "\t numberConnectionsLocalConnectome = " << connectivityModel->neuronModelConnectionsLocalConnectome.numberConnections << endl;
		cout << "\t numberConnectionsLocalConnectomeExcitatory = " << connectivityModel->neuronModelConnectionsLocalConnectomeExcitatory.numberConnections << endl;
		cout << "\t numberConnectionsLocalConnectomeInhibitory = " << connectivityModel->neuronModelConnectionsLocalConnectomeInhibitory.numberConnections << endl;
		cout << "\t\t fractionOfConnectionsLocalConnectomeExcitatory = " << double(connectivityModel->neuronModelConnectionsLocalConnectomeExcitatory.numberConnections) / double(connectivityModel->neuronModelConnectionsLocalConnectome.numberConnections) << endl;
		cout << "\t\t fractionOfConnectionsLocalConnectomeInhibitory = " << double(connectivityModel->neuronModelConnectionsLocalConnectomeInhibitory.numberConnections) / double(connectivityModel->neuronModelConnectionsLocalConnectome.numberConnections) << endl;
		numberConnectionsLocalConnectomeNormalisedZ = double(connectivityModel->neuronModelConnectionsLocalConnectome.numberConnections) * normalisationFactorZ;
		numberConnectionsLocalConnectomeExcitatoryNormalisedZ = double(connectivityModel->neuronModelConnectionsLocalConnectomeExcitatory.numberConnections) * normalisationFactorZ;
		numberConnectionsLocalConnectomeInhibitoryNormalisedZ = double(connectivityModel->neuronModelConnectionsLocalConnectomeInhibitory.numberConnections) * normalisationFactorZ;
		cout << "\t normalisationFactorZ = " << normalisationFactorZ << endl;
		cout << "\t numberConnectionsLocalConnectomeNormalisedZ = " << numberConnectionsLocalConnectomeNormalisedZ << endl;
		cout << "\t numberConnectionsLocalConnectomeExcitatoryNormalisedZ = " << numberConnectionsLocalConnectomeExcitatoryNormalisedZ << endl;
		cout << "\t numberConnectionsLocalConnectomeInhibitoryNormalisedZ = " << numberConnectionsLocalConnectomeInhibitoryNormalisedZ << endl;
	}
	if(countExternalConnectomeConnections)
	{
		cout << "countExternalConnectomeConnections:" << endl;
		cout << "\t numberConnectionsExternalConnectome = " << connectivityModel->neuronModelConnectionsExternalConnectome.numberConnections << endl;
		if(connectionTypesDerivedFromPresynapticNeuronsOrEMimages)
		{
			cout << "\t numberConnectionsExternalConnectomeExcitatory = " << "unknown [connectionTypesDerivedFromPresynapticNeuronsOrEMimages: externalConnectome neuron excitationType dependent value]" << endl;
			cout << "\t numberConnectionsExternalConnectomeInhibitory = " << "unknown [connectionTypesDerivedFromPresynapticNeuronsOrEMimages: externalConnectome neuron excitationType dependent value]" << endl;
			cout << "\t\t fractionConnectionsExternalConnectomeExcitatory = " << "unknown [connectionTypesDerivedFromPresynapticNeuronsOrEMimages: externalConnectome neuron excitationType dependent value]" << endl;
			cout << "\t\t fractionConnectionsExternalConnectomeInhibitory = " << "unknown [connectionTypesDerivedFromPresynapticNeuronsOrEMimages: externalConnectome neuron excitationType dependent value]" << endl;
		}
		else
		{
			cout << "\t numberConnectionsExternalConnectomeExcitatory = " << connectivityModel->neuronModelConnectionsExternalConnectomeExcitatory.numberConnections << endl;
			cout << "\t numberConnectionsExternalConnectomeInhibitory = " << connectivityModel->neuronModelConnectionsExternalConnectomeInhibitory.numberConnections << endl;
			cout << "\t\t fractionConnectionsExternalConnectomeExcitatory = " << double(connectivityModel->neuronModelConnectionsExternalConnectomeExcitatory.numberConnections) / double(connectivityModel->neuronModelConnectionsExternalConnectome.numberConnections) << endl;
			cout << "\t\t fractionConnectionsExternalConnectomeInhibitory = " << double(connectivityModel->neuronModelConnectionsExternalConnectomeInhibitory.numberConnections) / double(connectivityModel->neuronModelConnectionsExternalConnectome.numberConnections) << endl;
		}
		
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PROPORTION_LOCAL_VS_NONLOCAL_CONNECTIONS
		cout << "INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PROPORTION_LOCAL_VS_NONLOCAL_CONNECTIONS:" << endl;
		double fractionOfConnectionsLocalVsExternalConnectome = numberConnectionsLocalConnectomeNormalisedZ/double(numberConnectionsLocalConnectomeNormalisedZ+connectivityModel->neuronModelConnectionsExternalConnectome.numberConnections);
		double fractionOfConnectionsLocalVsExternalConnectomeExcitatory = numberConnectionsLocalConnectomeExcitatoryNormalisedZ/double(numberConnectionsLocalConnectomeExcitatoryNormalisedZ+connectivityModel->neuronModelConnectionsExternalConnectomeExcitatory.numberConnections);
		double fractionOfConnectionsLocalVsExternalConnectomeInhibitory = numberConnectionsLocalConnectomeInhibitoryNormalisedZ/double(numberConnectionsLocalConnectomeInhibitoryNormalisedZ+connectivityModel->neuronModelConnectionsExternalConnectomeInhibitory.numberConnections);
		//double fractionOfConnectionsLocalVsExternalConnectome = numberConnectionsLocalConnectome/(numberConnectionsLocalConnectome+numberConnectionsExternalConnectome);
		cout << "\t fractionOfConnectionsLocalVsExternalConnectome = " << fractionOfConnectionsLocalVsExternalConnectome << endl;
		if(connectionTypesDerivedFromPresynapticNeuronsOrEMimages)
		{
			cout << "\t fractionOfConnectionsLocalVsExternalConnectomeExcitatory = " << "unknown [connectionTypesDerivedFromPresynapticNeuronsOrEMimages: externalConnectome neuron excitationType dependent value]" << endl;
			cout << "\t fractionOfConnectionsLocalVsExternalConnectomeInhibitory = " << "unknown [connectionTypesDerivedFromPresynapticNeuronsOrEMimages: externalConnectome neuron excitationType dependent value]" << endl;		
		}
		else
		{
			cout << "\t fractionOfConnectionsLocalVsExternalConnectomeExcitatory = " << fractionOfConnectionsLocalVsExternalConnectomeExcitatory << endl;
			cout << "\t fractionOfConnectionsLocalVsExternalConnectomeInhibitory = " << fractionOfConnectionsLocalVsExternalConnectomeInhibitory << endl;
		}
		#endif
	}
	
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PER_NEURON		
	{
		cout << "INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PER_NEURON:" << endl;
		if(countInternalConnectomeConnections)
		{
			cout << "countInternalConnectomeConnections per neuron:" << endl;
			double numberConnectionsLocalConnectomePerNeuron = connectivityModel->neuronModelConnectionsLocalConnectome.numberConnections/double(connectivityModel->numberOfLocalConnectomeNeurons);
			double numberConnectionsLocalConnectomePerNeuronExcitatory = connectivityModel->neuronModelConnectionsLocalConnectomeExcitatory.numberConnections/double(connectivityModel->numberOfLocalConnectomeNeuronsExcitatory);
			double numberConnectionsLocalConnectomePerNeuronInhibitory = connectivityModel->neuronModelConnectionsLocalConnectomeInhibitory.numberConnections/double(connectivityModel->numberOfLocalConnectomeNeuronsInhibitory);
			double numberConnectionsLocalConnectomePerNeuronNormalisedZ = numberConnectionsLocalConnectomeNormalisedZ/double(connectivityModel->numberOfLocalConnectomeNeurons);
			double numberConnectionsLocalConnectomePerNeuronExcitatoryNormalisedZ = numberConnectionsLocalConnectomeExcitatoryNormalisedZ/double(connectivityModel->numberOfLocalConnectomeNeuronsExcitatory);
			double numberConnectionsLocalConnectomePerNeuronInhibitoryNormalisedZ = numberConnectionsLocalConnectomeInhibitoryNormalisedZ/double(connectivityModel->numberOfLocalConnectomeNeuronsInhibitory);
			cout << "\t numberConnectionsLocalConnectomePerNeuron = " << numberConnectionsLocalConnectomePerNeuron << endl;
			cout << "\t numberConnectionsLocalConnectomePerNeuronExcitatory = " << numberConnectionsLocalConnectomePerNeuronExcitatory << endl;
			cout << "\t numberConnectionsLocalConnectomePerNeuronInhibitory = " << numberConnectionsLocalConnectomePerNeuronInhibitory << endl;
			cout << "\t numberConnectionsLocalConnectomePerNeuronNormalisedZ = " << numberConnectionsLocalConnectomePerNeuronNormalisedZ << endl;
			cout << "\t numberConnectionsLocalConnectomePerNeuronExcitatoryNormalisedZ = " << numberConnectionsLocalConnectomePerNeuronExcitatoryNormalisedZ << endl;
			cout << "\t numberConnectionsLocalConnectomePerNeuronInhibitoryNormalisedZ = " << numberConnectionsLocalConnectomePerNeuronInhibitoryNormalisedZ << endl;		
		}
		if(countExternalConnectomeConnections)
		{
			cout << "countExternalConnectomeConnections per neuron:" << endl;
			double numberConnectionsExternalConnectomePerNeuron = connectivityModel->neuronModelConnectionsExternalConnectome.numberConnections/double(connectivityModel->numberOfLocalConnectomeNeurons);
			double numberConnectionsExternalConnectomePerNeuronExcitatory = connectivityModel->neuronModelConnectionsExternalConnectomeExcitatory.numberConnections/double(connectivityModel->numberOfLocalConnectomeNeuronsExcitatory);
			double numberConnectionsExternalConnectomePerNeuronInhibitory = connectivityModel->neuronModelConnectionsExternalConnectomeInhibitory.numberConnections/double(connectivityModel->numberOfLocalConnectomeNeuronsInhibitory);
			cout << "\t numberConnectionsExternalConnectomePerNeuron = " << numberConnectionsExternalConnectomePerNeuron << endl;
			if(connectionTypesDerivedFromPresynapticNeuronsOrEMimages)
			{
				cout << "\t numberConnectionsExternalConnectomePerNeuronExcitatory = " << "unknown [connectionTypesDerivedFromPresynapticNeuronsOrEMimages: externalConnectome excitationType dependent value]" << endl;
				cout << "\t numberConnectionsExternalConnectomePerNeuronInhibitory = " << "unknown [connectionTypesDerivedFromPresynapticNeuronsOrEMimages: externalConnectome excitationType dependent value]" << endl;		
			}
			else
			{
				cout << "\t numberConnectionsExternalConnectomePerNeuronExcitatory = " << numberConnectionsExternalConnectomePerNeuronExcitatory << endl;
				cout << "\t numberConnectionsExternalConnectomePerNeuronInhibitory = " << numberConnectionsExternalConnectomePerNeuronInhibitory << endl;
			}
		}
	}
	#endif

}
		
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
		H01indexedCSVdatabaseOperations.prepareLocalConnectomeDatasetRead(local_connectome_folder_base);
		
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
	
	ofstream writeFileObject = H01indexedCSVdatabaseOperations.prepareLocalConnectomeDatasetWrite(connectionDatasetWrite, appendToFile, connectionDatasetFileNameWrite);
	
	bool queryByPresynapticConnectionNeurons = !queryPresynapticConnectionNeurons;
	queryIndexedCSVdatabaseByConnectionNeuronList(queryMode, indexed_csv_database_folder, &neuronList, localConnectomeCSVdatasetConnections, queryByPresynapticConnectionNeurons, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, connectionDatasetWrite, &writeFileObject);
	
	H01indexedCSVdatabaseOperations.finaliseLocalConnectomeDatasetWrite(connectionDatasetWrite, &writeFileObject);
	
	return result;
}

bool H01indexedCSVdatabaseQueryClass::queryIndexedCSVdatabaseByConnectionNeuronList(const int queryMode, const string indexed_csv_database_folder, vector<string>* neuronList, vector<vector<string>>* localConnectomeCSVdatasetConnections, const bool queryByPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, const bool connectionDatasetWrite, ofstream* writeFileObject)
{
	bool result = true;
	
	if(connectionDatasetWrite)
	{
		#ifdef INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET
		string neuronConnectionsFileHeader = string(LOCAL_CONNECTOME_DATASET_CONNECTIONS_HEADER) + STRING_NEWLINE;
		SHAREDvars.writeStringToFileObject(neuronConnectionsFileHeader, writeFileObject);	
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
				LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_CLASS_LABEL_UNKNOWN + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionNumberOfSynapses) + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionExcitationType) + STRING_NEWLINE;
			}
			else
			{
				neuronConnectionString = 
				SHAREDvars.convertLongToString(connectionsTargetNeuronID) + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionsTargetNeuronCoordinates.x) + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionsTargetNeuronCoordinates.y) + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionsTargetNeuronCoordinates.z) + CSV_DELIMITER + connectionsTargetNeuronType + CSV_DELIMITER + 
				SHAREDvars.convertLongToString(connectionsSourceNeuronID) + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionsSourceNeuronCoordinates.x) + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionsSourceNeuronCoordinates.y) + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionsSourceNeuronCoordinates.z) + CSV_DELIMITER + connectionsSourceNeuronType + CSV_DELIMITER + 
				LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_CLASS_LABEL_UNKNOWN + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionNumberOfSynapses) + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionExcitationType) + STRING_NEWLINE;
			}

			if(connectionDatasetWrite)
			{
				SHAREDvars.writeStringToFileObject(neuronConnectionString, writeFileObject);
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



#endif

