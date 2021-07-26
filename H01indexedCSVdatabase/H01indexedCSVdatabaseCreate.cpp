/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseCreate.cpp
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

#include "H01indexedCSVdatabaseCreate.hpp"
#include "SHAREDvars.hpp"

#ifdef INDEXED_CSV_DATABASE_CREATE

SHAREDvarsClass SHAREDvars;

bool createIndexedCSVdatabase()
{
	bool result = true;
	
	int minJsonFileIndex = AVRO_JSON_DATABASE_FILEINDEX_START;
	int maxJsonFileIndex = AVRO_JSON_DATABASE_FILEINDEX_END;
	
	const string avroJsonDatabaseFolder = AVRO_JSON_DATABASE_FOLDER;
	const string avroJsonDatabaseFileExtension = JSON_FILE_EXTENSION;
		
	//int jsonFieldContentsStartRelativeIndex = neuronIDname.length() + 2;	//"neuron_id":"	[+ :"]
	string jsonObjectCharacterOpen = "{";
	string jsonObjectCharacterClose = "}";
	
	//synaptic site json fields;
	string neuronIDname = "\"neuron_id\"";	
	string synapticSiteTypeName = "\"type\"";
	string classLabelName = "\"class_label\"";
	string baseNeuronIDname = "\"base_neuron_id\"";
	
	//synapse json fields;
	string locationName = "\"location\"";
	string locationObjectContentsXcoordinatesName = "\"x\"";
	string locationObjectContentsYcoordinatesName = "\"y\"";
	string locationObjectContentsZcoordinatesName = "\"z\"";
	string synapseTypeName = "\"type\"";
	string confidenceName = "\"confidence\"";	
	//int confidenceSynapseTypeRelativeIndex = -3;	//"type":"1","confidence"

		
	for(int f=minJsonFileIndex; f<=maxJsonFileIndex; f++)
	{
		//cout << "f = " << f << endl;
		string format = "%012d";	//"000000000000";
		string fileToOpenName = string("export") + SHAREDvars.convertIntToString(f, format) + avroJsonDatabaseFileExtension;	//eg export000000000001.json
		vector<string> fileLinesList;
		int numberOfLinesInFile = 0;
		cout << "fileToOpenName = " << fileToOpenName << endl;
		//exit(0);
		
		SHAREDvars.setCurrentDirectory(avroJsonDatabaseFolder);
		SHAREDvars.getLinesFromFile(fileToOpenName, &fileLinesList, &numberOfLinesInFile);
		
		#ifdef DEBUG_CODE
		numberOfLinesInFile = 1;
		#endif
		
		int firstLineInFile = 0;
		#ifdef DEBUG_RESTART
		if(f == minJsonFileIndex)
		{
			firstLineInFile = DEBUG_RESTART_LINEINDEX_START;
		}
		#endif

		for(int l=firstLineInFile; l<numberOfLinesInFile; l++)
		{
			string currentLineText = fileLinesList[l];	//eg {"pre_synaptic_site":{"neuron_id":"5472528459","id":"19685462211","centroid":{"x":"461512","y":"196505","z":"256"},"type":"1","bounding_box":{"start":{"x":"461472","y":"196487","z":"240"},"size":{"x":"81","y":"37","z":"32"}},"class_label":"AXON","base_neuron_id":"5472528459"},"post_synaptic_partner":{"neuron_id":"5414315245","id":"19685462210","centroid":{"x":"461511","y":"196494","z":"256"},"type":"2","bounding_box":{"start":{"x":"461472","y":"196474","z":"240"},"size":{"x":"79","y":"40","z":"33"}},"class_label":"DENDRITE","base_neuron_id":"5472528717"},"location":{"x":"461512","y":"196499","z":"256"},"type":"2","confidence":0.95752114057540894,"bounding_box":{"start":{"x":"461472","y":"196474","z":"240"},"size":{"x":"81","y":"50","z":"33"}}}

			#ifndef MAX_SSD_SIZE
			cout << "l = " << l << endl;
			#endif
			#ifdef DEBUG_CODE
			cout << "currentLineText = " << currentLineText << endl;
			#endif

			string neuronIDcontents1 = findJsonFieldValue(&currentLineText, neuronIDname, false);
			string neuronIDcontents2 = findJsonFieldValue(&currentLineText, neuronIDname, true);
			if(((neuronIDcontents1 != "") && (neuronIDcontents2 != "")) && (neuronIDcontents1 != neuronIDcontents2))
			{
				/*//alternative;
				int confidenceStartIndex = currentLineText.find(confidenceName);
				int synapseTypeIndex = confidenceStartIndex+confidenceSynapseTypeRelativeIndex;
				//cout << "synapseTypeIndex = " << synapseTypeIndex << endl;
				string synapseTypeContents = currentLineText.substr(synapseTypeIndex, 1);	//1/2
				*/
				int synapseTypeNameIndex = CPP_STRING_FIND_RESULT_FAIL_VALUE2;
				string synapseTypeContents = findJsonFieldValue(&currentLineText, synapseTypeName, true, currentLineText.length(), &synapseTypeNameIndex);
				int locationNameStartIndex = currentLineText.find(locationName);
				int locationObjectContentsStartIndex = locationNameStartIndex + 2;	//eg "location":{	[+ :{]
				int locationObjectContentsCloseIndex = currentLineText.find(jsonObjectCharacterClose, locationObjectContentsStartIndex);
				string locationObjectContents = currentLineText.substr(locationObjectContentsStartIndex, locationObjectContentsCloseIndex-locationObjectContentsStartIndex);
				string locationObjectContentsXcoordinatesContent = findJsonFieldValue(&locationObjectContents, locationObjectContentsXcoordinatesName, false);
				string locationObjectContentsYcoordinatesContent = findJsonFieldValue(&locationObjectContents, locationObjectContentsYcoordinatesName, false);
				string locationObjectContentsZcoordinatesContent = findJsonFieldValue(&locationObjectContents, locationObjectContentsZcoordinatesName, false);				
		
				string synapticSiteTypeContents1 = findJsonFieldValue(&currentLineText, synapticSiteTypeName, false, 0);
				string synapticSiteTypeContents2 = findJsonFieldValue(&currentLineText, synapticSiteTypeName, true, synapseTypeNameIndex);		
				string classLabelContents1 = findJsonFieldValue(&currentLineText, classLabelName, false, 0);
				string classLabelContents2 = findJsonFieldValue(&currentLineText, classLabelName, true, synapseTypeNameIndex);			
				string baseNeuronIDcontents1 = findJsonFieldValue(&currentLineText, baseNeuronIDname, false, 0);
				string baseNeuronIDcontents2 = findJsonFieldValue(&currentLineText, baseNeuronIDname, true, synapseTypeNameIndex);			

				string classLabelContentsSmall1 = classLabelContents1.substr(0,1);	//save first character only (A:AXON, D:DENDRITE, C:SOMA, etc)
				string classLabelContentsSmall2 = classLabelContents2.substr(0,1);	//save first character only (A:AXON, D:DENDRITE, C:SOMA, etc)
		
				const string csvDelimiter = CSV_DELIMITER;
				//string csvText = neuronIDcontents1 + csvDelimiter + neuronIDcontents2 + csvDelimiter + synapticSiteTypeContents1 + csvDelimiter + synapticSiteTypeContents2 + csvDelimiter + classLabelContentsSmall1 + csvDelimiter + classLabelContentsSmall2 + csvDelimiter + baseNeuronIDcontents1 + csvDelimiter + baseNeuronIDcontents2 + csvDelimiter + locationObjectContentsXcoordinatesContent + csvDelimiter + locationObjectContentsYcoordinatesContent + csvDelimiter + locationObjectContentsZcoordinatesContent + csvDelimiter + synapseTypeContents + STRING_NEWLINE;
				string csvTextArray[INDEXED_CSV_DATABASE_NUMBER_FIELDS];
				csvTextArray[INDEXED_CSV_DATABASE_PRESYNAPTIC_SITE_NEURON_ID_FIELD_INDEX] = neuronIDcontents1;
				csvTextArray[INDEXED_CSV_DATABASE_POSTSYNAPTIC_SITE_NEURON_ID_FIELD_INDEX] = neuronIDcontents2;
				csvTextArray[INDEXED_CSV_DATABASE_PRESYNAPTIC_SITE_TYPE_FIELD_INDEX] = synapticSiteTypeContents1;
				csvTextArray[INDEXED_CSV_DATABASE_POSTSYNAPTIC_SITE_TYPE_FIELD_INDEX] = synapticSiteTypeContents2;
				csvTextArray[INDEXED_CSV_DATABASE_PRESYNAPTIC_SITE_CLASSLABEL_FIELD_INDEX] = classLabelContentsSmall1;
				csvTextArray[INDEXED_CSV_DATABASE_POSTSYNAPTIC_SITE_CLASSLABEL_FIELD_INDEX] = classLabelContentsSmall2;
				csvTextArray[INDEXED_CSV_DATABASE_PRESYNAPTIC_SITE_BASENEURON_ID_FIELD_INDEX] = baseNeuronIDcontents1;
				csvTextArray[INDEXED_CSV_DATABASE_POSTSYNAPTIC_SITE_BASENEURON_ID_FIELD_INDEX] = baseNeuronIDcontents2;
				csvTextArray[INDEXED_CSV_DATABASE_SYNAPSE_LOCATION_COORDINATE_X_FIELD_INDEX] = locationObjectContentsXcoordinatesContent;
				csvTextArray[INDEXED_CSV_DATABASE_SYNAPSE_LOCATION_COORDINATE_Y_FIELD_INDEX] = locationObjectContentsYcoordinatesContent;
				csvTextArray[INDEXED_CSV_DATABASE_SYNAPSE_LOCATION_COORDINATE_Z_FIELD_INDEX] = locationObjectContentsZcoordinatesContent;
				csvTextArray[INDEXED_CSV_DATABASE_SYNAPSE_TYPE_FIELD_INDEX] = synapseTypeContents;
				string csvText = "";
				for(int i=0; i<INDEXED_CSV_DATABASE_NUMBER_FIELDS; i++)
				{
					csvText = csvText + csvTextArray[i];
					if(i == INDEXED_CSV_DATABASE_NUMBER_FIELDS-1)
					{
						csvText = csvText + STRING_NEWLINE;
					}
					else
					{
						csvText = csvText + csvDelimiter;
					}
				}
				#ifdef DEBUG_CODE
				cout << "csvText = " << csvText << endl;
				#endif
				
				string rawText = currentLineText + STRING_NEWLINE;	
				addSynapseToCSVdatabase(neuronIDcontents1, &csvText, &rawText, true);
				addSynapseToCSVdatabase(neuronIDcontents2, &csvText, &rawText, false);
			}
			else
			{
				//ignore synapse where at least one neuron_id is missing (class_label: UNKNOWN)
			}
		}
	}
	
	return result;
}	

bool addSynapseToCSVdatabase(const string neuronIDcontents, string* csvText, string* rawText, const bool indexByPresynapticNeuron)
{
	bool result = true;
	
	//eg 1234/rawPre|PostSynapticNeuronID1234567.json|csv

	#ifdef INDEXED_CSV_DATABASE_STORE_MULTIPLE_PREPOSTSYNAPTIC_NEURONID_SYNAPSES_PER_FILE
	int fileNameNeuronIDmaxNumCharacters = INDEXED_CSV_DATABASE_FILENAME_NEURONID_MAX_NUM_CHARACTERS;
	#else
	int fileNameNeuronIDmaxNumCharacters = neuronIDcontents.length();	//not used
	#endif
	
	int numberLevels = INDEXED_CSV_DATABASE_NUMBER_LEVELS;	//eg 2
	int numberCharactersPerFolderLevel = INDEXED_CSV_DATABASE_NUMBER_CHARACTERS_PER_FOLDER_LEVEL;	//eg 4
	/*
	//reporting only;
	int numberCharactersPerFileLevel = fileNameNeuronIDmaxNumCharacters - (numberLevels-1)*numberCharactersPerFolderLevel;	//eg 3
	int numberFoldersPerLevel = pow(10, numberCharactersPerFolderLevel);	//eg 1000
	int numberFilesPerLevel = pow(10, numberCharactersPerFileLevel);	//eg 100
	*/
		
	const string indexedCSVdatabaseFolder = INDEXED_CSV_DATABASE_FOLDER;
	const string indexedCSVdatabaseFileNameRawExtension = JSON_FILE_EXTENSION;
	const string indexedCSVdatabaseFileNameCsvExtension = CSV_FILE_EXTENSION;
	
	string indexedCSVdatabaseFileNameCsvPrefix = "";
	string indexedCSVdatabaseFileNameRawPrefix = "";
	if(indexByPresynapticNeuron)
	{
		indexedCSVdatabaseFileNameCsvPrefix = INDEXED_CSV_DATABASE_FILENAME_PREFIX_PRESYNAPTIC_CSV;
		indexedCSVdatabaseFileNameRawPrefix = INDEXED_CSV_DATABASE_FILENAME_PREFIX_PRESYNAPTIC_RAW;
	}
	else
	{
		indexedCSVdatabaseFileNameCsvPrefix = INDEXED_CSV_DATABASE_FILENAME_PREFIX_POSTSYNAPTIC_CSV;
		indexedCSVdatabaseFileNameRawPrefix = INDEXED_CSV_DATABASE_FILENAME_PREFIX_POSTSYNAPTIC_RAW;
	}

	SHAREDvars.setCurrentDirectory(indexedCSVdatabaseFolder);
	for(int r=0; r<numberLevels; r++)
	{
		string neuronIDlevelName = neuronIDcontents.substr(r*numberCharactersPerFolderLevel, numberCharactersPerFolderLevel);
		if(r == numberLevels-1)
		{
			//last level (create file rather than folder)
			#ifdef INDEXED_CSV_DATABASE_STORE_MULTIPLE_PREPOSTSYNAPTIC_NEURONID_SYNAPSES_PER_FILE
			string fileNameNeuronIDidentifier = neuronIDcontents.substr(0, fileNameNeuronIDmaxNumCharacters);
			#else
			string fileNameNeuronIDidentifier = neuronIDcontents;
			#endif

			#ifdef SAVE_RAW_JSON
			string rawFileName = indexedCSVdatabaseFileNameRawPrefix + fileNameNeuronIDidentifier + indexedCSVdatabaseFileNameRawExtension;
			SHAREDvars.appendStringToFile(rawFileName, rawText);
			#endif

			string csvFileName = indexedCSVdatabaseFileNameCsvPrefix + fileNameNeuronIDidentifier + indexedCSVdatabaseFileNameCsvExtension;
			SHAREDvars.appendStringToFile(csvFileName, csvText);

			#ifdef DEBUG_CODE
			//cout << "rawFileName = " << rawFileName << endl;
			//cout << "rawText = " << *rawText << endl;
			cout << "csvFileName = " << csvFileName << endl;
			cout << "csvText = " << *csvText << endl;
			#endif
		}
		else
		{
			//open or create folder
			SHAREDvars.checkIfFolderExistsAndIfNotMakeAndSetAsCurrent(&neuronIDlevelName);
		}
	}
	
	return result;
}
				
string findJsonFieldValue(const string* currentLineText, const string jsonFieldName, bool lastInstance)
{
	int searchStartPos = 0;
	if(lastInstance)
	{
		searchStartPos = currentLineText->length();
	}
	int jsonFieldNameIndex = CPP_STRING_FIND_RESULT_FAIL_VALUE2;
	return findJsonFieldValue(currentLineText, jsonFieldName, lastInstance, searchStartPos, &jsonFieldNameIndex);
}
string findJsonFieldValue(const string* currentLineText, const string jsonFieldName, bool lastInstance, const int searchStartPos)
{
	int jsonFieldNameIndex = CPP_STRING_FIND_RESULT_FAIL_VALUE2;
	return findJsonFieldValue(currentLineText, jsonFieldName, lastInstance, searchStartPos, &jsonFieldNameIndex);
}
string findJsonFieldValue(const string* currentLineText, const string jsonFieldName, bool lastInstance, const int searchStartPos, int* jsonFieldNameIndex)
{
	string jsonFieldContents = "";
	
	int jsonFieldContentsStartRelativeIndex = jsonFieldName.length() + 2;	//eg "neuron_id":"	[+ :"]
	
	if(lastInstance)
	{
		*jsonFieldNameIndex = currentLineText->rfind(jsonFieldName, searchStartPos);
	}
	else
	{
		*jsonFieldNameIndex = currentLineText->find(jsonFieldName, searchStartPos);		
	}
	//cout << "*jsonFieldNameIndex = " << *jsonFieldNameIndex << endl;
	
	if(*jsonFieldNameIndex != CPP_STRING_FIND_RESULT_FAIL_VALUE2)
	{
		//eg "neuron_id":"51655419168"	//11 characters
		int jsonFieldContentsStartIndex = *jsonFieldNameIndex + jsonFieldContentsStartRelativeIndex;
		int jsonFieldContentsEndIndex = currentLineText->find("\"", jsonFieldContentsStartIndex);
		jsonFieldContents = currentLineText->substr(jsonFieldContentsStartIndex, jsonFieldContentsEndIndex-jsonFieldContentsStartIndex);
		#ifdef DEBUG_CODE
		//cout << "jsonFieldContents = " << jsonFieldContents << endl;
		#endif
	}
	
	return jsonFieldContents;
}

#endif
