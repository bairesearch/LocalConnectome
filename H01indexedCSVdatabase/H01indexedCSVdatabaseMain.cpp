/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseMain.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021-2023 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: H01LocalConnectome
 * Requirements: see H01indexedCSVdatabaseGlobalDefs.hpp
 * Compilation: see H01indexedCSVdatabaseGlobalDefs.hpp
 * Usage: see H01indexedCSVdatabaseGlobalDefs.hpp
 * Description: H01 indexed CSV database Main (INDEXED_CSV_DATABASE_CREATE/INDEXED_CSV_DATABASE_QUERY/INDEXED_CSV_DATABASE_VISUALISE_LOCAL_CONNECTOME/INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME)
 * Comments:
 * /
 *******************************************************************************/

#include "H01indexedCSVdatabaseMain.hpp"
#ifdef INDEXED_CSV_DATABASE_CREATE
#include "H01indexedCSVdatabaseCreate.hpp"
#endif
#ifdef INDEXED_CSV_DATABASE_QUERY
#include "H01indexedCSVdatabaseQuery.hpp"
#endif
#ifdef INDEXED_CSV_DATABASE_VISUALISE_LOCAL_CONNECTOME
#include "H01indexedCSVdatabaseVisualiseLocalConnectome.hpp"
#endif
#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME
#include "H01indexedCSVdatabaseTraceLocalConnectome.hpp"
#endif
#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME
#include "H01indexedCSVdatabaseReadLocalConnectome.hpp"
#endif
#ifdef INDEXED_CSV_DATABASE_PREPROCESS
#include "H01indexedCSVdatabasePreprocess.hpp"
#endif
#include "SHAREDvars.hpp"



#ifdef INDEXED_CSV_DATABASE_H01
	static string executionModesArray[EXECUTION_MODES_TOTAL] = {"create:INDEXED_CSV_DATABASE_CREATE","query:INDEXED_CSV_DATABASE_QUERY","visualise:INDEXED_CSV_DATABASE_VISUALISE_LOCAL_CONNECTOME","trace:INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME","read:INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME"};
	static string queryModesArray[QUERY_MODES_TOTAL] = {"extract:INDEXED_CSV_DATABASE_QUERY_EXTRACT_INCOMING_OUTGOING_CONNECTIONS","map:INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING","generate:INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET","count:INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS","complete:INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET","crawl:INDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS"};
	static string readModesArray[READ_MODES_TOTAL] = {"count:READ_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS","extrapolate:READ_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL"};

	static char errmessage[] = "Usage:  H01indexedCSVdatabase.exe [options]"
	"\n"
	"\n\twhere options are any of the following (see documentation)"
	"\n"
	"\n\t-mode [int]                             : execution mode (1: create, 2: query, 3: visualise, 4: trace, 5: read (def: 2) [required]"
	"\n\t-query [int]                            : query mode (1: extract, 2: map, 3: generate, 4: count, 5: complete, 6: crawl, 7:extrapolate (def: 4) [required for mode:query]"
	"\n\t-read [int]                             : read mode (1: count, 2: extrapolate (def: 1) [required for mode:read]"
	"\n"
	"\n\t-avro_json_database_folder [string]     : H01 C3 Synaptic connections database json folder (def: /media/user/large/h01data/data/exported/json)"
	"\n\t-indexed_csv_database_folder [string]   : H01 indexed csv database folder (def: /media/user/ssddata/indexed)"
	"\n\t-local_connectome_folder_base [string]  : H01 local connectome base folder containing \"datasets\" and \"visualisations\" (def: ../)"
	"\n"
	"\n";

	static char infomessage[] = "Information regarding execution/query modes:"
	"\n"
	"\nexecution mode 1 - INDEXED_CSV_DATABASE_CREATE - converts H01 Avro Json C3 Synaptic connections database to indexed CSV database (indexed by pre/postsynaptic neuron ID)"
	"\nexecution mode 2 - INDEXED_CSV_DATABASE_QUERY - queries H01 indexed CSV database, based on local connectome neuron id list"
	"\nexecution mode 3 - INDEXED_CSV_DATABASE_VISUALISE_LOCAL_CONNECTOME - visualises local connectome datasets"
	"\nexecution mode 4 - INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME - traces local connectome dataset (saving visualisation)"
	"\nexecution mode 5 - INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME - read local connectome dataset"
	"\n"
	"\nquery mode 1 - INDEXED_CSV_DATABASE_QUERY_EXTRACT_INCOMING_OUTGOING_CONNECTIONS - lookup indexed CSV database by neuron ID, and find incoming/outgoing target connections, and write them to file"
	"\nquery mode 2 - INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING - lookup indexed CSV database by neuron ID, find incoming target connections, and generate visualisation"
	"\nquery mode 3 - INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET - automatically generate localConnectomeConnections-typesFromPresynapticNeurons.csv/localConnectomeConnections-typesFromEMimages.csv from localConnectomeNeurons.csv and indexed CSV database"
	"\nquery mode 4 - INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS - lookup indexed CSV database by neuron ID, count/infer proportion of incoming/outgoing excitatory/inhibitory target connections to local vs distal neurons"
	"\nquery mode 5 - INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET - lookup indexed CSV database by post/pre synaptic connection neuron ID, and identify connection with pre/post synaptic X/Y coordinates (if pre/post synaptic type=UNKNOWN), add pre/post synaptic neuron ID, Z coordinates, and type coordinates to connection dataset [incomplete]"
	"\nquery mode 6 - INDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS - crawl indexed CSV database by pre/post synaptic connection neuron ID, and count number of unique axons/dendrites as specified by neuron ID - not explicitly connected to local connectome [incomplete]"
	"\n"
	"\nread mode 1 - INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS - count excitatory/inhibitory connections"
	"\nread mode 2 - INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL - generate large artificial cortical model; extrapolate z region same size as x/y [incomplete]"
	"\n"
	"\nexecution mode 1 - INDEXED_CSV_DATABASE_CREATE - converts Avro Json C3 Synaptic connections database to indexed CSV database, indexed by pre/postsynaptic neuron ID"
	"\n * Input: C3 Synaptic connections database (gs://h01-release/data/20210601/c3/synapses/exported/json)"
	"\n * Output Format: ssddata/indexed/123/csvPreSynapticNeuronID123456.csv - presynapticSiteNeuronID, postsynapticSiteNeuronID, presynapticSiteType, postsynapticSiteType, presynapticSiteClassLabel, postsynapticSiteClassLabel, presynapticSiteBaseNeuronID, postsynapticSiteBaseNeuronID, synapseLocationXcoordinates, synapseLocationYcoordinates, synapseLocationZcoordinates, synapseType"
	"\n"
	"\nexecution mode 2 - INDEXED_CSV_DATABASE_QUERY - queries indexed CSV database, based on local connectome neuron id list"
	"\n * Input Format: "
	"\n *   1 INDEXED_CSV_DATABASE_QUERY_OUTPUT_CONNECTIONS: localConnectomeNeurons.csv - id, x, y, z, type, excitation_type | somas.csv - soma_id, base_seg_id, c2_rep_strict, c2_rep_manual, c3_rep_strict, c3_rep_manual, proofread_104_rep, x, y, z, celltype, layer | localConnectomeNeuronIDlistDistinct.csv - id"
	"\n *   2 INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING: localConnectomeNeurons.csv - id, x, y, z, type, excitation_type | somas.csv - soma_id, base_seg_id, c2_rep_strict, c2_rep_manual, c3_rep_strict, c3_rep_manual, proofread_104_rep, x, y, z, celltype, layer | localConnectomeNeuronIDlistDistinct.csv - id"
	"\n *   3 INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: localConnectomeNeurons.csv - id, x, y, z, type, excitation_type | somas.csv - soma_id, base_seg_id, c2_rep_strict, c2_rep_manual, c3_rep_strict, c3_rep_manual, proofread_104_rep, x, y, z, celltype, layer | localConnectomeNeuronIDlistDistinct.csv - id"
	"\n *   4 INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS: localConnectomeNeurons.csv - id, x, y, z, type, excitation_type | somas.csv - soma_id, base_seg_id, c2_rep_strict, c2_rep_manual, c3_rep_strict, c3_rep_manual, proofread_104_rep, x, y, z, celltype, layer | localConnectomeNeuronIDlistDistinct.csv - id"
	"\n *   5 INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: localConnectomeConnections-typesFromEMimages-useAllValuesAvailableFromInBodyCellConnection.csv - pre_id, pre_x, pre_y, pre_z, pre_type, post_id, post_x, post_y, post_z, post_type, post_class_label, syn_num, excitation_type"
	"\n *   6 INDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS: N/A"
	"\n * Output:"
	"\n *   1 INDEXED_CSV_DATABASE_QUERY_OUTPUT_CONNECTIONS: localConnectomeNeuronIDlistConnectionsPresynaptic.csv/localConnectomeNeuronIDlistConnectionsPostsynaptic.csv - connectionNeuronID1, connectionType1 [, locationObjectContentsXcoordinatesContent1, locationObjectContentsYcoordinatesContent1, locationObjectContentsZcoordinatesContent1], connectionNeuronID2, connectionType2 [, locationObjectContentsXcoordinatesContent2, locationObjectContentsYcoordinatesContent2, locationObjectContentsZcoordinatesContent2], etc "
	"\n *   2 INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING:"
	"\n *       INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_3D_LINEAR_REGRESSION:"
	"\n *           INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING_LDR: localConnectomeIncomingAxonMapping.ldr - LDR_REFERENCE_TYPE_LINE ldrawColor plot3DpointStart.x plot3DpointStart.y plot3DpointStart.z plot3DpointEnd.x plot3DpointEnd.y plot3DpointEnd.z"
	"\n *           INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING_CSV: localConnectomeIncomingAxonMapping.csv - polyFit.connectionNeuronID, polyFit.estSynapseType, polyFit.origin.x, polyFit.origin.y, polyFit.origin.z, polyFit.axis.x, polyFit.axis.y, polyFit.axis.z"
	"\n *       INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_2D_POLY_REGRESSION:"
	"\n *           INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING_CSV: localConnectomeIncomingAxonMapping.csv - polyFit.connectionNeuronID, polyFit.estSynapseType, polyFit.a, polyFit.b, polyFit.c"
	"\n *   3 INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: localConnectomeConnections-typesFromPresynapticNeurons/typesFromEMimages.csv - pre_id, pre_x, pre_y, pre_z, pre_type, post_id, post_x, post_y, post_z, post_type, post_class_label, syn_num, excitation_type"
	"\n *   4 INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS: N/A"
	"\n *   5 INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: localConnectomeConnections-typesFromEMimages.csv - pre_id, pre_x, pre_y, pre_z, pre_type, post_id, post_x, post_y, post_z, post_type, post_class_label, syn_num, excitation_type"
	"\n *   6 INDEXED_CSV_DATABASE_QUERY_CRAWL_CONNECTIONS: N/A"
	"\n"
	"\nexecution mode 3 - INDEXED_CSV_DATABASE_VISUALISE_LOCAL_CONNECTOME - visualises local connectome datasets"
	"\n * Input: localConnectomeNeurons.csv / localConnectomeConnectionsX.csv"
	"\n * Output Format: SVG (2D) / LDR (3D)"
	"\n"
	"\nexecution mode 4 - INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME - traces local connectome dataset (saving visualisation)"
	"\n * Input: localConnectomeNeurons.csv / localConnectomeConnectionsX.csv"
	"\n * Output Format: SVG (2D) / LDR (3D)"
	"\n"
	"\nexecution mode 5 - INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME - read local connectome dataset"
	"\n * Input: "
	"\n *   1 INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS: localConnectomeNeurons.csv - id, x, y, z, type, excitation_type | somas.csv - soma_id, base_seg_id, c2_rep_strict, c2_rep_manual, c3_rep_strict, c3_rep_manual, proofread_104_rep, x, y, z, celltype, layer, localConnectomeConnections-typesFromPresynapticNeurons/typesFromEMimages.csv - pre_id, pre_x, pre_y, pre_z, pre_type, post_id, post_x, post_y, post_z, post_type, post_class_label, syn_num, excitation_type"
	"\n *   2 INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL: localConnectomeNeurons.csv - id, x, y, z, type, excitation_type | somas.csv - soma_id, base_seg_id, c2_rep_strict, c2_rep_manual, c3_rep_strict, c3_rep_manual, proofread_104_rep, x, y, z, celltype, layer, localConnectomeConnections-typesFromPresynapticNeurons/typesFromEMimages.csv - pre_id, pre_x, pre_y, pre_z, pre_type, post_id, post_x, post_y, post_z, post_type, post_class_label, syn_num, excitation_type"
	"\n * Output:"
	"\n *   1 INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS: N/A"
	"\n *   2 INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL: localConnectomeConnectionsLargeModel-typesFromPresynapticNeurons/typesFromEMimages.csv - pre_id, pre_x, pre_y, pre_z, pre_type, post_id, post_x, post_y, post_z, post_type, post_class_label, syn_num, excitation_type; localConnectomeNeuronsLargeModel.csv - id, x, y, z, type, excitation_type"
	"\n";
#elif defined INDEXED_CSV_DATABASE_LDC
	static string executionModesArray[EXECUTION_MODES_TOTAL] = {"visualise:INDEXED_CSV_DATABASE_VISUALISE_LOCAL_CONNECTOME","trace:INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME","read:INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME"};
	static string readModesArray[READ_MODES_TOTAL] = {"count:READ_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS"};
	static string preprocessModesArray[PREPROCESS_MODES_TOTAL] = {"connections:PREPROCESS_MODE_INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_FROM_MATRIX","neurons:PREPROCESS_MODE_INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_NEURONS_DATASET_FROM_SKELETONS"};

	static char errmessage[] = "Usage:  H01indexedCSVdatabase.exe [options]"
	"\n"
	"\n\twhere options are any of the following (see documentation)"
	"\n"
	"\n\t-mode [int]                             : execution mode (3: visualise, 4: trace, 5: read, 6: preprocess  (def: 5) [required]"
	"\n\t-read [int]                             : read mode (1: count (def: 1) [required for mode:read]"
	"\n\t-preprocess [int]                       : preprocess mode (1: connections, 2: neurons (def: 1) [required for mode:preprocess]"
	"\n"
	"\n\t-local_connectome_folder_base [string]  : H01 local connectome base folder containing \"datasets\" and \"visualisations\" (def: ../)"
	"\n"
	"\n";

	static char infomessage[] = "Information regarding execution/query modes:"
	"\n"
	"\nexecution mode 3 - INDEXED_CSV_DATABASE_VISUALISE_LOCAL_CONNECTOME - visualises local connectome datasets"
	"\nexecution mode 4 - INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME - traces local connectome dataset (saving visualisation)"
	"\nexecution mode 5 - INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME - read local connectome dataset"
	"\nexecution mode 6 - INDEXED_CSV_DATABASE_PREPROCESS - preprocess LDC dataset"
	"\n"
	"\nread mode 1 - INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS - count excitatory/inhibitory connections"
	"\n"
	"\npreprocess mode 1 - INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_FROM_MATRIX - automatically generate localConnectomeConnections-typesFromPresynapticNeurons.csv from Supplementary Material connections matrix"
	"\npreprocess mode 2 - INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_NEURONS_DATASET_FROM_SKELETONS - automatically generate localConnectomeNeurons.csv from Catmaid skeletons and Supplementary Material files"
	"\n"
	"\nexecution mode 3 - INDEXED_CSV_DATABASE_VISUALISE_LOCAL_CONNECTOME - visualises local connectome datasets"
	"\n * Input: localConnectomeNeurons.csv / localConnectomeConnectionsX.csv"
	"\n * Output Format: SVG (2D) / LDR (3D)"
	"\n"
	"\nexecution mode 4 - INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME - traces local connectome dataset (saving visualisation)"
	"\n * Input: localConnectomeNeurons.csv / localConnectomeConnectionsX.csv"
	"\n * Output Format: SVG (2D) / LDR (3D)"
	"\n"
	"\nexecution mode 5 - INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME - read local connectome dataset"
	"\n * Input: "
	"\n *   1 INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS: localConnectomeNeurons.csv - id, x, y, z, type, excitation_type | somas.csv - soma_id, base_seg_id, c2_rep_strict, c2_rep_manual, c3_rep_strict, c3_rep_manual, proofread_104_rep, x, y, z, celltype, layer, localConnectomeConnections-typesFromPresynapticNeurons/typesFromEMimages.csv - pre_id, pre_x, pre_y, pre_z, pre_type, post_id, post_x, post_y, post_z, post_type, post_class_label, syn_num, excitation_type"
	"\n * Output:"
	"\n *   1 INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS: N/A"
	"\n"
	"\nexecution mode 6 - INDEXED_CSV_DATABASE_PREPROCESS - preprocess datasets"
	"\n * Input: "
	"\n *   1 INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_FROM_MATRIX: aa_connectivity_matrix.csv/ad_connectivity_matrix.csv/da_connectivity_matrix.csv/dd_connectivity_matrix.csv"
	"\n *   2 INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_NEURONS_DATASET_FROM_SKELETONS: skeleton[SKID].swc, science.add9330_data_s2.csv/science.add9330_data_s3.csv/science.add9330_data_s4.csv, inputs.csv"
	"\n * Output:"
	"\n *   1 INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_FROM_MATRIX: localConnectomeConnections-typesFromPresynapticNeurons.csv - pre_id, post_id, pre_class_label, post_class_label, syn_num"
	"\n *   2 INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_NEURONS_DATASET_FROM_SKELETONS: localConnectomeNeurons.csv - id, x, y, z, type, excitation_type"
	"\n";
#endif


int main(const int argc, const char** argv)
{
 currentDirectory = SHAREDvarsClass().getCurrentDirectory();

	bool passInputReq = true;

	int executionMode = EXECUTION_MODE_DEFAULT;
	#ifdef INDEXED_CSV_DATABASE_QUERY
	int queryMode = QUERY_MODE_DEFAULT;
	#endif
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME
	int readMode = READ_MODE_DEFAULT;
	#endif
	#ifdef INDEXED_CSV_DATABASE_PREPROCESS
	int preprocessMode = PREPROCESS_MODE_DEFAULT;
	#endif

	#ifdef INDEXED_CSV_DATABASE_CREATE
	string avro_json_database_folder = AVRO_JSON_DATABASE_FOLDER;
	string indexed_csv_database_folder = INDEXED_CSV_DATABASE_FOLDER;
	#endif
	string local_connectome_folder_base = LOCAL_CONNECTOME_FOLDER_BASE;

	if(SHAREDvarsClass().argumentExists(argc, argv, "-mode"))
	{
		executionMode = SHAREDvarsClass().getFloatArgument(argc, argv, "-mode");
	}
	else
	{
		passInputReq = false;
	}
	
	#ifdef INDEXED_CSV_DATABASE_QUERY
	if(SHAREDvarsClass().argumentExists(argc, argv, "-query"))
	{
		queryMode = SHAREDvarsClass().getFloatArgument(argc, argv, "-query");
	}
	else
	{
		if(executionMode == EXECUTION_MODE_INDEXED_CSV_DATABASE_QUERY)
		{
			passInputReq = false;
		}
	}
	#endif
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME
	if(SHAREDvarsClass().argumentExists(argc, argv, "-read"))
	{
		readMode = SHAREDvarsClass().getFloatArgument(argc, argv, "-read");
	}
	else
	{
		if(executionMode == EXECUTION_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME)
		{
			passInputReq = false;
		}
	}
	#endif
	#ifdef INDEXED_CSV_DATABASE_PREPROCESS
	if(SHAREDvarsClass().argumentExists(argc, argv, "-preprocess"))
	{
		preprocessMode = SHAREDvarsClass().getFloatArgument(argc, argv, "-preprocess");
	}
	else
	{
		if(executionMode == EXECUTION_MODE_INDEXED_CSV_DATABASE_PREPROCESS)
		{
			passInputReq = false;
		}
	}
	#endif
		
	#ifdef INDEXED_CSV_DATABASE_CREATE
	if(SHAREDvarsClass().argumentExists(argc, argv, "-avro_json_database_folder"))
	{
		avro_json_database_folder = SHAREDvarsClass().getStringArgument(argc, argv, "-avro_json_database_folder");
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-indexed_csv_database_folder"))
	{
		indexed_csv_database_folder = SHAREDvarsClass().getStringArgument(argc, argv, "-indexed_csv_database_folder");
	}
	#endif
	if(SHAREDvarsClass().argumentExists(argc, argv, "-local_connectome_folder_base"))
	{
		local_connectome_folder_base = SHAREDvarsClass().getStringArgument(argc, argv, "-local_connectome_folder_base");
	}

	if(!passInputReq)
	{
		cerr << errmessage << endl;
		cerr << infomessage << endl;
		exit(EXIT_ERROR);
	}
	
	cout << "executionMode = " << executionMode << " " << executionModesArray[executionMode-1] << endl;
	#ifdef INDEXED_CSV_DATABASE_QUERY
	cout << "queryMode = " << queryMode << " " << queryModesArray[queryMode-1] << endl;
	#endif
	cout << "readMode = " << readMode << " " << readModesArray[readMode-1] << endl;
	
	#ifdef INDEXED_CSV_DATABASE_CREATE
	cout << "avro_json_database_folder = " << avro_json_database_folder << endl;
	cout << "indexed_csv_database_folder = " << indexed_csv_database_folder << endl;
	#endif
	cout << "local_connectome_folder_base = " << local_connectome_folder_base << endl;

	//exit(EXIT_ERROR);
	
	if(false) ;
	#ifdef INDEXED_CSV_DATABASE_CREATE
	else if(executionMode == EXECUTION_MODE_INDEXED_CSV_DATABASE_CREATE)
	{	
		#ifndef INDEXED_CSV_DATABASE_CREATE_DEBUG
		char userAnswer;
		cout << "The user has indicated they wish to create an indexed CSV database. This will take approximately 6 hours to generate, and will overwrite any existing indexed CSV database. Are you sure you wish to continue? (y/n)" << endl;
    	cin >> userAnswer;	
		if(userAnswer == 'y')
		{
		#endif
			H01indexedCSVdatabaseCreateClass().createIndexedCSVdatabase(avro_json_database_folder, indexed_csv_database_folder);
		#ifndef INDEXED_CSV_DATABASE_CREATE_DEBUG
		}
		else
		{
			exit(EXIT_ERROR);
		}
		#endif
	}
	#endif
	#ifdef INDEXED_CSV_DATABASE_QUERY
	else if(executionMode == EXECUTION_MODE_INDEXED_CSV_DATABASE_QUERY)
	{
		H01indexedCSVdatabaseQueryClass().queryIndexedCSVdatabase(queryMode, indexed_csv_database_folder, local_connectome_folder_base);
	}
	#endif
	#ifdef INDEXED_CSV_DATABASE_VISUALISE_LOCAL_CONNECTOME
	else if(executionMode == EXECUTION_MODE_INDEXED_CSV_DATABASE_VISUALISE_LOCAL_CONNECTOME)
	{
		H01indexedCSVdatabaseVisualiseLocalConnectomeClass().visualiseLocalConnectomeCSVdataset(local_connectome_folder_base);
	}
	#endif
	#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME
	else if(executionMode == EXECUTION_MODE_INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME)
	{
		H01indexedCSVdatabaseTraceLocalConnectomeClass().traceLocalConnectomeCSVdataset(local_connectome_folder_base);
	}
	#endif
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME
	else if(executionMode == EXECUTION_MODE_INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME)
	{
		H01indexedCSVdatabaseReadLocalConnectomeClass().readLocalConnectome(readMode, local_connectome_folder_base);
	}
	#endif
	#ifdef INDEXED_CSV_DATABASE_PREPROCESS
	else if(executionMode == EXECUTION_MODE_INDEXED_CSV_DATABASE_PREPROCESS)
	{
		H01indexedCSVdatabasePreprocessClass().preprocess(preprocessMode, local_connectome_folder_base);
	}
	#endif
	else
	{
		cerr << "main error: executionMode unknown: " << executionMode << endl;
		exit(EXIT_ERROR);
	}
	
}


