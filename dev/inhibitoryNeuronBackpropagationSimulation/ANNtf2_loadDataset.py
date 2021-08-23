# -*- coding: utf-8 -*-
"""ANNtf2_loadDataset.py

# Requirements:
Python 3 and Tensorflow 2.1+ 

# License:
MIT License

# Usage:
see ANNtf2.py

# Description:

Provide dataset load functions

- Author: Richard Bruce Baxter - Copyright (c) 2020-2021 Baxter AI (baxterai.com)

# Datasets:

	Terminology: 
	*   Note "batch" refers to the actual batch (not the minibatch) being trained
	*   Note "train" refers to the neural network train/validation/test data (it is automatically divided up into train/test sets)
	
## Dataset type 1:

	XtrainBatchXXXX.bat: 
	*   x input features (cols) per experience (row) [space delimited]

	YtrainBatchXXXX.bat:
	*   x output classes (cols) per experience (row) [space delimited]
	*   One-hot encoded with a 1 corresponding to the class, and 0 otherwise
	
## Dataset type 2:

	trainBatchXXXX.dat: 
	*   y class (first col) then x input features (cols) per experience (row) [comma delimited]
	*   y class is an integer between 1 and numberOfClasses (not hot encoded)	

## Dataset type 3:

	XtrainBatchXXXX.bat: 
	*   x input features (cols) per experience (row) [space delimited]

	
	
## Dataset type 1 example - POStagSequence (XtrainBatchXXXX.dat and YtrainBatchXXXX.dat)

	This example dataset is generated from wikidump. 

	All possible POS tags are identified for each word in n-word sequence (e.g. 11 words) using ~wordnet.
		POStagSequence: where the centre word has a non-ambigious POS tag.
	~53 POS tags identifiable per word.

	XtrainBatchXXXX.bat: 
	*   530 features (cols) per word sequence (row).
	*       POStagSequence: 10 words per sequence (missing centre word). 
	*   POStagSequence: includes POS_INDEX_OUT_OF_SENTENCE_BOUNDS if !GIA_PREPROCESSOR_POS_TAGGER_DATABASE_DO_NOT_TRAIN_POS_INDEX_OUT_OF_SENTENCE_BOUNDS
        *   supports ambiguous contextual POS data (ie more than valid POS index per word in the sequence)
	*   a 1 is assigned to each possible POS tag index, 0 otherwise
	
	YtrainBatchXXXX.bat:
	*   52 possible classes. 
	*   One-hot encoded with a 1 corresponding to its generated POS tag index.

	To generate the POS dataset with GIA (XtrainBatchXXXX.bat/YtrainBatchXXXX.bat):
	```
	download wikipedia archive (e.g. enwiki-20171201-pages-articles.xml.bz2)
	download wikiextractor tool (https://github.com/attardi/wikiextractor)
	execute WikiExtractor.py on wikipedia archive
	edit SHAREDglobalDefs.hpp; activate:
		#define COMPILE_GIA_WITH_ANN_GENERATE_POS_TAGGER_DATABASE
	POStagSequence: edit GIAglobalDefs.hpp;
		disable GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_TRAIN_SINGLE_BATCH_ONLY and set GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_DOC_XML_OUTPUT_START_FILE accordingly (or use batch script with GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_TRAIN_SINGLE_BATCH_ONLY)
		enable GIA_PREPROCESSOR_POS_TAGGER_SEQUENCE: GIA_PREPROCESSOR_POS_TAGGER_DATABASE_TRAIN_AMBIGUOUS_PERMUTATIONS
	make --makefile=makefile.GIAwithANNgeneratePOStaggerDatabase.UB16
	./GIAgeneratePOStaggerDatabase.exe -dbpostaggerfolder "/home/rich/source/GIAPOStaggerDatabase" -lrp -lrpfolder "/home/rich/source/source/LRPdata" -wikiDumpFolder "/home/rich/soft/wiki/output" (-wikiDumpFileBatchIndex X)
	this creates XtrainBatchXXXX.dat and YtrainBatchXXXX.dat
	```
## Dataset type 2 example - SmallDataset (trainBatchXXXX.dat - aka SmallDataset.data): 

	This example dataset is medical data relating to the diagnosis of thyroid disease.


## Dataset type 3 example - POStagSentence (XtrainBatchXXXX.dat)

	This example dataset is generated from wikidump. 

	All possible POS tags are identified for each word in n-word sequence (e.g. sentence) using ~wordnet.
		POStagSentence: where all words can have ambiguous POS tags
	~53 POS tags identifiable per word.

	XtrainBatchXXXX.bat: 
	*   53*numWordsPerSequence features (cols) per word sequence (row). 
	*	POStagSentence: arbitrary number of words per sentence
	*       53 features per word in sequence: ie 53*numWordsPerSequence - x53 features [word #1] x53 features [word #2] etc 
	*   a 1 is assigned to each possible POS tag index, 0 otherwise
	
	To generate the POS dataset with GIA (XtrainBatchXXXX.bat):
	```
	download wikipedia archive (e.g. enwiki-20171201-pages-articles.xml.bz2)
	download wikiextractor tool (https://github.com/attardi/wikiextractor)
	execute WikiExtractor.py on wikipedia archive
	edit SHAREDglobalDefs.hpp; activate:
		#define COMPILE_GIA_WITH_ANN_GENERATE_POS_TAGGER_DATABASE
	POStagSentence: edit GIAglobalDefs.hpp;
		disable GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_TRAIN_SINGLE_BATCH_ONLY and set GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_DOC_XML_OUTPUT_START_FILE accordingly (or use batch script with GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_TRAIN_SINGLE_BATCH_ONLY)
		enable GIA_PREPROCESSOR_POS_TAGGER_SENTENCE: GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_RAW
	make --makefile=makefile.GIAwithANNgeneratePOStaggerDatabase.UB16
	./GIAgeneratePOStaggerDatabase.exe -dbpostaggerfolder "/home/rich/source/GIAPOStaggerDatabase" -lrp -lrpfolder "/home/rich/source/source/LRPdata" -wikiDumpFolder "/home/rich/soft/wiki/output" (-wikiDumpFileBatchIndex X)
	this creates XtrainBatchXXXX.dat
	```
	
"""

import os
import csv
import tensorflow as tf
import numpy as np
from numpy import genfromtxt
import ANNtf2_globalDefs

datasetFolderRelative = "datasets"

GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES = 52
GIA_PREPROCESSOR_POS_TAGGER_DATABASE_POS_NUMBER_OF_TYPES = GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES+1	#includes GIA_PREPROCESSOR_POS_TAGGER_DATABASE_POS_INDEX_OUT_OF_SENTENCE_BOUNDS
GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES_EXTENDED = 64

percentageDatasetTrain = 80.0	#ie 80% train, 20% test

datasetType1alreadyNormalised = True	#if True, assume that the dataset includes values between 0 and 1 only
datasetType2alreadyNormalised = False	#if True, assume that the dataset includes values between 0 and 1 only
datasetType3alreadyNormalised = True	#if True, assume that the dataset includes values between 0 and 1 only

numberOfFeaturesPerWord = 53	#last feature identifies word as out of sentence padding (out of sentence padding is not expected by loadDatasetType3 as each row only contains data of a specific sentence length; out of sentence padding will be applied by ANNtf2_loadDataset after data is read)
optimiseFeedLength = True
if(optimiseFeedLength):
	paddingTagIndex = 9	#out of sentence features will be padded with this character
else:
	paddingTagIndex = 0

#"""**Please select both data files from local harddrive: XtrainBatchSmall.dat and YtrainBatchSmall.dat:**"""
#
#from google.colab import files
#uploaded = files.upload()
#for fn in uploaded.keys():
#  print('User uploaded file "{name}" with length {length} bytes'.format(
#		name=fn, length=len(uploaded[fn])))

storeRowLengths = False

def createFileAbsPath(fileName):
	scriptFolder = os.path.dirname(__file__) #<-- absolute dir the script is in
	relFilePath = datasetFolderRelative + '/' + fileName
	absFilePath = os.path.join(scriptFolder, relFilePath)
	return absFilePath

def loadtxtBasic(filename, delimiter=','):
	absFilePath = createFileAbsPath(filename)
	data = genfromtxt(absFilePath, delimiter=',')
	#print("data = ", data)
	return data
	
def loadtxt(filename, delimiter=',', classColumnFirst=True, numeriseClassColumn=True):

	
	absFilePath = createFileAbsPath(filename)

	#print("absFilePath = ", absFilePath)

	dtype=float
	classNamesDict = {}
	classIndexMax = 1
	
	#print("classColumnFirst = ", classColumnFirst)
	
	dataList = []
	with open(absFilePath, 'r') as file:
		#lines = file.read().split(',')
		reader = csv.reader(file)
		for row in reader:

			#print("row1 = ", row)

			if(numeriseClassColumn):
				#for rowIndex in len(lines):
					#row = lines[rowIndex]
				if(classColumnFirst):
					className = row[0]
				else:
					className = row[-1]
				#print("className = ", className)
									
				if className in classNamesDict:
					classIndex = classNamesDict[className]
				else:
					classNamesDict[className] = classIndexMax
					classIndex = classIndexMax
					classIndexMax += 1
					
				#print("classIndex = ", classIndex)
				
				
				if(classColumnFirst):
					row[0] = str(classIndex)
				else:
					row[-1] = str(classIndex)
					
			#print("row2 = ", row)
			
			dataList.append(row)
							
			numberOfClasses = len(classNamesDict.keys())
				
				
	data = np.array(dataList, dtype=dtype)
	#data = data.reshape((-1, iter_loadtxt.rowlength))

	#print("dataList = ", dataList)
	#print("data = ", data)

	return data


def iter_loadtxt(filename, delimiter=',', skiprows=0, dtype=float, normaliseRowLengthWithPad=False, normaliseRowLengthWithPadLimit=False, padCharacter='0', maxRowLength=100, minRowLength=0):
	
	absFilePath = createFileAbsPath(filename)
	
	normaliseRowLengthWithPadLimitDisgard = False
	if(normaliseRowLengthWithPad):
		normaliseRowLengthWithPadLimitDisgard = True	#disgard longer sentences completely
		iter_loadtxt.maxNumberOfItemsPerRow = 0
		if(normaliseRowLengthWithPadLimit):
			iter_loadtxt.maxNumberOfItemsPerRow = maxRowLength
			iter_loadtxt.minNumberOfItemsPerRow = minRowLength
		else:
			with open(absFilePath, 'r') as infile:
				for _ in range(skiprows):
					next(infile)
				for line in infile:
					line = line.rstrip().split(delimiter)
					numberOfItems = len(line)
					if(numberOfItems > iter_loadtxt.maxNumberOfItemsPerRow):
						iter_loadtxt.maxNumberOfItemsPerRow = numberOfItems
		print("iter_loadtxt.maxNumberOfItemsPerRow = ", iter_loadtxt.maxNumberOfItemsPerRow)
	
	if(storeRowLengths):
		num_lines = 0
		with open(absFilePath, 'r') as infile:
			for line in f:
        			num_lines += 1		
		rowLengthsArray = np.zeros(num_lines, dtype=int)
		with open(absFilePath, 'r') as infile:
			for count, line in enumerate(f):
				line = line.rstrip().split(delimiter)
				rowLengthsArray[count] = len(line)
														
	def iter_func():
		with open(absFilePath, 'r') as infile:
			for _ in range(skiprows):
				next(infile)
			for line in infile:
				line = line.rstrip().split(delimiter)
				
				passSentenceLengthReq = True
				if(normaliseRowLengthWithPadLimitDisgard):
					if(len(line) > iter_loadtxt.maxNumberOfItemsPerRow):
						passSentenceLengthReq = False
					if(len(line) < iter_loadtxt.minNumberOfItemsPerRow):
						passSentenceLengthReq = False
				
				if(passSentenceLengthReq):
					if(normaliseRowLengthWithPad):
						template = [padCharacter] * iter_loadtxt.maxNumberOfItemsPerRow
						lineCropped = line[:iter_loadtxt.maxNumberOfItemsPerRow]
						template[:len(lineCropped)] = lineCropped
						#print("len(template) = ", len(template))
						line = template
					
					for item in line:
						yield dtype(item)
						
		iter_loadtxt.rowlength = len(line)

	data = np.fromiter(iter_func(), dtype=dtype)
	data = data.reshape((-1, iter_loadtxt.rowlength))
	if(storeRowLengths):
		return data, rowLengthsArray
	else:
		return data
	
def hotEncode(y, maxY):
	yHotEncoded = np.zeros(maxY)
	yHotEncoded[y-1] = 1
	return yHotEncoded
			
def loadDatasetType1(datasetFileNameX, datasetFileNameY):
	
	#all_X = genfromtxt(datasetFileNameX, delimiter=' ')
	#all_Y = genfromtxt(datasetFileNameY, delimiter=' ')
	all_X = iter_loadtxt(datasetFileNameX, delimiter=' ')
	all_Y = iter_loadtxt(datasetFileNameY, delimiter=' ')
	
	all_Y = np.array(all_Y, np.uint8)

	#randomise data
	#dataRawRandomised = dataRaw
	#np.random.shuffle(dataRawRandomised)
	#all_X = dataRawRandomised[:,1:]
	#all_Y = dataRawRandomised[:,0]
		
	datasetNumExamples = all_Y.shape[0]
	datasetNumFeatures = all_X.shape[1]
	datasetNumClasses = all_Y.shape[1]

	datasetNumExamplesTrain = int(float(datasetNumExamples)*percentageDatasetTrain/100.0)
	datasetNumExamplesTest = int(float(datasetNumExamples)*(100.0-percentageDatasetTrain)/100.0)
	
	#reduce 1-hot encoding to class index:
	all_Ycollapsed = np.empty([datasetNumExamples])
	for r in range(datasetNumExamples):
		for i in range(datasetNumClasses):
			if all_Y[r,i] == 1:
				all_Ycollapsed[r] = i
	all_Ynormalised = all_Ycollapsed
	
	if(datasetType1alreadyNormalised):
		all_Xnormalised = all_X
	else:
		#normalise x data between 0.0 and 1.0:
		all_Xnormalised = all_X #np.zeros(all_X.shape)
		for i in range(datasetNumFeatures):
			maxXi = np.amax(all_X[:,i])
			minXi = np.amin(all_X[:,i])
			all_Xnormalised[:,i] = (all_X[:,i] - minXi) / (maxXi - minXi) #all_X[:,i]/maxXi		

	train_x = all_Xnormalised[0:datasetNumExamplesTrain, :]
	test_x = all_Xnormalised[-datasetNumExamplesTest:, :]
	train_y = all_Ynormalised[0:datasetNumExamplesTrain]
	test_y = all_Ynormalised[-datasetNumExamplesTest:]
			
	# Convert x/y data to float32/uint8.
	train_x, test_x = np.array(train_x, np.float32), np.array(test_x, np.float32)
	train_y, test_y = np.array(train_y, np.uint8), np.array(test_y, np.uint8) 
	#https://www.tensorflow.org/api_docs/python/tf/keras/datasets/mnist/load_data?version=stable
	#https://medium.com/@HojjatA/could-not-find-valid-device-for-node-while-eagerly-executing-8f2ff588d1e

	return datasetNumFeatures, datasetNumClasses, datasetNumExamples, train_x, train_y, test_x, test_y

	
def loadDatasetType2(datasetFileName, classColumnFirst=True):

	numeriseClassColumn = True
	
	#dataRaw = loadtxtBasic(datasetFileName, delimiter=',')
	dataRaw = loadtxt(datasetFileName, delimiter=',', classColumnFirst=classColumnFirst, numeriseClassColumn=numeriseClassColumn)
	
	datasetNumExamples = dataRaw.shape[0]
	#print (dataRaw)
	#print ("datasetNumExamples: " + str(datasetNumExamples))
	#randomise data
	dataRawRandomised = dataRaw
	np.random.shuffle(dataRawRandomised)
	#print (dataRawRandomised)
	if(classColumnFirst):
		all_X = dataRawRandomised[:,1:]
		all_Y = dataRawRandomised[:,0]
	else:
		all_X = dataRawRandomised[:,0:-1]
		all_Y = dataRawRandomised[:,-1]		
	#print("all_X = ", all_X)
	#print("all_Y = ", all_Y)
		
	datasetNumExamplesTrain = int(float(datasetNumExamples)*percentageDatasetTrain/100.0)
	datasetNumExamplesTest = int(float(datasetNumExamples)*(100.0-percentageDatasetTrain)/100.0)
		
	datasetNumFeatures = all_X.shape[1]
	maxY = int(np.amax(all_Y))
	datasetNumClasses = maxY
		 
	if(datasetType2alreadyNormalised):
		all_Ynormalised = all_Y
		all_Xnormalised = all_X
	else:
		#normalise y data between 0 and datasetNumClasses
		all_Ynormalised = all_Y - 1 #->0
		
		#normalise x data between 0.0 and 1.0:
		all_Xnormalised = all_X #np.zeros(all_X.shape)
		for i in range(datasetNumFeatures):
			maxXi = np.amax(all_X[:,i])
			minXi = np.amin(all_X[:,i])
			all_Xnormalised[:,i] = (all_X[:,i] - minXi) / (maxXi - minXi) #all_X[:,i]/maxXi
	
	train_x = all_Xnormalised[0:datasetNumExamplesTrain, :]
	test_x = all_Xnormalised[-datasetNumExamplesTest:, :]
	train_y = all_Ynormalised[0:datasetNumExamplesTrain]
	test_y = all_Ynormalised[-datasetNumExamplesTest:]

	#print("train_x = ", train_x)
	#print("train_y = ", train_y)
		
	#hot encode y data (not required):
	#all_Yhotencoded = np.zeros((datasetNumExamples, maxY))
	#for i in range(datasetNumExamples):	#for every row (ie example) in all_Y
	#	y = int(all_Y[i])
	#	yHotEncoded = hotEncode(y, maxY)
	#	all_Yhotencoded[i] = yHotEncoded
	#train_y = all_Yhotencoded[0:datasetNumExamplesTrain, :]
	#test_y = all_Yhotencoded[-datasetNumExamplesTest:, :]

	print("datasetNumClasses = ", datasetNumClasses)

	#print(datasetNumExamplesTrain)
	#print(datasetNumExamplesTest)
	#print(train_x.shape)
	#print(train_y.shape)
	#print(test_x.shape)
	#print(test_y.shape)

	# Convert x/y data to float32/uint8.
	train_x, test_x = np.array(train_x, np.float32), np.array(test_x, np.float32)
	train_y, test_y = np.array(train_y, np.uint8), np.array(test_y, np.uint8) 
	#https://www.tensorflow.org/api_docs/python/tf/keras/datasets/mnist/load_data?version=stable
	#https://medium.com/@HojjatA/could-not-find-valid-device-for-node-while-eagerly-executing-8f2ff588d1e

	return datasetNumFeatures, datasetNumClasses, datasetNumExamples, train_x, train_y, test_x, test_y
	

def loadDatasetType3(datasetFileNameX, generatePOSunambiguousInput, onlyAddPOSunambiguousInputToTrain, useSmallSentenceLengths):
	
	#parameters;
	getDataAsBinary = False	#boolean type does not allow padding	
	getDataAsInt = True
		
	padExamples = True
	cropExamples = True
	if(cropExamples):
		minimumSentenceLength = 3
		if(useSmallSentenceLengths):
			maximumSentenceLength = 30
		else:	
			maximumSentenceLength = 50
	else:
		minimumSentenceLength = 0
		maximumSentenceLength = 100
	maximumNumFeatures = maximumSentenceLength*numberOfFeaturesPerWord
	minimumNumFeatures = minimumSentenceLength*numberOfFeaturesPerWord
	generateNegativeExamples = False	#for backprop training
	generateYvalues = True
	if(generateYvalues):
		if(getDataAsBinary):
			yClassPositive = True
			yClassNegative = False	
		else:
			yClassPositive = 1 
			yClassNegative = 0
		
	#all_X = genfromtxt(datasetFileNameX, delimiter=' ')
	paddingCharacter = str(paddingTagIndex)[0]
	if(getDataAsBinary):
		dataType = bool
	else:
		if(getDataAsInt):
			dataType = int
		else:
			dataType = float
	
	if(getDataAsBinary):
		xPOStagActive = True
		xPOStagInactive = False	
	else:
		xPOStagActive = 1
		xPOStagInactive = 0
			
	if(ANNtf2_globalDefs.testHarness):	
		#1 0 0 ... 0 0   0 1 0 ... 0 0 0  0 0 1 ... 0 0 1 			
		line = np.zeros((ANNtf2_globalDefs.testHarnessNumWords*numberOfFeaturesPerWord), dataType)
		for w in range(ANNtf2_globalDefs.testHarnessNumWords):
			line[numberOfFeaturesPerWord*w+w] = xPOStagActive
		
		template = [paddingCharacter] * maximumNumFeatures
		template[:len(line)] = line
		all_X = np.expand_dims(template, 0)
	else:
		all_X = iter_loadtxt(datasetFileNameX, delimiter=' ', normaliseRowLengthWithPad=True, normaliseRowLengthWithPadLimit=True, padCharacter=paddingCharacter, maxRowLength=maximumNumFeatures, minRowLength=minimumNumFeatures, dtype=dataType)
	
	if(onlyAddPOSunambiguousInputToTrain):
		
		datasetNumExamples = all_X.shape[0]
		datasetNumFeatures = all_X.shape[1]
		all_Xunambiguous = np.empty((0, datasetNumFeatures), dataType)
		
		eIndex = 0
		for e in range(datasetNumExamples):
		
			#print("eIndex = " + str(eIndex))
			
			POSambiguityInfoPermutation = []
			currentWordPOSfound = False
			currentWordIndex = 0
			currentFeatureIndex = 0
			POSambiguousWordFoundInSentence = False
			outOfSentenceFound = False
			sentenceLength = -1
			for f in range(datasetNumFeatures):
				if(all_X[e][f] == xPOStagActive):
					if(currentWordPOSfound):
						#POS ambiguous word found
						POSambiguousWordFoundInSentence = True			
					else:
						currentWordPOSfound = True
			
				if(all_X[e][f] == paddingTagIndex):
					sentenceLength = currentWordIndex
					outOfSentenceFound = True
					break
					
				if(currentFeatureIndex == numberOfFeaturesPerWord-1):
					currentFeatureIndex = 0
					currentWordIndex = currentWordIndex + 1
					currentWordPOSfound = False
				else:
					currentFeatureIndex = currentFeatureIndex+1
					
			if(not POSambiguousWordFoundInSentence):
				Xexample = np.expand_dims(all_X[e], axis=0)
				all_Xunambiguous = np.append(all_Xunambiguous, Xexample, axis=0)
				
			eIndex = eIndex + 1
			
		all_X = all_Xunambiguous
							
	elif(generatePOSunambiguousInput):
	
		#Incomplete
		
		datasetNumExamples = all_X.shape[0]
		datasetNumFeatures = all_X.shape[1]
		all_Xunambiguous = np.empty((0, datasetNumFeatures), dataType)
		
		xPOStagActive = 1
		xPOStagInactive = 0
		eIndex = 0
		for e in range(datasetNumExamples):
		
			print("eIndex = " + str(eIndex))
			
			POSambiguityInfoPermutation = []
			currentWordPOS = [False]*GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES_EXTENDED
			currentWordIndex = 0
			currentFeatureIndex = 0
			for f in range(datasetNumFeatures):
				if(all_X[e][f] == xPOStagActive):
					currentWordPOS[currentFeatureIndex] = True
				if(currentFeatureIndex == numberOfFeaturesPerWord-1):
					currentFeatureIndex = 0
					currentWordIndex = currentWordIndex + 1
					POSambiguityInfoPermutation.append(currentWordPOS)
					currentWordPOS = [False]*GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES_EXTENDED	#TODO: verify that this doesn't overwrite previous appended currentWordPOS object
								
			POSambiguityInfoUnambiguousPermutationArray = []
			POSambiguityInfoUnambiguousPermutationNew = [[False]*GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES_EXTENDED]*len(POSambiguityInfoPermutation)
			POSambiguityInfoUnambiguousPermutationArray.append(POSambiguityInfoUnambiguousPermutationNew)
			generatePOSambiguityInfoUnambiguousPermutationArray(POSambiguityInfoUnambiguousPermutationArray, POSambiguityInfoPermutation, POSambiguityInfoUnambiguousPermutationNew, 0)
			
			POSambiguityInfoUnambiguousPermutationIndex = 0
			for POSambiguityInfoUnambiguousPermutation in POSambiguityInfoUnambiguousPermutationArray:
				print("POSambiguityInfoUnambiguousPermutationIndex = " + str(POSambiguityInfoUnambiguousPermutationIndex))
				XexamplePOSunambigousVersion = np.zeros((1, datasetNumFeatures), dtype=dataType)
				XexamplePOSunambigousVersionIndex = 0
				for word in POSambiguityInfoUnambiguousPermutation:
					for POSvalue in word:
						if(POSvalue == True):
							XexamplePOSunambigousVersion[0][XexamplePOSunambigousVersionIndex] = xPOStagActive
						else:
							XexamplePOSunambigousVersion[0][XexamplePOSunambigousVersionIndex] = xPOStagInactive
						XexamplePOSunambigousVersionIndex = XexamplePOSunambigousVersionIndex + 1
						
				np.append(all_Xunambiguous, XexamplePOSunambigousVersion, axis=0)
				
				print(all_Xunambiguous.shape)
				print(XexamplePOSunambigousVersion.shape)

				POSambiguityInfoUnambiguousPermutationIndex = POSambiguityInfoUnambiguousPermutationIndex + 1
			
			eIndex = eIndex + 1
			
		all_X = all_Xunambiguous
				

	datasetNumExamples = all_X.shape[0]
	
	print("datasetNumExamples = ", datasetNumExamples)
	if(generateNegativeExamples):
		datasetNumClasses = 2
	else:
		datasetNumClasses = 1
	datasetNumFeatures = all_X.shape[1]
	
	if(generateNegativeExamples):
	
		all_Xnegative = np.empty(all_X.shape) #all_X
		all_positive = np.empty([all_X.shape[0], all_X.shape[1]+1])
		all_negative = np.empty([all_X.shape[0], all_X.shape[1]+1])
		#NO: generate a negative (non-grammatical) example by randomly shuffling POS values in array?
		#np.random.shuffle(all_Xnegative)
		#generate a negative (non-grammatical) example by randomly shuffling words in array.
		for e in range(datasetNumExamples):
			#print("e = ", e)
			exampleX = all_X[e]
			if(optimiseFeedLength):
				exampleXmaxFeatureLength = np.argmax(exampleX==paddingTagIndex)
			else:
				exampleXmaxFeatureLength = len(exampleX)
			#print("exampleXmaxFeatureLength = ", exampleXmaxFeatureLength)
			#print("len(exampleX) = ", len(exampleX))
			exampleXnumWords = int(exampleXmaxFeatureLength/numberOfFeaturesPerWord)
			exampleXnp = np.asarray(exampleX[0:exampleXmaxFeatureLength])
			exampleXReshaped = np.reshape(exampleXnp, (exampleXnumWords, numberOfFeaturesPerWord))
			exampleXReshapedShuffled = exampleXReshaped
			np.random.shuffle(exampleXReshapedShuffled)
			exampleXReshapedShuffledFlattened = np.ndarray.flatten(exampleXReshapedShuffled)
			if(optimiseFeedLength):
				exampleXReshapedShuffledFlattened = np.pad(exampleXReshapedShuffledFlattened, (0, len(exampleX)-exampleXmaxFeatureLength), 'constant', constant_values=(paddingTagIndex, paddingTagIndex))	#pad
			all_Xnegative[e] = exampleXReshapedShuffledFlattened

			if(generateYvalues):
				all_positive[e] = np.concatenate(([yClassPositive], all_X[e]), axis=0)
				all_negative[e] = np.concatenate(([yClassNegative], all_Xnegative[e]), axis=0)

	
		all_ = np.concatenate((all_positive, all_negative), axis=0)
		
		#randomise data
		dataRawRandomised = all_
		np.random.shuffle(dataRawRandomised)
		all_X = dataRawRandomised[:,1:]
		all_Y = dataRawRandomised[:,0]
		
		datasetNumExamples = datasetNumExamples*2	#positive and negative examples
	else:
		all_Y = np.ones(datasetNumExamples, dtype=dataType)
		#all_Y = np.expand_dims(all_Y, axis=0)

	if(ANNtf2_globalDefs.testHarness):
		datasetNumExamplesTrain = datasetNumExamples
		datasetNumExamplesTest = 0
	else:
		datasetNumExamplesTrain = int(float(datasetNumExamples)*percentageDatasetTrain/100.0)
		datasetNumExamplesTest = int(float(datasetNumExamples)*(100.0-percentageDatasetTrain)/100.0)

	
	all_Ynormalised = all_Y
	
	if(datasetType3alreadyNormalised):
		all_Xnormalised = all_X
	else:
		#normalise x data between 0.0 and 1.0:
		all_Xnormalised = all_X #np.zeros(all_X.shape)
		for i in range(datasetNumFeatures):
			maxXi = np.amax(all_X[:,i])
			minXi = np.amin(all_X[:,i])
			all_Xnormalised[:,i] = (all_X[:,i] - minXi) / (maxXi - minXi) #all_X[:,i]/maxXi		

	train_x = all_Xnormalised[0:datasetNumExamplesTrain, :]
	test_x = all_Xnormalised[-datasetNumExamplesTest:, :]
	train_y = all_Ynormalised[0:datasetNumExamplesTrain]
	test_y = all_Ynormalised[-datasetNumExamplesTest:]
	
	#print(train_x)
	
	if(not getDataAsBinary):	
		# Convert x/y data to float32/uint8.
		if(getDataAsInt):
			train_x, test_x = np.array(train_x, np.int32), np.array(test_x, np.int32)
		else:
			train_x, test_x = np.array(train_x, np.float32), np.array(test_x, np.float32)
		train_y, test_y = np.array(train_y, np.uint8), np.array(test_y, np.uint8)
		#https://www.tensorflow.org/api_docs/python/tf/keras/datasets/mnist/load_data?version=stable
		#https://medium.com/@HojjatA/could-not-find-valid-device-for-node-while-eagerly-executing-8f2ff588d1e

	return numberOfFeaturesPerWord, paddingTagIndex, datasetNumFeatures, datasetNumClasses, datasetNumExamples, train_x, train_y, test_x, test_y


def generatePOSambiguityInfoUnambiguousPermutationArray(POSambiguityInfoUnambiguousPermutationArray, POSambiguityInfoPermutation, POSambiguityInfoUnambiguousPermutationLocal, wordIndex):

	if(wordIndex < len(POSambiguityInfoPermutation)):

		POSambiguityInfo = POSambiguityInfoPermutation[wordIndex]
		#maxValForPOSambiguousEntry = int64_t(1U) << GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES;
		#cout << "maxValForPOSambiguousEntry = " << maxValForPOSambiguousEntry << endl;
		#cout << "POSambiguityInfo = " << POSambiguityInfo << endl;
				
		if((max(POSambiguityInfo) > 0) and (max(POSambiguityInfo[GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES:]) == 0)):
			if(POSambiguityInfoUnambiguousPermutationLocal in POSambiguityInfoUnambiguousPermutationArray):
				#remove POSambiguityInfoUnambiguousPermutationLocal from POSambiguityInfoUnambiguousPermutationArray as it will be replicated
				POSambiguityInfoUnambiguousPermutationArray.remove(POSambiguityInfoUnambiguousPermutationLocal)
			else:
 				print("generatePOSambiguityInfoUnambiguousPermutationArray error{}: POSambiguityInfoUnambiguousPermutationLocal not found in POSambiguityInfoUnambiguousPermutationArray")
 				exit
			
			for x in range(GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES):
				if(SHAREDPOSambiguityInfo[x]):
					print("x = " + x)
					POSambiguityInfoUnambiguousPermutationNew = POSambiguityInfoUnambiguousPermutationLocal.copy()
					POSambiguityInfoUnambiguousPermutationArray.append(POSambiguityInfoUnambiguousPermutationNew)
					POSambiguityInfoUnambiguousPermutationNew[wordIndex][x] = True	#create a new unambigious hypothetical POS value
					generatePOSambiguityInfoUnambiguousPermutationArray(POSambiguityInfoUnambiguousPermutationArray, POSambiguityInfoPermutation, POSambiguityInfoUnambiguousPermutationNew, wordIndex+1)
			
			del POSambiguityInfoUnambiguousPermutationLocal
		else:
			#special POS type (e.g. punctuation; GIA_PREPROCESSOR_POS_TAGGER_DATABASE_POS_INDEX_OUT_OF_SENTENCE_BOUNDS)
			POSambiguityInfoUnambiguousPermutationLocal[wordIndex] = POSambiguityInfo
			generatePOSambiguityInfoUnambiguousPermutationArray(POSambiguityInfoUnambiguousPermutationArray, POSambiguityInfoPermutation, POSambiguityInfoUnambiguousPermutationLocal, wordIndex+1)

