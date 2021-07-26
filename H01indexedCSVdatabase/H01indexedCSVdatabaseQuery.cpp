/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseQuery.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: H01LocalConnectome
 * Requirements: see H01indexedCSVdatabase.hpp
 * Compilation: see H01indexedCSVdatabase.hpp
 * Usage: see H01indexedCSVdatabase.hpp
 * Description: H01 indexed CSV database query - lookup indexed CSV database by pre/postsynaptic neuron ID, and find target/source connections
 * Comments:
 * /
 *******************************************************************************/

#include "H01indexedCSVdatabaseQuery.hpp"
#include "SHAREDvars.hpp"

#ifdef INDEXED_CSV_DATABASE_QUERY

SHAREDvarsClass SHAREDvars;

#ifdef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_CURRENT_FOLDER
string currentDirectory;
#endif

bool queryIndexedCSVdatabase()
{
	bool result = true;

	#ifdef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_CURRENT_FOLDER
	currentDirectory = SHAREDvars.getCurrentDirectory();
	#endif
	
	queryIndexedCSVdatabase(INDEXED_CSV_DATABASE_QUERY_NEURON_LIST1_FILE_NAME, true, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST1_CONNECTIONS_PRESYNAPTIC_FILE_NAME);
	queryIndexedCSVdatabase(INDEXED_CSV_DATABASE_QUERY_NEURON_LIST1_FILE_NAME, false, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST1_CONNECTIONS_POSTSYNAPTIC_FILE_NAME);
	queryIndexedCSVdatabase(INDEXED_CSV_DATABASE_QUERY_NEURON_LIST2_FILE_NAME, true, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST2_CONNECTIONS_PRESYNAPTIC_FILE_NAME);
	queryIndexedCSVdatabase(INDEXED_CSV_DATABASE_QUERY_NEURON_LIST2_FILE_NAME, false, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST2_CONNECTIONS_POSTSYNAPTIC_FILE_NAME);
	
	return result;
}

bool queryIndexedCSVdatabase(const string neuronListFileName, const bool queryPresynapticConnectionNeurons, const string neuronListConnectionsFileName)
{
	bool result = true;
	
	vector<string> neuronList;
	int neuronListFileLength = 0;
	#ifdef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_CURRENT_FOLDER
	const string indexedCSVdatabaseQueryInputFolder = currentDirectory;	
	#else
	const string indexedCSVdatabaseQueryInputFolder = INDEXED_CSV_DATABASE_QUERY_INPUT_FOLDER;
	#endif
	cout << "neuronListFileName = " << neuronListFileName << endl;
	cout << "neuronListConnectionsFileName = " << neuronListConnectionsFileName << endl;
	cout << "indexedCSVdatabaseQueryInputFolder = " << indexedCSVdatabaseQueryInputFolder << endl;
	SHAREDvars.setCurrentDirectory(indexedCSVdatabaseQueryInputFolder);
	SHAREDvars.getLinesFromFile(neuronListFileName, &neuronList, &neuronListFileLength);
	
	const string csvDelimiter = CSV_DELIMITER;

	#ifdef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_CURRENT_FOLDER
	const string indexedCSVdatabaseQueryOutputFolder = currentDirectory;	
	#else
	const string indexedCSVdatabaseQueryOutputFolder = INDEXED_CSV_DATABASE_QUERY_OUTPUT_FOLDER;
	#endif
	
	#ifdef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_FILE_OBJECT
	SHAREDvars.setCurrentDirectory(indexedCSVdatabaseQueryOutputFolder);
	ofstream writeFileObject(neuronListConnectionsFileName.c_str());
	#else
	string neuronListConnectionsString;
	neuronListConnectionsString.reserve(2000000000);	//max expected file/string size = indexed csv database query output size (neuron_id ~11 bytes + , + type ~1 byte = ~13 bytes) * avg num connections per neuron ID (~2000) * number of neuron IDs in list (~30000) = 13*2000*30000 = ~1GB of RAM if string
	//string neuronListConnectionsString = "";	//inefficient
	#endif
			
	for(int l=0; l<neuronListFileLength; l++)
	{
		string neuronID = neuronList[l];
		vector<vector<string>> neuronConnectionList;
		queryIndexedCSVdatabaseByNeuronID(neuronID, queryPresynapticConnectionNeurons, &neuronConnectionList);

		if(neuronConnectionList.size() == 0)
		{
			//add a new line such that database lengths are matched
			#ifdef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_FILE_OBJECT
			SHAREDvars.writeStringToFileObject(STRING_NEWLINE, &writeFileObject);
			#else
			appendStringBufferToPreallocatedString(&neuronListConnectionsString, STRING_NEWLINE);
			//neuronListConnectionsString = neuronListConnectionsString + STRING_NEWLINE;	//inefficient
			#endif			
		}
		else
		{		
			for(int i=0; i<neuronConnectionList.size(); i++)
			{
				vector<string> csvLineVector = neuronConnectionList[i];

				string connectionNeuronID = "";
				if(queryPresynapticConnectionNeurons)
				{
					connectionNeuronID = csvLineVector[INDEXED_CSV_DATABASE_PRESYNAPTIC_SITE_NEURON_ID_FIELD_INDEX];
				}
				else
				{
					connectionNeuronID = csvLineVector[INDEXED_CSV_DATABASE_POSTSYNAPTIC_SITE_NEURON_ID_FIELD_INDEX];
				}
				string connectionType = csvLineVector[INDEXED_CSV_DATABASE_SYNAPSE_TYPE_FIELD_INDEX];
				//cout << "connectionNeuronID = " << connectionNeuronID << endl;
				//cout << "connectionType = " << connectionType << endl;

				string neuronConnectionString = connectionNeuronID + csvDelimiter + connectionType;
				#ifdef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_FILE_OBJECT
				SHAREDvars.writeStringToFileObject(neuronConnectionString, &writeFileObject);
				#else
				appendStringBufferToPreallocatedString(neuronListConnectionsString, neuronConnectionString);
				//neuronListConnectionsString = neuronListConnectionsString + neuronConnectionString	//inefficient
				#endif

				if(i == neuronConnectionList.size()-1)
				{
					#ifdef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_FILE_OBJECT
					SHAREDvars.writeStringToFileObject(STRING_NEWLINE, &writeFileObject);
					#else
					appendStringBufferToPreallocatedString(&neuronListConnectionsString, STRING_NEWLINE);
					//neuronListConnectionsString = neuronListConnectionsString + STRING_NEWLINE;	//inefficient
					#endif
				}
				else
				{
					#ifdef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_FILE_OBJECT
					SHAREDvars.writeStringToFileObject(csvDelimiter, &writeFileObject);
					#else
					appendStringBufferToPreallocatedString(&neuronListConnectionsString, csvDelimiter);
					//neuronListConnectionsString = neuronListConnectionsString + csvDelimiter;	//inefficient
					#endif
				}
			}
		}

	
		cout << "l = " << l << ", neuronID = " << neuronID << ", neuronConnectionList.size() = " << neuronConnectionList.size() << endl;

	}
	
	#ifdef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_FILE_OBJECT
	writeFileObject.close();
	#else
	//cout << "neuronListConnectionsString = " << neuronListConnectionsString << endl;
	SHAREDvars.setCurrentDirectory(indexedCSVdatabaseQueryOutputFolder);
	//SHAREDvars.writeStringToFile(neuronListConnectionsFileName, &neuronListConnectionsString);	//inefficient?
	ofstream out(neuronListConnectionsFileName);
    out << neuronListConnectionsString;
    out.close();
	#endif
	
	
	return result;
}


bool queryIndexedCSVdatabaseByNeuronID(const string neuronID, const bool queryPresynapticConnectionNeurons, vector<vector<string>>* neuronConnectionList)
{
	bool result = true;
	
	#ifdef INDEXED_CSV_DATABASE_STORE_MULTIPLE_PREPOSTSYNAPTIC_NEURONID_SYNAPSES_PER_FILE
	int fileNameNeuronIDmaxNumCharacters = INDEXED_CSV_DATABASE_FILENAME_NEURONID_MAX_NUM_CHARACTERS;
	#else
	int fileNameNeuronIDmaxNumCharacters = neuronID.length();	//not used
	#endif
	
	int numberLevels = INDEXED_CSV_DATABASE_NUMBER_LEVELS;	//eg 2
	int numberCharactersPerFolderLevel = INDEXED_CSV_DATABASE_NUMBER_CHARACTERS_PER_FOLDER_LEVEL;	//eg 4
		
	const string indexedCSVdatabaseFolder = INDEXED_CSV_DATABASE_FOLDER;
	const string indexedCSVdatabaseFileNameCsvExtension = CSV_FILE_EXTENSION;
	
	string indexedCSVdatabaseFileNameCsvPrefix = "";
	if(queryPresynapticConnectionNeurons)
	{
		indexedCSVdatabaseFileNameCsvPrefix = INDEXED_CSV_DATABASE_FILENAME_PREFIX_POSTSYNAPTIC_CSV;	//look for the presynaptic neuron/connection id within the database file indexed by postsynaptic neuron id	//note opposite of queryPresynapticConnectionNeurons	
	}
	else
	{
		indexedCSVdatabaseFileNameCsvPrefix = INDEXED_CSV_DATABASE_FILENAME_PREFIX_PRESYNAPTIC_CSV;		//look for the postsynaptic neuron/connection id within the database file indexed by presynaptic neuron id
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
				if(queryPresynapticConnectionNeurons)
				{
					//get INDEXED_CSV_DATABASE_POSTSYNAPTIC_SITE_NEURON_ID_FIELD_INDEX (optimised):
					int sourceNeuronIDindexStart = csvLineText.find(csvDelimiter)+1;
					int sourceNeuronIDindexEnd = csvLineText.find(csvDelimiter, sourceNeuronIDindexStart);
					sourceNeuronID = csvLineText.substr(sourceNeuronIDindexStart, sourceNeuronIDindexEnd-sourceNeuronIDindexStart);
					//sourceNeuronID = csvLineVector[INDEXED_CSV_DATABASE_POSTSYNAPTIC_SITE_NEURON_ID_FIELD_INDEX];	//inefficient;
				}
				else
				{
					//get INDEXED_CSV_DATABASE_PRESYNAPTIC_SITE_NEURON_ID_FIELD_INDEX (optimised): 
					int sourceNeuronIDindexEnd = csvLineText.find(csvDelimiter);
					sourceNeuronID = csvLineText.substr(0, sourceNeuronIDindexEnd);
					//sourceNeuronID = csvLineVector[INDEXED_CSV_DATABASE_PRESYNAPTIC_SITE_NEURON_ID_FIELD_INDEX];	//inefficient;
				}
								
				if(sourceNeuronID == neuronID)
				{ 
					//cout << "sourceNeuronID = " << sourceNeuronID << endl;

					vector<string> csvLineVector;
					convertCSVlineToVector(&csvLineText, &csvLineVector);
				
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

bool convertCSVlineToVector(const string* csvLineText, vector<string>* csvLineVector)
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
void appendStringBufferToPreallocatedString(string* s, string stringToAppend)
{
	s->append(fileName.c_str(), stringToAppend->length());
}
#endif

#endif

