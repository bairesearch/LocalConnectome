/*******************************************************************************
 *
 * File Name: LCindexedCSVdatabaseCreate.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021-2023 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: LCLocalConnectome
 * Requirements: see LCindexedCSVdatabaseGlobalDefs.hpp
 * Compilation: see LCindexedCSVdatabaseGlobalDefs.hpp
 * Usage: see LCindexedCSVdatabaseGlobalDefs.hpp
 * Description: LC indexed CSV database create - convert C3 Synaptic connections Avro Json To indexed CSV database (indexed by pre/postsynaptic neuron ID)
 * Input: C3 Synaptic connections database (gs://h01-release/data/20210601/c3/synapses/exported/json)
 * Output Format: ssddata/indexed/123/csvPreSynapticNeuronID123456.csv - presynapticSiteNeuronID, postsynapticSiteNeuronID, presynapticSiteType, postsynapticSiteType, presynapticSiteClassLabel, postsynapticSiteClassLabel, presynapticSiteBaseNeuronID, postsynapticSiteBaseNeuronID, synapseLocationXcoordinates, synapseLocationYcoordinates, synapseLocationZcoordinates, synapseType
 * Comments:
 * /
 *******************************************************************************/

#ifndef HEADER_LCindexedCSVdatabaseCreate
#define HEADER_LCindexedCSVdatabaseCreate

#include "LCindexedCSVdatabaseGlobalDefs.hpp"
#include "SHAREDvars.hpp"

#ifdef INDEXED_CSV_DATABASE_CREATE
class LCindexedCSVdatabaseCreateClass
{
	private: SHAREDvarsClass SHAREDvars;

	public: bool createIndexedCSVdatabase(const string avro_json_database_folder, const string indexed_csv_database_folder);
		private: bool addSynapseToCSVdatabase(const string indexed_csv_database_folder, const string neuronIDcontents, const string* csvText, const string* rawText, const bool indexByPresynapticNeuron);
		private: string findJsonFieldValue1(const string* currentLineText, const string jsonFieldName, bool lastInstance, const bool jsonFieldValueStringDelimiter=true, const string jsonFieldValueEndDelimiter="\"");
		private: string findJsonFieldValue2(const string* currentLineText, const string jsonFieldName, bool lastInstance, const int searchStartPos, const bool jsonFieldValueStringDelimiter=true, const string jsonFieldValueEndDelimiter="\"");
		private: string findJsonFieldValue3(const string* currentLineText, const string jsonFieldName, bool lastInstance, const int searchStartPos, int* jsonFieldNameIndex, const bool jsonFieldValueStringDelimiter=true, const string jsonFieldValueEndDelimiter="\"");
};

#endif

#endif
