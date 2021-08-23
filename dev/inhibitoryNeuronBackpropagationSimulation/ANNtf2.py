# -*- coding: utf-8 -*-
"""ANNtf2.py

# Requirements:
Python 3 and Tensorflow 2.1+ 

# License:
MIT License

# Usage:
python3 ANNtf2.py

# Description

Train an artificial neural network (EIANN)

- Author: Richard Bruce Baxter - Copyright (c) 2020-2021 Baxter AI (baxterai.com)

"""

# %tensorflow_version 2.x
import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3' 

import tensorflow as tf
import numpy as np

import sys
np.set_printoptions(threshold=sys.maxsize)

from ANNtf2_operations import *
import ANNtf2_globalDefs
from numpy import random
import ANNtf2_loadDataset

algorithm = "EIANN"

y_true = [[0.0, 1.]]
y_pred = [[0.9, 1.]]
# Using 'none' reduction type.
mse = tf.keras.losses.MeanSquaredError(reduction=tf.keras.losses.Reduction.NONE)
trialLoss = mse(y_true, y_pred).numpy()
print("trialLoss = ", trialLoss)
			
			
suppressGradientDoNotExistForVariablesWarnings = True

costCrossEntropyWithLogits = False

if(algorithm == "EIANN"):
	import ANNtf2_algorithmEIANN as ANNtf2_algorithmEIANN
					
#learningRate, trainingSteps, batchSize, displayStep, numEpochs = -1

#performance enhancements for development environment only: 
debugUseSmallPOStagSequenceDataset = True	#def:False	#switch increases performance during development	#eg data-POStagSentence-smallBackup
useSmallSentenceLengths = True	#def:False	#switch increases performance during development	#eg data-simple-POStagSentence-smallBackup
trainMultipleFiles = False	#def:True	#switch increases performance during development	#eg data-POStagSentence
trainMultipleNetworks = False	#improve classification accuracy by averaging over multiple independently trained networks (test)
if(trainMultipleNetworks):
	numberOfNetworks = 5
else:
	numberOfNetworks = 1
	
#loadDatasetType3 parameters:
#if generatePOSunambiguousInput=True, generate POS unambiguous permutations for every POS ambiguous data example/experience
#if onlyAddPOSunambiguousInputToTrain=True, do not train network with ambiguous POS possibilities
#if generatePOSunambiguousInput=False and onlyAddPOSunambiguousInputToTrain=False, requires simultaneous propagation of different (ambiguous) POS possibilities


if(algorithm == "EIANN"):
	#dataset = "POStagSequence"
	dataset = "SmallDataset"
	#trainMultipleNetworks = True	#default: False
	#numberOfNetworks = 3	#default: 1		
	
if(dataset == "SmallDataset"):
	smallDatasetIndex = 0 #default: 0 (New Thyroid)
	#trainMultipleFiles = False	#required
	smallDatasetDefinitionsHeader = {'index':0, 'name':1, 'fileName':2, 'classColumnFirst':3}	
	smallDatasetDefinitions = [
	(0, "New Thyroid", "new-thyroid.data", True),
	(1, "Swedish Auto Insurance", "UNAVAILABLE.txt", False),	#AutoInsurSweden.txt BAD
	(2, "Wine Quality Dataset", "winequality-whiteFormatted.csv", False),
	(3, "Pima Indians Diabetes Dataset", "pima-indians-diabetes.csv", False),
	(4, "Sonar Dataset", "sonar.all-data", False),
	(5, "Banknote Dataset", "data_banknote_authentication.txt", False),
	(6, "Iris Flowers Dataset", "iris.data", False),
	(7, "Abalone Dataset", "UNAVAILABLE", False),	#abaloneFormatted.data BAD
	(8, "Ionosphere Dataset", "ionosphere.data", False),
	(9, "Wheat Seeds Dataset", "seeds_datasetFormatted.txt", False),
	(10, "Boston House Price Dataset", "UNAVAILABLE", False)	#housingFormatted.data BAD
	]
	datasetFileName = smallDatasetDefinitions[smallDatasetIndex][smallDatasetDefinitionsHeader['fileName']]
	datasetClassColumnFirst = smallDatasetDefinitions[smallDatasetIndex][smallDatasetDefinitionsHeader['classColumnFirst']]
	print("datasetFileName = ", datasetFileName)
	print("datasetClassColumnFirst = ", datasetClassColumnFirst)
			
if(debugUseSmallPOStagSequenceDataset):
	datasetFileNameXstart = "XdatasetPartSmall"
	datasetFileNameYstart = "YdatasetPartSmall"
else:
	datasetFileNameXstart = "XdatasetPart"
	datasetFileNameYstart = "YdatasetPart"
datasetFileNameXend = ".dat"
datasetFileNameYend = ".dat"
datasetFileNameStart = "datasetPart"
datasetFileNameEnd = ".dat"


	
	
def neuralNetworkPropagation(x, networkIndex=1):
	if(algorithm == "EIANN"):
		pred = ANNtf2_algorithmEIANN.neuralNetworkPropagationEIANN(x, networkIndex)
	return pred
	
			
def executeOptimisation(x, y, networkIndex=1):
	with tf.GradientTape() as gt:
		pred = neuralNetworkPropagation(x, networkIndex)
		loss = crossEntropy(pred, y, datasetNumClasses, costCrossEntropyWithLogits)
		
	if(algorithm == "EIANN"):
		Wlist = []
		Blist = []
		for l in range(1, numberOfLayers+1):
			if(ANNtf2_algorithmEIANN.debugOnlyTrainFinalLayer):
				if(l == numberOfLayers):
					Wlist.append(ANNtf2_algorithmEIANN.W[generateParameterNameNetwork(networkIndex, l, "W")])
					Blist.append(ANNtf2_algorithmEIANN.B[generateParameterNameNetwork(networkIndex, l, "B")])				
			else:	
				Wlist.append(ANNtf2_algorithmEIANN.W[generateParameterNameNetwork(networkIndex, l, "W")])
				Blist.append(ANNtf2_algorithmEIANN.B[generateParameterNameNetwork(networkIndex, l, "B")])
		trainableVariables = Wlist + Blist
		WlistLength = len(Wlist)
		BlistLength = len(Blist)
		
	gradients = gt.gradient(loss, trainableVariables)
	
	#print("len(gradients) = ", len(gradients))
	#print("WlistLength = ", WlistLength)
	#print("BlistLength = ", BlistLength)
	
	#if(algorithm == "EIANN"):
	#	#gradientsWdeltAList = 
	#	#gradientsBdeltaList = 
	#	gradientIndex = 0
	#	for l in range(1, numberOfLayers+1):
	#		WdeltalistGradientIndex = gradientIndex+l-1
	#		neuronEIlayerPrevious = ANNtf2_algorithmEIANN.neuronEI[generateParameterNameNetwork(networkIndex, l-1, "neuronEI")]
	#		Wlayer = ANNtf2_algorithmEIANN.W[generateParameterNameNetwork(networkIndex, l, "W")]
	#		WdeltaLayer = gradients[WdeltalistGradientIndex]
	#		WdeltaLayerAdjustment = tf.multiply(WdeltaLayer, learningRate)	#CHECKTHIS (assumes constant learning rate; no learning rate optimisation - see optimizer = tf.optimizers.SGD(learningRate))
	#		WlayerNew = tf.subtract(Wlayer, WdeltaLayerAdjustment)
	#		WlayerNewSign = tf.sign(WlayerNew)
	#		WlayerNewSignBool = tf.dtypes.cast(WlayerNewSign, dtype=tf.dtypes.bool)
	#		neuronEIlayerPreviousTiled = tileDimension(neuronEIlayerPrevious, 1, ANNtf2_algorithmEIANN.n_h[l], True)
	#		WlayerNewSignOK = tf.equal(WlayerNewSignBool, neuronEIlayerPreviousTiled)
	#		WlayerNewSignOK = tf.dtypes.cast(WlayerNewSignOK, dtype=tf.dtypes.float32)
	#		WdeltaLayerMod = tf.multiply(WdeltaLayer, WlayerNewSignOK)
	#		gradients[WdeltalistGradientIndex] = WdeltaLayerMod
	#	gradientIndex = WlistLength
	#	for l in range(1, numberOfLayers+1):
	#		BdeltalistGradientIndex = gradientIndex+l-1
	#		neuronEIlayer = ANNtf2_algorithmEIANN.neuronEI[generateParameterNameNetwork(networkIndex, l, "neuronEI")]
	#		Blayer = ANNtf2_algorithmEIANN.B[generateParameterNameNetwork(networkIndex, l, "B")]
	#		BdeltaLayer = gradients[BdeltalistGradientIndex]
	#		BdeltaLayerAdjustment = tf.multiply(BdeltaLayer, learningRate)	#CHECKTHIS (assumes constant learning rate; no learning rate optimisation - see optimizer = tf.optimizers.SGD(learningRate))
	#		BlayerNew = tf.subtract(Blayer, BdeltaLayerAdjustment)
	#		BlayerNewSign = tf.sign(BlayerNew)
	#		BlayerNewSignBool = tf.dtypes.cast(BlayerNewSign, dtype=tf.dtypes.bool)
	#		BlayerNewSignOK = tf.equal(BlayerNewSignBool, neuronEIlayer)
	#		BlayerNewSignOK = tf.dtypes.cast(BlayerNewSignOK, dtype=tf.dtypes.float32)
	#		BdeltaLayerMod = tf.multiply(BdeltaLayer, BlayerNewSignOK)
	#		gradients[BdeltalistGradientIndex] = BdeltaLayerMod
						
	if(suppressGradientDoNotExistForVariablesWarnings):
		optimizer.apply_gradients([
    		(grad, var) 
    		for (grad, var) in zip(gradients, trainableVariables) 
    		if grad is not None
			])
	else:
		optimizer.apply_gradients(zip(gradients, trainableVariables))

	if(algorithm == "EIANN"):
		#set all W/B parameters to zero if their updated values violate the E/I neuron type condition
		for l in range(1, numberOfLayers+1):

			neuronEIlayerPrevious = ANNtf2_algorithmEIANN.neuronEI[generateParameterNameNetwork(networkIndex, l-1, "neuronEI")]
			neuronEIlayerPreviousTiled = tileDimension(neuronEIlayerPrevious, 1, ANNtf2_algorithmEIANN.n_h[l], True)
			neuronEI = ANNtf2_algorithmEIANN.neuronEI[generateParameterNameNetwork(networkIndex, l, "neuronEI")]

			Wlayer = ANNtf2_algorithmEIANN.W[generateParameterNameNetwork(networkIndex, l, "W")]
			WlayerSign = tf.sign(Wlayer)
			WlayerSignBool = convertSignOutputToBool(WlayerSign)
			Blayer = ANNtf2_algorithmEIANN.B[generateParameterNameNetwork(networkIndex, l, "B")]
			BlayerSign = tf.sign(Blayer)
			BlayerSignBool = convertSignOutputToBool(BlayerSign)
			
			WlayerSignCheck = tf.equal(WlayerSignBool, neuronEIlayerPreviousTiled)
			BlayerSignCheck = tf.equal(BlayerSignBool, neuronEI)
			
			#ignore 0.0 values in W/B arrays:
			WlayerSignCheck = tf.logical_or(WlayerSignCheck, tf.equal(WlayerSign, 0.0))
			BlayerSignCheck = tf.logical_or(BlayerSignCheck, tf.equal(BlayerSign, 0.0))
	
			WlayerCorrected = tf.where(WlayerSignCheck, Wlayer, 0.0)
			BlayerCorrected = tf.where(BlayerSignCheck, Blayer, 0.0)
			#print("WlayerCorrected = ", WlayerCorrected)	   
			#print("BlayerCorrected = ", BlayerCorrected)
						
			ANNtf2_algorithmEIANN.W[generateParameterNameNetwork(networkIndex, l, "W")] = WlayerCorrected
			ANNtf2_algorithmEIANN.B[generateParameterNameNetwork(networkIndex, l, "B")] = BlayerCorrected

		#excitatory/inhibitory weight verification (in accordance with neuron types):	
		for l in range(1, numberOfLayers+1):

			neuronEIlayerPrevious = ANNtf2_algorithmEIANN.neuronEI[generateParameterNameNetwork(networkIndex, l-1, "neuronEI")]
			neuronEIlayerPreviousTiled = tileDimension(neuronEIlayerPrevious, 1, ANNtf2_algorithmEIANN.n_h[l], True)
			neuronEI = ANNtf2_algorithmEIANN.neuronEI[generateParameterNameNetwork(networkIndex, l, "neuronEI")]

			Wlayer = ANNtf2_algorithmEIANN.W[generateParameterNameNetwork(networkIndex, l, "W")]
			WlayerSign = tf.sign(Wlayer)
			WlayerSignBool = convertSignOutputToBool(WlayerSign)
			Blayer = ANNtf2_algorithmEIANN.B[generateParameterNameNetwork(networkIndex, l, "B")]
			BlayerSign = tf.sign(Blayer)
			BlayerSignBool = convertSignOutputToBool(BlayerSign)
			
			WlayerSignCheck = tf.equal(WlayerSignBool, neuronEIlayerPreviousTiled)
			BlayerSignCheck = tf.equal(BlayerSignBool, neuronEI)
			
			#ignore 0.0 values in W/B arrays:
			WlayerSignCheck = tf.logical_or(WlayerSignCheck, tf.equal(WlayerSign, 0.0))
			BlayerSignCheck = tf.logical_or(BlayerSignCheck, tf.equal(BlayerSign, 0.0))

			WlayerSignCheck = tf.math.reduce_all(WlayerSignCheck).numpy()
			BlayerSignCheck = tf.math.reduce_all(WlayerSignCheck).numpy()
			
			#print("WlayerSignCheck = ", WlayerSignCheck)	   
			#print("BlayerSignCheck = ", BlayerSignCheck)
			#print("Wlayer = ", Wlayer)	   
			#print("Blayer = ", Blayer)
					
			if(not WlayerSignCheck):
			   print("!WlayerSignCheck, l = ", l)
			   print("neuronEIlayerPrevious = ", neuronEIlayerPrevious)
			   print("Wlayer = ", Wlayer)
			if(not BlayerSignCheck):
			   print("!BlayerSignCheck, l = ", l)
			   print("neuronEI = ", neuronEI)
			   print("Blayer = ", Blayer)
						
																
if __name__ == "__main__":

	#generate network parameters based on dataset properties:

	#global learningRate, trainingSteps, batchSize, displayStep, numEpochs
	
	datasetNumExamplesTemp = 0
	datasetNumFeatures = 0
	datasetNumClasses = 0

	fileIndexTemp = 0
	fileIndexStr = str(fileIndexTemp).zfill(4)
	if(dataset == "POStagSequence"):
		datasetType1FileNameX = datasetFileNameXstart + fileIndexStr + datasetFileNameXend
		datasetType1FileNameY = datasetFileNameYstart + fileIndexStr + datasetFileNameYend
	elif(dataset == "SmallDataset"):
		if(trainMultipleFiles):
			datasetType2FileName = datasetFileNameStart + fileIndexStr + datasetFileNameEnd
		else:
			datasetType2FileName = datasetFileName

	numberOfLayers = 0
	if(dataset == "POStagSequence"):
		datasetNumFeatures, datasetNumClasses, datasetNumExamplesTemp, train_xTemp, train_yTemp, test_xTemp, test_yTemp = ANNtf2_loadDataset.loadDatasetType1(datasetType1FileNameX, datasetType1FileNameY)
	elif(dataset == "POStagSentence"):
		numberOfFeaturesPerWord, paddingTagIndex, datasetNumFeatures, datasetNumClasses, datasetNumExamplesTemp, train_xTemp, train_yTemp, test_xTemp, test_yTemp = ANNtf2_loadDataset.loadDatasetType3(datasetType1FileNameX, generatePOSunambiguousInput, onlyAddPOSunambiguousInputToTrain, useSmallSentenceLengths)
	elif(dataset == "SmallDataset"):
		datasetNumFeatures, datasetNumClasses, datasetNumExamplesTemp, train_xTemp, train_yTemp, test_xTemp, test_yTemp = ANNtf2_loadDataset.loadDatasetType2(datasetType2FileName, datasetClassColumnFirst)


	#Model constants
	num_input_neurons = datasetNumFeatures  #train_x.shape[1]
	num_output_neurons = datasetNumClasses

	if(algorithm == "EIANN"):
		learningRate, trainingSteps, batchSize, displayStep, numEpochs = ANNtf2_algorithmEIANN.defineTrainingParametersEIANN(dataset, trainMultipleFiles)
		numberOfLayers = ANNtf2_algorithmEIANN.defineNetworkParametersEIANN(num_input_neurons, num_output_neurons, datasetNumFeatures, dataset, trainMultipleFiles, numberOfNetworks)
		ANNtf2_algorithmEIANN.defineNeuralNetworkParametersEIANN()
					
	#define epochs:

	if(trainMultipleFiles):
		fileIndexFirst = 0
		if(useSmallSentenceLengths):
			fileIndexLast = 11
		else:
			fileIndexLast = 1202

	noisySampleGeneration = False

	# Stochastic gradient descent optimizer.
	optimizer = tf.optimizers.SGD(learningRate)

	for e in range(numEpochs):

		print("epoch e = ", e)

		if(trainMultipleFiles):
			fileIndexArray = np.arange(fileIndexFirst, fileIndexLast+1, 1)
			#print("fileIndexArray = " + str(fileIndexArray))
			np.random.shuffle(fileIndexArray)
			fileIndexShuffledArray = fileIndexArray
			#print("fileIndexShuffledArray = " + str(fileIndexShuffledArray))
		else:
			fileIndexShuffledArray = [0]
			
		for fileIndex in fileIndexShuffledArray:	#range(fileIndexFirst, fileIndexLast+1):

			#print("fileIndex = ", fileIndex)
			
			datasetNumExamples = 0

			fileIndexStr = str(fileIndex).zfill(4)
			if(dataset == "POStagSequence"):
				datasetType1FileNameX = datasetFileNameXstart + fileIndexStr + datasetFileNameXend
				datasetType1FileNameY = datasetFileNameYstart + fileIndexStr + datasetFileNameYend
			elif(dataset == "SmallDataset"):
				if(trainMultipleFiles):
					datasetType2FileName = datasetFileNameStart + fileIndexStr + datasetFileNameEnd
				else:
					datasetType2FileName = datasetFileName

			if(dataset == "POStagSequence"):
				datasetNumFeatures, datasetNumClasses, datasetNumExamples, train_x, train_y, test_x, test_y = ANNtf2_loadDataset.loadDatasetType1(datasetType1FileNameX, datasetType1FileNameY)
			if(dataset == "POStagSentence"):
				numberOfFeaturesPerWord, paddingTagIndex, datasetNumFeatures, datasetNumClasses, datasetNumExamples, train_x, train_y, test_x, test_y = ANNtf2_loadDataset.loadDatasetType3(datasetType1FileNameX, generatePOSunambiguousInput, onlyAddPOSunambiguousInputToTrain, useSmallSentenceLengths)
			elif(dataset == "SmallDataset"):
				datasetNumFeatures, datasetNumClasses, datasetNumExamples, train_x, train_y, test_x, test_y = ANNtf2_loadDataset.loadDatasetType2(datasetType2FileName, datasetClassColumnFirst)

			shuffleSize = datasetNumExamples	#heuristic: 10*batchSize
			
			#new iteration method (only required for algorithm == "CANN_expAUANN/CANN_expCUANN"):	
			datasetNumClassesActual = datasetNumClasses
			trainDataIndex = 0
	
			trainData = generateTFtrainDataFromNParrays(train_x, train_y, shuffleSize, batchSize)
			trainDataList = []
			trainDataList.append(trainData)
			
			trainDataListIterators = []
			for trainData in trainDataList:
				trainDataListIterators.append(iter(trainData))
	
			#original iteration method:
			#trainData = generateTFtrainDataFromNParrays(train_x, train_y, shuffleSize, batchSize):	
			#for batchIndex, (batchX, batchY) in enumerate(trainData.take(trainingSteps), 1):	
					
			#new iteration method:			
			#print("trainingSteps = ", trainingSteps)
			#print("batchSize = ", batchSize)
			
			for batchIndex in range(int(trainingSteps)):
				(batchX, batchY) = trainDataListIterators[trainDataIndex].get_next()	#next(trainDataListIterators[trainDataIndex])
				
				batchYactual = batchY
														
				if(noisySampleGeneration):
					if(batchSize != 1):	#batchX.shape[0]
						print("error: noisySampleGeneration && batchSize != 1")
						exit()
					batchX = tf.tile(batchX, batchXmultiples)
					batchY = tf.tile(batchY, batchYmultiples)
					batchXnoise = tf.math.multiply(tf.constant(randomNormal(batchX.shape), tf.float32), noiseStandardDeviation)
					batchX = tf.math.add(batchX, batchXnoise)
					#print("batchX = ", batchX)
					#print("batchY = ", batchY)

				predNetworkAverage = tf.Variable(tf.zeros(datasetNumClasses))

				#print("datasetNumClasses = ", datasetNumClasses)
				#print("batchX.shape = ", batchX.shape)
				#print("batchY.shape = ", batchY.shape)
				
				for networkIndex in range(1, numberOfNetworks+1):

					if(algorithm == "EIANN"):
						executeOptimisation(batchX, batchY, networkIndex)
						if(batchIndex % displayStep == 0):
							pred = neuralNetworkPropagation(batchX, networkIndex)
							#print("pred.shape = ", pred.shape)
							loss = crossEntropy(pred, batchY, datasetNumClasses, costCrossEntropyWithLogits)
							acc = calculateAccuracy(pred, batchY)
							print("networkIndex: %i, batchIndex: %i, loss: %f, accuracy: %f" % (networkIndex, batchIndex, loss, acc))
							predNetworkAverage = predNetworkAverage + pred
											
				if(batchIndex % displayStep == 0):
					if(trainMultipleNetworks):
						predNetworkAverage = predNetworkAverage / numberOfNetworks
						loss = crossEntropy(predNetworkAverage, batchYactual, datasetNumClasses, costCrossEntropyWithLogits)
						acc = calculateAccuracy(predNetworkAverage, batchYactual)
						print("batchIndex: %i, loss: %f, accuracy: %f" % (batchIndex, loss, acc))	


			predNetworkAverageAll = tf.Variable(tf.zeros([test_y.shape[0], datasetNumClasses]))
			for networkIndex in range(1, numberOfNetworks+1):		
				if(algorithm == "EIANN"):
					pred = neuralNetworkPropagation(test_x, networkIndex)	#test_x batch may be too large to propagate simultaneously and require subdivision
					print("Test Accuracy: networkIndex: %i, %f" % (networkIndex, calculateAccuracy(pred, test_y)))
					predNetworkAverageAll = predNetworkAverageAll + pred
			
			if(trainMultipleNetworks):
					predNetworkAverageAll = predNetworkAverageAll / numberOfNetworks
					#print("predNetworkAverageAll", predNetworkAverageAll)
					acc = calculateAccuracy(predNetworkAverageAll, test_y)
					print("Test Accuracy: %f" % (acc))
