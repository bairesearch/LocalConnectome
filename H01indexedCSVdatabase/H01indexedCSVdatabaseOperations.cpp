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
