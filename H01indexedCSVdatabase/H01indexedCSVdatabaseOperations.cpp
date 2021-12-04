/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseOperations.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: H01LocalConnectome
 * Requirements: see H01indexedCSVdatabase.hpp
 * Compilation: see H01indexedCSVdatabase.hpp
 * Usage: see H01indexedCSVdatabase.hpp
 * Description: H01 indexed CSV database operations
 * Comments:
 * /
 *******************************************************************************/

#include "H01indexedCSVdatabaseOperations.hpp"

double H01indexedCSVdatabaseOperationsClass::calibrateCoordinateX(const double csvDatabaseCoordinateX)
{
	double visualisationCoordinateX = (csvDatabaseCoordinateX*LOCAL_CONNECTOME_VISUALISATION_CALIBRATION_FACTOR_X) + LOCAL_CONNECTOME_VISUALISATION_CALIBRATION_MIN_X;
	return visualisationCoordinateX;
}
double H01indexedCSVdatabaseOperationsClass::calibrateCoordinateY(const double csvDatabaseCoordinateY)
{
	double visualisationCoordinateY = (csvDatabaseCoordinateY*LOCAL_CONNECTOME_VISUALISATION_CALIBRATION_FACTOR_Y) + LOCAL_CONNECTOME_VISUALISATION_CALIBRATION_MIN_Y;
	return visualisationCoordinateY;
}
double H01indexedCSVdatabaseOperationsClass::calibrateCoordinateZ(const double csvDatabaseCoordinateZ)
{
	double visualisationCoordinateZ = (csvDatabaseCoordinateZ*LOCAL_CONNECTOME_VISUALISATION_CALIBRATION_FACTOR_Z) + LOCAL_CONNECTOME_VISUALISATION_CALIBRATION_MIN_Z;
	return visualisationCoordinateZ;
}

bool H01indexedCSVdatabaseOperationsClass::readLocalConnectomeNeuronsCSVdataset(const string LocalConnectomeCSVdatasetFileName, vector<vector<string>>* localConnectomeNeuronsCSVdataset, int* localConnectomeNeuronsCSVdatasetSize, vector<string>* neuronList, map<string, int>* neuronMap)
{
	SHAREDvars.getLinesFromFileCSV(LocalConnectomeCSVdatasetFileName, localConnectomeNeuronsCSVdataset, localConnectomeNeuronsCSVdatasetSize, CSV_DELIMITER_CHAR, true);
	
	#ifdef LOCAL_CONNECTOME_OFFICAL_RELEASE_C3_SOMAS	
	for(int i=0; i<localConnectomeNeuronsCSVdataset->size(); i++)
	{
		vector<string>* localConnectomeNeuron = &((*localConnectomeNeuronsCSVdataset)[i]);
		#ifdef LOCAL_CONNECTOME_OFFICAL_RELEASE_C3_SOMAS_EXPECT_STRING_DELIMITERS
		//reformat string cells;
		for(int j=0; j<localConnectomeNeuron->size(); j++)
		{
			string cell = (*localConnectomeNeuron)[j];	
			string cellFormatted = "";
			if((cell[0] == STRING_DELIMITER) && (cell[cell.length()-1] == STRING_DELIMITER))
			{
				cellFormatted = cell.substr(1, cell.length()-2);
				//cout << "cellFormatted = " << cellFormatted << endl;
			}
			(*localConnectomeNeuron)[j] = cellFormatted;
		}
		#endif
		#ifdef LOCAL_CONNECTOME_OFFICAL_RELEASE_C3_SOMAS_LAYERS
		//reformat layer cells;
		string layerString = (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER];
		int layerIndex = INT_DEFAULT_VALUE;	
		//cout << "layerString = " << layerString << endl;
		if(SHAREDvars.textInTextArray(layerString, C3somasLayerNames, CORTICAL_LAYER_NUMBER_OF_LAYERS, &layerIndex))
		{
			layerIndex++;
		}
		else
		{
			//"unclassified", "NULL"
			layerIndex = 0;
			//cerr << "LOCAL_CONNECTOME_OFFICAL_RELEASE_C3_SOMAS_LAYERS: H01indexedCSVdatabaseOperationsClass::readLocalConnectomeNeuronsCSVdataset error: !textInTextArray, layerIndex name unknown; layerString = " << layerString << endl;
		}
		//cout << "layerIndex = " << layerIndex << endl;
		(*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER] = SHAREDvars.convertIntToString(layerIndex);
		#endif
		string neuronType = (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE];
		string neuronExcitationTypeString = SHAREDvars.convertIntToString(LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN);
		if(neuronType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_PYRAMIDAL)
		{
			neuronExcitationTypeString = SHAREDvars.convertIntToString(LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY);
		}
		else if(neuronType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_INTERNEURON)
		{
			neuronExcitationTypeString = SHAREDvars.convertIntToString(LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY);
		}
		else
		{
			neuronExcitationTypeString = SHAREDvars.convertIntToString(LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN);		
		}
		//cout << "neuronType = " << neuronType << endl;
		//cout << "neuronExcitationTypeString = " << neuronExcitationTypeString << endl;
		localConnectomeNeuron->push_back(neuronExcitationTypeString);	//LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE	
	}
	#ifdef LOCAL_CONNECTOME_OFFICAL_RELEASE_C3_SOMAS_IGNORE_ADDITIONAL_NEURON_TYPES
	for(int i=0; i<localConnectomeNeuronsCSVdataset->size(); i++)
	{	
		vector<string>* localConnectomeNeuron = &((*localConnectomeNeuronsCSVdataset)[i]);
		int neuronExcitationType = SHAREDvars.convertStringToInt((*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE]);
		if(neuronExcitationType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN)
		{
			//remove neuron from localConnectomeNeuronsCSVdataset
			//cout << "remove neuron from localConnectomeNeuronsCSVdataset" << endl;
			localConnectomeNeuronsCSVdataset->erase(localConnectomeNeuronsCSVdataset->begin() + i);
			i--;
		}
	}
	#endif
	#endif

	for(int i=0; i<localConnectomeNeuronsCSVdataset->size(); i++)
	{
		vector<string>* localConnectomeNeuron = &((*localConnectomeNeuronsCSVdataset)[i]);
		
		//populate neuronList;
		string neuronID = (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_NEURON_ID];
		//cout << "neuronID = " << neuronID << endl;
		neuronList->push_back(neuronID);		
	}
	
	cout << "neuronList->size() = " << neuronList->size() << endl;
		
	#ifdef INDEXED_CSV_DATABASE_QUERY_EFFICIENT_STORE_NEURON_IDS_IN_MAP
	//populate neuronMap;
	for(int i=0; i < neuronList->size(); i++)
	{
    	(*neuronMap)[(*neuronList)[i]] = i;
	}
	#endif
}

bool H01indexedCSVdatabaseOperationsClass::readLocalConnectomeConnectionsCSVdataset(const string LocalConnectomeCSVdatasetFileName, vector<vector<string>>* localConnectomeConnectionsCSVdataset, int* localConnectomeConnectionsCSVdatasetSize)
{
	SHAREDvars.getLinesFromFileCSV(LocalConnectomeCSVdatasetFileName, localConnectomeConnectionsCSVdataset, localConnectomeConnectionsCSVdatasetSize, CSV_DELIMITER_CHAR, true);
	
	/*
	for(int i=0;i<localConnectomeConnectionsCSVdataset->size(); i++)
	{
		vector<string> localConnectionCSVdatasetNeuron = (*localConnectomeConnectionsCSVdataset)[i];
			
		cout << "localConnectionCSVdatasetNeuron: i = " << i << endl;
		for(int j=0;j<localConnectionCSVdatasetNeuron.size(); j++)
		{
			string cell = localConnectionCSVdatasetNeuron[j];
			cout << "cell = " << cell << endl;			
		}
	}
	*/		
}

