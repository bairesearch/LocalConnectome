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
#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING
#include "H01indexedCSVdatabaseAlgorithms.hpp"
#endif

#ifdef INDEXED_CSV_DATABASE_QUERY

SHAREDvarsClass SHAREDvars;

#ifdef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_CURRENT_FOLDER
string currentDirectory;
#endif

#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING
map<long,vector<H01indexedCSVdatabaseQueryObject>> neuronConnectionMap;	//map<connectionNeuronIDindex,vector<H01indexedCSVdatabaseQueryObject [with neuronIDindex]>> 
vector<H01indexedCSVdatabaseAlgorithmsFit> neuronConnectionPolyFitVector;

H01indexedCSVdatabaseQueryObject::H01indexedCSVdatabaseQueryObject(void)
{
	neuronID = 0;
	type = 0;
	locationObjectContentsXcoordinates = 0;
	locationObjectContentsYcoordinates = 0;
	locationObjectContentsZcoordinates = 0;
	/*
	neuronIDstring = "";
	typeString = "";
	locationObjectContentsXcoordinatesContent = "";
	locationObjectContentsYcoordinatesContent = "";
	locationObjectContentsZcoordinatesContent = "";
	*/
}
H01indexedCSVdatabaseQueryObject::~H01indexedCSVdatabaseQueryObject(void)
{
}

#endif


bool queryIndexedCSVdatabase()
{
	bool result = true;

	#ifdef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_CURRENT_FOLDER
	currentDirectory = SHAREDvars.getCurrentDirectory();
	#endif
	
	#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING
	queryIndexedCSVdatabase(INDEXED_CSV_DATABASE_QUERY_NEURON_LIST_DISTINCT_FILE_NAME, true, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST_DISTINCT_CONNECTIONS_PRESYNAPTIC_FILE_NAME);	
	#else
	queryIndexedCSVdatabase(INDEXED_CSV_DATABASE_QUERY_NEURON_LIST1_FILE_NAME, true, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST1_CONNECTIONS_PRESYNAPTIC_FILE_NAME);
	queryIndexedCSVdatabase(INDEXED_CSV_DATABASE_QUERY_NEURON_LIST1_FILE_NAME, false, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST1_CONNECTIONS_POSTSYNAPTIC_FILE_NAME);
	queryIndexedCSVdatabase(INDEXED_CSV_DATABASE_QUERY_NEURON_LIST2_FILE_NAME, true, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST2_CONNECTIONS_PRESYNAPTIC_FILE_NAME);
	queryIndexedCSVdatabase(INDEXED_CSV_DATABASE_QUERY_NEURON_LIST2_FILE_NAME, false, INDEXED_CSV_DATABASE_QUERY_NEURON_LIST2_CONNECTIONS_POSTSYNAPTIC_FILE_NAME);
	#endif
	
	return result;
}

bool queryIndexedCSVdatabase(const string neuronListFileName, const bool queryPresynapticConnectionNeurons, const string neuronListConnectionsFileName)
{
	bool result = true;
	
	vector<string> neuronList;
	#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_NON_LOCAL_NEURONS_REQUIRED
	map<string, string> neuronMap;	//for lookup efficiency //key and value are identical
	#endif
	
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
	#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_NON_LOCAL_NEURONS_REQUIRED
	for(int i=0; i < neuronList.size(); i++)
	{
    	neuronMap[neuronList[i]] = neuronList[i];
	}
	#endif
		
	const string csvDelimiter = CSV_DELIMITER;

	#ifdef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_CURRENT_FOLDER
	const string indexedCSVdatabaseQueryOutputFolder = currentDirectory;	
	#else
	const string indexedCSVdatabaseQueryOutputFolder = INDEXED_CSV_DATABASE_QUERY_OUTPUT_FOLDER;
	#endif
	
	#ifdef INDEXED_CSV_DATABASE_QUERY_OUTPUT
	#ifdef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_FILE_OBJECT
	SHAREDvars.setCurrentDirectory(indexedCSVdatabaseQueryOutputFolder);
	ofstream writeFileObject(neuronListConnectionsFileName.c_str());
	#else
	string neuronListConnectionsString;
	neuronListConnectionsString.reserve(2000000000);	//max expected file/string size = indexed csv database query output size (neuron_id ~11 bytes + , + type ~1 byte = ~13 bytes) * avg num connections per neuron ID (~2000) * number of neuron IDs in list (~30000) = 13*2000*30000 = ~1GB of RAM if string
	//string neuronListConnectionsString = "";	//inefficient
	#endif
	#endif
	
			
	for(int l=0; l<neuronListFileLength; l++)
	{
		string neuronID = neuronList[l];
		vector<vector<string>> neuronConnectionList;
		queryIndexedCSVdatabaseByNeuronID(neuronID, queryPresynapticConnectionNeurons, &neuronConnectionList);

		if(neuronConnectionList.size() == 0)
		{
			//add a new line such that database lengths are matched
			#ifdef INDEXED_CSV_DATABASE_QUERY_OUTPUT
			#ifdef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_FILE_OBJECT
			SHAREDvars.writeStringToFileObject(STRING_NEWLINE, &writeFileObject);
			#else
			appendStringBufferToPreallocatedString(&neuronListConnectionsString, STRING_NEWLINE);
			//neuronListConnectionsString = neuronListConnectionsString + STRING_NEWLINE;	//inefficient
			#endif			
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
				string neuronConnectionString = connectionNeuronID + csvDelimiter + connectionType;
				#ifdef INDEXED_CSV_DATABASE_QUERY_GET_LOCATION
				string locationObjectContentsXcoordinatesContent = csvLineVector[INDEXED_CSV_DATABASE_SYNAPSE_LOCATION_COORDINATE_X_FIELD_INDEX];
				string locationObjectContentsYcoordinatesContent = csvLineVector[INDEXED_CSV_DATABASE_SYNAPSE_LOCATION_COORDINATE_Y_FIELD_INDEX];
				string locationObjectContentsZcoordinatesContent = csvLineVector[INDEXED_CSV_DATABASE_SYNAPSE_LOCATION_COORDINATE_Z_FIELD_INDEX];
				neuronConnectionString = neuronConnectionString + csvDelimiter + locationObjectContentsXcoordinatesContent + csvDelimiter + locationObjectContentsYcoordinatesContent + csvDelimiter + locationObjectContentsZcoordinatesContent;
				#endif
								
				#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING			
				#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_NON_LOCAL_NEURONS_REQUIRED
				//if(!std::count(neuronList.begin(), neuronList.end(), connectionNeuronID))	//verify that connectionNeuronID is not in neuronList
				if(neuronMap.count(connectionNeuronID) == 0)	//verify that connectionNeuronID is not in neuronList
				{
				#endif
					long connectionNeuronIDindex = SHAREDvars.convertStringToLong(connectionNeuronID);
					long neuronIDIndex = SHAREDvars.convertStringToLong(neuronID);

					//H01indexedCSVdatabaseQueryObject* queryObject = new H01indexedCSVdatabaseQueryObject();
					H01indexedCSVdatabaseQueryObject queryObject;
					queryObject.neuronID = neuronIDIndex;
					queryObject.type = SHAREDvars.convertStringToInt(connectionType);
					queryObject.locationObjectContentsXcoordinates = SHAREDvars.convertStringToInt(locationObjectContentsXcoordinatesContent);
					queryObject.locationObjectContentsYcoordinates = SHAREDvars.convertStringToInt(locationObjectContentsYcoordinatesContent);
					queryObject.locationObjectContentsZcoordinates = SHAREDvars.convertStringToInt(locationObjectContentsZcoordinatesContent);
					vector<H01indexedCSVdatabaseQueryObject>* neuronIDvector;
					if(neuronConnectionMap.count(connectionNeuronIDindex) != 0)
					{
						neuronIDvector = &(neuronConnectionMap.at(connectionNeuronIDindex));
					}
					else
					{
						vector<H01indexedCSVdatabaseQueryObject> innerVectorNewObject; 
						neuronIDvector = &innerVectorNewObject;
						neuronConnectionMap.insert(std::pair<long, vector<H01indexedCSVdatabaseQueryObject>>(connectionNeuronIDindex, innerVectorNewObject)); 
					}
					neuronIDvector->push_back(queryObject); 
				#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_NON_LOCAL_NEURONS_REQUIRED
				}
				#endif
				#endif
				
				#ifdef INDEXED_CSV_DATABASE_QUERY_OUTPUT_CONNECTIONS
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
				#endif
			}
		}

		//#ifdef INDEXED_CSV_DATABASE_QUERY_OUTPUT_CONNECTIONS
		//notify user that program is still running
		cout << "l = " << l << ", neuronID = " << neuronID << ", neuronConnectionList.size() = " << neuronConnectionList.size() << endl;
		//#endif
		
	}
	
	#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING
    map<long,vector<H01indexedCSVdatabaseQueryObject>>::iterator it;
	for(it = neuronConnectionMap.begin(); it != neuronConnectionMap.end(); it++)
	{
		long connectionNeuronID = it->first;
		vector<H01indexedCSVdatabaseQueryObject>* neuronVector = &(it->second);
		#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_2D_POLY_REGRESSION
		vector<double> x;
		vector<double> y;
		//vector<double> z;
		#endif
		#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_3D_LINEAR_REGRESSION
		vector<Eigen::Vector3d> xyz;
		#endif
		
		if(neuronVector->size() >= INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_MIN_NUM_POINTS_REQUIRED)
		{
			#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_EST_TYPE_BY_AVERAGE_SYNAPSE_TYPE
			double totalSynapseType = 0.0;
			#endif
			
			for(int i=0; i<neuronVector->size(); i++)
			{
				H01indexedCSVdatabaseQueryObject* queryObject = &((*neuronVector)[i]);
				cout << "x = " << queryObject->locationObjectContentsXcoordinates << ", y = " << queryObject->locationObjectContentsYcoordinates << endl;
				double xCoord = (double)(queryObject->locationObjectContentsXcoordinates);
				double yCoord = (double)(queryObject->locationObjectContentsYcoordinates);
				double zCoord = (double)(queryObject->locationObjectContentsZcoordinates);
				#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_2D_POLY_REGRESSION
				x.push_back(xCoord);
				y.push_back(yCoord);
				//z.push_back(zCoord);	//not yet modelled/plotted
				#endif
				#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_3D_LINEAR_REGRESSION
				std::vector<double> vec = {xCoord, yCoord, zCoord};
				Eigen::Vector3d vec3D(vec.data());
				//Eigen::Vector3d vec3D(xCoord, yCoord, zCoord);
				xyz.push_back(vec3D);
				#endif
				#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_EST_TYPE_BY_AVERAGE_SYNAPSE_TYPE
				totalSynapseType = totalSynapseType + queryObject->type;
				#endif
			}
			
			int estSynapseType = 0;
			#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_EST_TYPE_BY_AVERAGE_SYNAPSE_TYPE
			double averageSynapseType = totalSynapseType/(neuronVector->size());
			estSynapseType = round(averageSynapseType);	//1: inhibitory, 2: exitatory
			#endif
			
			#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_2D_POLY_REGRESSION
			PolynomialRegression<double> polyreg;
			vector<double> coeffs;
			const int order = 2;
			polyreg.fitIt(x, y, order, coeffs);
			#endif
			#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_3D_LINEAR_REGRESSION
			std::pair<Eigen::Vector3d, Eigen::Vector3d> line3D = best_line_from_points<Eigen::Vector3d>(xyz);
			#endif
			
			#ifdef INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING
			H01indexedCSVdatabaseAlgorithmsFit polyFit;
			polyFit.connectionNeuronID = connectionNeuronID;
			#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_EST_TYPE_BY_AVERAGE_SYNAPSE_TYPE
			polyFit.estSynapseType = estSynapseType;
			#endif
			#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_2D_POLY_REGRESSION
			polyFit.a = coeffs[0];
			polyFit.b = coeffs[1];
			polyFit.c = coeffs[2];
			#endif
			#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_3D_LINEAR_REGRESSION
			polyFit.origin.x = line3D.first[0];
			polyFit.origin.y = line3D.first[1];
			polyFit.origin.z = line3D.first[2];
			polyFit.axis.x = line3D.second[0];
			polyFit.axis.y = line3D.second[1];
			polyFit.axis.z = line3D.second[2];
			#endif
			neuronConnectionPolyFitVector.push_back(polyFit);
			const string doubleFormat = "%0.9f";
			#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_2D_POLY_REGRESSION
			string neuronConnectionString = SHAREDvars.convertLongToString(polyFit.connectionNeuronID) + csvDelimiter + SHAREDvars.convertIntToString(polyFit.estSynapseType) + csvDelimiter + SHAREDvars.convertDoubleToString(polyFit.a, doubleFormat) + csvDelimiter + SHAREDvars.convertDoubleToString(polyFit.b, doubleFormat) + csvDelimiter + SHAREDvars.convertDoubleToString(polyFit.c, doubleFormat) + STRING_NEWLINE;
			#endif
			#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_3D_LINEAR_REGRESSION
			#ifdef INDEXED_CSV_DATABASE_QUERY_OUTPUT_INCOMING_AXON_MAPPING_LDR
			//2 <colour> x1 y1 z1 x2 y2 z2 - https://www.ldraw.org/article/218.html
			string ldrDelimiter = LDR_DELIMITER;
			int ldrawColor = 0;
			if(polyFit.estSynapseType == AVRO_JSON_DATABASE_EXCITATORY_SYNAPSE_TYPE)
			{
				ldrawColor = SHARED_COLOUR_GREEN;
			}
			else if(polyFit.estSynapseType == AVRO_JSON_DATABASE_INHIBITORY_SYNAPSE_TYPE)
			{
				ldrawColor = SHARED_COLOUR_RED;
			}
			else
			{
				cerr << "bool queryIndexedCSVdatabase unknown polyFit.estSynapseType; " << polyFit.estSynapseType << endl;
				exit(EXIT_ERROR);
			}
			vec plot3DpointStart;
			plot3DpointStart.x = polyFit.origin.x - polyFit.axis.x * (AVRO_JSON_DATABASE_COORDINATES_RANGE_X/2);
			plot3DpointStart.y = polyFit.origin.y - polyFit.axis.y * (AVRO_JSON_DATABASE_COORDINATES_RANGE_Y/2);
			plot3DpointStart.z = polyFit.origin.z - polyFit.axis.z * (AVRO_JSON_DATABASE_COORDINATES_RANGE_Z/2);
			vec plot3DpointEnd;
			plot3DpointEnd.x = polyFit.origin.x + polyFit.axis.x * (AVRO_JSON_DATABASE_COORDINATES_RANGE_X/2);
			plot3DpointEnd.y = polyFit.origin.y + polyFit.axis.y * (AVRO_JSON_DATABASE_COORDINATES_RANGE_Y/2);
			plot3DpointEnd.z = polyFit.origin.z + polyFit.axis.z * (AVRO_JSON_DATABASE_COORDINATES_RANGE_Z/2);
			string neuronConnectionString = SHAREDvars.convertIntToString(LDR_TYPE_LINE) + ldrDelimiter + SHAREDvars.convertIntToString(ldrawColor) + ldrDelimiter + SHAREDvars.convertDoubleToString(plot3DpointStart.x, doubleFormat) + ldrDelimiter + SHAREDvars.convertDoubleToString(plot3DpointStart.y, doubleFormat) + ldrDelimiter + SHAREDvars.convertDoubleToString(plot3DpointStart.z, doubleFormat) + ldrDelimiter + SHAREDvars.convertDoubleToString(plot3DpointEnd.x, doubleFormat) + ldrDelimiter + SHAREDvars.convertDoubleToString(plot3DpointEnd.y, doubleFormat) + ldrDelimiter + SHAREDvars.convertDoubleToString(plot3DpointEnd.z, doubleFormat) + STRING_NEWLINE;	
			#else
			string neuronConnectionString = SHAREDvars.convertLongToString(polyFit.connectionNeuronID) + csvDelimiter + SHAREDvars.convertIntToString(polyFit.estSynapseType) + csvDelimiter + SHAREDvars.convertDoubleToString(polyFit.origin.x, doubleFormat) + csvDelimiter + SHAREDvars.convertDoubleToString(polyFit.origin.y, doubleFormat) + csvDelimiter + SHAREDvars.convertDoubleToString(polyFit.origin.z, doubleFormat) + csvDelimiter + SHAREDvars.convertDoubleToString(polyFit.axis.x, doubleFormat) + csvDelimiter + SHAREDvars.convertDoubleToString(polyFit.axis.y, doubleFormat) + csvDelimiter + SHAREDvars.convertDoubleToString(polyFit.axis.z, doubleFormat) + STRING_NEWLINE;
			#endif
			#endif
			#ifdef INDEXED_CSV_DATABASE_QUERY_READ_WRITE_TO_FILE_OBJECT
			SHAREDvars.writeStringToFileObject(neuronConnectionString, &writeFileObject);
			#else
			appendStringBufferToPreallocatedString(neuronListConnectionsString, neuronConnectionString);
			//neuronListConnectionsString = neuronListConnectionsString + neuronConnectionString	//inefficient
			#endif
			#else
			cout << "INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING: queryIndexedCSVdatabase(): connectionNeuronID = " << connectionNeuronID << ", estSynapseType = " << estSynapseType << ", poly fit; a = " << coeffs[0] << ", b = " << coeffs[1] << ", c = " << coeffs[2] << endl;			
			#endif
		}
	}
	#endif
	
	#ifdef INDEXED_CSV_DATABASE_QUERY_OUTPUT
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

