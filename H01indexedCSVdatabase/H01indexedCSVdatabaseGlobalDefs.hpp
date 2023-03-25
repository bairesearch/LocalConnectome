/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseGlobalDefs.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021-2023 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: H01LocalConnectome
 * Requirements: BAI SHARED C++ library, Eigen 3 C++ library
 * Compilation: ./compileH01indexedCSVdatabase.sh
 * Usage:
 * 	./H01indexedCSVdatabase.exe -mode 1 -avro_json_database_folder "/media/user/large/h01data/data/exported/json" -indexed_csv_database_folder "/media/user/ssddata/indexed"
 * 	./H01indexedCSVdatabase.exe -mode 2 -query 1 -indexed_csv_database_folder "/media/user/ssddata/indexed"
 * 	./H01indexedCSVdatabase.exe -mode 2 -query 2 -indexed_csv_database_folder "/media/user/ssddata/indexed"
 * 	./H01indexedCSVdatabase.exe -mode 2 -query 3 -indexed_csv_database_folder "/media/user/ssddata/indexed"
 * 	./H01indexedCSVdatabase.exe -mode 2 -query 4 -indexed_csv_database_folder "/media/user/ssddata/indexed"
 * 	./H01indexedCSVdatabase.exe -mode 2 -query 5 -indexed_csv_database_folder "/media/user/ssddata/indexed"
 * 	./H01indexedCSVdatabase.exe -mode 2 -query 6 -indexed_csv_database_folder "/media/user/ssddata/indexed"
 * 	./H01indexedCSVdatabase.exe -mode 3
 * 	./H01indexedCSVdatabase.exe -mode 4
 *  ./H01indexedCSVdatabase.exe -mode 5 -read 1 -indexed_csv_database_folder "/media/user/ssddata/indexed"
 *  ./H01indexedCSVdatabase.exe -mode 5 -read 2 -indexed_csv_database_folder "/media/user/ssddata/indexed"
 *  ./H01indexedCSVdatabase.exe -mode 6 -preprocess 1 -indexed_csv_database_folder "/media/user/ssddata/indexed"
 *  ./H01indexedCSVdatabase.exe -mode 6 -preprocess 2 -indexed_csv_database_folder "/media/user/ssddata/indexed"
 * Description: H01 indexed CSV database (global defs)
 * Comments:
 * /
 *******************************************************************************/

#ifndef HEADER_H01indexedCSVdatabase
#define HEADER_H01indexedCSVdatabase

#include "SHAREDglobalDefs.hpp"

extern string currentDirectory;

//database selection;
//#define INDEXED_CSV_DATABASE_H01	//H01 Human Connectome
#define INDEXED_CSV_DATABASE_LDC	//larvalDrosophilaConnectome (local connectome analysis only)

//execution modes:
#ifdef INDEXED_CSV_DATABASE_H01
	#define INDEXED_CSV_DATABASE_CREATE	//mode 1 (converts Avro Json C3 Synaptic connections database to indexed CSV database, indexed by pre/postsynaptic neuron ID)
	#define INDEXED_CSV_DATABASE_QUERY	//mode 2 (queries indexed CSV database, based on local connectome neuron id list)
#endif
#define INDEXED_CSV_DATABASE_VISUALISE_LOCAL_CONNECTOME	//mode 3 (visualises local connectome datasets)	//aka LOCAL_CONNECTOME_CSV_DATASET_VISUALISE
#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME	//mode 4 (traces local connectome dataset, saving visualisation)
#define INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME	//mode 5 (reads local connectome dataset)
#ifdef INDEXED_CSV_DATABASE_LDC
	#define INDEXED_CSV_DATABASE_PREPROCESS	//mode 6 (preprocess dataset)
#endif
#ifdef INDEXED_CSV_DATABASE_H01
	#ifdef INDEXED_CSV_DATABASE_QUERY
		#define INDEXED_CSV_DATABASE_QUERY_EXTRACT_INCOMING_OUTGOING_CONNECTIONS	//mode 1 (lookup indexed CSV database by neuron ID, and find incoming/outgoing target connections, and write them to file)
		#define INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING	//mode 2 (lookup indexed CSV database by neuron ID, find incoming target connections, and generate visualisation)
		#define INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET	//mode 3 (automatically generate localConnectomeConnections-typesFromPresynapticNeurons.csv/localConnectomeConnections-typesFromEMimages.csv from localConnectomeNeurons.csv and H01 indexed CSV database)
		#define INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS	//mode 4 (lookup indexed CSV database by neuron ID, count/infer proportion of incoming/outgoing excitatory/inhibitory target connections to local vs distal neurons)
		#define INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET	//mode 5 (lookup indexed CSV database by pre/post synaptic connection neuron ID, and identify connection with post/pre synaptic X/Y coordinates (if type=UNKNOWN), add post/pre synaptic neuron ID, Z coordinates, and type to connection dataset)	//used to complete localConnectomeConnections-typesFromEMimages 
		#define INDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS	//mode 6 (crawl indexed CSV database by pre/post synaptic connection neuron ID, and count number of unique axons/dendrites as specified by neuron ID - not explicitly connected to local connectome)	//incomplete
	#endif
#endif
#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME
	#define INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS	//mode 1
	#ifdef INDEXED_CSV_DATABASE_H01
		#define INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL	//mode 2	(extrapolate z region same size as x/y)	//uses INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS code	//incomplete
	#endif
#endif
#ifdef INDEXED_CSV_DATABASE_LDC
	#define INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_FROM_MATRIX	//mode 1
	#define INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_NEURONS_DATASET_FROM_SKELETONS	//mode 2
#endif

#ifdef INDEXED_CSV_DATABASE_H01
	#define EXECUTION_MODE_INDEXED_CSV_DATABASE_CREATE 1
	#define EXECUTION_MODE_INDEXED_CSV_DATABASE_QUERY 2
	#define EXECUTION_MODE_INDEXED_CSV_DATABASE_VISUALISE_LOCAL_CONNECTOME 3
	#define EXECUTION_MODE_INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME 4
	#define EXECUTION_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME 5
	#ifdef INDEXED_CSV_DATABASE_QUERY
		#define QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_EXTRACT_INCOMING_OUTGOING_CONNECTIONS 1
		#define QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING 2
		#define QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET	3
		#define QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS 4
		#define QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET 5
		#define QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS 6
		#define QUERY_MODE_DEFAULT (QUERY_MODE_INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS)
		#define QUERY_MODES_TOTAL (6)
	#endif
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME
		#define READ_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS (1)
		#define READ_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL (2)
		#define READ_MODE_DEFAULT (READ_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS)
		#define READ_MODES_TOTAL (2)
	#endif
	#define EXECUTION_MODE_DEFAULT (EXECUTION_MODE_INDEXED_CSV_DATABASE_QUERY) 
	#define EXECUTION_MODES_TOTAL (5)
#elif defined INDEXED_CSV_DATABASE_LDC
	#define EXECUTION_MODE_INDEXED_CSV_DATABASE_VISUALISE_LOCAL_CONNECTOME 3
	#define EXECUTION_MODE_INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME 4
	#define EXECUTION_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME 5
	#define EXECUTION_MODE_INDEXED_CSV_DATABASE_PREPROCESS 6
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME
		#define READ_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS (1)
		#define READ_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL (2)	//not used
		#define READ_MODE_DEFAULT (READ_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS)
		#define READ_MODES_TOTAL (2)
	#endif
	#ifdef INDEXED_CSV_DATABASE_PREPROCESS
		#define PREPROCESS_MODE_INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_FROM_MATRIX (1)
		#define PREPROCESS_MODE_INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_NEURONS_DATASET_FROM_SKELETONS (2)
		#define PREPROCESS_MODE_DEFAULT (PREPROCESS_MODE_INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_FROM_MATRIX)
		#define PREPROCESS_MODES_TOTAL (2)
	#endif
	#define EXECUTION_MODE_DEFAULT (EXECUTION_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME) 
	#define EXECUTION_MODES_TOTAL (4)
#endif

//debug;
//#define DEBUG_INDEXED_CSV_DATABASE_CREATE_INDEX_CONNECTIONS_BY_BASENEURON_ID	//temp


#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_CLASS_LABEL_DENDRITE "DENDRITE"
#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_CLASS_LABEL_SOMA "SOMA"
#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_CLASS_LABEL_UNKNOWN "UNKNOWN"
#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_CLASS_LABEL_AXON "AXON"	//see INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_RESTRICT_PREPOSTSYNAPTIC_CLASS_LABELS
#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_CLASS_LABEL_AIS "AIS"		//see INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_RESTRICT_PREPOSTSYNAPTIC_CLASS_LABELS

//configuration:
#ifdef INDEXED_CSV_DATABASE_H01
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL
		#define INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL_NORMALISE_LOCAL_CONNECTIVITY	//generate higher accuracy model by measuring neuron position relative to precise cortical map
		#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL_NORMALISE_LOCAL_CONNECTIVITY
			//neural connectivity model will be aligned to localised/average cortical positive flow direction (rather than using default H01 C3 Synaptic connections database/H01 indexed CSV database x/y coordinates system)
				//this enables a more accurate spatial cortical connectivity model (that is independent of the curvature of the observed H01 cortical sheet)
			#define INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL_NORMALISE_LOCAL_CONNECTIVITY_USE_LAYER_BOUNDS	//neural spatial connectivity model aligned to localised cortical positive flow direction (based on precise position of source neuron in cortical map)
			//#define INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL_NORMALISE_LOCAL_CONNECTIVITY_USE_FLOW_VECTOR	//neural spatial connectivity model aligned to average cortical positive flow direction	//less accurate
		#endif
		//#define INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL_TYPE_CURVED_CORTEX_SIMPLE_Z_EXTRAPOLATION	//option 1 //copy data in z direction for x iterations	//NOT YET IMPLEMENTED
		//#define INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL_TYPE_FLAT_CORTEX	//option 2	//cannot use existing 2D/3D graph templates to visualise model	//NOT YET IMPLEMENTED
		//#define INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL_TYPE_CURVED_CORTEX	//option 3	//NOT YET IMPLEMENTED
		#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL_TYPE_SIMPLE_Z_EXTRAPOLATION
			#define INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL_TYPE_CURVED_CORTEX_SIMPLE_Z_EXTRAPOLATION_MULTIPLIER (10)	//number of copies of mapped cortical slice to add in z direction
		#endif
	#endif
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS
		#define INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT	//optional //measure immediate recurrent connectivity of neurons (a -> b -> a)
		#define INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURSIVE	//optional //measure recursive connectivity of neurons (axon to dendrite)
		#define INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS	//optional //independently count the connections within the local connectome connections dataset (layer to layer matrix)	//compare local connectome counts against counts from https://www.biorxiv.org/content/10.1101/2021.05.29.446289v3/v4 Supplementary Table 5. Summary of Machine Learning-identified connections	//added 7 December 2021
		//#define INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PRINT_OUTPUT_VERBOSE_LOCALORNONLOCAL	//optional	//print number excitatory/inhibitory neurons for each source neuronID	//compare against counts from https://h01-release.storage.googleapis.com/data.html - gs://h01-release/data/20210601/c3/tables/segments/counts000000000NNN.csv.gz	//added 24 November 2021a
		//#define INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PRINT_DISTANCES	//print distances between neuron somas and their connections at each layer - used to identify INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL lateral distance probability of connection functions
	#endif
	#ifdef INDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS
		#define INDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS_COUNT_NUMBER_INCOMING_OUTGOING_EXCITATORY_INHIBITORY_SYNAPSES	//added 30 November 2021 
	#endif
	#ifdef INDEXED_CSV_DATABASE_CREATE
		#define INDEXED_CSV_DATABASE_CREATE_RECORD_CONFIDENCES	//added 26 November 2021a
		#define INDEXED_CSV_DATABASE_CREATE_CLASS_LABELS_SAVE_VERBATIM //added 30 November 2021	//else save only first character //enabling this makes the compressed indexed CSV database ~10% larger, but provides more robust class label storage
	#endif
	#ifdef INDEXED_CSV_DATABASE_QUERY
		//#define LOCAL_CONNECTOME_OFFICAL_RELEASE_C3_SOMAS	//use official somas dataset (https://h01-release.storage.googleapis.com/data.html - gs://h01-release/data/20210601/c3/tables/somas.csv) //added 24 November 2021 
	#endif
	#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET
		#ifdef INDEXED_CSV_DATABASE_CREATE_CLASS_LABELS_SAVE_VERBATIM
			#define INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_RESTRICT_PREPOSTSYNAPTIC_CLASS_LABELS	//added 30 November 2021 
		#endif
		//#define INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_VERIFICATION	//verify local connectome connections dataset generation vs existing (e.g. dev/ODS generated) connections datasets
	#endif	
	#ifdef INDEXED_CSV_DATABASE_VISUALISE_LOCAL_CONNECTOME
		//#define LOCAL_CONNECTOME_VISUALISATION_BACKWARDS_COMPATIBILITY_WITH_ODS_GENERATED_FILES	//temporary for diff comparisons between H01indexedCSVdatabaseGlobalDefs generated visualisations and ODS generated visualisations (visualisation generation verification)	//slower as uses non-distinct neuron id lists
	#endif
	
	#ifndef INDEXED_CSV_DATABASE_CREATE_CLASS_LABELS_SAVE_VERBATIM
		#define INDEXED_CSV_DATABASE_CREATE_CLASS_LABELS_SAVE_NUMBER_CHARACTERS (1)	//orig: 1 (must set to greater than 1 to not override class labels with same first character; eg AXON/AIS)
	#endif

	//valid connection class labels;
	#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_RESTRICT_PREPOSTSYNAPTIC_CLASS_LABELS
		//valid pre_class_label/post_class_label derived based on prepublishedLocalConnectionsDatasets/localConnectomeConnections-typesFromPresynapticNeurons.csv (ie in_body_cell_connection.csv);
		#define INDEXED_CSV_DATABASE_PRESYNAPTIC_SITE_CLASSLABEL_VALID_NUM 1
		#define INDEXED_CSV_DATABASE_POSTSYNAPTIC_SITE_CLASSLABEL_VALID_NUM 3
		static string indexed_csv_database_presynaptic_site_classlabel_valid[INDEXED_CSV_DATABASE_PRESYNAPTIC_SITE_CLASSLABEL_VALID_NUM] = {LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_CLASS_LABEL_AXON};	//CHECKTHIS
		static string indexed_csv_database_postsynaptic_site_classlabel_valid[INDEXED_CSV_DATABASE_POSTSYNAPTIC_SITE_CLASSLABEL_VALID_NUM] = {LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_CLASS_LABEL_DENDRITE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_CLASS_LABEL_SOMA, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_CLASS_LABEL_AIS};	//CHECKTHIS
	#endif

	//official c3 local connectome neuron dataset parameters;
	#ifdef LOCAL_CONNECTOME_OFFICAL_RELEASE_C3_SOMAS
		#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_EXCITATION_TYPE_ARTIFICIAL	//automatically generate this field
		#define LOCAL_CONNECTOME_OFFICAL_RELEASE_C3_SOMAS_EXPECT_SOME_MISSING_FROM_C3_CONNECTIONS_DATABASE
		#define LOCAL_CONNECTOME_DATASET_NEURONS_IGNORE_ADDITIONAL_NEURON_TYPES	//only read PYRAMIDAL/INTERNEURON exitatory/inihibitory neuron types	//this could be changed in future but is currently required for compatibility with existing !LOCAL_CONNECTOME_OFFICAL_RELEASE_C3_SOMAS codebase
		#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_LAYERS
		#define LOCAL_CONNECTOME_OFFICAL_RELEASE_C3_SOMAS_EXPECT_STRING_DELIMITERS
		#define INDEXED_CSV_DATABASE_QUERY_EFFICIENT_STORE_NEURON_IDS_IN_MAP	//required for H01indexedCSVdatabaseCalculateNeuronLayer.transferLocalConnectomeNeuronLayersToConnectionsLayers
	#endif	

	//apache avro C3 Synaptic connections database parameters:
	#ifdef INDEXED_CSV_DATABASE_CREATE
		#ifdef INDEXED_CSV_DATABASE_CREATE_RECORD_CONFIDENCES
			//#define INDEXED_CSV_DATABASE_QUERY_CONFIDENCE_THRESHOLD_REQUIRED
			#ifdef INDEXED_CSV_DATABASE_QUERY_CONFIDENCE_THRESHOLD_REQUIRED
				#define INDEXED_CSV_DATABASE_QUERY_CONFIDENCE_THRESHOLD (0.90)	//CHECKTHIS
			#endif
		#endif
		//#define INDEXED_CSV_DATABASE_CREATE_DEBUG	//disable this DEBUG parameter to create the database (enabled for safety - prevents overwrite of indexed database; takes ~6 hours to regenerate)
		#define AVRO_JSON_DATABASE_FOLDER "/media/user/large/h01data/data/exported/json"
	#endif

	//common H01 indexed csv database parameters:
	#ifdef INDEXED_CSV_DATABASE_CREATE_DEBUG
		#define INDEXED_CSV_DATABASE_FOLDER "/media/user/large/source/h01Connectome/indexedSVGdatabase/trial/indexedTrial"
	#else
		#define INDEXED_CSV_DATABASE_FOLDER "/media/user/ssddata/indexed"
	#endif

	//common apache avro C3 Synaptic connections database parameters:
	#define AVRO_JSON_DATABASE_EXCITATORY_SYNAPSE_TYPE (2)	//CHECKTHIS
	#define AVRO_JSON_DATABASE_INHIBITORY_SYNAPSE_TYPE (1)	//CHECKTHIS
	#define AVRO_JSON_DATABASE_COORDINATES_RANGE_X (473357-107342)	//366015
	#define AVRO_JSON_DATABASE_COORDINATES_RANGE_Y (282046-45528)	//236518
	#define AVRO_JSON_DATABASE_COORDINATES_RANGE_Z (5248-0)	//5248
	#define AVRO_JSON_DATABASE_COORDINATES_CALIBRATION_X (1.0)
	#define AVRO_JSON_DATABASE_COORDINATES_CALIBRATION_Y (1.0)
	#define AVRO_JSON_DATABASE_COORDINATES_CALIBRATION_Z (4.0)	//requires calibration	//can be used to enhance Z axis visibility
#elif defined INDEXED_CSV_DATABASE_LDC
	#define INDEXED_CSV_DATABASE_LDC_NEURON_TYPES_REFORMAT	//change all Xs to X neuron type (e.g. PNs -> PN) - neuronTypesCreation-INDEXED_CSV_DATABASE_LDC_NEURON_TYPES_REFORMAT
	//#define INDEXED_CSV_DATABASE_LDC_NEURON_LAYERS_REUSE_H01_TEMPLATES	//layers are interpreted as specific neuronTypes (for visualisation only)	//mapped to INDEXED_CSV_DATABASE_H01 layers to reuse H01 templates
	//#define INDEXED_CSV_DATABASE_LDC_DISABLE_2D_VISUALISATIONS	//LDC connectome is not designed for 2D X/Y visualisation
	
	#define INDEXED_CSV_DATABASE_QUERY_LOCAL_ONLY	//no external (csv database) query
	#define INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT	//optional //measure immediate recurrent connectivity of neurons (a -> b -> a)
	#define INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURSIVE	//optional //measure recursive connectivity of neurons (axon to dendrite)
	#define INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS	//optional //independently count the connections within the local connectome connections dataset (layer to layer matrix)	//compare local connectome counts against counts from https://www.biorxiv.org/content/10.1101/2021.05.29.446289v3/v4 Supplementary Table 5. Summary of Machine Learning-identified connections	//added 7 December 2021

	#ifdef INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_FROM_MATRIX
	#define LOCAL_CONNECTOME_CONNECTIONS_MATRIX_NUMBER_OF_FILES (4)
	static string localConnectomeConnectionsMatrixFileNames[LOCAL_CONNECTOME_CONNECTIONS_MATRIX_NUMBER_OF_FILES] = {"ad_connectivity_matrix.csv", "aa_connectivity_matrix.csv", "dd_connectivity_matrix.csv", "da_connectivity_matrix.csv"};
	#endif
	
	#ifdef INDEXED_CSV_DATABASE_PREPROCESS
		#define INDEXED_CSV_DATABASE_PREPROCESS_NEURON_TYPES_NAME "neuronTypes.txt"	//list of neuronType and excitationType
		#define INDEXED_CSV_DATABASE_PREPROCESS_NEURON_TYPES_FIELD_INDEX_NEURONTYPE (0)
		#define INDEXED_CSV_DATABASE_PREPROCESS_NEURON_TYPES_FIELD_INDEX_EXCITATIONTYPE (1)
		#define INDEXED_CSV_DATABASE_PREPROCESS_SM_CELLTYPE_DATAFILE_2
		#define INDEXED_CSV_DATABASE_PREPROCESS_SM_CELLTYPE_DATAFILE_34
		#ifdef INDEXED_CSV_DATABASE_PREPROCESS_SM_CELLTYPE_DATAFILE_2
			//does not provide neuron types for all neurons
			#define INDEXED_CSV_DATABASE_PREPROCESS_SM_CELLTYPE_DATAFILE_2_NAME "science.add9330_data_s2.csv"
			#define INDEXED_CSV_DATABASE_PREPROCESS_SM_CELLTYPE_DATAFILE_2_FIELD_INDEX_SKID1 (0)
			#define INDEXED_CSV_DATABASE_PREPROCESS_SM_CELLTYPE_DATAFILE_2_FIELD_INDEX_SKID2 (1)
			#define INDEXED_CSV_DATABASE_PREPROCESS_SM_CELLTYPE_DATAFILE_2_FIELD_INDEX_CELLTYPE (2)
		#endif
		#ifdef INDEXED_CSV_DATABASE_PREPROCESS_SM_CELLTYPE_DATAFILE_34
			//does not provide neuron types for all neurons
			#define INDEXED_CSV_DATABASE_PREPROCESS_SM_CELLTYPE_DATAFILE_3_NAME "science.add9330_data_s3.csv"
			#define INDEXED_CSV_DATABASE_PREPROCESS_SM_CELLTYPE_DATAFILE_4_NAME "science.add9330_data_s4.csv"
			#define INDEXED_CSV_DATABASE_PREPROCESS_SM_CELLTYPE_DATAFILE_34_FIELD_INDEX_SKID (0)
			#define INDEXED_CSV_DATABASE_PREPROCESS_SM_CELLTYPE_DATAFILE_34_FIELD_INDEX_CELLTYPE (1)
		#endif
		#define INDEXED_CSV_DATABASE_PREPROCESS_SKELETON_DATAFILE_FOLDER_NAME "skeletons"
		#define INDEXED_CSV_DATABASE_PREPROCESS_SKELETON_DATAFILE_NAME_START "skeleton"
		#define INDEXED_CSV_DATABASE_PREPROCESS_SKELETON_DATAFILE_NAME_EXTENSION ".swc"
		#define INDEXED_CSV_DATABASE_PREPROCESS_SKELETON_DATAFILE_NODE_FIELD_INDEX_X (2)	//CHECKTHIS
		#define INDEXED_CSV_DATABASE_PREPROCESS_SKELETON_DATAFILE_NODE_FIELD_INDEX_Y (3)	//CHECKTHIS
		#define INDEXED_CSV_DATABASE_PREPROCESS_SKELETON_DATAFILE_NODE_FIELD_INDEX_Z (4)	//CHECKTHIS
		#define INDEXED_CSV_DATABASE_PREPROCESS_NEURON_LIST_FILE_NAME "localConnectomeNeuronIDlist.csv"
		#define INDEXED_CSV_DATABASE_PREPROCESS_SM_SKELETON_IDS_FILE_NAME "inputs.csv"
	#endif
#endif


//count connections local;
#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS
	#define INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_ONLY_COUNT_OUTGOING	//only count connections from preSynaptic to postSynaptic neuron (as internal connectome connection matrices are identical)
	#define INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_NUMBER_EXCITATORY_INHIBITORY_NEURONS	//count number excitatory/inhibitory neurons also	//disable to simplify/separate local connectome connection counts
	#define INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_NUMBER_EXCITATORY_INHIBITORY_CONNECTIONS	//disable to simplify/separate local connectome connection counts
	#define INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_BY_LAYER
	#define INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_BY_EXCITATION_TYPE
#endif
#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT
	#define INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_NEURONS	//also count number of neurons with recurrent connections
	#define INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_NUMBER_ITERATIONS (2)	//#default: 2 (forward/backward) //FUTURE: trace recurrent connections across multiple iterations (>2)
	//#define INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_NEURONS_NONLOCAL	//not used; do not calculate nonlocal connections because a) requires high computational power and b) all neuron_id with both incoming and outgoing connections will have soma in the local connectome
#endif
	
	
//common local connectome dataset parameters:
#define LOCAL_CONNECTOME_FOLDER_BASE_USE_RELATIVE_FOLDER
#ifdef LOCAL_CONNECTOME_FOLDER_BASE_USE_RELATIVE_FOLDER
	#define LOCAL_CONNECTOME_FOLDER_BASE "../"	
#else
	#define LOCAL_CONNECTOME_FOLDER_BASE "/media/user/large/source/h01Connectome/H01LocalConnectome/"	
#endif
#ifdef INDEXED_CSV_DATABASE_H01
	#define LOCAL_CONNECTOME_DATASET_FOLDER "datasets/"
#elif defined INDEXED_CSV_DATABASE_LDC
	#define LOCAL_CONNECTOME_DATASET_FOLDER "datasetsLDC/"
#endif

#ifdef INDEXED_CSV_DATABASE_H01
	#ifdef LOCAL_CONNECTOME_OFFICAL_RELEASE_C3_SOMAS
		#define LOCAL_CONNECTOME_DATASET_NEURONS_FILENAME "somas.csv"
	#else
		#ifdef LOCAL_CONNECTOME_VISUALISATION_BACKWARDS_COMPATIBILITY_WITH_ODS_GENERATED_FILES
			#define LOCAL_CONNECTOME_DATASET_NEURONS_FILENAME "localConnectomeNeurons-nonDistinct.csv"
		#else
			#define LOCAL_CONNECTOME_DATASET_NEURONS_FILENAME "localConnectomeNeurons.csv"
		#endif
	#endif
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS "localConnectomeConnections-typesFromPresynapticNeurons.csv"
	#ifdef LOCAL_CONNECTOME_VISUALISATION_BACKWARDS_COMPATIBILITY_WITH_ODS_GENERATED_FILES
		#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES "localConnectomeConnections-typesFromEMimages-backwardsCompatibilityWithDevODSGenerations.csv"
	#else
		#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES "localConnectomeConnections-typesFromEMimages.csv"
	#endif
	#ifdef INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET
		#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES_STILL_REQUIRING_COMPLETION	//once completion function has been successfully executed on localConnectomeConnections-typesFromEMimages.csv (with the necessary C3 neurons dataset), this can be disabled
		#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES_USEALLVALUESAVAILABLEFROMINBODYCELLCONNECTION "localConnectomeConnections-typesFromEMimages-useAllValuesAvailableFromInBodyCellConnection.csv"
		#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES_UNKNOWN_NEURON_ID "0"
	#endif
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL
		#define LOCAL_CONNECTOME_DATASET_LARGE_MODEL_NEURONS_FILENAME "localConnectomeNeuronsLargeModel.csv"
		#define LOCAL_CONNECTOME_DATASET_LARGE_MODEL_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS "localConnectomeConnectionsLargeModel-typesFromPresynapticNeurons.csv"
		#define LOCAL_CONNECTOME_DATASET_LARGE_MODEL_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES "localConnectomeConnectionsLargeModel-typesFromEMimages.csv"
	#endif
#elif defined INDEXED_CSV_DATABASE_LDC
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS "localConnectomeConnections-typesFromPresynapticNeurons.csv"
	#define LOCAL_CONNECTOME_DATASET_NEURONS_FILENAME "localConnectomeNeurons.csv"
#endif

#ifdef INDEXED_CSV_DATABASE_H01
	#ifdef LOCAL_CONNECTOME_OFFICAL_RELEASE_C3_SOMAS
		//"soma_id","base_seg_id","c2_rep_strict","c2_rep_manual","c3_rep_strict","c3_rep_manual","proofread_104_rep","x","y","z","celltype","layer"
		#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_SOMA_ID (0)
		#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_BASE_SEG_ID (1)
		#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_C2_REP_STRICT (2)
		#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_C2_REP_MANUAL (3)
		#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_C3_REP_STRICT (4)
		#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_C3_REP_MANUAL (5)
		#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_PROOFREAD_104_REP (6)
		#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_X (7)
		#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Y (8)
		#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Z (9)
		#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE (10)
		#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER (11)	//nonartifical for LOCAL_CONNECTOME_OFFICAL_RELEASE_C3_SOMAS	//reformatted: Layer 1->White matter to 1->7
		#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE (12)	//artificial //excitatory (1), inhibitory (0)	//derived from neuron type
		#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_NEURON_ID (LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_C3_REP_STRICT)	//CHECKTHIS
		#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_FIRST (LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE)+1
	#else
		//id,x,y,z,type,excitation_type
		#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_NEURON_ID (0)	//C3 database neuron_id
		#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_X (1)	//C3 database neuron centroid coordinates
		#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Y (2)	//C3 database neuron centroid coordinates
		#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Z (3)	//C3 database neuron centroid coordinates
		#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE (4)	//C3 database neuron classification (PYRAMIDAL, INTERNEURON)
		#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE (5)	//excitatory (1), inhibitory (0)	//derived from neuron type
		#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_FIRST (LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE)+1
	#endif
#elif defined INDEXED_CSV_DATABASE_LDC
	//id,x,y,z,type,excitation_type
	#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_NEURON_ID (0)	//C3 database neuron_id
	#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_X (1)	//C3 database neuron centroid coordinates
	#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Y (2)	//C3 database neuron centroid coordinates
	#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Z (3)	//C3 database neuron centroid coordinates
	#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE (4)	//C3 database neuron classification (PYRAMIDAL, INTERNEURON)
	#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE (5)	//excitatory (1), inhibitory (0)	//derived from neuron type
	#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_FIRST (LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE)+1
#endif
#define LOCAL_CONNECTOME_DATASET_NEURONS_HEADER "id,x,y,z,type,excitation_type"

#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY (1)
#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY (0)
#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN (-1)
#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_PYRAMIDAL "PYRAMIDAL"
#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_INTERNEURON "INTERNEURON"
#ifdef LOCAL_CONNECTOME_OFFICAL_RELEASE_C3_SOMAS
	#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_SPINY_STELLATE "SPINY_STELLATE"
	#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_OLIGO "OLIGO"
	#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_ASTROCYTE "ASTROCYTE"
	#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_MG_OPC "MG_OPC"
	#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_BLOOD_VESSEL_CELL "BLOOD_VESSEL_CELL"
	#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_UNCLASSIFIED_NEURON "UNCLASSIFIED_NEURON"
	#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_SPINY_ATYPICAL "SPINY_ATYPICAL"	
#endif
#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_UNKNOWN "UNKNOWN"	//not used in current local connectome dataset (localConnectomeNeurons.csv); for internal/intermediary processing only


#ifdef INDEXED_CSV_DATABASE_H01
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_ID (0)	//C3 database neuron_id
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_X (1)	//C3 database neuron centroid coordinates
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_Y (2)	//C3 database neuron centroid coordinates
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_Z (3)	//C3 database neuron centroid coordinates
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_TYPE (4)	//C3 database neuron classification (PYRAMIDAL, INTERNEURON)
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_ID (5)	//C3 database neuron_id
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_X (6)	//C3 database neuron centroid coordinates
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_Y (7)	//C3 database neuron centroid coordinates
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_Z (8)	//C3 database neuron centroid coordinates
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_TYPE (9)	//C3 database neuron classification (PYRAMIDAL, INTERNEURON)
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_CLASS (10)	//C3 database synapse connection site (DENDRITE, SOMA, UNKNOWN) //not used
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_SYN_NUM (11)	//number of connections
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE (12)	//excitatory (1), inhibitory (0)	//derived from neuron type [connectionTypesDerivedFromEMimages] or C3 connections database [connectionTypesDerivedFromPresynapticNeurons]
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_HEADER "pre_id,pre_x,pre_y,pre_z,pre_type,post_id,post_x,post_y,post_z,post_type,post_class_label,syn_num,excitation_type"
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_ARTIFICIAL_FIRST (LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE)+1
#elif defined INDEXED_CSV_DATABASE_LDC
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_ID (0)	//C3 database neuron_id
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_ID (1)	//C3 database neuron_id
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_CLASS (2)	//C3 database synapse connection site (DENDRITE, AXON, UNKNOWN)
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_CLASS (3)	//C3 database synapse connection site (DENDRITE, AXON, UNKNOWN)
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_SYN_NUM (4)	//number of connections
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_HEADER "pre_id,post_id,pre_class_label,post_class_label,syn_num"
	//artificial fields (generated from LOCAL_CONNECTOME_DATASET_NEURONS file);
	#define INDEXED_CSV_DATABASE_QUERY_READ_DATASET_LOCAL_CONNECTOME_CONNECTIONS_DYNAMICALLY_GENERATE_REDUNDANT_FIELDS	//use LOCAL_CONNECTOME_DATASET_NEURONS file to dynamically generate redundant fields
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_X (5)	//C3 database neuron centroid coordinates
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_Y (6)	//C3 database neuron centroid coordinates
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_Z (7)	//C3 database neuron centroid coordinates
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_TYPE (8)	//C3 database neuron classification (PYRAMIDAL, INTERNEURON)
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_X (9)	//C3 database neuron centroid coordinates
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_Y (10)	//C3 database neuron centroid coordinates
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_Z (11)	//C3 database neuron centroid coordinates
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_TYPE (12)	//C3 database neuron classification (PYRAMIDAL, INTERNEURON)
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE (13)	//excitatory (1), inhibitory (0)	//derived from neuron type [connectionTypesDerivedFromPresynapticNeurons]
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_ARTIFICIAL_FIRST (LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE)+1
#endif

#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY (1)
#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY (0)
#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN (-1)	//is this required?
#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_TYPE_PYRAMIDAL "PYRAMIDAL"
#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_TYPE_INTERNEURON "INTERNEURON"
#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_TYPE_UNKNOWN "UNKNOWN"
#ifdef INDEXED_CSV_DATABASE_LDC
	static string localConnectomeConnectionsMatrixSynapsePreClassLabel[LOCAL_CONNECTOME_CONNECTIONS_MATRIX_NUMBER_OF_FILES] = {LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_CLASS_LABEL_AXON, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_CLASS_LABEL_AXON, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_CLASS_LABEL_DENDRITE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_CLASS_LABEL_DENDRITE};
	static string localConnectomeConnectionsMatrixSynapsePostClassLabel[LOCAL_CONNECTOME_CONNECTIONS_MATRIX_NUMBER_OF_FILES] = {LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_CLASS_LABEL_DENDRITE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_CLASS_LABEL_AXON, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_CLASS_LABEL_DENDRITE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_CLASS_LABEL_AXON};
#endif

#ifdef INDEXED_CSV_DATABASE_H01
	//common H01 indexed csv database parameters:
	#define AVRO_JSON_DATABASE_NEURON_ID_MAX_NUM_CHARACTERS (11)
	#define INDEXED_CSV_DATABASE_STORE_MULTIPLE_PREPOSTSYNAPTIC_NEURONID_SYNAPSES_PER_FILE	//reduces number of files, requires further searching of files by application reading indexed csv database
	#ifdef INDEXED_CSV_DATABASE_STORE_MULTIPLE_PREPOSTSYNAPTIC_NEURONID_SYNAPSES_PER_FILE
		//eg 123/rawPre|PostSynapticNeuronID123.json|csv
		#define INDEXED_CSV_DATABASE_NUMBER_LEVELS (2)
		#define INDEXED_CSV_DATABASE_NUMBER_CHARACTERS_PER_FOLDER_LEVEL (3)
		#define INDEXED_CSV_DATABASE_FILENAME_NEURONID_MAX_NUM_CHARACTERS (6)	//must be greater than (INDEXED_CSV_DATABASE_NUMBER_LEVELS-1)*INDEXED_CSV_DATABASE_NUMBER_CHARACTERS_PER_FOLDER_LEVEL
	#else
		//eg 1234/5678/rawPre|PostSynapticNeuronID123456789AB.json|csv
		#define INDEXED_CSV_DATABASE_NUMBER_LEVELS (3)
		#define INDEXED_CSV_DATABASE_NUMBER_CHARACTERS_PER_FOLDER_LEVEL (4)
		#define INDEXED_CSV_DATABASE_FILENAME_NEURONID_MAX_NUM_CHARACTERS (AVRO_JSON_DATABASE_NEURON_ID_MAX_NUM_CHARACTERS)
	#endif
	#define INDEXED_CSV_DATABASE_FILENAME_PREFIX_PRESYNAPTIC_CSV "csvPreSynapticNeuronID"
	#define INDEXED_CSV_DATABASE_FILENAME_PREFIX_PRESYNAPTIC_RAW "rawPreSynapticNeuronID"
	#define INDEXED_CSV_DATABASE_FILENAME_PREFIX_POSTSYNAPTIC_CSV "csvPostSynapticNeuronID"
	#define INDEXED_CSV_DATABASE_FILENAME_PREFIX_POSTSYNAPTIC_RAW "rawPostSynapticNeuronID"
	#define INDEXED_CSV_DATABASE_PRESYNAPTIC_SITE_NEURON_ID_FIELD_INDEX (0)
	#define INDEXED_CSV_DATABASE_POSTSYNAPTIC_SITE_NEURON_ID_FIELD_INDEX (1)
	#define INDEXED_CSV_DATABASE_PRESYNAPTIC_SITE_TYPE_FIELD_INDEX (2)
	#define INDEXED_CSV_DATABASE_POSTSYNAPTIC_SITE_TYPE_FIELD_INDEX (3)
	#define INDEXED_CSV_DATABASE_PRESYNAPTIC_SITE_CLASSLABEL_FIELD_INDEX (4)
	#define INDEXED_CSV_DATABASE_POSTSYNAPTIC_SITE_CLASSLABEL_FIELD_INDEX (5)
	#define INDEXED_CSV_DATABASE_PRESYNAPTIC_SITE_BASENEURON_ID_FIELD_INDEX (6)
	#define INDEXED_CSV_DATABASE_POSTSYNAPTIC_SITE_BASENEURON_ID_FIELD_INDEX (7)
	#define INDEXED_CSV_DATABASE_SYNAPSE_LOCATION_COORDINATE_X_FIELD_INDEX (8)
	#define INDEXED_CSV_DATABASE_SYNAPSE_LOCATION_COORDINATE_Y_FIELD_INDEX (9)
	#define INDEXED_CSV_DATABASE_SYNAPSE_LOCATION_COORDINATE_Z_FIELD_INDEX (10)
	#define INDEXED_CSV_DATABASE_SYNAPSE_TYPE_FIELD_INDEX (11)
	#ifdef INDEXED_CSV_DATABASE_CREATE_RECORD_CONFIDENCES
		#define INDEXED_CSV_DATABASE_SYNAPSE_CONFIDENCE_FIELD_INDEX (12)
		#define INDEXED_CSV_DATABASE_NUMBER_FIELDS (13)
	#else
		#define INDEXED_CSV_DATABASE_NUMBER_FIELDS (12)
	#endif
#endif

//common file format parameters:
#define LDR_REFERENCE_TYPE_SUBREFERENCE (1)
#define LDR_REFERENCE_TYPE_LINE (2)
#define LDR_REFERENCE_TYPE_TRIANGLE (3)
#define LDR_REFERENCE_TYPE_QUADRILATERAL (4)
#define JSON_FILE_EXTENSION ".json"
#define CSV_FILE_EXTENSION  ".csv"
#define LDR_FILE_EXTENSION  ".ldr"
#define SVG_FILE_EXTENSION  ".svg"
#define CSV_DELIMITER ","
#define CSV_DELIMITER_CHAR ','
#define LDR_DELIMITER " "
#define STRING_DELIMITER '"'



//mode INDEXED_CSV_DATABASE_CREATE parameters:
#ifdef INDEXED_CSV_DATABASE_CREATE

	#define INDEXED_CSV_DATABASE_CREATE_PRINT_STATUS	//notify user of status during database creation
	
	//apache avro C3 Synaptic connections database parameters:
	#define MAX_SSD_SIZE	//impose database size limit (50GB)
	#ifdef MAX_SSD_SIZE
		#define EXECUTE_SUBSET
		#define AVRO_JSON_DATABASE_FILEINDEX_START (0)
		#define AVRO_JSON_DATABASE_FILEINDEX_END (165)
	#else
		#define AVRO_JSON_DATABASE_FILEINDEX_START (0)
		#define AVRO_JSON_DATABASE_FILEINDEX_END (165)
		#define SAVE_RAW_JSON	//unrestricted by size 
	#endif

	#ifdef INDEXED_CSV_DATABASE_CREATE_DEBUG
		#define AVRO_JSON_DATABASE_FILEINDEX_START (0)
		#define AVRO_JSON_DATABASE_FILEINDEX_END (0)
	#endif
	//#define DEBUG_RESTART
	#ifdef DEBUG_RESTART
		#define AVRO_JSON_DATABASE_FILEINDEX_START (0)
		#define AVRO_JSON_DATABASE_FILEINDEX_END (0)
		#define DEBUG_RESTART_LINEINDEX_START (620306) //614124
	#endif
#endif

//mode INDEXED_CSV_DATABASE_QUERY parameters:
#ifdef INDEXED_CSV_DATABASE_QUERY
	
	#define INDEXED_CSV_DATABASE_QUERY_PREFER_DATASET_LOCAL_CONNECTOME_NEURONS
	#ifdef INDEXED_CSV_DATABASE_QUERY_PREFER_DATASET_LOCAL_CONNECTOME_NEURONS
		//always read neuron lists from local connectome dataset ("localConnectomeNeurons.csv")
		#define INDEXED_CSV_DATABASE_QUERY_PREFER_DATASET_LOCAL_CONNECTOME_NEURONS_VALUE true
		#define INDEXED_CSV_DATABASE_QUERY_READ_DATASET_LOCAL_CONNECTOME_NEURONS	//localConnectomeNeurons.csv derived from in_body_cell_connection.csv - pre/postsynaptic distinct neuron ids
		#define INDEXED_CSV_DATABASE_QUERY_NEURON_LIST_DISTINCT_FILE_NAME (LOCAL_CONNECTOME_DATASET_NEURONS_FILENAME)
	#else
		//read neuron lists from separate file if possible ("localConnectomeNeuronIDlistDistinct.csv")
		#define INDEXED_CSV_DATABASE_QUERY_PREFER_DATASET_LOCAL_CONNECTOME_NEURONS_VALUE false
		#define INDEXED_CSV_DATABASE_QUERY_NEURON_LIST_DISTINCT_FILE_NAME "localConnectomeNeuronIDlistDistinct.csv"	//from in_body_cell_connection.csv - pre/postsynaptic distinct neuron ids		
	#endif
	
	//mode INDEXED_CSV_DATABASE_QUERY_EXTRACT_INCOMING_OUTGOING_CONNECTIONS parameters:
	#ifdef INDEXED_CSV_DATABASE_QUERY_EXTRACT_INCOMING_OUTGOING_CONNECTIONS
		#define INDEXED_CSV_DATABASE_QUERY_OUTPUT	//mandatory

		//input:
		//see above
		
		/*	
		//depreciated;
		#define INDEXED_CSV_DATABASE_QUERY_NEURON_LIST1_FILE_NAME "localConnectomeNeuronIDlist1.csv"	//from in_body_cell_connection.csv - presynaptic neuron ids
		#define INDEXED_CSV_DATABASE_QUERY_NEURON_LIST2_FILE_NAME "localConnectomeNeuronIDlist2.csv"	//from in_body_cell_connection.csv - postsynaptic neuron ids
		#define INDEXED_CSV_DATABASE_QUERY_NEURON_LIST1_CONNECTIONS_PRESYNAPTIC_FILE_NAME "localConnectomeNeuronIDlist1connectionsPresynaptic.csv"
		#define INDEXED_CSV_DATABASE_QUERY_NEURON_LIST1_CONNECTIONS_POSTSYNAPTIC_FILE_NAME "localConnectomeNeuronIDlist1connectionsPostsynaptic.csv"
		#define INDEXED_CSV_DATABASE_QUERY_NEURON_LIST2_CONNECTIONS_PRESYNAPTIC_FILE_NAME "localConnectomeNeuronIDlist2connectionsPresynaptic.csv"
		#define INDEXED_CSV_DATABASE_QUERY_NEURON_LIST2_CONNECTIONS_POSTSYNAPTIC_FILE_NAME "localConnectomeNeuronIDlist2connectionsPostsynaptic.csv"
		*/
		//output:
		#define INDEXED_CSV_DATABASE_QUERY_WRITE_CURRENT_FOLDER
		#define INDEXED_CSV_DATABASE_QUERY_EXTRACT_INCOMING_OUTGOING_CONNECTIONS_NEURON_LIST_CONNECTIONS_PRESYNAPTIC_FILE_NAME "localConnectomeNeuronIDlistConnectionsPresynaptic.csv"
		#define INDEXED_CSV_DATABASE_QUERY_EXTRACT_INCOMING_OUTGOING_CONNECTIONS_NEURON_LIST_CONNECTIONS_POSTSYNAPTIC_FILE_NAME "localConnectomeNeuronIDlistConnectionsPostsynaptic.csv"
	#endif
	
	//mode INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING parameters:
	#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING
		
		//input:
		//see above

		#define INDEXED_CSV_DATABASE_ALGORITHMS
		#define INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_MIN_NUM_POINTS_REQUIRED (20)	//(2)
		#define INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_NON_LOCAL_NEURONS_REQUIRED
		#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_NON_LOCAL_NEURONS_REQUIRED
			#define INDEXED_CSV_DATABASE_QUERY_EFFICIENT_STORE_NEURON_IDS_IN_MAP
		#endif
		#define INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_EST_TYPE_BY_AVERAGE_SYNAPSE_TYPE
		#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_EST_TYPE_BY_AVERAGE_SYNAPSE_TYPE
			#define INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_CONNECTION_TYPE_REQUIRED
			#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_CONNECTION_TYPE_REQUIRED
				#define INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_CONNECTION_TYPE (AVRO_JSON_DATABASE_EXCITATORY_SYNAPSE_TYPE)	//excitatory
			#endif
		#endif
		
		//MANDATORY: INDEXED_CSV_DATABASE_QUERY_GET_LOCATION	//used to perform incoming long range axonal tract position/direction estimation (not just statistical comparison of distal vs local input)
		#define INDEXED_CSV_DATABASE_QUERY_OUTPUT	//mandatory	//performance speed by disabling primary output file write
		#ifdef INDEXED_CSV_DATABASE_QUERY_OUTPUT
			#define INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING
		#endif
		
		//#define INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_2D_POLY_REGRESSION
		#define INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_3D_LINEAR_REGRESSION
		#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_3D_LINEAR_REGRESSION
			#define INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING_LDR
		#else
			#define INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING_CSV
		#endif
	
		#define INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_CONNECTION_TYPE_EXCITATORY_COLOUR (SHARED_COLOUR_GREEN)
		#define INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_CONNECTION_TYPE_INHIBITORY_COLOUR (SHARED_COLOUR_RED)
			
		//output:
		#define INDEXED_CSV_DATABASE_QUERY_WRITE_CURRENT_FOLDER
		#ifdef INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING_LDR
			#define INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_FILE_NAME "localConnectomeIncomingAxonMapping.ldr"		
		#else
			#define INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_FILE_NAME "localConnectomeIncomingAxonMapping.csv"	//"localConnectomeNeuronIDdistinctListconnectionsPresynaptic.csv"	
		#endif
	#endif

	//mode INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET parameters:
	#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET
		
		//#define INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_ORIG
		
		//input:
		#define INDEXED_CSV_DATABASE_QUERY_READ_DATASET_LOCAL_CONNECTOME_NEURONS	//localConnectomeNeurons.csv derived from in_body_cell_connection.csv - pre/postsynaptic distinct neuron ids	
		
		#define INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_LOCAL_NEURONS_REQUIRED
		#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_LOCAL_NEURONS_REQUIRED		
			#define INDEXED_CSV_DATABASE_QUERY_EFFICIENT_STORE_NEURON_IDS_IN_MAP
		#endif
		
		//output:
		#define INDEXED_CSV_DATABASE_QUERY_WRITE_CURRENT_FOLDER
		#define INDEXED_CSV_DATABASE_QUERY_OUTPUT	//mandatory
		
	#endif
		
	//mode INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS parameters:
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS
				
		//input:
		//see above

		#define INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_EM_IMAGES
		#define INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_PRESYNAPTIC_NEURONS //print connections for CONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS where local presynaptic neuron is available (not just CONNECTION_TYPES_DERIVED_FROM_EM_IMAGES)	//added 7 December 2021
			//note excitationType will be LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN if presynaptic neuron is not in local connectome (so numberConnectionsExternalConnectomeExcitatory+numberConnectionsExternalConnectomeInhibitory != numberConnectionsExternalConnectome)
		
		#define INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PROPORTION_LOCAL_VS_NONLOCAL_CONNECTIONS	//original functionality	//mandatory
		
		#define INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_NUMBER_EXCITATORY_INHIBITORY_NEURONS
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_NUMBER_EXCITATORY_INHIBITORY_NEURONS
			#define INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PER_NEURON
			#define INDEXED_CSV_DATABASE_QUERY_READ_DATASET_LOCAL_CONNECTOME_NEURONS
		#endif
		
		#define INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
			#define INDEXED_CSV_DATABASE_QUERY_READ_DATASET_LOCAL_CONNECTOME_NEURONS	
			#define INDEXED_CSV_DATABASE_QUERY_LAYERS
			#ifdef INDEXED_CSV_DATABASE_QUERY_LAYERS
				#define INDEXED_CSV_DATABASE_CALCULATE_NEURON_LAYERS
			#endif
		#endif
			
		#define INDEXED_CSV_DATABASE_QUERY_EFFICIENT_STORE_NEURON_IDS_IN_MAP
		
	#endif
	
	//mode INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET parameters:
	#ifdef INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET
				
		//input:	
				
		//output:
		#define INDEXED_CSV_DATABASE_QUERY_WRITE_CURRENT_FOLDER
		#define INDEXED_CSV_DATABASE_QUERY_OUTPUT	//mandatory
		
	#endif
	
	//mode INDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS parameters:
	#ifdef INDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS
		#define INDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS_COUNT_NUMBER_UNIQUE_AXONS_DENDRITES
	#endif	
	
	
	#ifndef INDEXED_CSV_DATABASE_QUERY_WRITE_CURRENT_FOLDER
		#define INDEXED_CSV_DATABASE_QUERY_OUTPUT_FOLDER "/media/user/large/source/h01Connectome/indexedSVGdatabase/H01indexedCSVdatabaseQueryOutput"
	#endif
	#ifndef INDEXED_CSV_DATABASE_QUERY_READ_DATASET_LOCAL_CONNECTOME_NEURONS
		//#define INDEXED_CSV_DATABASE_QUERY_READ_CURRENT_FOLDER
	#endif
	

	#define INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_FILE_OBJECT	//mandatory - only use this if output folder exists on a different harddrive (not just partition) to indexed CSV database folder
	//max expected file/string size = indexed csv database query output size (neuron_id ~11 bytes + , + type ~1 byte = ~13 bytes) * avg num connections per neuron ID (~2000) * number of neuron IDs in list (~30000) = 13*2000*30000 = ~1GB of RAM if string	
#endif

//mode INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME parameters:
#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME

	//mode INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL parameters:
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL
				
		//input:
		//see above
		
		#define INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS

	#endif	
		
	//mode INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS parameters:
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS
				
		//input:
		//see above

		#ifdef INDEXED_CSV_DATABASE_H01
			#define INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_EM_IMAGES
		#endif
		#define INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_PRESYNAPTIC_NEURONS //print connections for CONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS where local presynaptic neuron is available (not just CONNECTION_TYPES_DERIVED_FROM_EM_IMAGES)	//added 7 December 2021
			//note excitationType will be LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN if presynaptic neuron is not in local connectome (so numberConnectionsExternalConnectomeExcitatory+numberConnectionsExternalConnectomeInhibitory != numberConnectionsExternalConnectome)
		
		#define INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PROPORTION_LOCAL_VS_NONLOCAL_CONNECTIONS	//original functionality	//mandatory
		
		#define INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_NUMBER_EXCITATORY_INHIBITORY_NEURONS
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_NUMBER_EXCITATORY_INHIBITORY_NEURONS
			#define INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PER_NEURON
			#define INDEXED_CSV_DATABASE_QUERY_READ_DATASET_LOCAL_CONNECTOME_NEURONS
		#endif
		
		#define INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_BY_LAYER
			#define INDEXED_CSV_DATABASE_QUERY_READ_DATASET_LOCAL_CONNECTOME_NEURONS	
			#define INDEXED_CSV_DATABASE_QUERY_LAYERS
			#ifdef INDEXED_CSV_DATABASE_QUERY_LAYERS
				#define INDEXED_CSV_DATABASE_CALCULATE_NEURON_LAYERS
			#endif
		#endif
			
		#define INDEXED_CSV_DATABASE_QUERY_EFFICIENT_STORE_NEURON_IDS_IN_MAP
		
	#endif

	//mode INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_FROM_MATRIX parameters:
	#ifdef INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_FROM_MATRIX
		
		//output:
		#define INDEXED_CSV_DATABASE_QUERY_WRITE_CURRENT_FOLDER
		//#define INDEXED_CSV_DATABASE_QUERY_OUTPUT	//mandatory
		
	#endif
		
#endif
	
//mode INDEXED_CSV_DATABASE_VISUALISE_LOCAL_CONNECTOME parameters:
#ifdef INDEXED_CSV_DATABASE_VISUALISE_LOCAL_CONNECTOME
	
	//2D/3D visualisations;
	#ifndef INDEXED_CSV_DATABASE_LDC_DISABLE_2D_VISUALISATIONS
		#define LOCAL_CONNECTOME_VISUALISATION_2D_SVG
	#endif
	#define LOCAL_CONNECTOME_VISUALISATION_3D_LDR
	#ifdef LOCAL_CONNECTOME_VISUALISATION_3D_LDR
		#define LOCAL_CONNECTOME_VISUALISATION_3D
	#endif
	
	//layer calculations;
	#define LOCAL_CONNECTOME_VISUALISATION_LAYERS
	#ifdef LOCAL_CONNECTOME_VISUALISATION_LAYERS
		#define INDEXED_CSV_DATABASE_CALCULATE_NEURON_LAYERS
	#endif

	//visualisation folder names;	
	#define LOCAL_CONNECTOME_VISUALISATION_FOLDER "visualisations/"	//"visualisations/"
	#define LOCAL_CONNECTOME_VISUALISATION_FOLDER_CONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS "connectionTypesFromPresynapticNeurons/"
	#define LOCAL_CONNECTOME_VISUALISATION_FOLDER_CONNECTION_TYPES_DERIVED_FROM_EM_IMAGES "connectionTypesFromEMimages/"
	#define LOCAL_CONNECTOME_VISUALISATION_FOLDER_2D "2D/"
	#define LOCAL_CONNECTOME_VISUALISATION_FOLDER_3D "3D/"
	#ifdef INDEXED_CSV_DATABASE_H01
		#define LOCAL_CONNECTOME_VISUALISATION_TEMPLATES_FOLDER "visualisations/templates"
	#elif defined INDEXED_CSV_DATABASE_LDC
		#define LOCAL_CONNECTOME_VISUALISATION_TEMPLATES_FOLDER "visualisations/templatesLDC"
	#endif
	//visualisation file names;		
	#define LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_DIRECTION "connections_IE"
	#define LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_DIRECTION_FLOW "connections_IE_flow"
	#define LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_DIRECTION_NUMBER_COLOURSETS (2)
	#define LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_DIRECTION_FLOW_NUMBER_COLOURSETS (3)
	#ifdef LOCAL_CONNECTOME_VISUALISATION_LAYERS
		#define LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_LAYERED_DIRECTION "connections_IE_layered"
		#define LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_LAYERED_DIRECTION_FLOW "connections_IE_layered_flow"
		#define LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_LAYERED_DIRECTION_NUMER_COLOURSETS (2)
		#define LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_LAYERED_DIRECTION_FLOW_NUMBER_COLOURSETS (2)
		#define LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_LAYERED_SPECIFIC_LAYER "_L"
	#endif
	#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
		#define LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_DIRECTION "connections_IE"
		#define LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_DIRECTION_FLOW "connections_IE_flow"
		#define LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_DIRECTION_NUMBER_COLOURSETS (2)
		#define LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_DIRECTION_FLOW_NUMBER_COLOURSETS (1)
		#ifdef LOCAL_CONNECTOME_VISUALISATION_LAYERS
			#define LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_LAYERED_DIRECTION "connections_IE_layered"
			#define LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_LAYERED_DIRECTION_FLOW "connections_IE_layered_flow"
			#define LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_LAYERED_DIRECTION_NUMER_COLOURSETS (2)
			#define LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_LAYERED_DIRECTION_FLOW_NUMBER_COLOURSETS (1)
			#define LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_LAYERED_SPECIFIC_LAYER "_L"
		#endif
	#endif
	#define LOCAL_CONNECTOME_VISUALISATION_FILENAME_COLOURSET_PREPEND ""	//_colourset
	
	//visualisation template file names;
	#define LOCAL_CONNECTOME_VISUALISATION_SVG_PART_IMPORT_FILENAME_PREPEND "connections_IE_part"
	#define LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_PREPEND "connections_IE"
	#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
		#define LOCAL_CONNECTOME_VISUALISATION_LDR_PART_IMPORT_FILENAME_PREPEND "connections_IE_part"
		#define LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_PREPEND "connections_IE"
	#endif
	#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME
		#define LOCAL_CONNECTOME_VISUALISATION_SVG_PART_IMPORT_FILENAME_APPEND_TRACE_LOCAL_CONNECTOME "-traceVisualisationVideoCrop"
	#endif
	#define LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_DIRECTION "-Direction"
	#define LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_DIRECTION_FLOW "-DirectionFlow"
	#define LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_LAYERED_DIRECTION_FLOW "-LayeredDirectionFlow"
	#define LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_LAYERED_DIRECTION "-LayeredDirection"
	#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME
		#define LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_TRACE_VISUALISATION "-traceVisualisation"	
	#endif
	
	//visualisation coloursets (neurons);
	#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOURSET_REFERENCE_EXCITATION_TYPE_EXCITATORY "exciteNeuron"
	#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOURSET_REFERENCE_EXCITATION_TYPE_INHIBITORY "inhibitNeuron"
	#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOURSET_REFERENCE_EXCITATION_TYPE_UNKNOWN "unknownNeuron"
	#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOURSET_REFERENCE_APPEND "Color"
	#ifdef LOCAL_CONNECTOME_VISUALISATION_LAYERS
		#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOURSET_REFERENCE_LAYER "Layer"	//LayerX
	#endif
	#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME
		#define LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_TRACE_ACTIVE "traceActive"
		#define LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_TRACE_ACTIVE_HIGHLIGHT1 LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_TRACE_ACTIVE "Highlight"	
		#define LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_TRACE_ACTIVE_HIGHLIGHT2 LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_TRACE_ACTIVE "Mark"	
		#define LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_TRACE_ACTIVE_HIGHLIGHT1_EXCITATORY_NEURON LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_TRACE_ACTIVE_HIGHLIGHT1 LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOURSET_REFERENCE_EXCITATION_TYPE_EXCITATORY
		#define LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_TRACE_ACTIVE_HIGHLIGHT1_INHIBITORY_NEURON LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_TRACE_ACTIVE_HIGHLIGHT1 LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOURSET_REFERENCE_EXCITATION_TYPE_INHIBITORY
		#define LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_TRACE_ACTIVE_HIGHLIGHT1_INACTIVE_NEURON LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_TRACE_ACTIVE_HIGHLIGHT1 LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOURSET_REFERENCE_EXCITATION_TYPE_UNKNOWN
		#define LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_TRACE_ACTIVE_HIGHLIGHT2_EXCITATORY_NEURON LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_TRACE_ACTIVE_HIGHLIGHT2 LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOURSET_REFERENCE_EXCITATION_TYPE_EXCITATORY
		#define LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_TRACE_ACTIVE_HIGHLIGHT2_INHIBITORY_NEURON LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_TRACE_ACTIVE_HIGHLIGHT2 LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOURSET_REFERENCE_EXCITATION_TYPE_INHIBITORY
		#define LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_TRACE_ACTIVE_HIGHLIGHT2_INACTIVE_NEURON LOCAL_CONNECTOME_VISUALISATION_SVG_FILENAME_APPEND_COLOURSET_TRACE_ACTIVE_HIGHLIGHT2 LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOURSET_REFERENCE_EXCITATION_TYPE_UNKNOWN
	#endif	
	#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_SVG_HEADER_TEXT "     <!-- neurons with excitatory/inhibitory connections -->"
	#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
		#ifdef LOCAL_CONNECTOME_VISUALISATION_BACKWARDS_COMPATIBILITY_WITH_ODS_GENERATED_FILES
			static int local_connectome_visualisation_neurons_colour_excitation_type_excitatory[LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_DIRECTION_NUMBER_COLOURSETS] = {SHARED_COLOUR_GREEN,SHARED_COLOUR_LIGHT_GREEN};
			static int local_connectome_visualisation_neurons_colour_excitation_type_inhibitory[LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_DIRECTION_NUMBER_COLOURSETS] = {SHARED_COLOUR_RED,SHARED_COLOUR_LIGHT_RED};
			static int local_connectome_visualisation_neurons_colour_excitation_type_unknown[LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_DIRECTION_NUMBER_COLOURSETS] = {SHARED_COLOUR_WHITE,SHARED_COLOUR_WHITE};

		#else
			static int local_connectome_visualisation_neurons_colour_excitation_type_excitatory[LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_DIRECTION_NUMBER_COLOURSETS] = {SHARED_COLOUR_YELLOW,SHARED_COLOUR_LIGHT_GREEN};
			static int local_connectome_visualisation_neurons_colour_excitation_type_inhibitory[LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_DIRECTION_NUMBER_COLOURSETS] = {SHARED_COLOUR_MAGENTA,SHARED_COLOUR_LIGHT_RED};
			static int local_connectome_visualisation_neurons_colour_excitation_type_unknown[LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_DIRECTION_NUMBER_COLOURSETS] = {SHARED_COLOUR_WHITE,SHARED_COLOUR_WHITE};
		#endif
		#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_LDR_REFERENCE_TYPE (LDR_REFERENCE_TYPE_SUBREFERENCE)
		#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_LDR_MODEL_TRANSFORMATION_MATRIX "1.0 0.0 0.0 0.0 1.0 0.0 0.0 0.0 1.0"
		#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_LDR_MODEL_FILE_NAME "4-4CUBE.DAT"	//4-4SPHE.DAT
		#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_LDR_HEADER_TEXT "0 postSynapticNeuronLDRcontent"
		
		#ifdef INDEXED_CSV_DATABASE_H01
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITENEURON_LAYER1 (SHARED_COLOUR_BLUE)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITENEURON_LAYER2 (SHARED_COLOUR_RED)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITENEURON_LAYER3 (SHARED_COLOUR_YELLOW)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITENEURON_LAYER4 (SHARED_COLOUR_CYAN)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITENEURON_LAYER5 (SHARED_COLOUR_MAGENTA)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITENEURON_LAYER6 (SHARED_COLOUR_ORANGE)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITENEURON_LAYER7 (SHARED_COLOUR_GREEN)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_INHIBITNEURON_LAYER1 (SHARED_COLOUR_DARKBLUE)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_INHIBITNEURON_LAYER2 (SHARED_COLOUR_DARKRED)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_INHIBITNEURON_LAYER3 (SHARED_COLOUR_DARKYELLOW)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_INHIBITNEURON_LAYER4 (SHARED_COLOUR_AQUA)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_INHIBITNEURON_LAYER5 (SHARED_COLOUR_PURPLE)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_INHIBITNEURON_LAYER6 (SHARED_COLOUR_BROWN)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_INHIBITNEURON_LAYER7 (SHARED_COLOUR_DARKGREEN)
			/*
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_UNKNOWNNEURON_LAYER1 (SHARED_COLOUR_WHITE)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_UNKNOWNNEURON_LAYER2 (SHARED_COLOUR_WHITE)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_UNKNOWNNEURON_LAYER3 (SHARED_COLOUR_WHITE)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_UNKNOWNNEURON_LAYER4 (SHARED_COLOUR_WHITE)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_UNKNOWNNEURON_LAYER5 (SHARED_COLOUR_WHITE)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_UNKNOWNNEURON_LAYER6 (SHARED_COLOUR_WHITE
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_UNKNOWNNEURON_LAYER7 (SHARED_COLOUR_WHITE)
			*/
		#elif defined INDEXED_CSV_DATABASE_LDC
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITENEURON_LAYER1 (SHARED_COLOUR_BLUE)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITENEURON_LAYER2 (SHARED_COLOUR_RED)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITENEURON_LAYER3 (SHARED_COLOUR_YELLOW)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITENEURON_LAYER4 (SHARED_COLOUR_CYAN)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITENEURON_LAYER5 (SHARED_COLOUR_MAGENTA)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITENEURON_LAYER6 (SHARED_COLOUR_ORANGE)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_EXCITENEURON_LAYER7 (SHARED_COLOUR_GREEN)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_INHIBITNEURON_LAYER1 (SHARED_COLOUR_DARKBLUE)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_INHIBITNEURON_LAYER2 (SHARED_COLOUR_DARKRED)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_INHIBITNEURON_LAYER3 (SHARED_COLOUR_DARKYELLOW)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_INHIBITNEURON_LAYER4 (SHARED_COLOUR_AQUA)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_INHIBITNEURON_LAYER5 (SHARED_COLOUR_PURPLE)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_INHIBITNEURON_LAYER6 (SHARED_COLOUR_BROWN)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_INHIBITNEURON_LAYER7 (SHARED_COLOUR_DARKGREEN)
			/*
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_UNKNOWNNEURON_LAYER1 (SHARED_COLOUR_WHITE)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_UNKNOWNNEURON_LAYER2 (SHARED_COLOUR_WHITE)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_UNKNOWNNEURON_LAYER3 (SHARED_COLOUR_WHITE)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_UNKNOWNNEURON_LAYER4 (SHARED_COLOUR_WHITE)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_UNKNOWNNEURON_LAYER5 (SHARED_COLOUR_WHITE)
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_UNKNOWNNEURON_LAYER6 (SHARED_COLOUR_WHITE
			#define LOCAL_CONNECTOME_VISUALISATION_NEURONS_COLOUR_UNKNOWNNEURON_LAYER7 (SHARED_COLOUR_WHITE)
			*/		
		#endif
	#endif
	#ifdef INDEXED_CSV_DATABASE_CALCULATE_NEURON_LAYERS		
		#ifndef LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_LAYERS
			#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER (LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_FIRST+0)	//dynamically pregenerated once and saved for efficiency
				//INDEXED_CSV_DATABASE_LDC: neuron type (or cluster) id (0->CORTICAL_LAYER_NUMBER_OF_LAYERS-1)
		#endif
		#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL_NORMALISE_LOCAL_CONNECTIVITY
			//data storage not currently compatible with INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME execution; 
			#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER_SURFACE_NORM_X (LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_FIRST+1)
			#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER_SURFACE_NORM_Y (LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_FIRST+2)
		#endif
	#endif
	#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME
		#ifdef LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_LAYERS
			#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_FIRSTTRACE (LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_FIRST+0)
		#else
			#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_FIRSTTRACE (LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_FIRST+1)	//LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER
		#endif
		#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_TRACE (LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_FIRSTTRACE+0)	//dynamically pregenerated once and saved for efficiency
		#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_TRACE_HIGHLIGHT1 (LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_FIRSTTRACE+1)	//dynamically pregenerated once and saved for efficiency
		#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_TRACE_HIGHLIGHT2 (LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_FIRSTTRACE+2)	//dynamically pregenerated once and saved for efficiency
		//#ifdef INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION
		#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_ACTIVATION_LEVEL_POSITIVE (LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_FIRSTTRACE+3)	//dynamically pregenerated once and saved for efficiency
		#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_ACTIVATION_LEVEL_NEGATIVE (LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_FIRSTTRACE+4)	//dynamically pregenerated once and saved for efficiency
		//#endif		
		#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL_NORMALISE_LOCAL_CONNECTIVITY
			#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER_SURFACE_NORM_X (LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_FIRSTTRACE+5)	//overwrite
			#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER_SURFACE_NORM_Y (LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_FIRSTTRACE+6)	//overwrite
		#endif
		#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_VALUE_INACTIVE (0)
		#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_VALUE_SOURCE (1)
		#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_VALUE_TARGET (2)
		#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_HIGHLIGHT1_VALUE_INACTIVE (0)		
		#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_HIGHLIGHT1_VALUE_INHIBITORY (1)
		#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_HIGHLIGHT1_VALUE_EXCITATORY (2)
		#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_HIGHLIGHT2_VALUE_INACTIVE (0)		
		#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_HIGHLIGHT2_VALUE_INHIBITORY (1)
		#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_HIGHLIGHT2_VALUE_EXCITATORY (2)	
		static int local_connectome_visualisation_neurons_colour_trace_highlight1_value_inactive[LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_DIRECTION_NUMBER_COLOURSETS] = {SHARED_COLOUR_DARKGREY,SHARED_COLOUR_DARKGREY};	
		static int local_connectome_visualisation_neurons_colour_trace_highlight1_value_excitatory[LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_DIRECTION_NUMBER_COLOURSETS] = {SHARED_COLOUR_ORANGE,SHARED_COLOUR_LIGHT_RED};
		static int local_connectome_visualisation_neurons_colour_trace_highlight1_value_inhibitory[LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_DIRECTION_NUMBER_COLOURSETS] = {SHARED_COLOUR_DARKBLUE,SHARED_COLOUR_LIGHT_GREEN};
		static int local_connectome_visualisation_neurons_colour_trace_highlight2_value_inactive[LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_DIRECTION_NUMBER_COLOURSETS] = {SHARED_COLOUR_WHITE,SHARED_COLOUR_WHITE};	
		static int local_connectome_visualisation_neurons_colour_trace_highlight2_value_excitatory[LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_DIRECTION_NUMBER_COLOURSETS] = {SHARED_COLOUR_YELLOW,SHARED_COLOUR_YELLOW};	
		static int local_connectome_visualisation_neurons_colour_trace_highlight2_value_inhibitory[LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_DIRECTION_NUMBER_COLOURSETS] = {SHARED_COLOUR_BLUE,SHARED_COLOUR_BLUE};			
	#endif
		
		
	//visualisation coloursets (connections);
	#define LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_COLOURSET_REFERENCE_EXCITATION_TYPE_EXCITATORY "exciteConnection"
	#define LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_COLOURSET_REFERENCE_EXCITATION_TYPE_INHIBITORY "inhibitConnection"
	#ifdef LOCAL_CONNECTOME_VISUALISATION_LAYERS
		#define LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_COLOURSET_REFERENCE_LAYER "Layer"	//LayerX
	#endif
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_RADIALGRADIENT_TYPE_POSITIVE (0)    //radial gradient: SVG pre/post synaptic X coordinates drawn left to right
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_RADIALGRADIENT_TYPE_NEGATIVE (1)    //radial gradient: SVG pre/post synaptic X coordinates drawn right to left (requires colour gradient switch)
	#define LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_COLOURSET_REFERENCE_RADIALGRADIENT_TYPE_POSITIVE "RadialGradient2"
	#define LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_COLOURSET_REFERENCE_RADIALGRADIENT_TYPE_NEGATIVE "RadialGradient1"
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_FLOW_TYPE_POSITIVE (1)    //flow	INDEXED_CSV_DATABASE_H01: from layer 1->7	INDEXED_CSV_DATABASE_LDC: Y coordinates up to down
	#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_FLOW_TYPE_NEGATIVE (0)    //flow	INDEXED_CSV_DATABASE_H01: from layer 7->1	INDEXED_CSV_DATABASE_LDC: Y coordinates up to down
	#define LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_COLOURSET_REFERENCE_FLOW_TYPE_POSITIVE "FlowDirection1"
	#define LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_COLOURSET_REFERENCE_FLOW_TYPE_NEGATIVE "FlowDirection2"  
	#define LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_SVG_HEADER_TEXT "   	<!-- excitatory/inhibitory connections -->"
	#ifdef LOCAL_CONNECTOME_VISUALISATION_3D
		static int local_connectome_visualisation_connections_colour_excitation_type_unknown[LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_DIRECTION_NUMBER_COLOURSETS] = {SHARED_COLOUR_WHITE,SHARED_COLOUR_WHITE};
		static int local_connectome_visualisation_connections_colour_excitation_type_excitatory[LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_DIRECTION_NUMBER_COLOURSETS] = {SHARED_COLOUR_GREEN,SHARED_COLOUR_GREEN};
		static int local_connectome_visualisation_connections_colour_excitation_type_inhibitory[LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_DIRECTION_NUMBER_COLOURSETS] = {SHARED_COLOUR_RED,SHARED_COLOUR_RED};	
		#define LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_LDR_REFERENCE_TYPE (LDR_REFERENCE_TYPE_LINE)
		#define LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_COLOUR_EXCITATION_TYPE_EXCITATORY_FLOW_TYPE_POSITIVE (SHARED_COLOUR_GREEN)
		#define LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_COLOUR_EXCITATION_TYPE_INHIBITORY_FLOW_TYPE_POSITIVE (SHARED_COLOUR_RED)
		#define LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_COLOUR_EXCITATION_TYPE_EXCITATORY_FLOW_TYPE_NEGATIVE (SHARED_COLOUR_BLUE)
		#define LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_COLOUR_EXCITATION_TYPE_INHIBITORY_FLOW_TYPE_NEGATIVE (SHARED_COLOUR_MAGENTA)
		#define LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_LDR_HEADER_TEXT "0 connectionLDRcontent"
	#endif 
	#define LOCAL_CONNECTOME_VISUALISATION_CONNECTIONS_COLOURSET_REFERENCE_APPEND ""
	#ifdef LOCAL_CONNECTOME_VISUALISATION_LAYERS
		#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_ARTIFICIAL_LAYER (LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_ARTIFICIAL_FIRST+0)	//dynamically pregenerated once and saved for efficiency
		#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_ARTIFICIAL_LAYER (LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_ARTIFICIAL_FIRST+1)	//dynamically pregenerated once and saved for efficiency
	#endif
	#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME
		#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_ARTIFICIAL_TRACE (LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_ARTIFICIAL_FIRST+2)	//dynamically pregenerated once and saved for efficiency
		#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_ARTIFICIAL_TRACE_HIGHLIGHT1 (LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_ARTIFICIAL_FIRST+3)	//dynamically pregenerated once and saved for efficiency
		#define LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_ARTIFICIAL_TRACE_HIGHLIGHT2 (LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_ARTIFICIAL_FIRST+4)	//dynamically pregenerated once and saved for efficiency
		#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_CONNECTION_TRACE_VALUE_INACTIVE (0)
		#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_CONNECTION_TRACE_VALUE_ACTIVE (1)
		#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_CONNECTION_TRACE_HIGHLIGHT1_VALUE_INACTIVE (0)
		#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_CONNECTION_TRACE_HIGHLIGHT1_VALUE_INHIBITORY (1)
		#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_CONNECTION_TRACE_HIGHLIGHT1_VALUE_EXCITATORY (2)
		#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_CONNECTION_TRACE_HIGHLIGHT2_VALUE_INACTIVE (0)
		#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_CONNECTION_TRACE_HIGHLIGHT2_VALUE_INHIBITORY (1)
		#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_CONNECTION_TRACE_HIGHLIGHT2_VALUE_EXCITATORY (2)
		static int local_connectome_visualisation_connections_colour_trace_highlight1_value_inactive[LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_DIRECTION_NUMBER_COLOURSETS] = {SHARED_COLOUR_DARKGREY,SHARED_COLOUR_DARKGREY};
		static int local_connectome_visualisation_connections_colour_trace_highlight1_value_excitatory[LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_DIRECTION_NUMBER_COLOURSETS] = {SHARED_COLOUR_YELLOW,SHARED_COLOUR_GREEN};
		static int local_connectome_visualisation_connections_colour_trace_highlight1_value_inhibitory[LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_DIRECTION_NUMBER_COLOURSETS] = {SHARED_COLOUR_MAGENTA,SHARED_COLOUR_RED};
		static int local_connectome_visualisation_connections_colour_trace_highlight2_value_inactive[LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_DIRECTION_NUMBER_COLOURSETS] = {SHARED_COLOUR_WHITE,SHARED_COLOUR_WHITE};
		static int local_connectome_visualisation_connections_colour_trace_highlight2_value_excitatory[LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_DIRECTION_NUMBER_COLOURSETS] = {SHARED_COLOUR_YELLOW,SHARED_COLOUR_YELLOW};
		static int local_connectome_visualisation_connections_colour_trace_highlight2_value_inhibitory[LOCAL_CONNECTOME_VISUALISATION_LDR_FILENAME_DIRECTION_NUMBER_COLOURSETS] = {SHARED_COLOUR_MAGENTA,SHARED_COLOUR_MAGENTA};
	#endif
				
	#ifdef INDEXED_CSV_DATABASE_H01
		//calibration values extracted from dev/working/calibration/readme.txt - conversion from C3/CSV database to SVG/LDR visualisation:		
		#define LOCAL_CONNECTOME_VISUALISATION_CALIBRATION_FACTOR_X (0.00744)
		#define LOCAL_CONNECTOME_VISUALISATION_CALIBRATION_FACTOR_Y (0.00744)
		#ifdef LOCAL_CONNECTOME_VISUALISATION_BACKWARDS_COMPATIBILITY_WITH_ODS_GENERATED_FILES
			#define LOCAL_CONNECTOME_VISUALISATION_CALIBRATION_FACTOR_Z (0.00744)
		#else
			#define LOCAL_CONNECTOME_VISUALISATION_CALIBRATION_FACTOR_Z (0.00744*AVRO_JSON_DATABASE_COORDINATES_CALIBRATION_Z)	//requires calibration	//can be used to enhance Z axis visibility
		#endif
		#define LOCAL_CONNECTOME_VISUALISATION_CALIBRATION_MIN_X (576)	
		#define LOCAL_CONNECTOME_VISUALISATION_CALIBRATION_MIN_Y (1176.48)
		#define LOCAL_CONNECTOME_VISUALISATION_CALIBRATION_MIN_Z (0)
		#define LOCAL_CONNECTOME_VISUALISATION_CALIBRATION_DOUBLE_PRECISION (5)	//based on connections_I/E.svg precision
		//flow vectors;
		#define LOCAL_CONNECTOME_VISUALISATION_POSITIVE_FLOW_VECTOR_X (-(3471.18672 - 1925.47464))	//(L1->L6)
		#define LOCAL_CONNECTOME_VISUALISATION_POSITIVE_FLOW_VECTOR_Y (-(2069.5776 - 2837.22192))	//(L1->L6)	
	#elif defined INDEXED_CSV_DATABASE_LDC
		//calibration values extracted from dev/working/calibration/readme.txt - conversion from C3/CSV database to SVG/LDR visualisation:		
		#define LOCAL_CONNECTOME_VISUALISATION_CALIBRATION_FACTOR_X (0.01)
		#define LOCAL_CONNECTOME_VISUALISATION_CALIBRATION_FACTOR_Y (0.01)
		#define LOCAL_CONNECTOME_VISUALISATION_CALIBRATION_FACTOR_Z (0.01)
		#define LOCAL_CONNECTOME_VISUALISATION_CALIBRATION_MIN_X (0)	
		#define LOCAL_CONNECTOME_VISUALISATION_CALIBRATION_MIN_Y (0)
		#define LOCAL_CONNECTOME_VISUALISATION_CALIBRATION_MIN_Z (0)
		#define LOCAL_CONNECTOME_VISUALISATION_CALIBRATION_DOUBLE_PRECISION (5)	//based on connections_I/E.svg precision
		//flow vectors;
		#define LOCAL_CONNECTOME_VISUALISATION_POSITIVE_FLOW_VECTOR_X (0.0)	//Y -ve to +ve
		#define LOCAL_CONNECTOME_VISUALISATION_POSITIVE_FLOW_VECTOR_Y (1.0)	//Y -ve to +ve
	#endif
#endif

//mode INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME parameters:
#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME

	#define INDEXED_CSV_DATABASE_QUERY_EFFICIENT_STORE_NEURON_IDS_IN_MAP	//required
			
	#define INDEXED_CSV_DATABASE_TRACE_VISUALISATION_VIDEO
	#ifdef INDEXED_CSV_DATABASE_TRACE_VISUALISATION_VIDEO
		#define INDEXED_CSV_DATABASE_TRACE_VISUALISATION_VIDEO_CROP	//removes cortical layers border such that when output images are cropped border is not cut through
		/*
		note to convert output visualisations to mp4 eg;
		mogrify -format png connections_IE_flow1TraceIndex*.svg
		mogrify -crop 3840x2160+1400+2030 *.png
		ffmpeg -framerate 1/2 -i connections_IE_flow1TraceIndex%03d.png -c:v libx264 -r 30 -pix_fmt yuv420p connections_IE_flow1Trace.mp4
		*/
	#endif
	
	#ifdef INDEXED_CSV_DATABASE_H01
		#define INDEXED_CSV_DATABASE_TRACE_START_LAYER_INDEX (4)	//cortex entry layer index; primary incoming connections from thalamus/cortex (see H01 Release publication, section "Synaptic Connectivity")
	#elif defined INDEXED_CSV_DATABASE_LDC
		#define LDC_INPUT_LAYER_INDEX (0)	//CHECKTHIS
		#define INDEXED_CSV_DATABASE_TRACE_START_LAYER_INDEX (LDC_INPUT_LAYER_INDEX)
	#endif
	
	//#define INDEXED_CSV_DATABASE_TRACE_START_LAYER_SUBSET_RANDOM	//take a random subset of the start activation neurons
	#ifdef INDEXED_CSV_DATABASE_TRACE_START_LAYER_SUBSET_RANDOM
		#define INDEXED_CSV_DATABASE_TRACE_START_LAYER_SUBSET_RANDOM_FRACTION (0.3)
	#endif
	
	#define INDEXED_CSV_DATABASE_TRACE_INDEX_PREPEND "TraceIndex"
	
	//#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_EMULATE_PUBLICATION_INFORMATION_FLOW_THROUGH_THE_H01_NETWORK	//optional (see H01 Release publication, video "Information flow through the h01 network")
	#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_EMULATE_PUBLICATION_INFORMATION_FLOW_THROUGH_THE_H01_NETWORK
		#define INDEXED_CSV_DATABASE_TRACE_START_LAYER_EXCITATORY_NEURONS	//only activate excitatory neurons on start layer
		#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_EMULATE_PUBLICATION_INFORMATION_FLOW_THROUGH_THE_H01_NETWORK_DIRECTION_COLOURSET_INDEX (2) //use colourset direction2
		#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_MAX_NUMBER_ITERATIONS (20)	//2
		#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_SUB	//perform visualisation subiteration (draw/marks/highlights of neurons/connection)
		#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_SUB
			#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_SUB_CONNECTION_SOURCES	//additional visualisation stage useful for clarity (only first iteration drawn in publication)
			#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_SUB_CONNECTION
			//#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_SUB_CONNECTION_TARGETS_OF_INHIBITORY_CONNECTIONS	//depreciated (use INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_HIGHLIGHT2_INHIBITED_NEURONS instead)
			#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_SUB_CONNECTION_TARGETS
		#endif
		#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_HIDE_INACTIVE_NEURONS
		#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_HIDE_INACTIVE_CONNECTIONS
		//#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_COLOUR_HIGHLIGHT1_ACTIVE_NEURONS	//optional
		//#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_COLOUR_HIGHLIGHT1_INACTIVE_NEURONS	//optional
		//#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_COLOUR_HIGHLIGHT1_ACTIVE_CONNECTIONS	//optional
		//#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_COLOUR_HIGHLIGHT1_INACTIVE_CONNECTIONS	//optional
		#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_COLOUR_HIGHLIGHT2_ACTIVE_NEURONS
		//#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_COLOUR_HIGHLIGHT2_INACTIVE_NEURONS	//optional
		//#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_COLOUR_HIGHLIGHT2_ACTIVE_CONNECTIONS	//optional
		//#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_COLOUR_HIGHLIGHT2_INACTIVE_CONNECTIONS	//optional
		#define INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION	//adds positive and negative presynaptic inputs to determine whether to fire neuron (see H01 Release publication, section "Analysis of information flow through network") 
		#ifdef INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION
			#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_TRACE_INHIBITORY_CONNECTIONS
			#define INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_ACTIVATION_LEVEL_RESOLUTION (1)	//add/subtract 1 to each neuron input
			#define INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_ACTIVATION_LEVEL_THRESHOLD_CRITERIA (0)	//required because inhibitory neurons contribute
			#define INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_ACTIVATION_LEVEL_INITIAL (0)
			#define INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_DRAW_CONNECTIONS_IF_SOURCE_ACTIVATED	//always draw connections from source even if target is never activated
			//#define INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_DRAW_CONNECTIONS_IF_TARGET_ACTIVATED
			#ifdef INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_DRAW_CONNECTIONS_IF_SOURCE_ACTIVATED
				#define INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_HIGHLIGHT2_INHIBITED_NEURONS	//will draw/colour targets based on activation level; green/red: sufficiently excited, blue: inhibited - temporarily draw all inhibited target neurons before shifting to next frame
				//#define INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_HIGHLIGHT2_EXCITED_NEURONS	//note existing limitation of INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_EMULATE_PUBLICATION_INFORMATION_FLOW_THROUGH_THE_H01_NETWORK:INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION - does not temporarily draw all partially excited target neurons before shifting to next frame
			#endif
		#endif
	#else
		//#define INDEXED_CSV_DATABASE_TRACE_START_LAYER_EXCITATORY_NEURONS
		//use any activated colourset
		#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_MAX_NUMBER_ITERATIONS (20)	//10
		//#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_SUB	//perform visualisation subiteration (draw/marks/highlights of neurons/connection)
		#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_SUB
		#endif
		#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_HIDE_INACTIVE_NEURONS	//optional
		#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_HIDE_INACTIVE_CONNECTIONS	//optional
		//#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_COLOUR_HIGHLIGHT1_ACTIVE_NEURONS	//optional
		//#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_COLOUR_HIGHLIGHT1_INACTIVE_NEURONS	//optional
		//#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_COLOUR_HIGHLIGHT1_ACTIVE_CONNECTIONS	//optional
		//#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_COLOUR_HIGHLIGHT1_INACTIVE_CONNECTIONS	//optional
		//#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_COLOUR_HIGHLIGHT2_ACTIVE_NEURONS	//optional
		//#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_COLOUR_HIGHLIGHT2_INACTIVE_NEURONS	//optional
		//#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_COLOUR_HIGHLIGHT2_ACTIVE_CONNECTIONS	//optional
		//#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_COLOUR_HIGHLIGHT2_INACTIVE_CONNECTIONS	//optional
		#define INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION	//optional
		#ifdef INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION
			//#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_TRACE_INHIBITORY_CONNECTIONS
			#define INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_ACTIVATION_LEVEL_RESOLUTION (1)	//add/subtract 1 to each neuron input
			#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_TRACE_INHIBITORY_CONNECTIONS
				#define INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_ACTIVATION_LEVEL_THRESHOLD_CRITERIA (0)	//required because inhibitory neurons contribute
			#else
				#define INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_ACTIVATION_LEVEL_THRESHOLD_CRITERIA (0)	//default: (0) - equivalent to !INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION because inhibitory neurons do not contribute
			#endif
			#define INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_ACTIVATION_LEVEL_INITIAL (0)
			//#define INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_DRAW_CONNECTIONS_IF_SOURCE_ACTIVATED
			#define INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_DRAW_CONNECTIONS_IF_TARGET_ACTIVATED	
			#ifdef INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_DRAW_CONNECTIONS_IF_SOURCE_ACTIVATED
				//#define INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_HIGHLIGHT2_INHIBITED_NEURONS
				//#define INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_HIGHLIGHT2_EXCITED_NEURONS
			#endif	
		#endif
	#endif	
	#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_TRACE_INHIBITORY_CONNECTIONS
		#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_DRAW_INHIBITORY_NEURONS
		#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_DRAW_INHIBITORY_CONNECTIONS		
	#else
		//#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_DRAW_INHIBITORY_NEURONS	//optional
		//#define INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_DRAW_INHIBITORY_CONNECTIONS	//optional
	#endif	
#endif

#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_PRESYNAPTIC_NEURONS
	#define INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_PRESYNAPTIC_NEURONS_LOOKUP	//mandatory for INDEXED_CSV_DATABASE_LDC //lookup presynaptic excitation type from neuron dataset (rather than recalculating from neuron type)
#endif

#ifdef INDEXED_CSV_DATABASE_H01
	#ifdef INDEXED_CSV_DATABASE_CALCULATE_NEURON_LAYERS
		#define CORTICAL_LAYER_KEYPOINT_UNAVAILABLE_VALUE 0
		#define CORTICAL_LAYER_NUMBER_OF_LAYERS (7)	//L2toL1 (1), L3toL2 (2), L4toL3 (3), L5toL4 (4), L6toL5 (5), WMtoL6 (6), WM (7)
		#define CORTICAL_LAYER_UNKNOWN (0)
		#ifdef LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_LAYERS
			#define LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_LAYERS_STRING
			#ifdef LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_LAYERS_STRING
				static string somasLayerNames[CORTICAL_LAYER_NUMBER_OF_LAYERS] = {"Layer 1", "Layer 2", "Layer 3", "Layer 4", "Layer 5", "Layer 6", "White matter"};	//"unclassified", "NULL"	
			#endif
		#endif
		#define CORTICAL_LAYER_BOUNDARY_KEYPOINT_TABLE_FILE_NAME "corticalLayersBoundaryKeypoints.csv"	//this dataset has already had its X/Y coordindates calibrated for visualisation (as it was derived from connections_I/E.svg visualisation; see working/calibration/connections_IE-FlattenBezier.svg)

		#define LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL (0)
	#endif
#elif defined INDEXED_CSV_DATABASE_LDC
	#ifdef INDEXED_CSV_DATABASE_CALCULATE_NEURON_LAYERS
		#define CORTICAL_LAYER_KEYPOINT_UNAVAILABLE_VALUE 0
		#ifdef INDEXED_CSV_DATABASE_LDC_NEURON_LAYERS_REUSE_H01_TEMPLATES
			#define CORTICAL_LAYER_NUMBER_OF_LAYERS (7)
		#else
			#ifdef INDEXED_CSV_DATABASE_LDC_NEURON_TYPES_REFORMAT
				#define CORTICAL_LAYER_NUMBER_OF_LAYERS (18)
			#else
				#define CORTICAL_LAYER_NUMBER_OF_LAYERS (25)	//number of neuronTypes + LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_TYPE_UNKNOWN //(91)	//OLD: Connectivity-based clustering reveals 90 distinct types of brain neurons
			#endif
		#endif
		#define CORTICAL_LAYER_UNKNOWN (0)
		#define CORTICAL_LAYER_BOUNDARY_KEYPOINT_TABLE_FILE_NAME "corticalLayersBoundaryKeypoints.csv"	//this dataset has already had its X/Y coordindates calibrated for visualisation (as it was derived from connections_I/E.svg visualisation; see working/calibration/connections_IE-FlattenBezier.svg)

		#define LOCAL_CONNECTOME_LAYERS_LAYER_INDEX_ALL (0)
	#endif
#endif
			

#define CPP_STRING_FIND_RESULT_FAIL_VALUE2 int(CPP_STRING_FIND_RESULT_FAIL_VALUE)

#define WRITE_FILE_TRUE (true)
#define WRITE_FILE_FALSE (false)
#define READ_FILE_TRUE (true)
#define READ_FILE_FALSE (false)
#define APPEND_FILE_TRUE (true)
#define APPEND_FILE_FALSE (false)
#define CONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS (true)
#define CONNECTION_TYPES_DERIVED_FROM_EM_IMAGES (false)
#define QUERY_PRESYNAPTIC_CONNECTION_NEURONS (true)
#define QUERY_POSTSYNAPTIC_CONNECTION_NEURONS (false)
#define LAYER_ENFORCEMENT_TRUE (true)
#define LAYER_ENFORCEMENT_FALSE (false)

#endif
