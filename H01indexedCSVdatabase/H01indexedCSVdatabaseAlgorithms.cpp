/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseAlgorithms.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021-2023 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: H01LocalConnectome
 * Requirements: see H01indexedCSVdatabaseGlobalDefs.hpp
 * Compilation: see H01indexedCSVdatabaseGlobalDefs.hpp
 * Usage: see H01indexedCSVdatabaseGlobalDefs.hpp
 * Description: H01 indexed CSV database algorithms
 * Comments:
 * /
 *******************************************************************************/

#include "H01indexedCSVdatabaseAlgorithms.hpp"


#ifdef INDEXED_CSV_DATABASE_ALGORITHMS
H01indexedCSVdatabaseAlgorithmsFit::H01indexedCSVdatabaseAlgorithmsFit(void)
{	
	connectionNeuronID = 0;
	estSynapseType = 0;
	#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_2D_POLY_REGRESSION
	a = 0.0;
	b = 0.0;
	c = 0.0;
	#endif
	#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_3D_LINEAR_REGRESSION
	origin.x = 0.0;
	origin.y = 0.0;
	origin.z = 0.0;
	axis.x = 0.0;
	axis.y = 0.0;
	axis.z = 0.0;
	#endif
}
H01indexedCSVdatabaseAlgorithmsFit::~H01indexedCSVdatabaseAlgorithmsFit(void)
{
}
#ifdef INDEXED_CSV_DATABASE_QUERY_PERFORM_INCOMING_AXON_MAPPING_2D_POLY_REGRESSION
double H01indexedCSVdatabaseAlgorithmsFit::calculatePoly(int xx)
{
	return a + b * xx + c * xx*xx;
}
#endif
#endif
