/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabase.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: H01LocalConnectome
 * Requirements: BAI SHARED C++ library, Eigen 3 C++ library
 * Compilation: ./compileH01indexedCSVdatabase.sh
 * Usage: ./H01indexedCSVdatabase.exe
 * Description: H01 indexed CSV database
 * Comments:
 * /
 *******************************************************************************/

#include "SHAREDglobalDefs.hpp"


//compilation modes:
//#define INDEXED_CSV_DATABASE_CREATE
#define INDEXED_CSV_DATABASE_QUERY

#ifdef INDEXED_CSV_DATABASE_CREATE
	#define DEBUG_CODE	//disable this DEBUG parameter to create the database (enabled for safety - prevents overwrite of indexed database; takes ~6 hours to regenerate)
	#define AVRO_JSON_DATABASE_FOLDER "/media/user/large/h01data/data/exported/json"
#endif

#ifdef INDEXED_CSV_DATABASE_QUERY
	#define INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING
	#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING
		//input:
		#define INDEXED_CSV_DATABASE_QUERY_NEURON_LIST_DISTINCT_FILE_NAME "localConnectomeNeuronIDlistDistinct.csv"	//from in_body_cell_connection.csv - pre/postsynaptic distinct neuron ids		
		
		#define INDEXED_CSV_DATABASE_ALGORITHMS
		#define INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_MIN_NUM_POINTS_REQUIRED (3)	//(2)
		#define INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_NON_LOCAL_NEURONS_REQUIRED
		#define INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_EST_TYPE_BY_AVERAGE_SYNAPSE_TYPE
		
		#define INDEXED_CSV_DATABASE_QUERY_GET_LOCATION	//used to perform incoming long range axonal tract position/direction estimation (not just statistical comparison of distal vs local input)
		#define INDEXED_CSV_DATABASE_QUERY_OUTPUT	//performance speed by disabling primary output file write
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
		
		//output:
		#ifdef INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING_LDR
			#define INDEXED_CSV_DATABASE_QUERY_NEURON_LIST_DISTINCT_CONNECTIONS_PRESYNAPTIC_FILE_NAME "localConnectomeIncomingAxonMapping.ldr"		
		#else
			#define INDEXED_CSV_DATABASE_QUERY_NEURON_LIST_DISTINCT_CONNECTIONS_PRESYNAPTIC_FILE_NAME "localConnectomeIncomingAxonMapping.csv"	//"localConnectomeNeuronIDdistinctListconnectionsPresynaptic.csv"	
		#endif
	#else
		#define INDEXED_CSV_DATABASE_QUERY_OUTPUT
		#ifdef INDEXED_CSV_DATABASE_QUERY_OUTPUT
			#define INDEXED_CSV_DATABASE_QUERY_OUTPUT_CONNECTIONS
		#endif
		//input:
		#define INDEXED_CSV_DATABASE_QUERY_NEURON_LIST1_FILE_NAME "localConnectomeNeuronIDlist1.csv"	//from in_body_cell_connection.csv - presynaptic neuron ids
		#define INDEXED_CSV_DATABASE_QUERY_NEURON_LIST2_FILE_NAME "localConnectomeNeuronIDlist2.csv"	//from in_body_cell_connection.csv - postsynaptic neuron ids
		//output:
		#define INDEXED_CSV_DATABASE_QUERY_NEURON_LIST1_CONNECTIONS_PRESYNAPTIC_FILE_NAME "localConnectomeNeuronIDlist1connectionsPresynaptic.csv"
		#define INDEXED_CSV_DATABASE_QUERY_NEURON_LIST1_CONNECTIONS_POSTSYNAPTIC_FILE_NAME "localConnectomeNeuronIDlist1connectionsPostsynaptic.csv"
		#define INDEXED_CSV_DATABASE_QUERY_NEURON_LIST2_CONNECTIONS_PRESYNAPTIC_FILE_NAME "localConnectomeNeuronIDlist2connectionsPresynaptic.csv"
		#define INDEXED_CSV_DATABASE_QUERY_NEURON_LIST2_CONNECTIONS_POSTSYNAPTIC_FILE_NAME "localConnectomeNeuronIDlist2connectionsPostsynaptic.csv"
	#endif
	
	#define INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_CURRENT_FOLDER
	#ifndef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_CURRENT_FOLDER
		#define INDEXED_CSV_DATABASE_QUERY_INPUT_FOLDER "/media/user/large/source/h01Connectome/indexedSVGdatabase/H01indexedCSVdatabaseQueryOutput"
		#define INDEXED_CSV_DATABASE_QUERY_OUTPUT_FOLDER "/media/user/large/source/h01Connectome/indexedSVGdatabase/H01indexedCSVdatabaseQueryOutput"
	#endif
	#define INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_FILE_OBJECT	//only use this if output folder exists on a different harddrive (not just partition) to indexed CSV database folder
	//max expected file/string size = indexed csv database query output size (neuron_id ~11 bytes + , + type ~1 byte = ~13 bytes) * avg num connections per neuron ID (~2000) * number of neuron IDs in list (~30000) = 13*2000*30000 = ~1GB of RAM if string	
#endif

#ifdef DEBUG_CODE
	#define INDEXED_CSV_DATABASE_FOLDER "/media/user/large/source/h01Connectome/indexedSVGdatabase/trial"
#else
	#define INDEXED_CSV_DATABASE_FOLDER "/media/user/ssddata/indexed"
#endif

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

#ifdef DEBUG_CODE
	#define AVRO_JSON_DATABASE_FILEINDEX_START (0)
	#define AVRO_JSON_DATABASE_FILEINDEX_END (0)
#endif
//#define DEBUG_RESTART
#ifdef DEBUG_RESTART
	#define AVRO_JSON_DATABASE_FILEINDEX_START (0)
	#define AVRO_JSON_DATABASE_FILEINDEX_END (0)
	#define DEBUG_RESTART_LINEINDEX_START (620306) //614124
#endif

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

#define JSON_FILE_EXTENSION ".json"
#define CSV_FILE_EXTENSION  ".csv"
#define LDR_FILE_EXTENSION  ".ldr"
#define CSV_DELIMITER ","
#define LDR_DELIMITER " "

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
#define INDEXED_CSV_DATABASE_NUMBER_FIELDS (12)

#define CPP_STRING_FIND_RESULT_FAIL_VALUE2 int(CPP_STRING_FIND_RESULT_FAIL_VALUE)

#define AVRO_JSON_DATABASE_EXCITATORY_SYNAPSE_TYPE (2)
#define AVRO_JSON_DATABASE_INHIBITORY_SYNAPSE_TYPE (1)
#define AVRO_JSON_DATABASE_COORDINATES_RANGE_X (473357-107342)	//366015
#define AVRO_JSON_DATABASE_COORDINATES_RANGE_Y (282046-45528)	//236518
#define AVRO_JSON_DATABASE_COORDINATES_RANGE_Z (5248-0)	//5248
#define LDR_TYPE_LINE (2)

int main();


