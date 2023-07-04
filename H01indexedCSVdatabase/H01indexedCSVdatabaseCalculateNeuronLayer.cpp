/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseCalculateNeuronLayer.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021-2023 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: H01LocalConnectome
 * Requirements: see H01indexedCSVdatabaseGlobalDefs.hpp
 * Compilation: see H01indexedCSVdatabaseGlobalDefs.hpp
 * Usage: see H01indexedCSVdatabaseGlobalDefs.hpp
 * Description: H01 indexed CSV database calculate neuron layer
 * Input: corticalLayersBoundaryKeypoints.csv
 * Output Format: vector<vector<vec>> corticalLayersKeypoints
 * Comments:
 * /
 *******************************************************************************/

#include "H01indexedCSVdatabaseCalculateNeuronLayer.hpp"

void initialiseGaussianQuad(GaussianQuad* gaussianQuad)
{
	gaussianQuad->numberConnections = 0;
	initialiseQuad(&(gaussianQuad->mean));
	initialiseQuad(&(gaussianQuad->variance));
	initialiseQuad(&(gaussianQuad->count));
}
void initialiseQuad(Quad* q)
{
	q->xPos = 0.0;
	q->yPos = 0.0;
	q->xNeg = 0.0;
	q->yNeg = 0.0;
}
	
//#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS
H01connectivityModelClass::H01connectivityModelClass(void)
{
	numberOfLocalConnectomeNeurons = 0;
	numberOfLocalConnectomeNeuronsExcitatory = 0;
	numberOfLocalConnectomeNeuronsInhibitory = 0;
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_NEURONS
	for(int r=0; r<INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_NUMBER_ITERATIONS; r++)
	{
		numberOfNeuronsWithRecurrentConnections[r] = 0;
		numberOfNeuronsWithRecurrentConnectionsExcitatory[r] = 0;
		numberOfNeuronsWithRecurrentConnectionsInhibitory[r] = 0;
	}
	#endif
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_PRINT_OUTPUT_VERBOSE_LOCALORNONLOCAL
	numberConnectionsLocalOrNonLocalConnectomeExcitatory = 0;
	numberConnectionsLocalOrNonLocalConnectomeInhibitory = 0;
	#endif
	initialiseGaussianQuad(&neuronModelConnectionsLocalConnectome);
	initialiseGaussianQuad(&neuronModelConnectionsLocalConnectomeExcitatory);
	initialiseGaussianQuad(&neuronModelConnectionsLocalConnectomeInhibitory);
	initialiseGaussianQuad(&neuronModelConnectionsExternalConnectome);
	initialiseGaussianQuad(&neuronModelConnectionsExternalConnectomeExcitatory);
	initialiseGaussianQuad(&neuronModelConnectionsExternalConnectomeInhibitory);
	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT
	for(int r=0; r<INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_NUMBER_ITERATIONS; r++)
	{
		initialiseGaussianQuad(&neuronModelConnectionsLocalConnectomeRecurrent[r]);
		initialiseGaussianQuad(&neuronModelConnectionsLocalConnectomeExcitatoryRecurrent[r]);
		initialiseGaussianQuad(&neuronModelConnectionsLocalConnectomeInhibitoryRecurrent[r]);
		#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_COUNT_CONNECTIONS_RECURRENT_NEURONS
		//map<string, int> neuronsWithRecurrentConnectionMap = ;
		//map<string, int> neuronsWithRecurrentConnectionExcitatoryMap = ;
		//map<string, int> neuronsWithRecurrentConnectionInhibitoryMap = ;
		#endif
	}
	#endif
}
H01connectivityModelClass::~H01connectivityModelClass(void)
{
}
vector<H01connectivityModelClass>* H01indexedCSVdatabaseCalculateNeuronLayerClass::generateNumberOfConnectionsLayers()
{
	vector<H01connectivityModelClass>* numberConnectionsLayers = new vector<H01connectivityModelClass>(CORTICAL_LAYER_NUMBER_OF_LAYERS+1);
	return numberConnectionsLayers;
}
vector<vector<H01connectivityModelClass>>* H01indexedCSVdatabaseCalculateNeuronLayerClass::generateNumberOfConnectionsLayersLayers()
{
	vector<vector<H01connectivityModelClass>>* numberConnectionsLayersLayers = new vector<vector<H01connectivityModelClass>>(CORTICAL_LAYER_NUMBER_OF_LAYERS+1, vector<H01connectivityModelClass>(CORTICAL_LAYER_NUMBER_OF_LAYERS+1));
	return numberConnectionsLayersLayers;
}
//#endif
	
double H01indexedCSVdatabaseCalculateNeuronLayerClass::getZNormalisationFactor()
{
	#ifdef INDEXED_CSV_DATABASE_H01
	double normalisationFactorZ = double(AVRO_JSON_DATABASE_COORDINATES_RANGE_X*AVRO_JSON_DATABASE_COORDINATES_CALIBRATION_X + AVRO_JSON_DATABASE_COORDINATES_RANGE_Y*AVRO_JSON_DATABASE_COORDINATES_CALIBRATION_Y)/2.0 / (AVRO_JSON_DATABASE_COORDINATES_RANGE_Z*AVRO_JSON_DATABASE_COORDINATES_CALIBRATION_Z);
	#elif defined INDEXED_CSV_DATABASE_DC
	double normalisationFactorZ = 1.0;	//CHECKTHIS
	#endif
	return normalisationFactorZ;
}
	
bool H01indexedCSVdatabaseCalculateNeuronLayerClass::readLocalNeuronsAndConnections(const string local_connectome_folder_base, const string neuronDatasetOrListFileName, const bool neuronListIsDataset, vector<string>* neuronList, map<string, int>* neuronMap, vector<vector<string>>* localConnectomeCSVdatasetNeurons, const bool readConnections, const string neuronListConnectionsFileName, vector<vector<string>>* localConnectomeCSVdatasetConnections, const bool buildConnectionsMap, map<string, int>* connectionsMap, const bool readLayers, const bool readLayersConnections, const bool prepareRead)
{
	bool result = true;

	cout << "neuronDatasetOrListFileName = " << neuronDatasetOrListFileName << endl;
	cout << "neuronListConnectionsFileName = " << neuronListConnectionsFileName << endl;
		
	//read neurons;
	if(prepareRead)
	{
		H01indexedCSVdatabaseOperations.prepareLocalConnectomeDatasetRead(local_connectome_folder_base);
	}
	
	int neuronListFileLength = 0;
	
	#ifdef INDEXED_CSV_DATABASE_QUERY_READ_DATASET_LOCAL_CONNECTOME_NEURONS	
	if(neuronListIsDataset)
	{
		int localNeuronCSVdatasetNeuronsSize = 0;
		H01indexedCSVdatabaseOperations.readLocalConnectomeNeuronsCSVdataset(neuronDatasetOrListFileName, localConnectomeCSVdatasetNeurons, &neuronListFileLength, neuronList, neuronMap);
	}
	else
	{
	#endif		
		SHAREDvars.getLinesFromFile(neuronDatasetOrListFileName, neuronList, &neuronListFileLength);
	#ifdef INDEXED_CSV_DATABASE_QUERY_READ_DATASET_LOCAL_CONNECTOME_NEURONS
	}
	#endif
	
	//read connections;
	if(readConnections)
	{
		int localConnectionCSVdatasetConnectionsSize = 0;
		H01indexedCSVdatabaseOperations.readLocalConnectomeConnectionsCSVdataset(neuronListConnectionsFileName, localConnectomeCSVdatasetConnections, &localConnectionCSVdatasetConnectionsSize, buildConnectionsMap, connectionsMap);
	}
	
	//read layers;
	#ifdef INDEXED_CSV_DATABASE_NEURON_LAYERS
	#ifdef INDEXED_CSV_DATABASE_H01
	if(readLayers)
	{
		calculateLocalConnectomeLayers(localConnectomeCSVdatasetNeurons, localConnectomeCSVdatasetConnections, neuronMap, readLayersConnections);
	}
	#elif defined INDEXED_CSV_DATABASE_DC
	calculateNeuronLayersDC(localConnectomeCSVdatasetNeurons);
	#endif
	#endif
	
	#ifdef INDEXED_CSV_DATABASE_QUERY_READ_DATASET_LOCAL_CONNECTOME_CONNECTIONS_DYNAMICALLY_GENERATE_REDUNDANT_FIELDS
	if(readConnections)
	{
		dynamicallyGenerateLocalConnectomeConnectionsRedundantFields(localConnectomeCSVdatasetNeurons, localConnectomeCSVdatasetConnections, neuronMap);
	}
	#endif
	
	return result;
}

#ifdef INDEXED_CSV_DATABASE_QUERY_READ_DATASET_LOCAL_CONNECTOME_CONNECTIONS_DYNAMICALLY_GENERATE_REDUNDANT_FIELDS
bool H01indexedCSVdatabaseCalculateNeuronLayerClass::dynamicallyGenerateLocalConnectomeConnectionsRedundantFields(vector<vector<string>>* localConnectomeCSVdatasetNeurons, vector<vector<string>>* localConnectomeCSVdatasetConnections, map<string, int>* neuronMap)
{
	for(int i=0; i<localConnectomeCSVdatasetConnections->size(); i++)
	{
		vector<string>* localConnectionCSVdatasetLine = &((*localConnectomeCSVdatasetConnections)[i]);
		string sourceNeuronID = (*localConnectionCSVdatasetLine)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_ID];
		string targetNeuronID = (*localConnectionCSVdatasetLine)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_ID];	
		
		string layerIndexPre = "";
		string layerIndexPost = "";
		if(neuronMap->count(sourceNeuronID) != 0)	//verify that sourceNeuronID is in neuronList
		{
			int neuronIndex = (*neuronMap)[sourceNeuronID];
			vector<string>* localConnectomeNeuron = &((*localConnectomeCSVdatasetNeurons)[neuronIndex]);	
			vec pos;
			string posx = (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_X];
			string posy = (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Y];
			string posz = (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Z];
			string neuronType = (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE];	//C3 database neuron classification (PYRAMIDAL, INTERNEURON)
			layerIndexPre = (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER];
						
			localConnectionCSVdatasetLine->push_back(posx);	//LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_X	
			localConnectionCSVdatasetLine->push_back(posy);	//LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_Y	
			localConnectionCSVdatasetLine->push_back(posz);	//LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_Z	
			localConnectionCSVdatasetLine->push_back(neuronType);	//LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_TYPE	
		}
		else
		{
			cerr << "H01indexedCSVdatabaseCalculateNeuronLayerClass::dynamicallyGenerateLocalConnectomeConnectionsRedundantFields warning: sourceNeuronID cannot be found; " << sourceNeuronID << endl;
			exit(EXIT_ERROR);
		}
		
		if(neuronMap->count(targetNeuronID) != 0)	//verify that sourceNeuronID is in neuronList
		{
			int neuronIndex = (*neuronMap)[targetNeuronID];
			vector<string>* localConnectomeNeuron = &((*localConnectomeCSVdatasetNeurons)[neuronIndex]);	
			vec pos;
			string posx = (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_X];
			string posy = (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Y];
			string posz = (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Z];
			string neuronType = (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE];	//C3 database neuron classification (PYRAMIDAL, INTERNEURON)
			layerIndexPost = (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER];
			
			localConnectionCSVdatasetLine->push_back(posx);	//LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_X	
			localConnectionCSVdatasetLine->push_back(posy);	//LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_Y	
			localConnectionCSVdatasetLine->push_back(posz);	//LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_Z	
			localConnectionCSVdatasetLine->push_back(neuronType);	//LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_TYPE	
		}
		else
		{
			cerr << "H01indexedCSVdatabaseCalculateNeuronLayerClass::dynamicallyGenerateLocalConnectomeConnectionsRedundantFields warning: sourceNeuronID cannot be found; " << sourceNeuronID << endl;
			exit(EXIT_ERROR);
		}
		
		#ifdef INDEXED_CSV_DATABASE_NEURON_LAYERS
		localConnectionCSVdatasetLine->push_back(layerIndexPre);	//LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_ARTIFICIAL_LAYER	
		localConnectionCSVdatasetLine->push_back(layerIndexPost);	//LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_ARTIFICIAL_LAYER
		#endif
	}
}
#endif


#ifdef INDEXED_CSV_DATABASE_NEURON_LAYERS
#ifdef INDEXED_CSV_DATABASE_H01

void H01indexedCSVdatabaseCalculateNeuronLayerClass::calculateLocalConnectomeLayers(vector<vector<string>>* localConnectomeCSVdatasetNeurons, vector<vector<string>>* localConnectomeCSVdatasetConnections, map<string, int>* neuronMap, const bool readConnections)
{
	#ifdef LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_LAYERS
	#ifdef LOCAL_CONNECTOME_DATASET_CONNECTIONS_REDUNDANT_DEPRECIATED	//ie #ifndef INDEXED_CSV_DATABASE_QUERY_READ_DATASET_LOCAL_CONNECTOME_CONNECTIONS_DYNAMICALLY_GENERATE_REDUNDANT_FIELDS
	if(readConnections)
	{
		transferLocalConnectomeNeuronLayersToConnectionsLayers(localConnectomeCSVdatasetNeurons, localConnectomeCSVdatasetConnections, neuronMap);
	}
	#endif
	#else
	//initialise connection/neuron layer indices;
	//int corticalLayersNumKeypointsMax;	//= 28	//maximum number keypoints (number cols/2)
	const string corticalLayersBoundaryKeypointTableFileName = CORTICAL_LAYER_BOUNDARY_KEYPOINT_TABLE_FILE_NAME;
	vector<vector<vec>> corticalLayersKeypoints;
	readCorticalLayersBoundaryKeypointTable(corticalLayersBoundaryKeypointTableFileName, &corticalLayersKeypoints);
	calculateNeuronLayers(true, localConnectomeCSVdatasetNeurons, &corticalLayersKeypoints);
	if(readConnections)
	{
		calculateNeuronLayers(false, localConnectomeCSVdatasetConnections, &corticalLayersKeypoints);
	}
	#endif
}
		
#ifdef LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_LAYERS
bool H01indexedCSVdatabaseCalculateNeuronLayerClass::transferLocalConnectomeNeuronLayersToConnectionsLayers(vector<vector<string>>* localConnectomeCSVdatasetNeurons, vector<vector<string>>* localConnectomeCSVdatasetConnections, map<string, int>* neuronMap)
{
	bool result = true;
	int corticalLayersNumLayers = CORTICAL_LAYER_NUMBER_OF_LAYERS;
	
	for(int i=0; i<localConnectomeCSVdatasetConnections->size(); i++)
	{
		vector<string>* localConnectionCSVdatasetLine = &((*localConnectomeCSVdatasetConnections)[i]);
		string sourceNeuronID = (*localConnectionCSVdatasetLine)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_ID];
		string targetNeuronID = (*localConnectionCSVdatasetLine)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_ID];	
		
		string layerIndexString = "";		
		if(neuronMap->count(sourceNeuronID) != 0)	//verify that sourceNeuronID is in neuronList
		{
			int neuronIndex = (*neuronMap)[sourceNeuronID];
			vector<string>* localConnectomeNeuron = &((*localConnectomeCSVdatasetNeurons)[neuronIndex]);	
			layerIndexString = (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER];
		}
		else
		{
			#ifdef LOCAL_CONNECTOME_VISUALISATION_BACKWARDS_COMPATIBILITY_WITH_ODS_GENERATED_FILES	//CHECKTHIS
			//assume before executing generatedLocalConnectionsDatasets:INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET (on indexed csv database and local connectome neurons list) or prepublishedLocalConnectionsDatasets:INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET (on ods generated datasets)
			layerIndexString = SHAREDvars.convertIntToString(CORTICAL_LAYER_UNKNOWN);	//TODO: verify this is supported
			//cerr << "H01indexedCSVdatabaseCalculateNeuronLayerClass::transferLocalConnectomeNeuronLayersToConnectionsLayers warning: sourceNeuronID cannot be found; " << sourceNeuronID << endl;	//approx 222 non-unique instances of unknown neurons being referenced by localConnectomeCSVdatasetConnections
			#else
			cerr << "H01indexedCSVdatabaseCalculateNeuronLayerClass::transferLocalConnectomeNeuronLayersToConnectionsLayers warning: sourceNeuronID cannot be found; " << sourceNeuronID << endl;
			exit(EXIT_ERROR);
			#endif
		}
		localConnectionCSVdatasetLine->push_back(layerIndexString);	//LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_ARTIFICIAL_LAYER	
	
		layerIndexString = "";
		if(neuronMap->count(targetNeuronID) != 0)	//verify that targetNeuronID is in neuronList
		{
			int neuronIndex = (*neuronMap)[targetNeuronID];
			vector<string>* localConnectomeNeuron = &((*localConnectomeCSVdatasetNeurons)[neuronIndex]);	
			layerIndexString = (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER];
		}
		else
		{
			#ifdef LOCAL_CONNECTOME_VISUALISATION_BACKWARDS_COMPATIBILITY_WITH_ODS_GENERATED_FILES	//CHECKTHIS
			//assume before executing generatedLocalConnectionsDatasets:INDEXED_CSV_DATABASE_QUERY_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET (on indexed csv database and local connectome neurons list) or prepublishedLocalConnectionsDatasets:INDEXED_CSV_DATABASE_QUERY_COMPLETE_LOCAL_CONNECTOME_CONNECTIONS_DATASET (on ods generated datasets)
			layerIndexString = SHAREDvars.convertIntToString(CORTICAL_LAYER_UNKNOWN);	//TODO: verify this is supported
			//cerr << "H01indexedCSVdatabaseCalculateNeuronLayerClass::transferLocalConnectomeNeuronLayersToConnectionsLayers warning: targetNeuronID cannot be found; " << targetNeuronID << endl;	//approx 222 non-unique instances of unknown neurons being referenced by localConnectomeCSVdatasetConnections
			#else
			cerr << "H01indexedCSVdatabaseCalculateNeuronLayerClass::transferLocalConnectomeNeuronLayersToConnectionsLayers warning: targetNeuronID cannot be found; " << targetNeuronID << endl;
			exit(EXIT_ERROR);
			#endif
		}
		localConnectionCSVdatasetLine->push_back(layerIndexString);	//LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_ARTIFICIAL_LAYER		
	}
	
	return result;
}
#endif

bool H01indexedCSVdatabaseCalculateNeuronLayerClass::readCorticalLayersBoundaryKeypointTable(const string corticalLayersBoundaryKeypointTableFileName, vector<vector<vec>>* corticalLayersKeypoints)
{
	bool result = true;
	
	vector<vector<string>> dataSetFile;
	int dataSetFileSize = 0;
	SHAREDvars.getLinesFromFileCSV(corticalLayersBoundaryKeypointTableFileName, &dataSetFile, &dataSetFileSize, CSV_DELIMITER_CHAR, true);
	for(int layerIndex=0; layerIndex<dataSetFile.size(); layerIndex++)
	{
		vector<string> corticalLayerKeypointsXY = dataSetFile[layerIndex];
		int corticalLayersNumKeypointsMax = corticalLayerKeypointsXY.size();
		vector<vec> corticalLayerKeypoints(corticalLayersNumKeypointsMax);
		for(int i=0; i<corticalLayersNumKeypointsMax; i=i+2)
		{
			int k = i/2;
			corticalLayerKeypoints[k].x = SHAREDvars.convertStringToDouble(corticalLayerKeypointsXY[i]);
			corticalLayerKeypoints[k].y = SHAREDvars.convertStringToDouble(corticalLayerKeypointsXY[i+1]);
			//cout << "corticalLayerKeypoints[k].x = " << corticalLayerKeypoints[k].x << endl;
			//cout << "corticalLayerKeypoints[k].y = " << corticalLayerKeypoints[k].y << endl;
		}
		corticalLayersKeypoints->push_back(corticalLayerKeypoints);
	}
	
	return result;	
}

bool H01indexedCSVdatabaseCalculateNeuronLayerClass::calculateNeuronLayers(const bool localConnectomeDatasetType, vector<vector<string>>* localConnectomeCSVdataset, vector<vector<vec>>* corticalLayersKeypoints)
{
	bool result = true;
	int corticalLayersNumLayers = CORTICAL_LAYER_NUMBER_OF_LAYERS;
	
	for(int i=0; i<localConnectomeCSVdataset->size(); i++)
	{
		vector<string>* localConnectomeCSVdatasetLine = &((*localConnectomeCSVdataset)[i]);
		
		if(localConnectomeDatasetType)	//localConnectomeCSVdatasetNeurons
		{
			double neuronX = H01indexedCSVdatabaseOperations.calibrateCoordinateX(SHAREDvars.convertStringToDouble((*localConnectomeCSVdatasetLine)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_X]));
			double neuronY = H01indexedCSVdatabaseOperations.calibrateCoordinateY(SHAREDvars.convertStringToDouble((*localConnectomeCSVdatasetLine)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Y]));
			vec neuronPos = {neuronX, neuronY, 0.0};
			vec layerSurfaceNormVectorNOTUSED = {0.0, 0.0, 0.0};
			//cout << "neuronX = " << neuronX << endl;
			//cout << "neuronY = " << neuronY << endl;
			int layerIndex = this->calculateNeuronLayer(corticalLayersNumLayers, corticalLayersKeypoints, &neuronPos, &layerSurfaceNormVectorNOTUSED);
			localConnectomeCSVdatasetLine->push_back(SHAREDvars.convertIntToString(layerIndex));	//LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER	
		}
		else	//localConnectomeCSVdatasetConnections
		{
			double neuronX = H01indexedCSVdatabaseOperations.calibrateCoordinateX(SHAREDvars.convertStringToDouble((*localConnectomeCSVdatasetLine)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_X]));
			double neuronY = H01indexedCSVdatabaseOperations.calibrateCoordinateY(SHAREDvars.convertStringToDouble((*localConnectomeCSVdatasetLine)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_Y]));
			vec neuronPos = {neuronX, neuronY, 0.0};
			vec layerSurfaceNormVectorNOTUSED = {0.0, 0.0, 0.0};
			//cout << "neuronX = " << neuronX << endl;
			//cout << "neuronY = " << neuronY << endl;
			int layerIndex = this->calculateNeuronLayer(corticalLayersNumLayers, corticalLayersKeypoints, &neuronPos, &layerSurfaceNormVectorNOTUSED);
			localConnectomeCSVdatasetLine->push_back(SHAREDvars.convertIntToString(layerIndex));	//LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_ARTIFICIAL_LAYER	
			neuronX = H01indexedCSVdatabaseOperations.calibrateCoordinateX(SHAREDvars.convertStringToDouble((*localConnectomeCSVdatasetLine)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_X]));
			neuronY = H01indexedCSVdatabaseOperations.calibrateCoordinateY(SHAREDvars.convertStringToDouble((*localConnectomeCSVdatasetLine)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_Y]));
			neuronPos = {neuronX, neuronY, 0.0};
			layerSurfaceNormVectorNOTUSED = {0.0, 0.0, 0.0};
			//cout << "neuronX = " << neuronX << endl;
			//cout << "neuronY = " << neuronY << endl;
			layerIndex = this->calculateNeuronLayer(corticalLayersNumLayers, corticalLayersKeypoints, &neuronPos, &layerSurfaceNormVectorNOTUSED);
			localConnectomeCSVdatasetLine->push_back(SHAREDvars.convertIntToString(layerIndex));	//LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_ARTIFICIAL_LAYER	
		}
	}
	
	return result;
}

int H01indexedCSVdatabaseCalculateNeuronLayerClass::calculateNeuronLayer(const int corticalLayersNumLayers, vector<vector<vec>>* corticalLayersKeypoints, const vec* neuronPos, vec* layerSurfaceNormVector)
{
	//so algorithm for testing whether a neuron is to the left of a cortical layer boundary;
	//for every cortical layer boundary (L2toL1 (1), L3toL2 (2), L4toL3 (3), L5toL4 (4), L6toL5 (5), WMtoL6 (6), WM (7)):
	//	get y value of neuron (height)
	//	get two nearest (y) points on cortical layer boundary, draw a straight line between them
	//		check if neuron x value is right of the line

	double foundLayer = false;
	int foundLayerIndex = INT_DEFAULT_VALUE;
									
	for(int layerIndex=1; layerIndex<=corticalLayersNumLayers; layerIndex++)
	{
		//cout << "layerIndex = " << layerIndex << endl;			
		if(!foundLayer)
		{
			if(layerIndex < corticalLayersNumLayers)
			{
				vector<vec>* corticalLayerKeypoints = &((*corticalLayersKeypoints)[layerIndex-1]);
				if(this->isNeuronInCorticalLayer(layerIndex, neuronPos, corticalLayerKeypoints, true, layerSurfaceNormVector))
				{
					foundLayer = true;
					foundLayerIndex = layerIndex;
				}
			}
			else
			{
				vector<vec>* corticalLayerKeypoints = &((*corticalLayersKeypoints)[layerIndex-1-1]);	//CHECKTHIS
				if(this->isNeuronInCorticalLayer(layerIndex-1, neuronPos, corticalLayerKeypoints, false, layerSurfaceNormVector))
				{
					foundLayer = true;
					foundLayerIndex = layerIndex;
				}
				else
				{
					cerr << "H01indexedCSVdatabaseCalculateNeuronLayerClass::calculateNeuronLayer error: WM neurons not found left of layer 6 left boundary" << endl;
					exit(EXIT_ERROR);
				}
			}
		}
	}
			
	return foundLayerIndex;
}

bool H01indexedCSVdatabaseCalculateNeuronLayerClass::isNeuronInCorticalLayer(const int layerIndex, const vec* neuronPos, vector<vec>* corticalLayerKeypoints, const bool rightOfLine, vec* layerSurfaceNormVector)
{
	//get two nearest (y) points on cortical layer boundary, draw a straight line between them

	double someLargeNumber = 99999999;
	double blankValue = CORTICAL_LAYER_KEYPOINT_UNAVAILABLE_VALUE;
	double nearest1CorticalLayerKeypointYdistance = someLargeNumber;
	double nearest2CorticalLayerKeypointYdistance = someLargeNumber;
	int nearest1CorticalLayerKeypointIndex = INT_DEFAULT_VALUE;
	int nearest2CorticalLayerKeypointIndex = INT_DEFAULT_VALUE;
	
	//int corticalLayersNumKeypointsMax = corticalLayerKeypoints->size();
	
	for(int corticalLayerKeypointIndex = 0; corticalLayerKeypointIndex<corticalLayerKeypoints->size(); corticalLayerKeypointIndex++)
	{
		vec corticalLayerKeypoint = (*corticalLayerKeypoints)[corticalLayerKeypointIndex];
		double corticalLayersKeypointX = corticalLayerKeypoint.x;
		double corticalLayersKeypointY = corticalLayerKeypoint.y;
		//cout << "corticalLayersKeypointX = " << corticalLayersKeypointX << endl;
		//cout << "corticalLayersKeypointZ = " << corticalLayersKeypointY << endl;
		
		if(corticalLayersKeypointX != blankValue) 
		{
			double corticalLayerKeypointYdistance = SHAREDvars.absDouble(neuronPos->y - corticalLayersKeypointY);
			if(corticalLayerKeypointYdistance < nearest1CorticalLayerKeypointYdistance)
			{ 	
				nearest2CorticalLayerKeypointIndex = nearest1CorticalLayerKeypointIndex;
				nearest2CorticalLayerKeypointYdistance = nearest1CorticalLayerKeypointYdistance;
				nearest1CorticalLayerKeypointIndex = corticalLayerKeypointIndex;
				nearest1CorticalLayerKeypointYdistance = corticalLayerKeypointYdistance;
			}
			else if(corticalLayerKeypointYdistance < nearest2CorticalLayerKeypointYdistance)
			{ 
				nearest2CorticalLayerKeypointIndex = corticalLayerKeypointIndex;
				nearest2CorticalLayerKeypointYdistance = corticalLayerKeypointYdistance;
			}
		}
		else
		{
			//cout << "corticalLayersKeypointX == blankValue" << endl;
		}
	}
		
	vec nearest1CorticalLayerKeypoint = {(*corticalLayerKeypoints)[nearest1CorticalLayerKeypointIndex].x, (*corticalLayerKeypoints)[nearest1CorticalLayerKeypointIndex].y, 0.0};
	vec nearest2CorticalLayerKeypoint = {(*corticalLayerKeypoints)[nearest2CorticalLayerKeypointIndex].x, (*corticalLayerKeypoints)[nearest2CorticalLayerKeypointIndex].y, 0.0};
	//order nearest1CorticalLayerKeypointX < nearest2CorticalLayerKeypointX
	if(nearest1CorticalLayerKeypoint.x > nearest2CorticalLayerKeypoint.x)
	{
		SHAREDvector.performVectorXYswitch(&nearest1CorticalLayerKeypoint, &nearest2CorticalLayerKeypoint);
	}
	
	double pointRightOrLeftOfLine = this->isPointRightOrLeftOfLine(nearest1CorticalLayerKeypoint.x, nearest1CorticalLayerKeypoint.y, nearest2CorticalLayerKeypoint.x, nearest2CorticalLayerKeypoint.y, neuronPos->x, neuronPos->y, rightOfLine);

	#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL_NORMALISE_LOCAL_CONNECTIVITY_USE_LAYER_BOUNDS
	//SHAREDvector should have an operation for x y z avg
	vec nearestCorticalLayerKeypointAvg;
	nearestCorticalLayerKeypointAvg.x = (nearest1CorticalLayerKeypoint.x + nearest2CorticalLayerKeypoint.x)/2.0;
	nearestCorticalLayerKeypointAvg.y = (nearest1CorticalLayerKeypoint.y + nearest2CorticalLayerKeypoint.y)/2.0;
	//measure in the direction of cortical flow (ie L1->L6, from right to left of default visualisation/graph);
	if(rightOfLine)
	{
		SHAREDvector.subtractVectors(layerSurfaceNormVector, &nearestCorticalLayerKeypointAvg, neuronPos);
	}
	else
	{
		SHAREDvector.subtractVectors(layerSurfaceNormVector, neuronPos, &nearestCorticalLayerKeypointAvg);
	}
	#endif
		
	bool result = pointRightOrLeftOfLine;
	
	return result;
}
				
bool H01indexedCSVdatabaseCalculateNeuronLayerClass::isPointRightOrLeftOfLine(const double Ax, const double Ay, const double Bx, const double By, const double X, const double Y, const bool rightOfLine)
{
	//note left/right is defined when looking at point B from point A
	double position = ((Bx - Ax) * (Y - Ay) - (By - Ay) * (X - Ax));
	bool positionSign = false;
	if(position < 0)
	{
		if(rightOfLine)
		{
			positionSign = true;
		}
		else
		{
			positionSign = false;
		}
	}
	else
	{
		if(rightOfLine)
		{
			positionSign = false;
		}
		else
		{
			positionSign = true;
		}
	}
	bool result = positionSign;
	//isRight = ((b.X - a.X)*(c.Y - a.Y) - (b.Y - a.Y)*(c.X - a.X)) < 0;
	
	return result;
}	

#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL_NORMALISE_LOCAL_CONNECTIVITY
bool H01indexedCSVdatabaseCalculateNeuronLayerClass::calculateLocalConnectomeNeuronLayerSurfaceNorms(vector<vector<string>>* localConnectomeCSVdatasetNeurons, map<string, int>* neuronMap)
{
	//initialise connection/neuron layer indices;
	//int corticalLayersNumKeypointsMax;	//= 28	//maximum number keypoints (number cols/2)
	const string corticalLayersBoundaryKeypointTableFileName = CORTICAL_LAYER_BOUNDARY_KEYPOINT_TABLE_FILE_NAME;
	vector<vector<vec>> corticalLayersKeypoints;
	readCorticalLayersBoundaryKeypointTable(corticalLayersBoundaryKeypointTableFileName, &corticalLayersKeypoints);
	calculateNeuronLayerSurfaceNorms(localConnectomeCSVdatasetNeurons, &corticalLayersKeypoints);
}

bool H01indexedCSVdatabaseCalculateNeuronLayerClass::calculateNeuronLayerSurfaceNorms(vector<vector<string>>* localConnectomeCSVdatasetNeurons, vector<vector<vec>>* corticalLayersKeypoints)
{
	bool result = true;
	int corticalLayersNumLayers = CORTICAL_LAYER_NUMBER_OF_LAYERS;
	
	for(int i=0; i<localConnectomeCSVdatasetNeurons->size(); i++)
	{
		vector<string>* localConnectionCSVdatasetLine = &((*localConnectomeCSVdatasetNeurons)[i]);

		double neuronX = H01indexedCSVdatabaseOperations.calibrateCoordinateX(SHAREDvars.convertStringToDouble((*localConnectionCSVdatasetLine)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_X]));
		double neuronY = H01indexedCSVdatabaseOperations.calibrateCoordinateY(SHAREDvars.convertStringToDouble((*localConnectionCSVdatasetLine)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Y]));
		//cout << "neuronX = " << neuronX << endl;
		//cout << "neuronY = " << neuronY << endl;
		vec neuronPos = {neuronX, neuronY, 0.0};
		vec layerSurfaceNormVector = {0.0, 0.0, 0.0};
		#ifdef INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL_NORMALISE_LOCAL_CONNECTIVITY_USE_LAYER_BOUNDS
		int layerIndexNOTUSED = this->calculateNeuronLayer(corticalLayersNumLayers, corticalLayersKeypoints, &neuronPos, &layerSurfaceNormVector);
		#elif defined INDEXED_CSV_DATABASE_READ_LOCAL_CONNECTOME_GENERATE_LARGE_MODEL_NORMALISE_LOCAL_CONNECTIVITY_USE_FLOW_VECTOR
		vec corticalPositiveFlowVector = {LOCAL_CONNECTOME_VISUALISATION_POSITIVE_FLOW_VECTOR_X, LOCAL_CONNECTOME_VISUALISATION_POSITIVE_FLOW_VECTOR_Y, 0.0};
		SHAREDvector.subtractVectors(&layerSurfaceNormVector, &corticalPositiveFlowVector, &neuronPos);	//SHAREDvector.addVectors(&layerSurfaceNormVector, &neuronPos, &corticalPositiveFlowVector);
		#endif
		localConnectionCSVdatasetLine->push_back(SHAREDvars.convertIntToString(layerSurfaceNormVector.x));	//LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER_SURFACE_NORM_X	
		localConnectionCSVdatasetLine->push_back(SHAREDvars.convertIntToString(layerSurfaceNormVector.y));	//LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER_SURFACE_NORM_Y
	}
	
	return result;
}

int H01indexedCSVdatabaseCalculateNeuronLayerClass::calculateNeuronLayerSurfaceNorm(const int corticalLayersNumLayers, vector<vector<vec>>* corticalLayersKeypoints, const vec* neuronPos, vec* layerSurfaceNormVector)
{
	//so algorithm for testing whether a neuron is to the left of a cortical layer boundary;
	//for every cortical layer boundary (L2toL1 (1), L3toL2 (2), L4toL3 (3), L5toL4 (4), L6toL5 (5), WMtoL6 (6), WM (7)):
	//	get y value of neuron (height)
	//	get two nearest (y) points on cortical layer boundary, draw a straight line between them
	//		check if neuron x value is right of the line
	//			record local norm vector wrt layer surface

	double foundLayer = false;
	int foundLayerIndex = INT_DEFAULT_VALUE;
									
	for(int layerIndex=1; layerIndex<=corticalLayersNumLayers; layerIndex++)
	{
		//cout << "layerIndex = " << layerIndex << endl;			
		if(!foundLayer)
		{
			if(layerIndex < corticalLayersNumLayers)
			{
				vector<vec>* corticalLayerKeypoints = &((*corticalLayersKeypoints)[layerIndex-1]);
				if(this->isNeuronInCorticalLayer(layerIndex, neuronPos, corticalLayerKeypoints, true, layerSurfaceNormVector))
				{
					foundLayer = true;
					foundLayerIndex = layerIndex;
				}
			}
			else
			{
				vector<vec>* corticalLayerKeypoints = &((*corticalLayersKeypoints)[layerIndex-1-1]);
				if(this->isNeuronInCorticalLayer(layerIndex-1, neuronPos, corticalLayerKeypoints, false, layerSurfaceNormVector))
				{
					foundLayer = true;
					foundLayerIndex = layerIndex;
				}
				else
				{
					cerr << "H01indexedCSVdatabaseCalculateNeuronLayerClass::calculateNeuronLayerSurfaceNorm error: WM neurons not found left of layer 6 left boundary" << endl;
					exit(EXIT_ERROR);
				}
			}
		}
	}
			
	return foundLayerIndex;
}
#endif
#endif

#ifdef INDEXED_CSV_DATABASE_DC
void H01indexedCSVdatabaseCalculateNeuronLayerClass::calculateNeuronLayersDC(vector<vector<string>>* localConnectomeCSVdatasetNeurons)
{
	map<string, int> neuronTypeIndexMap;

	int neuronTypeFileSize = 0;
	vector<vector<string>> neuronTypeData;
	bool expectFirstLineHeader = true;
	SHAREDvars.getLinesFromFileCSV(INDEXED_CSV_DATABASE_PREPROCESS_LAYERS_FILE_NAME, &neuronTypeData, &neuronTypeFileSize, CHAR_TAB, expectFirstLineHeader);
	
	string neuronIDlistDataString;
	for(int neuronTypeIndex=0; neuronTypeIndex<neuronTypeFileSize; neuronTypeIndex++)
	{
		vector<string>* row = &(neuronTypeData[neuronTypeIndex]);
		string neuronType = (*row)[INDEXED_CSV_DATABASE_PREPROCESS_NEURON_TYPES_FIELD_INDEX_NEURONTYPE];
		//cout << "neuronType = " << neuronType << endl;
		neuronTypeIndexMap[neuronType] = neuronTypeIndex;
	}
		
	for(int i=0; i<localConnectomeCSVdatasetNeurons->size(); i++)
	{
		vector<string>* localNeuronCSVdatasetLine = &((*localConnectomeCSVdatasetNeurons)[i]);
		
		//cout << "neuronX = " << neuronX << endl;
		//cout << "neuronY = " << neuronY << endl;
		string neuronType = (*localNeuronCSVdatasetLine)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE];
	
		int neuronTypeIndex = neuronTypeIndexMap[neuronType];
		#ifdef INDEXED_CSV_DATABASE_DC_NEURON_LAYERS_REUSE_H01_TEMPLATES
		int layerIndex = neuronTypeIndex%CORTICAL_LAYER_NUMBER_OF_LAYERS;
		#else
		int layerIndex = neuronTypeIndex;
		#endif
		//cout << "layerIndex = " << layerIndex << endl;
		localNeuronCSVdatasetLine->push_back(SHAREDvars.convertIntToString(layerIndex));	//LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER	
		//cout << "localNeuronCSVdatasetLine->size() = " << localNeuronCSVdatasetLine->size() << endl;
	}
}
#endif
#endif





