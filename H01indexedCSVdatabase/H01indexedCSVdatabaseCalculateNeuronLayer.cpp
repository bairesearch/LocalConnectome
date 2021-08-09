/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseCalculateNeuronLayer.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021 Baxter AI (baxterai.com)
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

SHAREDvarsClass SHAREDvars2;

#ifdef INDEXED_CSV_DATABASE_CALCULATE_NEURON_LAYERS

bool readCorticalLayersBoundaryKeypointTable(const string corticalLayersBoundaryKeypointTableFileName, vector<vector<vec>>* corticalLayersKeypoints)
{
	bool result = true;
	
	vector<vector<string>> dataSetFile;
	int dataSetFileSize = 0;
	getLinesFromFileCSV(corticalLayersBoundaryKeypointTableFileName, &dataSetFile, &dataSetFileSize, CSV_DELIMITER_CHAR, true);
	for(int layerIndex=0; layerIndex<dataSetFile.size(); layerIndex++)
	{
		vector<string> corticalLayerKeypointsXY = dataSetFile[layerIndex];
		int corticalLayersNumKeypointsMax = corticalLayerKeypointsXY.size();
		vector<vec> corticalLayerKeypoints(corticalLayersNumKeypointsMax);
		for(int i=0; i<corticalLayersNumKeypointsMax; i=i+2)
		{
			int k = i/2;
			corticalLayerKeypoints[k].x = SHAREDvars2.convertStringToDouble(corticalLayerKeypointsXY[i]);
			corticalLayerKeypoints[k].y = SHAREDvars2.convertStringToDouble(corticalLayerKeypointsXY[i+1]);
			//cout << "corticalLayerKeypoints[k].x = " << corticalLayerKeypoints[k].x << endl;
			//cout << "corticalLayerKeypoints[k].y = " << corticalLayerKeypoints[k].y << endl;
		}
		corticalLayersKeypoints->push_back(corticalLayerKeypoints);
	}
	
	return result;	
}

bool calculateNeuronLayers(const bool localConnectomeDatasetType, vector<vector<string>>* localConnectionCSVdataset, vector<vector<vec>>* corticalLayersKeypoints)
{
	bool result = true;
	int corticalLayersNumLayers = CORTICAL_LAYER_NUMBER_OF_LAYERS;
	
	for(int i=0; i<localConnectionCSVdataset->size(); i++)
	{
		vector<string>* localConnectionCSVdatasetLine = &((*localConnectionCSVdataset)[i]);
		
		if(localConnectomeDatasetType)	//localConnectionCSVdatasetNeurons
		{
			double neuronX = calibrateCoordinateX(SHAREDvars2.convertStringToDouble((*localConnectionCSVdatasetLine)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_X]));
			double neuronY = calibrateCoordinateY(SHAREDvars2.convertStringToDouble((*localConnectionCSVdatasetLine)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_Y]));
			//cout << "neuronX = " << neuronX << endl;
			//cout << "neuronY = " << neuronY << endl;
			int layerIndex = calculateNeuronLayer(corticalLayersNumLayers, corticalLayersKeypoints, neuronX, neuronY);
			localConnectionCSVdatasetLine->push_back(SHAREDvars2.convertIntToString(layerIndex));
		}
		else	//localConnectionCSVdatasetConnections
		{
			double neuronX = calibrateCoordinateX(SHAREDvars2.convertStringToDouble((*localConnectionCSVdatasetLine)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_X]));
			double neuronY = calibrateCoordinateY(SHAREDvars2.convertStringToDouble((*localConnectionCSVdatasetLine)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_Y]));
			//cout << "neuronX = " << neuronX << endl;
			//cout << "neuronY = " << neuronY << endl;
			int layerIndex = calculateNeuronLayer(corticalLayersNumLayers, corticalLayersKeypoints, neuronX, neuronY);
			localConnectionCSVdatasetLine->push_back(SHAREDvars2.convertIntToString(layerIndex));
			neuronX = calibrateCoordinateX(SHAREDvars2.convertStringToDouble((*localConnectionCSVdatasetLine)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_X]));
			neuronY = calibrateCoordinateY(SHAREDvars2.convertStringToDouble((*localConnectionCSVdatasetLine)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_Y]));
			//cout << "neuronX = " << neuronX << endl;
			//cout << "neuronY = " << neuronY << endl;
			layerIndex = calculateNeuronLayer(corticalLayersNumLayers, corticalLayersKeypoints, neuronX, neuronY);
			localConnectionCSVdatasetLine->push_back(SHAREDvars2.convertIntToString(layerIndex));		
		}
	}
	
	return result;
}

int calculateNeuronLayer(int corticalLayersNumLayers, vector<vector<vec>>* corticalLayersKeypoints, double neuronX, double neuronY)
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
				if(isNeuronInCorticalLayer(layerIndex, neuronX, neuronY, corticalLayerKeypoints))
				{
					foundLayer = true;
					foundLayerIndex = layerIndex;
				}
			}
			else
			{
				foundLayer = true;
				foundLayerIndex = layerIndex;
			}
		}
	}
			
	return foundLayerIndex;
}

bool isNeuronInCorticalLayer(int layerIndex, double neuronX, double neuronY, vector<vec>* corticalLayerKeypoints)
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
			double corticalLayerKeypointYdistance = abs(neuronY - corticalLayersKeypointY);
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
		
	double nearest1CorticalLayerKeypointX = (*corticalLayerKeypoints)[nearest1CorticalLayerKeypointIndex].x;
	double nearest1CorticalLayerKeypointY = (*corticalLayerKeypoints)[nearest1CorticalLayerKeypointIndex].y;
	double nearest2CorticalLayerKeypointX = (*corticalLayerKeypoints)[nearest2CorticalLayerKeypointIndex].x;
	double nearest2CorticalLayerKeypointY = (*corticalLayerKeypoints)[nearest2CorticalLayerKeypointIndex].y;

	//order nearest1CorticalLayerKeypointX < nearest2CorticalLayerKeypointX
	if(nearest1CorticalLayerKeypointX > nearest2CorticalLayerKeypointX)
	{
		//switch
		double nearest1CorticalLayerKeypointXtemp = nearest1CorticalLayerKeypointX;
		double nearest1CorticalLayerKeypointYtemp = nearest1CorticalLayerKeypointY;
		nearest1CorticalLayerKeypointX = nearest2CorticalLayerKeypointX;
		nearest1CorticalLayerKeypointY = nearest2CorticalLayerKeypointY;
		nearest2CorticalLayerKeypointX = nearest1CorticalLayerKeypointXtemp;
		nearest2CorticalLayerKeypointY = nearest1CorticalLayerKeypointYtemp;
	}
	
	double pointRightOfLine = isPointRightOfLine(nearest1CorticalLayerKeypointX, nearest1CorticalLayerKeypointY, nearest2CorticalLayerKeypointX, nearest2CorticalLayerKeypointY, neuronX, neuronY);
	
	bool result = pointRightOfLine;
	
	return result;
}
				
bool isPointRightOfLine(double Ax, double Ay, double Bx, double By, double X, double Y)
{
	//note left/right is defined when looking at point B from point A
	double position = ((Bx - Ax) * (Y - Ay) - (By - Ay) * (X - Ax));
	bool positionSign = false;
	if(position < 0)
	{
		positionSign = true;
	}
	else
	{
		positionSign = false;
	}
	bool result = positionSign;
	//isRight = ((b.X - a.X)*(c.Y - a.Y) - (b.Y - a.Y)*(c.X - a.X)) < 0;
	
	return result;
}	

#endif

void getLinesFromFileCSV(const string fileName, vector<vector<string>>* CSVdatasetFile, int* CSVdatasetFileSize, const char delimiter, const bool expectFirstLineHeader)
{
	vector<string> dataSetFile;
	int dataSetFileSize = 0;
	SHAREDvars2.getLinesFromFile(fileName, &dataSetFile, &dataSetFileSize);
	
	bool firstLine = true;
	
	for(int i=0; i<dataSetFileSize; i++)
	{
		vector<string> lineContentsVector;
		string lineContents = dataSetFile[i];
		string fieldContents = "";
		int fieldIndex = 0;
		for(int charIndex=0; charIndex<lineContents.length(); charIndex++)
		{
			if(lineContents[charIndex] == delimiter)
			{	
				//cout << "fieldContents = " << fieldContents << endl;
				lineContentsVector.push_back(fieldContents);
				fieldContents = "";
				fieldIndex++;
			}
			else
			{
				fieldContents = fieldContents+lineContents[charIndex];
			}
		}
		//fill final field on line;
		lineContentsVector.push_back(fieldContents);	
		fieldContents = "";
		fieldIndex++;
		
		if(expectFirstLineHeader && firstLine)
		{
			//ignore first line header
		}
		else
		{
			CSVdatasetFile->push_back(lineContentsVector);
		}
		
		firstLine = false;
	}
	
	*CSVdatasetFileSize = CSVdatasetFile->size();
}

double calibrateCoordinateX(const double csvDatabaseCoordinateX)
{
	double visualisationCoordinateX = (csvDatabaseCoordinateX*LOCAL_CONNECTOME_VISUALISATION_CALIBRATION_FACTOR_X) + LOCAL_CONNECTOME_VISUALISATION_CALIBRATION_MIN_X;
	return visualisationCoordinateX;
}
double calibrateCoordinateY(const double csvDatabaseCoordinateY)
{
	double visualisationCoordinateY = (csvDatabaseCoordinateY*LOCAL_CONNECTOME_VISUALISATION_CALIBRATION_FACTOR_Y) + LOCAL_CONNECTOME_VISUALISATION_CALIBRATION_MIN_Y;
	return visualisationCoordinateY;
}
double calibrateCoordinateZ(const double csvDatabaseCoordinateZ)
{
	double visualisationCoordinateZ = (csvDatabaseCoordinateZ*LOCAL_CONNECTOME_VISUALISATION_CALIBRATION_FACTOR_Z) + LOCAL_CONNECTOME_VISUALISATION_CALIBRATION_MIN_Z;
	return visualisationCoordinateZ;
}

