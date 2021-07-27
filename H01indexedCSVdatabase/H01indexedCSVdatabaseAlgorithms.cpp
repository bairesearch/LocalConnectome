/*******************************************************************************
 *
 * File Name: H01indexedCSVdatabaseAlgorithms.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: H01LocalConnectome
 * Requirements: see H01indexedCSVdatabase.hpp
 * Compilation: see H01indexedCSVdatabase.hpp
 * Usage: see H01indexedCSVdatabase.hpp
 * Description: H01 indexed CSV database algorithms
 * Comments:
 * /
 *******************************************************************************/

#include "H01indexedCSVdatabaseAlgorithms.hpp"

H01indexedCSVdatabaseAlgorithmsPolyFit::H01indexedCSVdatabaseAlgorithmsPolyFit(void)
{	
	connectionNeuronID = 0;
	estSynapseType = 0;
	a = 0.0;
	b = 0.0;
	c = 0.0;
}
H01indexedCSVdatabaseAlgorithmsPolyFit::~H01indexedCSVdatabaseAlgorithmsPolyFit(void)
{
}
double H01indexedCSVdatabaseAlgorithmsPolyFit::calculatePoly(int xx)
{
	return a + b * xx + c * xx*xx;
}
