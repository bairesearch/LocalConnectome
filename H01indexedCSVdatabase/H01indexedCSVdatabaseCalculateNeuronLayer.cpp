/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseCalculateNeuronLayer.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021-2022 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: H01LocalConnectome
 * Requirements: see H01indexedCSVdatabase.hpp
 * Compilation: see H01indexedCSVdatabase.hpp
 * Usage: see H01indexedCSVdatabase.hpp
 * Description: H01 indexed CSV database calculate neuron layer
 * Input: corticalLayersBoundaryKeypoints.csv
 * Output Format: vector<vector<vec>> corticalLayersKeypoints
 * Comments:
 * /
 *******************************************************************************/

#include "H01indexedCSVdatabaseCalculateNeuronLayer.hpp"

bool H01indexedCSVdatabaseCalculateNeuronLayerClass::readLocalNeuronsAndConnections(const string local_connectome_folder_base, const string neuronDatasetOrListFileName, const bool neuronListIsDataset, vector<string>* neuronList, map<string, int>* neuronMap, vector<vector<string>>* localConnectomeCSVdatasetNeurons, const bool readConnections, const string neuronListConnectionsFileName, vector<vector<string>>* localConnectomeCSVdatasetConnections, const bool buildConnectionsMap, map<string, int>* connectionsMap, const bool readLayers, const bool readLayersConnections)
{
	bool result = true;

	cout << "neuronDatasetOrListFileName = " << neuronDatasetOrListFileName << endl;
	cout << "neuronListConnectionsFileName = " << neuronListConnectionsFileName << endl;
		
	//read neurons;
	#ifdef INDEXED_CSV_DATABASE_QUERY_READ_CURRENT_FOLDER
	const string indexedCSVdatabaseQueryInputFolder = currentDirectory;	
	SHAREDvars.setCurrentDirectory(indexedCSVdatabaseQueryInputFolder);
	cout << "indexedCSVdatabaseQueryInputFolder = " << indexedCSVdatabaseQueryInputFolder << endl;
	#else
	#ifdef LOCAL_CONNECTOME_FOLDER_BASE_USE_RELATIVE_FOLDER
	SHAREDvars.setCurrentDirectory(currentDirectory);
	#endif
	string localConnectomeCSVdatasetFolder = local_connectome_folder_base;
	SHAREDvars.setCurrentDirectory(localConnectomeCSVdatasetFolder);
	localConnectomeCSVdatasetFolder = LOCAL_CONNECTOME_DATASET_FOLDER;
	SHAREDvars.setCurrentDirectory(localConnectomeCSVdatasetFolder);
	#endif

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
	#ifdef INDEXED_CSV_DATABASE_CALCULATE_NEURON_LAYERS
	if(readLayers)
	{
		calculateLocalConnectomeLayers(localConnectomeCSVdatasetNeurons, localConnectomeCSVdatasetConnections, neuronMap, readLayersConnections);
	}
	#endif
	
	return result;
}


#ifdef INDEXED_CSV_DATABASE_CALCULATE_NEURON_LAYERS

bool H01indexedCSVdatabaseCalculateNeuronLayerClass::calculateLocalConnectomeLayers(vector<vector<string>>* localConnectomeCSVdatasetNeurons, vector<vector<string>>* localConnectomeCSVdatasetConnections, map<string, int>* neuronMap, const bool readConnections)
{
	#ifdef LOCAL_CONNECTOME_OFFICAL_RELEASE_C3_SOMAS_LAYERS
	if(readConnections)
	{
		transferLocalConnectomeNeuronLayersToConnectionsLayers(localConnectomeCSVdatasetNeurons, localConnectomeCSVdatasetConnections, neuronMap);
	}
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
		
#ifdef LOCAL_CONNECTOME_OFFICAL_RELEASE_C3_SOMAS_LAYERS
bool H01indexedCSVdatabaseCalculateNeuronLayerClass::transferLocalConnectomeNeuronLayersToConnectionsLayers(vector<vector<string>>* localConnectomeCSVdatasetNeurons, vector<vector<string>>* localConnectionCSVdataset, map<string, int>* neuronMap)
{
	bool result = true;
	int corticalLayersNumLayers = CORTICAL_LAYER_NUMBER_OF_LAYERS;
	
	for(int i=0; i<localConnectionCSVdataset->size(); i++)
	{
		vector<string>* localConnectionCSVdatasetLine = &((*localConnectionCSVdataset)[i]);
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
			layerIndexString = SHAREDvars.convertIntToString(CORTICAL_LAYER_UNKNOWN);	//TODO: verify this is supported
			//cerr << "H01indexedCSVdatabaseCalculateNeuronLayerClass::transferLocalConnectomeNeuronLayersToConnectionsLayers warning: sourceNeuronID cannot be found; " << sourceNeuronID << endl;	//approx 222 non-unique instances of unknown neurons being referenced by localConnectionCSVdataset
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
			layerIndexString = SHAREDvars.convertIntToString(CORTICAL_LAYER_UNKNOWN);	//TODO: verify this is supported
			//cerr << "H01indexedCSVdatabaseCalculateNeuronLayerClass::transferLocalConnectomeNeuronLayersToConnectionsLayers warning: targetNeuronID cannot be found; " << targetNeuronID << endl;	//approx 222 non-unique instances of unknown neurons being referenced by localConnectionCSVdataset
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

bool H01indexedCSVdatabaseCalculateNeuronLayerClass::calculateNeuronLayers(const bool localConnectomeDatasetType, vector<vector<string>>* localConnectionCSVdataset, vector<vector<vec>>* corticalLayersKeypoints)
{
	bool result = true;
	int corticalLayersNumLayers = CORTICAL_LAYER_NUMBER_OF_LAYERS;
	
	for(int i=0; i<localConnectionCSVdataset->size(); i++)
	{
		vector<string>* localConnectionCSVdatasetLine = &((*localConnectionCSVdataset)[i]);
		
		if(localConnectomeDatasetType)	//localConnectomeCSVdatasetNeurons
		{
			double neuronX = H01indexedCSVdatabaseOperations.calibrateCoordinateX(SHAREDvars.convertStringToDouble((*localConnectionCSVdatasetLine)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_X]));
			double neuronY = H01indexedCSVdatabaseOperations.calibrateCoordinateY(SHAREDvars.convertStringToDouble((*localConnectionCSVdatasetLine)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Y]));
			vec neuronPos = {neuronX, neuronY, 0.0};
			vec layerSurfaceNormVectorNOTUSED = {0.0, 0.0, 0.0};
			//cout << "neuronX = " << neuronX << endl;
			//cout << "neuronY = " << neuronY << endl;
			int layerIndex = this->calculateNeuronLayer(corticalLayersNumLayers, corticalLayersKeypoints, &neuronPos, &layerSurfaceNormVectorNOTUSED);
			localConnectionCSVdatasetLine->push_back(SHAREDvars.convertIntToString(layerIndex));	//LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER	
		}
		else	//localConnectomeCSVdatasetConnections
		{
			double neuronX = H01indexedCSVdatabaseOperations.calibrateCoordinateX(SHAREDvars.convertStringToDouble((*localConnectionCSVdatasetLine)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_X]));
			double neuronY = H01indexedCSVdatabaseOperations.calibrateCoordinateY(SHAREDvars.convertStringToDouble((*localConnectionCSVdatasetLine)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_Y]));
			vec neuronPos = {neuronX, neuronY, 0.0};
			vec layerSurfaceNormVectorNOTUSED = {0.0, 0.0, 0.0};
			//cout << "neuronX = " << neuronX << endl;
			//cout << "neuronY = " << neuronY << endl;
			int layerIndex = this->calculateNeuronLayer(corticalLayersNumLayers, corticalLayersKeypoints, &neuronPos, &layerSurfaceNormVectorNOTUSED);
			localConnectionCSVdatasetLine->push_back(SHAREDvars.convertIntToString(layerIndex));	//LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_ARTIFICIAL_LAYER	
			neuronX = H01indexedCSVdatabaseOperations.calibrateCoordinateX(SHAREDvars.convertStringToDouble((*localConnectionCSVdatasetLine)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_X]));
			neuronY = H01indexedCSVdatabaseOperations.calibrateCoordinateY(SHAREDvars.convertStringToDouble((*localConnectionCSVdatasetLine)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_Y]));
			neuronPos = {neuronX, neuronY, 0.0};
			layerSurfaceNormVectorNOTUSED = {0.0, 0.0, 0.0};
			//cout << "neuronX = " << neuronX << endl;
			//cout << "neuronY = " << neuronY << endl;
			layerIndex = this->calculateNeuronLayer(corticalLayersNumLayers, corticalLayersKeypoints, &neuronPos, &layerSurfaceNormVectorNOTUSED);
			localConnectionCSVdatasetLine->push_back(SHAREDvars.convertIntToString(layerIndex));	//LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_ARTIFICIAL_LAYER	
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

	#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL_NORMALISE_LOCAL_CONNECTIVITY_USE_LAYER_BOUNDS
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

#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL_NORMALISE_LOCAL_CONNECTIVITY
bool H01indexedCSVdatabaseCalculateNeuronLayerClass::calculateLocalConnectomeNeuronLayerSurfaceNorms(vector<vector<string>>* localConnectomeCSVdatasetNeurons, map<string, int>* neuronMap)
{
	//initialise connection/neuron layer indices;
	//int corticalLayersNumKeypointsMax;	//= 28	//maximum number keypoints (number cols/2)
	const string corticalLayersBoundaryKeypointTableFileName = CORTICAL_LAYER_BOUNDARY_KEYPOINT_TABLE_FILE_NAME;
	vector<vector<vec>> corticalLayersKeypoints;
	readCorticalLayersBoundaryKeypointTable(corticalLayersBoundaryKeypointTableFileName, &corticalLayersKeypoints);
	calculateNeuronLayerSurfaceNorms(localConnectomeCSVdatasetNeurons, &corticalLayersKeypoints);
}

bool H01indexedCSVdatabaseCalculateNeuronLayerClass::calculateNeuronLayerSurfaceNorms(vector<vector<string>>* localConnectionCSVdataset, vector<vector<vec>>* corticalLayersKeypoints)
{
	bool result = true;
	int corticalLayersNumLayers = CORTICAL_LAYER_NUMBER_OF_LAYERS;
	
	for(int i=0; i<localConnectionCSVdataset->size(); i++)
	{
		vector<string>* localConnectionCSVdatasetLine = &((*localConnectionCSVdataset)[i]);

		double neuronX = H01indexedCSVdatabaseOperations.calibrateCoordinateX(SHAREDvars.convertStringToDouble((*localConnectionCSVdatasetLine)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_X]));
		double neuronY = H01indexedCSVdatabaseOperations.calibrateCoordinateY(SHAREDvars.convertStringToDouble((*localConnectionCSVdatasetLine)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Y]));
		//cout << "neuronX = " << neuronX << endl;
		//cout << "neuronY = " << neuronY << endl;
		vec neuronPos = {neuronX, neuronY, 0.0};
		vec layerSurfaceNormVector = {0.0, 0.0, 0.0};
		#ifdef INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL_NORMALISE_LOCAL_CONNECTIVITY_USE_LAYER_BOUNDS
		int layerIndexNOTUSED = this->calculateNeuronLayer(corticalLayersNumLayers, corticalLayersKeypoints, &neuronPos, &layerSurfaceNormVector);
		#elif defined INDEXED_CSV_DATABASE_QUERY_GENERATE_LARGE_MODEL_NORMALISE_LOCAL_CONNECTIVITY_USE_FLOW_VECTOR
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


