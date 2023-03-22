/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabasePreprocess.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021-2023 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: H01LocalConnectome
 * Requirements: see H01indexedCSVdatabaseGlobalDefs.hpp
 * Compilation: see H01indexedCSVdatabaseGlobalDefs.hpp
 * Usage: see H01indexedCSVdatabaseGlobalDefs.hpp
 * Description: H01 indexed CSV database preprocess - 
 * /
 *******************************************************************************/


#include "H01indexedCSVdatabasePreprocess.hpp"

#ifdef INDEXED_CSV_DATABASE_PREPROCESS

bool H01indexedCSVdatabasePreprocessClass::preprocess(const int preprocessMode, const string local_connectome_folder_base)
{
	bool result = true;
	
	#ifdef INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_FROM_MATRIX
	if(preprocessMode == PREPROCESS_MODE_INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_FROM_MATRIX)
	{
		generateLocalConnectomeConnectionsDatasetFromMatrix(local_connectome_folder_base, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS);
	}
	#endif
	#ifdef INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_NEURONS_DATASET_FROM_SKELETONS
	else if(preprocessMode == PREPROCESS_MODE_INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_NEURONS_DATASET_FROM_SKELETONS)
	{
		generateLocalConnectomeNeuronsDatasetFromSkeletons(local_connectome_folder_base, LOCAL_CONNECTOME_DATASET_NEURONS_FILENAME);
	}
	#endif
	else
	{
		cerr << "H01indexedCSVdatabasePreprocessClass::preprocess error: preprocessMode unknown: " << preprocessMode << endl;
		exit(EXIT_ERROR);
	}
	
	return result;
}

#ifdef INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_NEURONS_DATASET_FROM_SKELETONS

void H01indexedCSVdatabasePreprocessClass::generateLocalConnectomeNeuronsDatasetFromSkeletons(const string local_connectome_folder_base, const string neuronDatasetFileNameWrite)
{	
	vector<string> neuronIDlist = getNeuronIDlist(local_connectome_folder_base, true);
	vector<string> neuronTypeList = getNeuronTypeList(local_connectome_folder_base, &neuronIDlist);
	vector<vecString> neuronPOSlist = getNeuronPosList(local_connectome_folder_base, &neuronIDlist);
	
	string neuronDatasetDataString = string(LOCAL_CONNECTOME_DATASET_NEURONS_HEADER) + STRING_NEWLINE;
	for(int neuronIndex=0; neuronIndex<neuronIDlist.size(); neuronIndex++)
	{
		string excitationTypeString = SHAREDvars.convertIntToString(getNeuronExcitationTypeFromType(neuronTypeList[neuronIndex]));
		string neuronDatasetLine = neuronIDlist[neuronIndex] + STRING_SPACE + neuronPOSlist[neuronIndex].x + STRING_SPACE + neuronPOSlist[neuronIndex].y + STRING_SPACE + neuronPOSlist[neuronIndex].z + STRING_SPACE + neuronTypeList[neuronIndex] + STRING_SPACE + excitationTypeString; 
		neuronDatasetDataString += neuronDatasetLine + STRING_NEWLINE;
	}
	
	bool write = true;
	if(write)
	{
		bool appendToFile = false;
		ofstream writeFileObject = H01indexedCSVdatabaseOperations.prepareLocalConnectomeDatasetWrite(write, appendToFile, neuronDatasetFileNameWrite);
		SHAREDvars.writeStringToFileObject(neuronDatasetDataString, &writeFileObject);
		H01indexedCSVdatabaseOperations.finaliseLocalConnectomeDatasetWrite(write, &writeFileObject);
	}
}

vector<string> H01indexedCSVdatabasePreprocessClass::getNeuronIDlist(const string local_connectome_folder_base, const bool writeToFile)
{
	vector<string> neuronIDlist;

	H01indexedCSVdatabaseOperations.prepareLocalConnectomeDatasetRead(local_connectome_folder_base);
	int smSkeletonIDsDataFileSize = 0;
	vector<vector<string>> smSkeletonIDsData;
	bool expectFirstLineHeader = true;
	SHAREDvars.getLinesFromFileCSV(INDEXED_CSV_DATABASE_PREPROCESS_SM_SKELETON_IDS_FILE_NAME, &smSkeletonIDsData, &smSkeletonIDsDataFileSize, CSV_DELIMITER_CHAR, expectFirstLineHeader);
	
	string neuronIDlistDataString;
	for(int neuronIndex=0; neuronIndex<smSkeletonIDsDataFileSize; neuronIndex++)
	{
		vector<string>* row = &(smSkeletonIDsData[neuronIndex]);
		string neuronID = (*row)[0];
		neuronIDlist.push_back(neuronID);
		neuronIDlistDataString += neuronID + STRING_NEWLINE;
	}
	
	if(writeToFile)
	{
		bool appendToFile = false;
		ofstream writeFileObject = H01indexedCSVdatabaseOperations.prepareLocalConnectomeDatasetWrite(writeToFile, appendToFile, INDEXED_CSV_DATABASE_PREPROCESS_NEURON_LIST_FILE_NAME);
		SHAREDvars.writeStringToFileObject(neuronIDlistDataString, &writeFileObject);
		H01indexedCSVdatabaseOperations.finaliseLocalConnectomeDatasetWrite(writeToFile, &writeFileObject);
	}
	
	return neuronIDlist;
}

vector<string> H01indexedCSVdatabasePreprocessClass::getNeuronTypeList(const string local_connectome_folder_base, vector<string>* neuronIDlist)
{	
	vector<string> neuronTypeList;

	map<string, int> neuronMap;
	for(int neuronIndex=0; neuronIndex<neuronIDlist->size(); neuronIndex++)
	{
		string neuronID = (*neuronIDlist)[neuronIndex];
		//int neuronIDint = SHAREDvars.convertStringToInt(neuronID);
    	(neuronMap)[neuronID] = neuronIndex;
		neuronTypeList.push_back(LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_TYPE_UNKNOWN);
	}	

	H01indexedCSVdatabaseOperations.prepareLocalConnectomeDatasetRead(local_connectome_folder_base);
	
	#ifdef INDEXED_CSV_DATABASE_PREPROCESS_SM_CELLTYPE_DATAFILE_2
	{
		int smCelltypeDataFileSize = 0;
		vector<vector<string>> smCelltypeData;
		bool expectFirstLineHeader = true;
		SHAREDvars.getLinesFromFileCSV(INDEXED_CSV_DATABASE_PREPROCESS_SM_CELLTYPE_DATAFILE_2_NAME, &smCelltypeData, &smCelltypeDataFileSize, CSV_DELIMITER_CHAR, expectFirstLineHeader);
		for(int rowIndex=0; rowIndex<smCelltypeDataFileSize; rowIndex++)
		{
			vector<string>* row = &(smCelltypeData[rowIndex]);
			string rowNeuronID1 = (*row)[INDEXED_CSV_DATABASE_PREPROCESS_SM_CELLTYPE_DATAFILE_2_FIELD_INDEX_SKID1];
			string rowNeuronID2 = (*row)[INDEXED_CSV_DATABASE_PREPROCESS_SM_CELLTYPE_DATAFILE_2_FIELD_INDEX_SKID2];
			string rowNeuronType = (*row)[INDEXED_CSV_DATABASE_PREPROCESS_SM_CELLTYPE_DATAFILE_2_FIELD_INDEX_CELLTYPE];
			/*
			cout << "rowNeuronID1 = " << rowNeuronID1 << endl;
			cout << "rowNeuronID2 = " << rowNeuronID2 << endl;
			cout << "rowNeuronType = " << rowNeuronType << endl;
			*/
			if(neuronMap.find(rowNeuronID1) != neuronMap.end())
			{
				int neuronListIndex = neuronMap[rowNeuronID1];
				neuronTypeList[neuronListIndex] = rowNeuronType;
			}
			if(neuronMap.find(rowNeuronID2) != neuronMap.end())
			{
				int neuronListIndex = neuronMap[rowNeuronID2];
				neuronTypeList[neuronListIndex] = rowNeuronType;
			}
		}
	}
	#endif
	#ifdef INDEXED_CSV_DATABASE_PREPROCESS_SM_CELLTYPE_DATAFILE_34
	{
		int smCelltypeDataFileSize = 0;
		vector<vector<string>> smCelltypeData;
		bool expectFirstLineHeader = true;
		SHAREDvars.getLinesFromFileCSV(INDEXED_CSV_DATABASE_PREPROCESS_SM_CELLTYPE_DATAFILE_3_NAME, &smCelltypeData, &smCelltypeDataFileSize, CSV_DELIMITER_CHAR, expectFirstLineHeader);
		SHAREDvars.getLinesFromFileCSV(INDEXED_CSV_DATABASE_PREPROCESS_SM_CELLTYPE_DATAFILE_4_NAME, &smCelltypeData, &smCelltypeDataFileSize, CSV_DELIMITER_CHAR, expectFirstLineHeader);
		for(int rowIndex=0; rowIndex<smCelltypeDataFileSize; rowIndex++)
		{
			vector<string>* row = &(smCelltypeData[rowIndex]);
			string rowNeuronID = (*row)[INDEXED_CSV_DATABASE_PREPROCESS_SM_CELLTYPE_DATAFILE_34_FIELD_INDEX_SKID];
			string rowNeuronType = (*row)[INDEXED_CSV_DATABASE_PREPROCESS_SM_CELLTYPE_DATAFILE_34_FIELD_INDEX_CELLTYPE];
			/*
			cout << "rowNeuronID = " << rowNeuronID << endl;
			cout << "rowNeuronType = " << rowNeuronType << endl;
			*/
			if(neuronMap.find(rowNeuronID) != neuronMap.end())
			{
				int neuronListIndex = neuronMap[rowNeuronID];
				neuronTypeList[neuronListIndex] = rowNeuronType;
			}
			else
			{
				cerr << "H01indexedCSVdatabasePreprocessClass::getNeuronTypeList error: key not found:  rowNeuronID = " << rowNeuronID << endl;
			}
		}
	}
	#endif
	
	return neuronTypeList;
}
	
vector<vecString> H01indexedCSVdatabasePreprocessClass::getNeuronPosList(const string local_connectome_folder_base, vector<string>* neuronIDlist)
{	
	vector<vecString> neuronPOSlist;

	map<string, int> neuronMap;
	for(int neuronIndex=0; neuronIndex<neuronIDlist->size(); neuronIndex++)
	{
		string neuronID = (*neuronIDlist)[neuronIndex];
		string skeletonFileName = string(INDEXED_CSV_DATABASE_PREPROCESS_SKELETON_DATAFILE_NAME_START) + neuronID + INDEXED_CSV_DATABASE_PREPROCESS_SKELETON_DATAFILE_NAME_EXTENSION;
		string skeletonsFolderName = string(LOCAL_CONNECTOME_DATASET_FOLDER) + INDEXED_CSV_DATABASE_PREPROCESS_SKELETON_DATAFILE_FOLDER_NAME;
		/*
		cout << "skeletonFileName = " << skeletonFileName << endl;
		cout << "skeletonsFolderName = " << skeletonsFolderName << endl;
		*/
		H01indexedCSVdatabaseOperations.prepareLocalConnectomeDatasetReadCustom(local_connectome_folder_base, skeletonsFolderName);
		int skeletonDataFileSize = 0;
		vector<vector<string>> skeletonData;
		bool expectFirstLineHeader = false;
		SHAREDvars.getLinesFromFileCSV(skeletonFileName, &skeletonData, &skeletonDataFileSize, CHAR_SPACE, expectFirstLineHeader);
		
		vec neuronPosCentroid = {0.0, 0.0, 0.0};
		for(int nodeIndex=0; nodeIndex<skeletonData.size(); nodeIndex++)
		{
			vec pos; 
			vector<string>* row = &(skeletonData[nodeIndex]);						
			pos.x = SHAREDvars.convertStringToDouble((*row)[INDEXED_CSV_DATABASE_PREPROCESS_SKELETON_DATAFILE_NODE_FIELD_INDEX_X]);
			pos.y = SHAREDvars.convertStringToDouble((*row)[INDEXED_CSV_DATABASE_PREPROCESS_SKELETON_DATAFILE_NODE_FIELD_INDEX_Y]);
			pos.z = SHAREDvars.convertStringToDouble((*row)[INDEXED_CSV_DATABASE_PREPROCESS_SKELETON_DATAFILE_NODE_FIELD_INDEX_Z]);
			/*
			cout << "pos.x = " << pos.x << endl;
			cout << "pos.y = " << pos.y << endl;
			cout << "pos.z = " << pos.z << endl;
			*/
			neuronPosCentroid.x += pos.x;
			neuronPosCentroid.y += pos.y;
			neuronPosCentroid.z += pos.z;
		}
		neuronPosCentroid.x /= skeletonData.size();
		neuronPosCentroid.y /= skeletonData.size();
		neuronPosCentroid.z /= skeletonData.size();
		
		vecString neuronPosCentroidString;
		string doubleFormat = "%0.3f";
		neuronPosCentroidString.x = SHAREDvars.convertDoubleToString(neuronPosCentroid.x, doubleFormat);
		neuronPosCentroidString.y = SHAREDvars.convertDoubleToString(neuronPosCentroid.y, doubleFormat);
		neuronPosCentroidString.z = SHAREDvars.convertDoubleToString(neuronPosCentroid.z, doubleFormat);
		
		neuronPOSlist.push_back(neuronPosCentroidString);
	}	

	return neuronPOSlist;
}

#endif

#ifdef INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET_FROM_MATRIX

void H01indexedCSVdatabasePreprocessClass::generateLocalConnectomeConnectionsDatasetFromMatrix(const string local_connectome_folder_base, const string connectionDatasetFileNameWrite)
{
	ofstream writeFileObject = H01indexedCSVdatabaseOperations.prepareLocalConnectomeDatasetWrite(true, false, connectionDatasetFileNameWrite);

	H01indexedCSVdatabaseOperations.prepareLocalConnectomeDatasetRead(local_connectome_folder_base);
	
	string neuronConnectionsFileHeader = string(LOCAL_CONNECTOME_DATASET_CONNECTIONS_HEADER) + STRING_NEWLINE;
	SHAREDvars.writeStringToFileObject(neuronConnectionsFileHeader, &writeFileObject);

	for(int matrixFileIndex=0; matrixFileIndex<LOCAL_CONNECTOME_CONNECTIONS_MATRIX_NUMBER_OF_FILES; matrixFileIndex++)
	{
		cout << "matrixFileIndex = " << matrixFileIndex << endl;
		string matrixFileName = localConnectomeConnectionsMatrixFileNames[matrixFileIndex];
		string matrixFilePreClassLabel = localConnectomeConnectionsMatrixSynapsePreClassLabel[matrixFileIndex];
		string matrixFilePostClassLabel = localConnectomeConnectionsMatrixSynapsePostClassLabel[matrixFileIndex];
		convertConnectionsListMatrixFile(local_connectome_folder_base, matrixFileName, matrixFilePreClassLabel, matrixFilePostClassLabel, &writeFileObject);
	} 
	
	H01indexedCSVdatabaseOperations.finaliseLocalConnectomeDatasetWrite(true, &writeFileObject);
}

void H01indexedCSVdatabasePreprocessClass::convertConnectionsListMatrixFile(const string local_connectome_folder_base, const string matrixFileName, const string matrixFilePreClassLabel, const string matrixFilePostClassLabel, ofstream* writeFileObject)
{	
	//matrix format: Row neurons are presynaptic, while column neurons are postsynaptic, first row/column contain neuronID (skid)
		
	int matrixFileSize = 0;
	vector<vector<string>> matrixFileData;
	bool expectFirstLineHeader = false;
	SHAREDvars.getLinesFromFileCSV(matrixFileName, &matrixFileData, &matrixFileSize, CSV_DELIMITER_CHAR, expectFirstLineHeader);
	int numberOfNeurons = matrixFileSize-1;	//ignore header
	
	cout << "matrixFileName = " << matrixFileName << endl;
	cout << "numberOfNeurons = " << numberOfNeurons << endl;
	
	vector<string> neuronIDlist = matrixFileData[0];	//skid
	neuronIDlist.erase(neuronIDlist.begin());
	/*
	vector<string> matrixFileHeader = matrixFileData[0];
	vector<string> neuronIDlist;
	for(int neuronIndex=1; neuronIndex<matrixFileSize; neuronIndex++)
	{
		string neuronID = matrixFileHeader[neuronIndex];	//skid
		neuronIDlist.push_back(neuronID);
	}
	*/
		
	for(int sourceNeuronIndex=0; sourceNeuronIndex<numberOfNeurons; sourceNeuronIndex++)
	{
		vector<string> matrixFileDataRow = matrixFileData[sourceNeuronIndex+1];
		for(int targetNeuronIndex=0; targetNeuronIndex<numberOfNeurons; targetNeuronIndex++)
		{
			string matrixFileDataCell = matrixFileDataRow[targetNeuronIndex+1];
			int connectionNumberOfSynapsesInt = int(SHAREDvars.convertStringToFloat(matrixFileDataCell));
			if(connectionNumberOfSynapsesInt > 0)
			{
				string connectionNumberOfSynapses = SHAREDvars.convertIntToString(connectionNumberOfSynapsesInt);

				string connectionsSourceNeuronID = neuronIDlist[sourceNeuronIndex];
				string connectionsTargetNeuronID = neuronIDlist[targetNeuronIndex];

				string neuronConnectionString = 
				connectionsSourceNeuronID + CSV_DELIMITER + 
				connectionsTargetNeuronID + CSV_DELIMITER +  
				matrixFilePreClassLabel + CSV_DELIMITER + matrixFilePostClassLabel + CSV_DELIMITER + connectionNumberOfSynapses + STRING_NEWLINE;
				/*
				string neuronConnectionString = 
				connectionsSourceNeuronID + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionsSourceNeuronCoordinates.x) + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionsSourceNeuronCoordinates.y) + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionsSourceNeuronCoordinates.z) + CSV_DELIMITER + connectionsSourceNeuronType + CSV_DELIMITER + 
				connectionsTargetNeuronID + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionsTargetNeuronCoordinates.x) + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionsTargetNeuronCoordinates.y) + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionsTargetNeuronCoordinates.z) + CSV_DELIMITER + connectionsTargetNeuronType + CSV_DELIMITER + 
				matrixFileClassLabel + CSV_DELIMITER + connectionNumberOfSynapses + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionExcitationType) + STRING_NEWLINE;
				*/

				//cout << "neuronConnectionString = " << neuronConnectionString << endl;

				SHAREDvars.writeStringToFileObject(neuronConnectionString, writeFileObject);
			}
		}
	}
}

#endif


int H01indexedCSVdatabasePreprocessClass::getNeuronExcitationTypeFromType(string neuronType)
{
	//TODO
	int excitationType = LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY;
	return excitationType;
}

#endif
