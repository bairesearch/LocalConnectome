/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseCreate.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: H01LocalConnectome
 * Requirements: see H01indexedCSVdatabase.hpp
 * Compilation: see H01indexedCSVdatabase.hpp
 * Usage: see H01indexedCSVdatabase.hpp
 * Description: H01 indexed CSV database create - convert C3 Synaptic connections Avro Json To indexed CSV database (indexed by pre/postsynaptic neuron ID)
 * Input: C3 Synaptic connections database (gs://h01-release/data/20210601/c3/synapses/exported/json)
 * Output Format: ssddata/indexed/123/csvPreSynapticNeuronID123456.csv - presynapticSiteNeuronID, postsynapticSiteNeuronID, presynapticSiteType, postsynapticSiteType, presynapticSiteClassLabel, postsynapticSiteClassLabel, presynapticSiteBaseNeuronID, postsynapticSiteBaseNeuronID, synapseLocationXcoordinates, synapseLocationYcoordinates, synapseLocationZcoordinates, synapseType
 * Comments:
 * /
 *******************************************************************************/

#include "H01indexedCSVdatabase.hpp"
#include "SHAREDvars.hpp"

#ifdef INDEXED_CSV_DATABASE_CREATE
class H01indexedCSVdatabaseCreateClass
{
	private: SHAREDvarsClass SHAREDvars;

	public: bool createIndexedCSVdatabase(const string avro_json_database_folder, const string indexed_csv_database_folder);
		private: bool addSynapseToCSVdatabase(const string indexed_csv_database_folder, const string neuronIDcontents, const string* csvText, const string* rawText, const bool indexByPresynapticNeuron);
		private: string findJsonFieldValue(const string* currentLineText, const string jsonFieldName, bool lastInstance);
		private: string findJsonFieldValue(const string* currentLineText, const string jsonFieldName, bool lastInstance, const int searchStartPos);
		private: string findJsonFieldValue(const string* currentLineText, const string jsonFieldName, bool lastInstance, const int searchStartPos, int* jsonFieldNameIndex);
};

#endif


