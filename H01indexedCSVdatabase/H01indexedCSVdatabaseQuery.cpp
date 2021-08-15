/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseQuery.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: H01LocalConnectome
 * Requirements: requires H01 indexed CSV database to have already been generated (see INDEXED_CSV_DATABASE_CREATE: H01indexedCSVdatabaseCreate.cpp/.hpp)
 * Compilation: see H01indexedCSVdatabase.hpp
 * Usage: see H01indexedCSVdatabase.hpp
 * Description: H01 indexed CSV database query - 
 *  INDEXED_CSV_DATABASE_QUERY_EXTRACT_INCOMING_OUTGOING_CONNECTIONS: mode 1 (lookup indexed CSV database by neuron ID, and find incoming/outgoing target connections, and write them to file)
 *  INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING: mode 2 (lookup indexed CSV database by neuron ID, find incoming target connections, and generate visualisation)
 *  INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: mode 3 (automatically generate localConnectomeConnections-typesFromPresynapticNeurons.csv/localConnectomeConnections-typesFromEMimages.csv from localConnectomeNeurons.csv and indexed CSV database)
 *  INDEXED_CSV_DATABASE_QUERY_COUNT_PROPORTION_LOCAL_VS_NONLOCAL_CONNECTIONS: mode 4 (lookup indexed CSV database by neuron ID, count/infer proportion of incoming/outgoing excitatory/inhibitory target connections to local vs distal neurons)
 *  INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: mode 5 (lookup indexed CSV database by post/pre synaptic connection neuron ID, and identify connection with pre/post synaptic X/Y coordinates (if pre/post synaptic neuron type=UNKNOWN), add pre/post synaptic neuron ID, Z coordinates, and type coordinates to connection dataset)
 * Input: 
 *  INDEXED_CSV_DATABASE_QUERY_OUTPUT_CONNECTIONS: localConnectomeNeurons.csv (or localConnectomeNeuronIDlistDistinct.csv) - id, x, y, z, type, excitation_type (or id)
 *  INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING: localConnectomeNeurons.csv (or localConnectomeNeuronIDlistDistinct.csv) - id, x, y, z, type, excitation_type (or id)
 *  INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: localConnectomeNeurons.csv - id, x, y, z, type, excitation_type
 *  INDEXED_CSV_DATABASE_QUERY_COUNT_PROPORTION_LOCAL_VS_NONLOCAL_CONNECTIONS: localConnectomeNeurons.csv (or localConnectomeNeuronIDlistDistinct.csv) - id, x, y, z, type, excitation_type (or id)
 *  INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: localConnectomeConnections-typesFromEMimages-useAllValuesAvailableFromInBodyCellConnection.csv - pre_id, pre_x, pre_y, pre_z, pre_type, post_id, post_x, post_y, post_z, post_type, post_class_label, syn_num, excitation_type
 * Output Format:
 *  INDEXED_CSV_DATABASE_QUERY_OUTPUT_CONNECTIONS: localConnectomeNeuronIDlistConnectionsPresynaptic.csv/localConnectomeNeuronIDlistConnectionsPostsynaptic.csv - connectionNeuronID1, connectionType1 [, locationObjectContentsXcoordinatesContent1, locationObjectContentsYcoordinatesContent1, locationObjectContentsZcoordinatesContent1], connectionNeuronID2, connectionType2 [, locationObjectContentsXcoordinatesContent2, locationObjectContentsYcoordinatesContent2, locationObjectContentsZcoordinatesContent2], etc 
 *  INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING:
 *      INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_3D_LINEAR_REGRESSION:
 *          INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING_LDR: localConnectomeIncomingAxonMapping.ldr - LDR_REFERENCE_TYPE_LINE ldrawColor plot3DpointStart.x plot3DpointStart.y plot3DpointStart.z plot3DpointEnd.x plot3DpointEnd.y plot3DpointEnd.z
 *          INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING_CSV: localConnectomeIncomingAxonMapping.csv - polyFit.connectionNeuronID, polyFit.estSynapseType, polyFit.origin.x, polyFit.origin.y, polyFit.origin.z, polyFit.axis.x, polyFit.axis.y, polyFit.axis.z
 *      INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_2D_POLY_REGRESSION:
 *          INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING_CSV: localConnectomeIncomingAxonMapping.csv - polyFit.connectionNeuronID, polyFit.estSynapseType, polyFit.a, polyFit.b, polyFit.c
 *  INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: localConnectomeConnections-typesFromPresynapticNeurons.csv/localConnectomeConnections-typesFromEMimages.csv - pre_id, pre_x, pre_y, pre_z, pre_type, post_id, post_x, post_y, post_z, post_type, post_class_label, syn_num, excitation_type
 *  INDEXED_CSV_DATABASE_QUERY_COUNT_PROPORTION_LOCAL_VS_NONLOCAL_CONNECTIONS: N/A
 *  INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: localConnectomeConnections-typesFromEMimages.csv - pre_id, pre_x, pre_y, pre_z, pre_type, post_id, post_x, post_y, post_z, post_type, post_class_label, syn_num, excitation_type
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
		this->queryIndexedCSVdatabaseByNeuronDatasetOrListFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, LOCAL_CONNECTOME_DATASET_NEURONS_FILENAME, true, QUERY_PRESYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_TRUE, APPEND_FILE_TRUE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, CONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS);
		this->queryIndexedCSVdatabaseByNeuronDatasetOrListFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, LOCAL_CONNECTOME_DATASET_NEURONS_FILENAME, true, QUERY_POSTSYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_TRUE, APPEND_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, CONNECTION_TYPES_DERIVED_FROM_EM_IMAGES);
		this->queryIndexedCSVdatabaseByNeuronDatasetOrListFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, LOCAL_CONNECTOME_DATASET_NEURONS_FILENAME, true, QUERY_PRESYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_TRUE, APPEND_FILE_TRUE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, CONNECTION_TYPES_DERIVED_FROM_EM_IMAGES);
		#endif
	}	
	#endif
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_PROPORTION_LOCAL_VS_NONLOCAL_CONNECTIONS
	else if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_COUNT_PROPORTION_LOCAL_VS_NONLOCAL_CONNECTIONS)
	{
		string neuronListConnectionsFileNameNA = "";
		this->queryIndexedCSVdatabaseByNeuronDatasetOrListFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST_DISTINCT_FILE_NAME, INDEXED_CSV_DATABASE_QUERY_PREFER_DATASET_LOCAL_CONNECTOME_NEURONS_VALUE, QUERY_PRESYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_FALSE, APPEND_FILE_FALSE, neuronListConnectionsFileNameNA, na);	//count incoming connections
		this->queryIndexedCSVdatabaseByNeuronDatasetOrListFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST_DISTINCT_FILE_NAME, INDEXED_CSV_DATABASE_QUERY_PREFER_DATASET_LOCAL_CONNECTOME_NEURONS_VALUE, QUERY_POSTSYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_FALSE, APPEND_FILE_FALSE, neuronListConnectionsFileNameNA, na);	//count outgoing connections
	}
	#endif
	#ifdef INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET
	else if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET)
	{	
		cout << "warning: query mode 5 (INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET) is awaiting release of H01 Release C3 neurons dataset; will print UNKNOWN neurons (with x/y coordinates only) along with candidate neuron_ids but not reconcile them" << endl;
		
		//only need to perform operation on !connectionTypesDerivedFromPresynapticNeuronsOrEMimages
		vector<vector<string>> localConnectomeConnections;
		this->queryIndexedCSVdatabaseByConnectionDatasetFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, READ_FILE_TRUE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES_USEALLVALUESAVAILABLEFROMINBODYCELLCONNECTION, &localConnectomeConnections, QUERY_PRESYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_FALSE, APPEND_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, CONNECTION_TYPES_DERIVED_FROM_EM_IMAGES);
		this->queryIndexedCSVdatabaseByConnectionDatasetFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, READ_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES_USEALLVALUESAVAILABLEFROMINBODYCELLCONNECTION, &localConnectomeConnections, QUERY_POSTSYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_TRUE, APPEND_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, CONNECTION_TYPES_DERIVED_FROM_EM_IMAGES);
		/*
		this->queryIndexedCSVdatabaseByConnectionDatasetFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, READ_FILE_TRUE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, QUERY_PRESYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_TRUE, APPEND_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, CONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS);
		this->queryIndexedCSVdatabaseByConnectionDatasetFile(queryMode, indexed_csv_database_folder, local_connectome_folder_base, READ_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, QUERY_POSTSYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_TRUE, APPEND_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, CONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS);
		*/		
	}	
	#endif
	
	return result;
}

bool H01indexedCSVdatabaseQueryClass::queryIndexedCSVdatabaseByNeuronDatasetOrListFile(const int queryMode, const string indexed_csv_database_folder, const string local_connectome_folder_base, const string neuronDatasetOrListFileName, const bool neuronListIsDataset, const bool queryPresynapticConnectionNeurons, const bool write, const bool appendToFile, const string neuronListConnectionsFileName, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages)
{
	bool result = true;
		
	vector<string> neuronList;

	int neuronListFileLength = 0;
	
	vector<vector<string>> localConnectomeNeurons;	

	cout << "neuronDatasetOrListFileName = " << neuronDatasetOrListFileName << endl;
	cout << "neuronListConnectionsFileName = " << neuronListConnectionsFileName << endl;
	
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
	
	#ifdef INDEXED_CSV_DATABASE_QUERY_READ_DATASET_LOCAL_CONNECTOME_NEURONS	
	if(neuronListIsDataset)
	{
		SHAREDvars.getLinesFromFileCSV(neuronDatasetOrListFileName, &localConnectomeNeurons, &neuronListFileLength, CSV_DELIMITER_CHAR, true);
		for(int i=0; i<localConnectomeNeurons.size(); i++)
		{
			vector<string>* localConnectomeNeuron = &(localConnectomeNeurons[i]);
			string neuronID = (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_NEURON_ID];
			//cout << "neuronID = " << neuronID << endl;
			neuronList.push_back(neuronID);
		}
	}
	else
	{
	#endif
		SHAREDvars.getLinesFromFile(neuronDatasetOrListFileName, &neuronList, &neuronListFileLength);
	#ifdef INDEXED_CSV_DATABASE_QUERY_READ_DATASET_LOCAL_CONNECTOME_NEURONS
	}
	#endif
	
	vector<vector<string>> localConnectomeConnections;
	#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_VERIFICATION
	map<string, int> connectionsMap;
	if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET)
	{
		//verify local connectome connections dataset generation vs existing (dev/ODS generated) connections datasets
		int localConnectionCSVdatasetConnectionsSize = 0;
		if(connectionTypesDerivedFromPresynapticNeuronsOrEMimages)
		{
			SHAREDvars.getLinesFromFileCSV(LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, &localConnectomeConnections, &localConnectionCSVdatasetConnectionsSize, CSV_DELIMITER_CHAR, true);	
		}
		else
		{
			SHAREDvars.getLinesFromFileCSV(LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, &localConnectomeConnections, &localConnectionCSVdatasetConnectionsSize, CSV_DELIMITER_CHAR, true);			
		}
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
		queryIndexedCSVdatabaseByNeuronList(queryMode, indexed_csv_database_folder, &neuronList, &localConnectomeNeurons, &localConnectomeConnections, true, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, &writeFileObject, &writeFileString);
		queryIndexedCSVdatabaseByNeuronList(queryMode, indexed_csv_database_folder, &neuronList, &localConnectomeNeurons, &localConnectomeConnections, false, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, &writeFileObject, &writeFileString);	
	}
	else
	{
	#endif
		bool queryByPresynapticConnectionNeurons = !queryPresynapticConnectionNeurons;
		queryIndexedCSVdatabaseByNeuronList(queryMode, indexed_csv_database_folder, &neuronList, &localConnectomeNeurons, &localConnectomeConnections, queryByPresynapticConnectionNeurons, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, &writeFileObject, &writeFileString);
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

bool H01indexedCSVdatabaseQueryClass::queryIndexedCSVdatabaseByNeuronList(const int queryMode, const string indexed_csv_database_folder, vector<string>* neuronList, vector<vector<string>>* localConnectomeNeurons, vector<vector<string>>* localConnectomeConnections, const bool queryByPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, ofstream* writeFileObject, string* writeFileString)
{
	bool result = true;

	#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_VERIFICATION
	//note vector<vector<string>>*localConnectomeConnections parameter is only used by INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_VERIFICATION
	map<string, int> connectionsMap;
	if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET)
	{
		for(int i=0; i < localConnectomeConnections->size(); i++)
		{
			vector<string>* localConnectomeConnection = &((*localConnectomeConnections)[i]);
			string connectionsMapKey = (*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_ID] + (*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_ID];
			//cout << "connectionsMapKey to insert = " << connectionsMapKey << endl;
    		connectionsMap[connectionsMapKey] = i;
		}
	}
	#endif

	#ifdef INDEXED_CSV_DATABASE_QUERY_EFFICIENT_STORE_NEURON_IDS_IN_MAP
	map<string, int> neuronMap;
	for(int i=0; i < neuronList->size(); i++)
	{
    	neuronMap[(*neuronList)[i]] = i;
	}
	#endif
	
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_PROPORTION_LOCAL_VS_NONLOCAL_CONNECTIONS
	int numberConnectionsToLocalConnectome = 0;
	int numberConnectionsToExternalConnectome = 0;
	int numberConnectionsToLocalConnectomeExcitatory = 0;
	int numberConnectionsToExternalConnectomeExcitatory = 0;
	int numberConnectionsToLocalConnectomeInhibitory = 0;
	int numberConnectionsToExternalConnectomeInhibitory = 0;
	#endif
		
	const string csvDelimiter = CSV_DELIMITER;
	
	for(int neuronIndex=0; neuronIndex<neuronList->size(); neuronIndex++)
	{
		string neuronID = (*neuronList)[neuronIndex];
		vector<vector<string>> neuronConnectionList;
										
		this->queryIndexedCSVdatabaseByNeuronID(indexed_csv_database_folder, neuronID, queryByPresynapticConnectionNeurons, &neuronConnectionList);
						
		#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET
		vector<string>* localConnectomeNeuron = &((*localConnectomeNeurons)[neuronIndex]);
		#endif
		
		if(neuronConnectionList.size() == 0)
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
		}
		else
		{	
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
				string locationObjectContentsXcoordinatesContent = csvLineVector[INDEXED_CSV_DATABASE_SYNAPSE_LOCATION_COORDINATE_X_FIELD_INDEX];	//connection/synapse location
				string locationObjectContentsYcoordinatesContent = csvLineVector[INDEXED_CSV_DATABASE_SYNAPSE_LOCATION_COORDINATE_Y_FIELD_INDEX];	//connection/synapse location
				string locationObjectContentsZcoordinatesContent = csvLineVector[INDEXED_CSV_DATABASE_SYNAPSE_LOCATION_COORDINATE_Z_FIELD_INDEX];	//connection/synapse location
				
				
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
					if(neuronMap.count(targetNeuronID) == 0)	//verify that targetNeuronID is not in neuronList
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
					#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_LOCAL_NEURONS_REQUIRED		
					//verify that targetNeuronID centroid/soma is also in local connectome;
					if(neuronMap.count(targetNeuronID) != 0)	//verify that targetNeuronID is in neuronList
					{
					#endif
						string neuronConnectionString = "";

						int neuronIndexTarget = neuronMap[targetNeuronID];
						vector<string>* localConnectomeNeuronTarget = &((*localConnectomeNeurons)[neuronIndexTarget]);

						int excitationType;
						if(connectionTypesDerivedFromPresynapticNeuronsOrEMimages)
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
						else
						{	
							string sourceNeuronType = "";
							if(queryByPresynapticConnectionNeurons)
							{
								sourceNeuronType = (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE];
							}
							else
							{
								sourceNeuronType = (*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE];
							}
							if(sourceNeuronType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_PYRAMIDAL)
							{
								excitationType = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY;
							}
							else if(sourceNeuronType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_INTERNEURON)
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
						if(queryByPresynapticConnectionNeurons)
						{
							#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_VERIFICATION
							pre_id = sourceNeuronID;
							post_id = targetNeuronID;
							#endif
							neuronConnectionString = 
							sourceNeuronID + CSV_DELIMITER + (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_X] + CSV_DELIMITER + (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_Y] + CSV_DELIMITER + (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_Z] + CSV_DELIMITER + (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE] + CSV_DELIMITER + 
							targetNeuronID + CSV_DELIMITER + (*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_X] + CSV_DELIMITER + (*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_Y] + CSV_DELIMITER + (*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_Z] + CSV_DELIMITER + (*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE] + CSV_DELIMITER + 
							LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_CLASS_LABEL_UNKNOWN + CSV_DELIMITER + SHAREDvars.convertIntToString(numberOfSynapses) + CSV_DELIMITER + SHAREDvars.convertIntToString(excitationType) + STRING_NEWLINE;
						}
						else
						{
							#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_VERIFICATION
							pre_id = targetNeuronID;
							post_id = sourceNeuronID;
							#endif
							neuronConnectionString = 
							targetNeuronID + CSV_DELIMITER + (*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_X] + CSV_DELIMITER + (*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_Y] + CSV_DELIMITER + (*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_Z] + CSV_DELIMITER + (*localConnectomeNeuronTarget)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE] + CSV_DELIMITER + 
							sourceNeuronID + CSV_DELIMITER + (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_X] + CSV_DELIMITER + (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_Y] + CSV_DELIMITER + (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_Z] + CSV_DELIMITER + (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE] + CSV_DELIMITER + 
							LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_CLASS_LABEL_UNKNOWN + CSV_DELIMITER + SHAREDvars.convertIntToString(numberOfSynapses) + CSV_DELIMITER + SHAREDvars.convertIntToString(excitationType) + STRING_NEWLINE;		
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
				}
				#endif	
	
	
				#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_PROPORTION_LOCAL_VS_NONLOCAL_CONNECTIONS
				if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_COUNT_PROPORTION_LOCAL_VS_NONLOCAL_CONNECTIONS)
				{
					if(neuronMap.count(targetNeuronID) != 0)	//verify that targetNeuronID is in neuronList
					{
						numberConnectionsToLocalConnectome++;
						if(connectionTypeInt == AVRO_JSON_DATABASE_EXCITATORY_SYNAPSE_TYPE)
						{
							numberConnectionsToLocalConnectomeExcitatory++;
						}
						else if(connectionTypeInt == AVRO_JSON_DATABASE_INHIBITORY_SYNAPSE_TYPE)
						{
							numberConnectionsToLocalConnectomeInhibitory++;
						}
						else
						{
							cerr << "H01indexedCSVdatabaseQueryClass::queryIndexedCSVdatabaseByNeuronList warning, connection type unknown, connectionType = " << connectionType << endl;
						}
					}
					else
					{
						numberConnectionsToExternalConnectome++;
						if(connectionTypeInt == AVRO_JSON_DATABASE_EXCITATORY_SYNAPSE_TYPE)
						{
							numberConnectionsToExternalConnectomeExcitatory++;
						}
						else if(connectionTypeInt == AVRO_JSON_DATABASE_INHIBITORY_SYNAPSE_TYPE)
						{
							numberConnectionsToExternalConnectomeInhibitory++;
						}
						else
						{
							cerr << "H01indexedCSVdatabaseQueryClass::queryIndexedCSVdatabaseByNeuronList warning, connection type unknown, connectionType = " << connectionType << endl;
						}
					}
				}
				#endif
			}
		}
				
		if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_EXTRACT_INCOMING_OUTGOING_CONNECTIONS)
		{
			//notify user that program is still running
			cout << "neuronIndex = " << neuronIndex << ", neuronID = " << neuronID << ", neuronConnectionList.size() = " << neuronConnectionList.size() << endl;
		}
		
	}

	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_PROPORTION_LOCAL_VS_NONLOCAL_CONNECTIONS
	if(queryMode == QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_COUNT_PROPORTION_LOCAL_VS_NONLOCAL_CONNECTIONS)
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
		cout << "\nINDEXED_CSV_DATABASE_QUERY_COUNT_PROPORTION_LOCAL_VS_NONLOCAL_CONNECTIONS: queryByPresynapticConnectionNeurons (incoming/outgoing) = " << queryByPresynapticConnectionNeurons << endl;
		cout << "numberConnectionsToLocalConnectome = " << numberConnectionsToLocalConnectome << endl;
		cout << "numberConnectionsToLocalConnectomeExcitatory = " << numberConnectionsToLocalConnectomeExcitatory << endl;
		cout << "numberConnectionsToLocalConnectomeInhibitory = " << numberConnectionsToLocalConnectomeInhibitory << endl;
		cout << "numberConnectionsToExternalConnectome = " << numberConnectionsToExternalConnectome << endl;
		cout << "numberConnectionsToExternalConnectomeExcitatory = " << numberConnectionsToExternalConnectomeExcitatory << endl;
		cout << "numberConnectionsToExternalConnectomeInhibitory = " << numberConnectionsToExternalConnectomeInhibitory << endl;
		double normalisationFactor = double(AVRO_JSON_DATABASE_COORDINATES_RANGE_X*AVRO_JSON_DATABASE_COORDINATES_CALIBRATION_X + AVRO_JSON_DATABASE_COORDINATES_RANGE_Y*AVRO_JSON_DATABASE_COORDINATES_CALIBRATION_Y)/2.0 / AVRO_JSON_DATABASE_COORDINATES_RANGE_Z*AVRO_JSON_DATABASE_COORDINATES_CALIBRATION_Z;
		double numberConnectionsToLocalConnectomeNormalisedZ = double(numberConnectionsToLocalConnectome) * normalisationFactor;
		double numberConnectionsToLocalConnectomeExcitatoryNormalisedZ = double(numberConnectionsToLocalConnectomeExcitatory) * normalisationFactor;
		double numberConnectionsToLocalConnectomeInhibitoryNormalisedZ = double(numberConnectionsToLocalConnectomeInhibitory) * normalisationFactor;
		cout << "numberConnectionsToLocalConnectomeNormalisedZ = " << numberConnectionsToLocalConnectomeNormalisedZ << endl;
		cout << "numberConnectionsToLocalConnectomeExcitatoryNormalisedZ = " << numberConnectionsToLocalConnectomeExcitatoryNormalisedZ << endl;
		cout << "numberConnectionsToLocalConnectomeInhibitoryNormalisedZ = " << numberConnectionsToLocalConnectomeInhibitoryNormalisedZ << endl;
		double fractionOfConnectionsToLocalConnectome = numberConnectionsToLocalConnectomeNormalisedZ/double(numberConnectionsToLocalConnectomeNormalisedZ+numberConnectionsToExternalConnectome);
		double fractionOfConnectionsToLocalConnectomeExcitatory = numberConnectionsToLocalConnectomeExcitatoryNormalisedZ/double(numberConnectionsToLocalConnectomeExcitatoryNormalisedZ+numberConnectionsToExternalConnectomeExcitatory);
		double fractionOfConnectionsToLocalConnectomeInhibitory = numberConnectionsToLocalConnectomeInhibitoryNormalisedZ/double(numberConnectionsToLocalConnectomeInhibitoryNormalisedZ+numberConnectionsToExternalConnectomeInhibitory);
		//double fractionOfConnectionsToLocalConnectome = numberConnectionsToLocalConnectome/(numberConnectionsToLocalConnectome+numberConnectionsToExternalConnectome);
		cout << "fractionOfConnectionsToLocalConnectome = " << fractionOfConnectionsToLocalConnectome << endl;
		cout << "fractionOfConnectionsToLocalConnectomeExcitatory = " << fractionOfConnectionsToLocalConnectomeExcitatory << endl;
		cout << "fractionOfConnectionsToLocalConnectomeInhibitory = " << fractionOfConnectionsToLocalConnectomeInhibitory << endl;
	}
	#endif
			
	return result;
}



#ifdef INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET
bool H01indexedCSVdatabaseQueryClass::queryIndexedCSVdatabaseByConnectionDatasetFile(const int queryMode, const string indexed_csv_database_folder, const string local_connectome_folder_base, const bool connectionDatasetRead, const string connectionDatasetFileNameRead, vector<vector<string>>* localConnectomeConnections, const bool queryPresynapticConnectionNeurons, const bool connectionDatasetWrite, const bool appendToFile, const string connectionDatasetFileNameWrite, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages)
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
		SHAREDvars.getLinesFromFileCSV(connectionDatasetFileNameRead, localConnectomeConnections, &localConnectionCSVdatasetConnectionsSize, CSV_DELIMITER_CHAR, true);
	}
	
	for(int i=0; i<localConnectomeConnections->size(); i++)
	{
		vector<string>* localConnectomeConnection = &((*localConnectomeConnections)[i]);
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
	queryIndexedCSVdatabaseByConnectionNeuronList(queryMode, indexed_csv_database_folder, &neuronList, localConnectomeConnections, queryByPresynapticConnectionNeurons, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, connectionDatasetWrite, &writeFileObject, &writeFileString);
	
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

bool H01indexedCSVdatabaseQueryClass::queryIndexedCSVdatabaseByConnectionNeuronList(const int queryMode, const string indexed_csv_database_folder, vector<string>* neuronList, vector<vector<string>>* localConnectomeConnections, const bool queryByPresynapticConnectionNeurons, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, const bool connectionDatasetWrite, ofstream* writeFileObject, string* writeFileString)
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
		vector<string>* localConnectomeConnection = &((*localConnectomeConnections)[neuronIndex]);
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
	bool result = true;
	
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
				cerr << "queryIndexedCSVdatabaseByNeuronID error: csv index folder does not exist: " << neuronIDlevelName << ", neuronID = " << neuronID << endl;
				exit(EXIT_ERROR);
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



#ifndef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_FILE_OBJECT
void H01indexedCSVdatabaseQueryClass::appendStringBufferToPreallocatedString(string* s, string stringToAppend)
{
	s->append(fileName.c_str(), stringToAppend->length());
}
#endif

#endif

