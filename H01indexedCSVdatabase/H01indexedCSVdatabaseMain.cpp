/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseMain.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: H01LocalConnectome
 * Requirements: see H01indexedCSVdatabase.hpp
 * Compilation: see H01indexedCSVdatabase.hpp
 * Usage: see H01indexedCSVdatabase.hpp
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
#include "SHAREDvars.hpp"



static string executionModesArray[EXECUTION_MODES_TOTAL] = {"create:INDEXED_CSV_DATABASE_CREATE","query:INDEXED_CSV_DATABASE_QUERY","visualise:INDEXED_CSV_DATABASE_VISUALISE_LOCAL_CONNECTOME","trace:INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME"};
static string queryModesArray[QUERY_MODES_TOTAL] = {"extract:INDEXED_CSV_DATABASE_QUERY_EXTRACT_INCOMING_OUTGOING_CONNECTIONS","map:INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING","generate:INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET","count:INDEXED_CSV_DATABASE_QUERY_COUNT_PROPORTION_LOCAL_VS_NONLOCAL_CONNECTIONS","complete:INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET"};

static char errmessage[] = "Usage:  H01indexedCSVdatabase.exe [options]"
"\n"
"\n\twhere options are any of the following (see documentation)"
"\n"
"\n\t-mode [int]                             : execution mode (1: create, 2: query, 3: visualise, 4: trace (def: 2) [required]"
"\n\t-query [int]                            : query mode (1: extract, 2: map, 3: generate, 4: count, 5: complete, 6: crawl (def: 4) [required for mode:query]"
"\n"
"\n\t-avro_json_database_folder [string]     : H01 C3 Synaptic connections database json folder (def: /media/user/large/h01data/data/exported/json)"
"\n\t-indexed_csv_database_folder [string]   : H01 indexed csv database folder (def: /media/user/ssddata/indexed)"
"\n\t-local_connectome_folder_base [string]  : H01 local connectome base folder containing \"datasets\" and \"visualisations\" (def: ../)"
"\n"
"\n";

static char infomessage[] = "Information regarding execution/query modes:"
"\n"
"\nexecution mode 1 - INDEXED_CSV_DATABASE_CREATE - converts Avro Json C3 Synaptic connections database to indexed CSV database (indexed by pre/postsynaptic neuron ID)"
"\nexecution mode 2 - INDEXED_CSV_DATABASE_QUERY - queries indexed CSV database, based on local connectome neuron id list"
"\nexecution mode 3 - INDEXED_CSV_DATABASE_VISUALISE_LOCAL_CONNECTOME - visualises local connectome datasets"
"\nexecution mode 4 - INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME - traces local connectome dataset (saving visualisation)"
"\n"
"\nquery mode 1 - INDEXED_CSV_DATABASE_QUERY_EXTRACT_INCOMING_OUTGOING_CONNECTIONS - lookup indexed CSV database by neuron ID, and find incoming/outgoing target connections, and write them to file"
"\nquery mode 2 - INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING - lookup indexed CSV database by neuron ID, find incoming target connections, and generate visualisation"
"\nquery mode 3 - INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET - automatically generate localConnectomeConnections-typesFromPresynapticNeurons.csv/localConnectomeConnections-typesFromEMimages.csv from localConnectomeNeurons.csv and indexed CSV database"
"\nquery mode 4 - INDEXED_CSV_DATABASE_QUERY_COUNT_PROPORTION_LOCAL_VS_NONLOCAL_CONNECTIONS - lookup indexed CSV database by neuron ID, count/infer proportion of incoming/outgoing excitatory/inhibitory target connections to local vs distal neurons"
"\nquery mode 5 - INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET - lookup indexed CSV database by post/pre synaptic connection neuron ID, and identify connection with pre/post synaptic X/Y coordinates (if pre/post synaptic type=UNKNOWN), add pre/post synaptic neuron ID, Z coordinates, and type coordinates to connection dataset [incomplete]"
"\nquery mode 6 - INDEXED_CSV_DATABASE_QUERY_COUNT_NUMBER_UNIQUE_AXONS_DENDRITES - crawl indexed CSV database by pre/post synaptic connection neuron ID, and count number of unique axons/dendrites as specified by neuron ID - not explicitly connected to local connectome [incomplete]"
"\n"
"\nexecution mode 1 - INDEXED_CSV_DATABASE_CREATE - converts Avro Json C3 Synaptic connections database to indexed CSV database, indexed by pre/postsynaptic neuron ID"
"\n * Input: C3 Synaptic connections database (gs://h01-release/data/20210601/c3/synapses/exported/json)"
"\n * Output Format: ssddata/indexed/123/csvPreSynapticNeuronID123456.csv - presynapticSiteNeuronID, postsynapticSiteNeuronID, presynapticSiteType, postsynapticSiteType, presynapticSiteClassLabel, postsynapticSiteClassLabel, presynapticSiteBaseNeuronID, postsynapticSiteBaseNeuronID, synapseLocationXcoordinates, synapseLocationYcoordinates, synapseLocationZcoordinates, synapseType"
"\n"
"\nexecution mode 2 - INDEXED_CSV_DATABASE_QUERY - queries indexed CSV database, based on local connectome neuron id list"
"\n * Input: "
"\n *   1 INDEXED_CSV_DATABASE_QUERY_OUTPUT_CONNECTIONS: localConnectomeNeurons.csv (or localConnectomeNeuronIDlistDistinct.csv) - id, x, y, z, type, excitation_type (or id)"
"\n *   2 INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING: localConnectomeNeurons.csv (or localConnectomeNeuronIDlistDistinct.csv) - id, x, y, z, type, excitation_type (or id)"
"\n *   3 INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: localConnectomeNeurons.csv - id, x, y, z, type, excitation_type"
"\n *   4 INDEXED_CSV_DATABASE_QUERY_COUNT_PROPORTION_LOCAL_VS_NONLOCAL_CONNECTIONS: localConnectomeNeurons.csv (or localConnectomeNeuronIDlistDistinct.csv) - id, x, y, z, type, excitation_type (or id)"
"\n *   5 INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: localConnectomeConnections-typesFromEMimages-useAllValuesAvailableFromInBodyCellConnection.csv - pre_id, pre_x, pre_y, pre_z, pre_type, post_id, post_x, post_y, post_z, post_type, post_class_label, syn_num, excitation_type"
"\n *   6 INDEXED_CSV_DATABASE_QUERY_COUNT_NUMBER_UNIQUE_AXONS_DENDRITES: N/A"
"\n * Output Format:"
"\n *   1 INDEXED_CSV_DATABASE_QUERY_OUTPUT_CONNECTIONS: localConnectomeNeuronIDlistConnectionsPresynaptic.csv/localConnectomeNeuronIDlistConnectionsPostsynaptic.csv - connectionNeuronID1, connectionType1 [, locationObjectContentsXcoordinatesContent1, locationObjectContentsYcoordinatesContent1, locationObjectContentsZcoordinatesContent1], connectionNeuronID2, connectionType2 [, locationObjectContentsXcoordinatesContent2, locationObjectContentsYcoordinatesContent2, locationObjectContentsZcoordinatesContent2], etc "
"\n *   2 INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING:"
"\n *       INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_3D_LINEAR_REGRESSION:"
"\n *           INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING_LDR: localConnectomeIncomingAxonMapping.ldr - LDR_REFERENCE_TYPE_LINE ldrawColor plot3DpointStart.x plot3DpointStart.y plot3DpointStart.z plot3DpointEnd.x plot3DpointEnd.y plot3DpointEnd.z"
"\n *           INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING_CSV: localConnectomeIncomingAxonMapping.csv - polyFit.connectionNeuronID, polyFit.estSynapseType, polyFit.origin.x, polyFit.origin.y, polyFit.origin.z, polyFit.axis.x, polyFit.axis.y, polyFit.axis.z"
"\n *       INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_2D_POLY_REGRESSION:"
"\n *           INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING_CSV: localConnectomeIncomingAxonMapping.csv - polyFit.connectionNeuronID, polyFit.estSynapseType, polyFit.a, polyFit.b, polyFit.c"
"\n *   3 INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: localConnectomeConnections-typesFromPresynapticNeurons.csv/localConnectomeConnections-typesFromEMimages.csv - pre_id, pre_x, pre_y, pre_z, pre_type, post_id, post_x, post_y, post_z, post_type, post_class_label, syn_num, excitation_type"
"\n *   4 INDEXED_CSV_DATABASE_QUERY_COUNT_PROPORTION_LOCAL_VS_NONLOCAL_CONNECTIONS: N/A"
"\n *   5 INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET: localConnectomeConnections-typesFromEMimages.csv - pre_id, pre_x, pre_y, pre_z, pre_type, post_id, post_x, post_y, post_z, post_type, post_class_label, syn_num, excitation_type"
"\n *   6 INDEXED_CSV_DATABASE_QUERY_COUNT_NUMBER_UNIQUE_AXONS_DENDRITES: N/A"
"\n"
"\nexecution mode 3 - INDEXED_CSV_DATABASE_VISUALISE_LOCAL_CONNECTOME - visualises local connectome datasets"
"\n * Input: localConnectomeNeurons.csv / localConnectomeConnectionsX.csv"
"\n * Output Format: SVG (2D) / LDR (3D)"
"\n"
"\nexecution mode 4 - INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME - traces local connectome dataset (saving visualisation)"
"\n * Input: localConnectomeNeurons.csv / localConnectomeConnectionsX.csv"
"\n * Output Format: SVG (2D) / LDR (3D)"
"\n";


int main(const int argc, const char** argv)
{
 currentDirectory = SHAREDvarsClass().getCurrentDirectory();

	bool passInputReq = true;

	int executionMode = EXECUTION_MODE_DEFAULT;
	int queryMode = QUERY_MODE_DEFAULT;

	string avro_json_database_folder = AVRO_JSON_DATABASE_FOLDER;
	string indexed_csv_database_folder = INDEXED_CSV_DATABASE_FOLDER;
	string local_connectome_folder_base = LOCAL_CONNECTOME_FOLDER_BASE;

	if(SHAREDvarsClass().argumentExists(argc, argv, "-mode"))
	{
		executionMode = SHAREDvarsClass().getFloatArgument(argc, argv, "-mode");
	}
	else
	{
		passInputReq = false;
	}
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
	
	if(SHAREDvarsClass().argumentExists(argc, argv, "-avro_json_database_folder"))
	{
		avro_json_database_folder = SHAREDvarsClass().getStringArgument(argc, argv, "-avro_json_database_folder");
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-indexed_csv_database_folder"))
	{
		indexed_csv_database_folder = SHAREDvarsClass().getStringArgument(argc, argv, "-indexed_csv_database_folder");
	}
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
	cout << "queryMode = " << queryMode << " " << queryModesArray[queryMode-1] << endl;
	
	cout << "avro_json_database_folder = " << avro_json_database_folder << endl;
	cout << "indexed_csv_database_folder = " << indexed_csv_database_folder << endl;
	cout << "local_connectome_folder_base = " << local_connectome_folder_base << endl;

	//exit(EXIT_ERROR);
	
	#ifdef INDEXED_CSV_DATABASE_CREATE
	if(executionMode == EXECUTION_MODE_INDEXED_CSV_DATABASE_CREATE)
	{	
		char userAnswer;
		cout << "The user has indicated they wish to create an indexed CSV database. This will take approximately 6 hours to generate, and will overwrite any existing indexed CSV database. Are you sure you wish to continue? (y/n)" << endl;
    	cin >> userAnswer;	
		if(userAnswer == 'y')
		{
			H01indexedCSVdatabaseCreateClass().createIndexedCSVdatabase(avro_json_database_folder, indexed_csv_database_folder);
		}
		else
		{
			exit(EXIT_ERROR);
		}
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
	else
	{
		cerr << "main error: executionMode unknown: " << executionMode << endl;
		exit(EXIT_ERROR);
	}
	
}


