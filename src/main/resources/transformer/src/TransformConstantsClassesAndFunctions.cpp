/**
 * @file TransformConstantsClassesAndFunctions.cpp
 * Defines the constants, classes and some functions necessary for the
 * transform programs.
 *
 * @brief Defines the constants, types and some functions necessary for
 * the transform programs.
 *
 * @author Ulrich Pferschy and Rostislav Stanek
 * (Institut fuer Statistik und Operations Research, Universitaet Graz)
 */

#include <iostream>
#include <stdexcept>
#include <limits>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

#include "TransformDOMErrorHandler.hpp"

#include "TransformConstantsClassesAndFunctions.hpp"

using namespace std;
using namespace xercesc;

XERCES_CPP_NAMESPACE_USE


void TransformInstance::init(
		const std::string &type,
		const std::string &name,
		const std::string &source,
		const std::string &description,
		const std::vector<std::vector<double> >::size_type n) {
	this->type = type;
	this->name = name;
	this->source = source;
	this->description = description;
	this->n = n;
}

TransformInstance::TransformInstance(
		const std::string &type,
		const std::string &name,
		const std::string &source,
		const std::string &description,
		const std::vector<std::vector<double> >::size_type n) {
	init(type, name, source, description, n);

	adjacencyMatrix.resize(n);
	if (type == VALUE_TYPE_TSP) {
		for (vector<vector<double> >::size_type i = 0; i < n; i++) {
			adjacencyMatrix.at(i).resize(i + 1);
		}
	}
	else {  //if (type == VALUE_TYPE_ATSP)
		for (vector<vector<double> >::size_type i = 0; i < n; i++) {
			adjacencyMatrix.at(i).resize(n);
		}
	}
}

TransformInstance::TransformInstance(const TransformInstance &transformInstance) {
	init(
			transformInstance.getType(),
			transformInstance.getName(),
			transformInstance.getSource(),
			transformInstance.getDescription(),
			transformInstance.getN());

	adjacencyMatrix.resize(n);
	for (vector<vector<double> >::size_type i = 0; i < n; i++) {
		adjacencyMatrix.at(i).resize(i + 1);
		for (vector<double>::size_type j = 0; j < n; j++) {
			setAdjacencyMatrixElement(i, j, getAdjacencyMatrixElement(i, j));
		}
	}
}

TransformInstance &TransformInstance::operator=(const TransformInstance &transformInstance) {
	if (this == &transformInstance) {
		return (*this);
	}
	else {
		init(
				transformInstance.getType(),
				transformInstance.getName(),
				transformInstance.getSource(),
				transformInstance.getDescription(),
				transformInstance.getN());

		adjacencyMatrix.resize(n);
		for (vector<vector<double> >::size_type i = 0; i < n; i++) {
			adjacencyMatrix.at(i).resize(i + 1);
			for (vector<double>::size_type j = 0; j < n; j++) {
				setAdjacencyMatrixElement(i, j, getAdjacencyMatrixElement(i, j));
			}
		}

		return (*this);
	}
}


/**
 * Saves one point in the two-dimensional plain.
 */
struct Point {
	double x;
	double y;
};

/**
 * Rounds a double to the
 * <ul>
 *   <li>nearest lower integer (int) if the number is positive and</li>
 *   <li>to the nearest upper (int) integer otherwise.</li>
 * </ul>
 * @param x Double to be rounded.
 * @return Double rounded to the
 *   <ul>
 *     <li>nearest lower integer (int) if the number is positive and</li>
 *     <li>to the nearest upper (int) integer otherwise.</li>
 *   </ul>
 */
inline int roundToZero(double x) {
	if (x >= std::numeric_limits<double>::max()) {
		throw std::overflow_error("A double value in the input file is too large!");
	}

	double resultDouble;

	if (x > 0.0) {
		resultDouble = floor(x);
	}
	else {
		resultDouble = ceil(x);
	}

	if (resultDouble > std::numeric_limits<int>::max()) {
		throw std::overflow_error("A double value in the input file is too large!");
	}

	if (resultDouble < std::numeric_limits<int>::min()) {
		throw std::overflow_error("A double value in the input file is too large!");
	}

	return (static_cast<int>(resultDouble));
}

TransformInstance *readInputFileTSPLIB(const string &inputFileName) {
	/*
	 * Creating of the instance of the class TransformInstance.
	 */
	TransformInstance *transformInstance = 0;

	/*
	 * Indicator indicating if the file contains a symmetric travelling salesman problem.
	 * <ul>
	 *   <li>true The file contains a symmetric travelling salesman problem.</li>
	 *   <li>false The file contains an asymmetric travelling salesman problem.</li>
	 * </ul>
	 */
	bool symmetricTravellingSalesmanProblem;
	string inputFileFilenameExtensionTSP;
	if (inputFileName.size() >= INPUT_FILE_FILENAME_EXTENSION_TSP.size() + 1) {
		inputFileFilenameExtensionTSP =
				inputFileName.substr(
						inputFileName.size() - INPUT_FILE_FILENAME_EXTENSION_TSP.size(),
						INPUT_FILE_FILENAME_EXTENSION_TSP.size());
	}
	else {
		inputFileFilenameExtensionTSP = "";
	}

	string inputFileFilenameExtensionATSP;
	if (inputFileName.size() >= INPUT_FILE_FILENAME_EXTENSION_ATSP.size() + 1) {
		inputFileFilenameExtensionATSP =
				inputFileName.substr(
						inputFileName.size() - INPUT_FILE_FILENAME_EXTENSION_ATSP.size(),
						INPUT_FILE_FILENAME_EXTENSION_ATSP.size());
	}
	else {
		inputFileFilenameExtensionATSP = "";
	}

	transform(
			inputFileFilenameExtensionTSP.begin(),
			inputFileFilenameExtensionTSP.end(),
			inputFileFilenameExtensionTSP.begin(), ::tolower);
	transform(
			inputFileFilenameExtensionATSP.begin(),
			inputFileFilenameExtensionATSP.end(),
			inputFileFilenameExtensionATSP.begin(), ::tolower);

	if (inputFileFilenameExtensionTSP == INPUT_FILE_FILENAME_EXTENSION_TSP) {
		symmetricTravellingSalesmanProblem = true;
	}
	if (inputFileFilenameExtensionATSP == INPUT_FILE_FILENAME_EXTENSION_ATSP) {
		symmetricTravellingSalesmanProblem = false;
	}

	ifstream inputFile;
	inputFile.exceptions(ifstream::failbit | ifstream::badbit);
	try {
		inputFile.open(inputFileName.c_str());

		string inputLine = "";

		//NAME.
		if (!getline(inputFile, inputLine)) {
			throw ifstream::failure("Unexpected input! test 1");
		}
		trim(inputLine);
		if (inputLine.substr(0, TAG_NAME.length()) != TAG_NAME) {
			throw ifstream::failure("Unexpected input! test 2" + TAG_NAME);
		}
		inputLine.erase(0, TAG_NAME.length());
		trim(inputLine);
		if (inputLine == "") {
			throw ifstream::failure("Unexpected input! test 3");
		}
		const string name = inputLine;

		//TYPE.
		if (!getline(inputFile, inputLine)) {
			throw ifstream::failure("Unexpected input! test 3");
		}
		trim(inputLine);
		if (inputLine.substr(0, TAG_TYPE.length()) != TAG_TYPE) {
			throw ifstream::failure("Unexpected input! test 4");
		}
		inputLine.erase(0, TAG_TYPE.length());
		trim(inputLine);
		string type;
		if ((symmetricTravellingSalesmanProblem) && (inputLine == VALUE_TYPE_TSP)) {
			type = VALUE_TYPE_TSP;
			//COMMENT.
			if (!getline(inputFile, inputLine)) {
				throw ifstream::failure("Unexpected input! test 5");
			}
			trim(inputLine);
			if (inputLine.substr(0, TAG_COMMENT.length()) != TAG_COMMENT) {
				throw ifstream::failure("Unexpected input! test 6");
			}
			inputLine.erase(0, TAG_COMMENT.length());
			trim(inputLine);
			const string comment = inputLine;

			//DIMENSION.
			if (!getline(inputFile, inputLine)) {
				throw ifstream::failure("Unexpected input! test 7");
			}
			trim(inputLine);
			if (inputLine.substr(0, TAG_DIMENSION.length()) != TAG_DIMENSION) {
				throw ifstream::failure("Unexpected input! test 8");
			}
			inputLine.erase(0, TAG_DIMENSION.length());
			trim(inputLine);
			vector<vector<double> >::size_type n;
			istringstream nIStringstream(inputLine);
			nIStringstream.exceptions(ifstream::failbit | ifstream::badbit);
			nIStringstream >> n;
			if (!nIStringstream.eof()) {
				throw ifstream::failure("Unexpected input! test 9");
			}

			//EDGE_WEIGHT_TYPE.
			if (!getline(inputFile, inputLine)) {
				throw ifstream::failure("Unexpected input! test 10");
			}
			trim(inputLine);
			if (inputLine.substr(0, TAG_EDGE_WEIGHT_TYPE.length()) != TAG_EDGE_WEIGHT_TYPE) {
				throw ifstream::failure("Unexpected input! test 11" + inputLine + " ---- " + TAG_EDGE_WEIGHT_TYPE);
			}
			inputLine.erase(0, TAG_EDGE_WEIGHT_TYPE.length());
			trim(inputLine);
			const string edgeWeightType = inputLine;
			if (edgeWeightType == VALUE_EDGE_WEIGHT_TYPE_GEO) {
				//EDGE_WEIGHT_FORMAT.
				if (!getline(inputFile, inputLine)) {
					throw ifstream::failure("Unexpected input! test 12");
				}
				trim(inputLine);
				if (inputLine.substr(0, TAG_EDGE_WEIGHT_FORMAT.length()) == TAG_EDGE_WEIGHT_FORMAT) {
					inputLine.erase(0, TAG_EDGE_WEIGHT_FORMAT.length());
					trim(inputLine);
					const string edgeWeightFormat = inputLine;
					if (edgeWeightFormat != VALUE_EDGE_WEIGHT_FORMAT_FUNCTION) {
						throw ifstream::failure("Unexpected input! test 13");
					}

					if (!getline(inputFile, inputLine)) {
						throw ifstream::failure("Unexpected input! test 14");
					}
					trim(inputLine);
				}

				//DISPLAY_DATA_TYPE.
				if (inputLine.substr(0, TAG_DISPLAY_DATA_TYPE.length()) != TAG_DISPLAY_DATA_TYPE) {
					throw ifstream::failure("Unexpected input!");
				}
				inputLine.erase(0, TAG_DISPLAY_DATA_TYPE.length());
				trim(inputLine);
				const string displayDataType = inputLine;
				if (displayDataType == VALUE_DISPLAY_DATA_TYPE_COORD_DISPLAY) {

					//NODE_COORD_SECTION.
					if (!getline(inputFile, inputLine)) {
						throw ifstream::failure("Unexpected input!");
					}
					trim(inputLine);
					if (inputLine != TAG_NODE_COORD_SECTION) {
						throw ifstream::failure("Unexpected input!");
					}

					vector<Point> points(n);
					for (vector<Point>::size_type i = 0; i < n; i++) {
						if (!getline(inputFile, inputLine)) {
							throw ifstream::failure("Unexpected input!");
						}
						trim(inputLine);
						istringstream pointIStringstream(inputLine);
						pointIStringstream.exceptions(ifstream::failbit | ifstream::badbit);
						vector<Point>::size_type j;
						pointIStringstream >> j;
						if (j != i + 1) {
							throw ifstream::failure("Unexpected input!");
						}
						Point point;
						pointIStringstream >> point.x;
						pointIStringstream >> point.y;
						if (!nIStringstream.eof()) {
							throw ifstream::failure("Unexpected input!");
						}

						points.at(i) = point;
					}
					transformInstance = new TransformInstance(
							type,
							name,
							XML_VALUE_SOURCE_TSPLIB,
							comment,
							n);
					for (vector<vector<double> >::size_type i = 0; i < n; i++) {
						for (vector<double>::size_type j = 0; j < i; j++) {
							double latitude1;
							{
								double degrees1 = roundToZero(
										points.at(static_cast<vector<Point>::size_type>(i)).x);
								double minutes1 =
										points.at(static_cast<vector<Point>::size_type>(i)).x -
										degrees1;
								degrees1 += 5.0 * minutes1 / 3.0;
								latitude1 = M_PI * degrees1 / 180.0;
							}
							double longtitude1;
							{
								double degrees2 = roundToZero(
										points.at(static_cast<vector<Point>::size_type>(i)).y);
								double minutes2 =
										points.at(static_cast<vector<Point>::size_type>(i)).y -
										degrees2;
								degrees2 += 5.0 * minutes2 / 3.0;
								longtitude1 = M_PI * degrees2 / 180.0;
							}

							double latitude2;
							{
								double degrees1 = roundToZero(
										points.at(static_cast<vector<Point>::size_type>(j)).x);
								double minutes1 =
										points.at(static_cast<vector<Point>::size_type>(j)).x -
										degrees1;
								degrees1 += 5.0 * minutes1 / 3.0;
								latitude2 = M_PI * degrees1 / 180.0;
							}
							double longtitude2;
							{
								double degrees2 = roundToZero(
										points.at(static_cast<vector<Point>::size_type>(j)).y);
								double minutes2 =
										points.at(static_cast<vector<Point>::size_type>(j)).y -
										degrees2;
								degrees2 += 5.0 * minutes2 / 3.0;
								longtitude2 = M_PI * degrees2 / 180.0;
							}

							double q1 = cos(longtitude1 - longtitude2);
							double q2 = cos(latitude1 - latitude2);
							double q3 = cos(latitude1 + latitude2);
							double distance =
									floor(
											RRR * acos(0.5 *((1.0 + q1) * q2 - (1.0 - q1) * q3)) + 1.0);

							transformInstance->setAdjacencyMatrixElement(
									i,
									j,
									distance);
						}
					}
				}
				else {
					throw ifstream::failure("Unexpected input!");
				}

				//EOF.
				if (!getline(inputFile, inputLine)) {
					throw ifstream::failure("Unexpected input!");
				}
				trim(inputLine);
				if (inputLine != TAG_EOF) {
					throw ifstream::failure("Unexpected input!");
				}
			}
			else if (
					(edgeWeightType == VALUE_EDGE_WEIGHT_TYPE_EUC_2D) ||
					(edgeWeightType == VALUE_EDGE_WEIGHT_TYPE_CEIL_2D)) {
				//NODE_COORD_SECTION.
				if (!getline(inputFile, inputLine)) {
					throw ifstream::failure("Unexpected input!");
				}
				trim(inputLine);
				if (inputLine != TAG_NODE_COORD_SECTION) {
					throw ifstream::failure("Unexpected input!");
				}

				vector<Point> points(n);
				for (vector<Point>::size_type i = 0; i < n; i++) {
					if (!getline(inputFile, inputLine)) {
						throw ifstream::failure("Unexpected input!");
					}
					trim(inputLine);
					istringstream pointIStringstream(inputLine);
					pointIStringstream.exceptions(ifstream::failbit | ifstream::badbit);
					vector<Point>::size_type j;
					pointIStringstream >> j;
					if (j != i + 1) {
						throw ifstream::failure("Unexpected input!");
					}
					Point point;
					pointIStringstream >> point.x;
					pointIStringstream >> point.y;
					if (!nIStringstream.eof()) {
						throw ifstream::failure("Unexpected input!");
					}

					points.at(i) = point;
				}

				transformInstance = new TransformInstance(
						type,
						name,
						XML_VALUE_SOURCE_TSPLIB,
						comment,
						n);
				for (vector<vector<double> >::size_type i = 0; i < n; i++) {
					for (vector<double>::size_type j = 0; j < i; j++) {
						double deltaX = points.at(i).x - points.at(j).x;
						double deltaY = points.at(i).y - points.at(j).y;
						double deltaSquare = deltaX * deltaX + deltaY * deltaY;
						if (deltaSquare == numeric_limits<double>::infinity()) {
							throw range_error("The range error occurs!");
						}
						double delta = sqrt(deltaSquare);
						transformInstance->setAdjacencyMatrixElement(i, j, delta);
					}
				}

				//EOF.
				if (!getline(inputFile, inputLine)) {
					throw ifstream::failure("Unexpected input!");
				}
				trim(inputLine);
				if (inputLine != TAG_EOF) {
					throw ifstream::failure("Unexpected input!");
				}
			}
			else if (edgeWeightType == VALUE_EDGE_WEIGHT_TYPE_ATT) {
				//NODE_COORD_SECTION.
				if (!getline(inputFile, inputLine)) {
					throw ifstream::failure("Unexpected input!");
				}
				trim(inputLine);
				if (inputLine != TAG_NODE_COORD_SECTION) {
					throw ifstream::failure("Unexpected input!");
				}

				vector<Point> points(n);
				for (vector<Point>::size_type i = 0; i < n; i++) {
					if (!getline(inputFile, inputLine)) {
						throw ifstream::failure("Unexpected input!");
					}
					trim(inputLine);
					istringstream pointIStringstream(inputLine);
					pointIStringstream.exceptions(ifstream::failbit | ifstream::badbit);
					vector<Point>::size_type j;
					pointIStringstream >> j;
					if (j != i + 1) {
						throw ifstream::failure("Unexpected input!");
					}
					Point point;
					pointIStringstream >> point.x;
					pointIStringstream >> point.y;
					if (!nIStringstream.eof()) {
						throw ifstream::failure("Unexpected input!");
					}

					points.at(i) = point;
				}

				transformInstance = new TransformInstance(
						type,
						name,
						XML_VALUE_SOURCE_TSPLIB,
						comment,
						n);
				for (vector<vector<double> >::size_type i = 0; i < n; i++) {
					for (vector<double>::size_type j = 0; j < i; j++) {
						double deltaX = points.at(i).x - points.at(j).x;
						double deltaY = points.at(i).y - points.at(j).y;
						double r = deltaX * deltaX + deltaY * deltaY;
						if (r == numeric_limits<double>::infinity()) {
							throw range_error("The range error occurs!");
						}
						r /= 10.0;
						r = sqrt(r);
						double t = floor(r + 0.5);
						double distance;
						if (t < r) {
							distance = t + 1;
						}
						else {
							distance = t;
						}

						transformInstance->setAdjacencyMatrixElement(i, j, distance);
					}
				}

				//EOF.
				if (!getline(inputFile, inputLine)) {
					throw ifstream::failure("Unexpected input!");
				}
				trim(inputLine);
				if (inputLine != TAG_EOF) {
					throw ifstream::failure("Unexpected input!");
				}
			}
			else if (edgeWeightType == VALUE_EDGE_WEIGHT_TYPE_EXPLICIT) {
				//EDGE_WEIGHT_FORMAT.
				if (!getline(inputFile, inputLine)) {
					throw ifstream::failure("Unexpected input!");
				}
				trim(inputLine);
				if (inputLine.substr(0, TAG_EDGE_WEIGHT_FORMAT.length()) != TAG_EDGE_WEIGHT_FORMAT) {
					throw ifstream::failure("Unexpected input!");
				}
				inputLine.erase(0, TAG_EDGE_WEIGHT_FORMAT.length());
				trim(inputLine);
				const string edgeWeightFormat = inputLine;
				if (edgeWeightFormat == VALUE_EDGE_WEIGHT_FORMAT_FULL_MATRIX) {
					if (!getline(inputFile, inputLine)) {
						throw ifstream::failure("Unexpected input!");
					}
					trim(inputLine);
					if (inputLine == EDGE_WEIGHT_SECTION) {
						//EDGE_WEIGHT_SECTION.
						transformInstance = new TransformInstance(
								type,
								name,
								XML_VALUE_SOURCE_TSPLIB,
								comment,
								n);

						inputLine = "";
						while (true) {
							string partOfMatrixInputLine;
							if (!getline(inputFile, partOfMatrixInputLine)) {
								throw ifstream::failure("Unexpected input!");
							}
							trim(partOfMatrixInputLine);
							if (partOfMatrixInputLine == TAG_EOF) {
								break;
							}
							inputLine.append(" ");
							inputLine.append(partOfMatrixInputLine);
						}
						trim(inputLine);
						istringstream rowIStringstream(inputLine);
						rowIStringstream.exceptions(ifstream::failbit | ifstream::badbit);
						for (vector<vector<double> >::size_type i = 0; i < n; i++) {
							double cost;
							for (vector<double>::size_type j = 0; j < i; j++) {
								rowIStringstream >> cost;
								if (
										abs(transformInstance->getAdjacencyMatrixElement(i, j) - cost) >
										TRANSFORM_DOUBLE_ZERO) {
									throw ifstream::failure("Unexpected input!");
								}
							}
							rowIStringstream >> cost;
							if (abs(cost) > TRANSFORM_DOUBLE_ZERO) {
								throw ifstream::failure("Unexpected input!");
							}
							for (vector<double>::size_type j = i + 1; j < n; j++) {
								rowIStringstream >> cost;
								transformInstance->setAdjacencyMatrixElement(j, i, cost);
							}
						}
						if (!rowIStringstream.eof()) {
							throw ifstream::failure("Unexpected input!");
						}
					}
					else {
						//DISPLAY_DATA_TYPE.
						if (inputLine.substr(0, TAG_DISPLAY_DATA_TYPE.length()) !=
								TAG_DISPLAY_DATA_TYPE) {
							throw ifstream::failure("Unexpected input!");
						}
						inputLine.erase(0, TAG_DISPLAY_DATA_TYPE.length());
						trim(inputLine);
						const string displayDataType = inputLine;
						if (displayDataType == VALUE_DISPLAY_DATA_TYPE_TWOD_DISPLAY) {
							//EDGE_WEIGHT_SECTION.
							if (!getline(inputFile, inputLine)) {
								throw ifstream::failure("Unexpected input!");
							}
							trim(inputLine);
							if (inputLine != EDGE_WEIGHT_SECTION) {
								throw ifstream::failure("Unexpected input!");
							}

							transformInstance = new TransformInstance(
									type,
									name,
									XML_VALUE_SOURCE_TSPLIB,
									comment,
									n);

							inputLine = "";
							while (true) {
								string partOfMatrixInputLine;
								if (!getline(inputFile, partOfMatrixInputLine)) {
									throw ifstream::failure("Unexpected input!");
								}
								trim(partOfMatrixInputLine);
								if (partOfMatrixInputLine == DISPLAY_DATA_SECTION) {
									break;
								}
								inputLine.append(" ");
								inputLine.append(partOfMatrixInputLine);
							}
							trim(inputLine);
							istringstream rowIStringstream(inputLine);
							rowIStringstream.exceptions(ifstream::failbit | ifstream::badbit);
							for (vector<vector<double> >::size_type i = 0; i < n; i++) {
								double cost;
								for (vector<double>::size_type j = 0; j < i; j++) {
									rowIStringstream >> cost;
									if (
											abs(transformInstance->getAdjacencyMatrixElement(i, j) - cost) >
											TRANSFORM_DOUBLE_ZERO) {
										throw ifstream::failure("Unexpected input!");
									}
								}
								rowIStringstream >> cost;
								if (abs(cost) > TRANSFORM_DOUBLE_ZERO) {
									throw ifstream::failure("Unexpected input!");
								}
								for (vector<double>::size_type j = i + 1; j < n; j++) {
									rowIStringstream >> cost;
									transformInstance->setAdjacencyMatrixElement(j, i, cost);
								}
							}
							if (!rowIStringstream.eof()) {
								throw ifstream::failure("Unexpected input!");
							}

							//DISPLAY_DATA_SECTION.
							for (vector<vector<double> >::size_type i = 0; i < n; i++) {
								if (!getline(inputFile, inputLine)) {
									throw ifstream::failure("Unexpected input!");
								}
							}

							//EOF.
							if (!getline(inputFile, inputLine)) {
								throw ifstream::failure("Unexpected input!");
							}
							trim(inputLine);
							if (inputLine != TAG_EOF) {
								throw ifstream::failure("Unexpected input!");
							}
						}
						else {
							throw ifstream::failure("Unexpected input!");
						}
					}
				}
				else if (edgeWeightFormat == VALUE_EDGE_WEIGHT_FORMAT_LOWER_DIAG_ROW) {
					if (!getline(inputFile, inputLine)) {
						throw ifstream::failure("Unexpected input!");
					}
					trim(inputLine);
					if (inputLine == EDGE_WEIGHT_SECTION) {
						//EDGE_WEIGHT_SECTION.
						transformInstance = new TransformInstance(
								type,
								name,
								XML_VALUE_SOURCE_TSPLIB,
								comment,
								n);

						inputLine = "";
						while (true) {
							string partOfMatrixInputLine;
							if (!getline(inputFile, partOfMatrixInputLine)) {
								throw ifstream::failure("Unexpected input!");
							}
							trim(partOfMatrixInputLine);
							if (partOfMatrixInputLine == TAG_EOF) {
								break;
							}
							inputLine.append(" ");
							inputLine.append(partOfMatrixInputLine);
						}
						trim(inputLine);
						istringstream rowIStringstream(inputLine);
						rowIStringstream.exceptions(ifstream::failbit | ifstream::badbit);
						for (vector<vector<double> >::size_type i = 0; i < n; i++) {
							double cost;
							for (vector<double>::size_type j = 0; j < i; j++) {
								rowIStringstream >> cost;
								transformInstance->setAdjacencyMatrixElement(i, j, cost);
							}
							rowIStringstream >> cost;
							if (abs(cost) > TRANSFORM_DOUBLE_ZERO) {
								throw ifstream::failure("Unexpected input!");
							}
						}
						if (!rowIStringstream.eof()) {
							throw ifstream::failure("Unexpected input!");
						}
					}
					else {
						//DISPLAY_DATA_TYPE.
						if (inputLine.substr(0, TAG_DISPLAY_DATA_TYPE.length()) !=
								TAG_DISPLAY_DATA_TYPE) {
							throw ifstream::failure("Unexpected input!");
						}
						inputLine.erase(0, TAG_DISPLAY_DATA_TYPE.length());
						trim(inputLine);
						const string displayDataType = inputLine;
						if (displayDataType == VALUE_DISPLAY_DATA_TYPE_TWOD_DISPLAY) {
							//EDGE_WEIGHT_SECTION.
							if (!getline(inputFile, inputLine)) {
								throw ifstream::failure("Unexpected input!");
							}
							trim(inputLine);
							if (inputLine != EDGE_WEIGHT_SECTION) {
								throw ifstream::failure("Unexpected input!");
							}

							transformInstance = new TransformInstance(
									type,
									name,
									XML_VALUE_SOURCE_TSPLIB,
									comment,
									n);

							inputLine = "";
							while (true) {
								string partOfMatrixInputLine;
								if (!getline(inputFile, partOfMatrixInputLine)) {
									throw ifstream::failure("Unexpected input!");
								}
								trim(partOfMatrixInputLine);
								if (partOfMatrixInputLine == DISPLAY_DATA_SECTION) {
									break;
								}
								inputLine.append(" ");
								inputLine.append(partOfMatrixInputLine);
							}
							trim(inputLine);
							istringstream rowIStringstream(inputLine);
							rowIStringstream.exceptions(ifstream::failbit | ifstream::badbit);
							for (vector<vector<double> >::size_type i = 0; i < n; i++) {
								double cost;
								for (vector<double>::size_type j = 0; j < i; j++) {
									rowIStringstream >> cost;
									transformInstance->setAdjacencyMatrixElement(i, j, cost);
								}
								rowIStringstream >> cost;
								if (abs(cost) > TRANSFORM_DOUBLE_ZERO) {
									throw ifstream::failure("Unexpected input!");
								}
							}
							if (!rowIStringstream.eof()) {
								throw ifstream::failure("Unexpected input!");
							}

							//DISPLAY_DATA_SECTION.
							for (vector<vector<double> >::size_type i = 0; i < n; i++) {
								if (!getline(inputFile, inputLine)) {
									throw ifstream::failure("Unexpected input!");
								}
							}

							//EOF.
							if (!getline(inputFile, inputLine)) {
								throw ifstream::failure("Unexpected input!");
							}
							trim(inputLine);
							if (inputLine != TAG_EOF) {
								throw ifstream::failure("Unexpected input!");
							}
						}
						else {
							throw ifstream::failure("Unexpected input!");
						}
					}
				}
				else if (edgeWeightFormat == VALUE_EDGE_WEIGHT_FORMAT_UPPER_DIAG_ROW) {
					if (!getline(inputFile, inputLine)) {
						throw ifstream::failure("Unexpected input!");
					}
					trim(inputLine);
					if (inputLine == EDGE_WEIGHT_SECTION) {
						//EDGE_WEIGHT_SECTION.
						transformInstance = new TransformInstance(
								type,
								name,
								XML_VALUE_SOURCE_TSPLIB,
								comment,
								n);

						inputLine = "";
						while (true) {
							string partOfMatrixInputLine;
							if (!getline(inputFile, partOfMatrixInputLine)) {
								throw ifstream::failure("Unexpected input!");
							}
							trim(partOfMatrixInputLine);
							if (partOfMatrixInputLine == TAG_EOF) {
								break;
							}
							inputLine.append(" ");
							inputLine.append(partOfMatrixInputLine);
						}
						trim(inputLine);
						istringstream rowIStringstream(inputLine);
						rowIStringstream.exceptions(ifstream::failbit | ifstream::badbit);
						for (vector<vector<double> >::size_type i = 0; i < n; i++) {
							double cost;
							rowIStringstream >> cost;
							if (abs(cost) > TRANSFORM_DOUBLE_ZERO) {
								throw ifstream::failure("Unexpected input!");
							}
							for (vector<double>::size_type j = i + 1; j < n; j++) {
								rowIStringstream >> cost;
								transformInstance->setAdjacencyMatrixElement(j, i, cost);
							}
						}
						if (!rowIStringstream.eof()) {
							throw ifstream::failure("Unexpected input!");
						}
					}
					else {
						//DISPLAY_DATA_TYPE.
						if (inputLine.substr(0, TAG_DISPLAY_DATA_TYPE.length()) !=
								TAG_DISPLAY_DATA_TYPE) {
							throw ifstream::failure("Unexpected input!");
						}
						inputLine.erase(0, TAG_DISPLAY_DATA_TYPE.length());
						trim(inputLine);
						const string displayDataType = inputLine;
						if (displayDataType == VALUE_DISPLAY_DATA_TYPE_TWOD_DISPLAY) {
							//EDGE_WEIGHT_SECTION.
							if (!getline(inputFile, inputLine)) {
								throw ifstream::failure("Unexpected input!");
							}
							trim(inputLine);
							if (inputLine != EDGE_WEIGHT_SECTION) {
								throw ifstream::failure("Unexpected input!");
							}

							transformInstance = new TransformInstance(
									type,
									name,
									XML_VALUE_SOURCE_TSPLIB,
									comment,
									n);

							inputLine = "";
							while (true) {
								string partOfMatrixInputLine;
								if (!getline(inputFile, partOfMatrixInputLine)) {
									throw ifstream::failure("Unexpected input!");
								}
								trim(partOfMatrixInputLine);
								if (partOfMatrixInputLine == DISPLAY_DATA_SECTION) {
									break;
								}
								inputLine.append(" ");
								inputLine.append(partOfMatrixInputLine);
							}
							trim(inputLine);
							istringstream rowIStringstream(inputLine);
							rowIStringstream.exceptions(ifstream::failbit | ifstream::badbit);
							for (vector<vector<double> >::size_type i = 0; i < n; i++) {
								double cost;
								rowIStringstream >> cost;
								if (abs(cost) > TRANSFORM_DOUBLE_ZERO) {
									throw ifstream::failure("Unexpected input!");
								}
								for (vector<double>::size_type j = i + 1; j < n; j++) {
									rowIStringstream >> cost;
									transformInstance->setAdjacencyMatrixElement(j, i, cost);
								}
							}
							if (!rowIStringstream.eof()) {
								throw ifstream::failure("Unexpected input!");
							}

							//DISPLAY_DATA_SECTION.
							for (vector<vector<double> >::size_type i = 0; i < n; i++) {
								if (!getline(inputFile, inputLine)) {
									throw ifstream::failure("Unexpected input!");
								}
							}

							//EOF.
							if (!getline(inputFile, inputLine)) {
								throw ifstream::failure("Unexpected input!");
							}
							trim(inputLine);
							if (inputLine != TAG_EOF) {
								throw ifstream::failure("Unexpected input!");
							}
						}
						else {
							throw ifstream::failure("Unexpected input!");
						}
					}
				}
				else if (edgeWeightFormat == VALUE_EDGE_WEIGHT_FORMAT_UPPER_ROW) {
					if (!getline(inputFile, inputLine)) {
						throw ifstream::failure("Unexpected input!");
					}
					trim(inputLine);
					if (inputLine == EDGE_WEIGHT_SECTION) {
						//EDGE_WEIGHT_SECTION.
						transformInstance = new TransformInstance(
								type,
								name,
								XML_VALUE_SOURCE_TSPLIB,
								comment,
								n);

						inputLine = "";
						while (true) {
							string partOfMatrixInputLine;
							if (!getline(inputFile, partOfMatrixInputLine)) {
								throw ifstream::failure("Unexpected input!");
							}
							trim(partOfMatrixInputLine);
							if (partOfMatrixInputLine == TAG_EOF) {
								break;
							}
							inputLine.append(" ");
							inputLine.append(partOfMatrixInputLine);
						}
						trim(inputLine);
						istringstream rowIStringstream(inputLine);
						rowIStringstream.exceptions(ifstream::failbit | ifstream::badbit);
						for (vector<vector<double> >::size_type i = 0; i < n - 1; i++) {
							double cost;
							for (vector<double>::size_type j = i + 1; j < n; j++) {
								rowIStringstream >> cost;
								transformInstance->setAdjacencyMatrixElement(j, i, cost);
							}
						}
						if (!rowIStringstream.eof()) {
							throw ifstream::failure("Unexpected input!");
						}
					}
					else {
						//DISPLAY_DATA_TYPE.
						if (inputLine.substr(0, TAG_DISPLAY_DATA_TYPE.length()) !=
								TAG_DISPLAY_DATA_TYPE) {
							throw ifstream::failure("Unexpected input!");
						}
						inputLine.erase(0, TAG_DISPLAY_DATA_TYPE.length());
						trim(inputLine);
						const string displayDataType = inputLine;
						if (displayDataType == VALUE_DISPLAY_DATA_TYPE_TWOD_DISPLAY) {
							//EDGE_WEIGHT_SECTION.
							if (!getline(inputFile, inputLine)) {
								throw ifstream::failure("Unexpected input!");
							}
							trim(inputLine);
							if (inputLine != EDGE_WEIGHT_SECTION) {
								throw ifstream::failure("Unexpected input!");
							}

							transformInstance = new TransformInstance(
									type,
									name,
									XML_VALUE_SOURCE_TSPLIB,
									comment,
									n);

							inputLine = "";
							while (true) {
								string partOfMatrixInputLine;
								if (!getline(inputFile, partOfMatrixInputLine)) {
									throw ifstream::failure("Unexpected input!");
								}
								trim(partOfMatrixInputLine);
								if (partOfMatrixInputLine == DISPLAY_DATA_SECTION) {
									break;
								}
								inputLine.append(" ");
								inputLine.append(partOfMatrixInputLine);
							}
							trim(inputLine);
							istringstream rowIStringstream(inputLine);
							rowIStringstream.exceptions(ifstream::failbit | ifstream::badbit);
							for (vector<vector<double> >::size_type i = 0; i < n - 1; i++) {
								double cost;
								for (vector<double>::size_type j = i + 1; j < n; j++) {
									rowIStringstream >> cost;
									transformInstance->setAdjacencyMatrixElement(j, i, cost);
								}
							}
							if (!rowIStringstream.eof()) {
								throw ifstream::failure("Unexpected input!");
							}

							//DISPLAY_DATA_SECTION.
							for (vector<vector<double> >::size_type i = 0; i < n; i++) {
								if (!getline(inputFile, inputLine)) {
									throw ifstream::failure("Unexpected input!");
								}
							}

							//EOF.
							if (!getline(inputFile, inputLine)) {
								throw ifstream::failure("Unexpected input!");
							}
							trim(inputLine);
							if (inputLine != TAG_EOF) {
								throw ifstream::failure("Unexpected input!");
							}
						}
						else {
							throw ifstream::failure("Unexpected input!");
						}
					}
				}
				else {
					throw ifstream::failure("Unexpected input!");
				}
			}
			else {
				throw InputFileFormatNotSupported();
			}

			//Rest.
			inputFile.exceptions(ifstream::badbit);
			while (!inputFile.eof()) {
				getline(inputFile, inputLine);
				trim(inputLine);
				if (inputLine != "") {
					throw ifstream::failure("Unexpected input!");
				}
			}

			//The main diagonal must be set.
			for (vector<vector<double> >::size_type i = 0; i < n; i++) {
				transformInstance->setAdjacencyMatrixElement(
						i,
						static_cast<vector<double>::size_type>(i),
						0);
			}
		}
		else if ((!symmetricTravellingSalesmanProblem) && (inputLine == VALUE_TYPE_ATSP)) {
			type = VALUE_TYPE_ATSP;

			//COMMENT.
			if (!getline(inputFile, inputLine)) {
				throw ifstream::failure("Unexpected input!");
			}
			trim(inputLine);
			if (inputLine.substr(0, TAG_COMMENT.length()) != TAG_COMMENT) {
				throw ifstream::failure("Unexpected input!");
			}
			inputLine.erase(0, TAG_COMMENT.length());
			trim(inputLine);
			const string comment = inputLine;

			//DIMENSION.
			if (!getline(inputFile, inputLine)) {
				throw ifstream::failure("Unexpected input!");
			}
			trim(inputLine);
			if (inputLine.substr(0, TAG_DIMENSION.length()) != TAG_DIMENSION) {
				throw ifstream::failure("Unexpected input!");
			}
			inputLine.erase(0, TAG_DIMENSION.length());
			trim(inputLine);
			vector<vector<double> >::size_type n;
			istringstream nIStringstream(inputLine);
			nIStringstream.exceptions(ifstream::failbit | ifstream::badbit);
			nIStringstream >> n;
			if (!nIStringstream.eof()) {
				throw ifstream::failure("Unexpected input!");
			}

			//EDGE_WEIGHT_TYPE.
			if (!getline(inputFile, inputLine)) {
				throw ifstream::failure("Unexpected input!");
			}
			trim(inputLine);
			if (inputLine.substr(0, TAG_EDGE_WEIGHT_TYPE.length()) != TAG_EDGE_WEIGHT_TYPE) {
				throw ifstream::failure("Unexpected input!");
			}
			inputLine.erase(0, TAG_EDGE_WEIGHT_TYPE.length());
			trim(inputLine);
			const string edgeWeightType = inputLine;
			if (edgeWeightType == VALUE_EDGE_WEIGHT_TYPE_EXPLICIT) {
				//EDGE_WEIGHT_FORMAT.
				if (!getline(inputFile, inputLine)) {
					throw ifstream::failure("Unexpected input!");
				}
				trim(inputLine);
				if (inputLine.substr(0, TAG_EDGE_WEIGHT_FORMAT.length()) != TAG_EDGE_WEIGHT_FORMAT) {
					throw ifstream::failure("Unexpected input!");
				}
				inputLine.erase(0, TAG_EDGE_WEIGHT_FORMAT.length());
				trim(inputLine);
				const string edgeWeightFormat = inputLine;
				if (edgeWeightFormat == VALUE_EDGE_WEIGHT_FORMAT_FULL_MATRIX) {
					//EDGE_WEIGHT_SECTION.
					if (!getline(inputFile, inputLine)) {
						throw ifstream::failure("Unexpected input!");
					}
					trim(inputLine);
					if (inputLine != EDGE_WEIGHT_SECTION) {
						throw ifstream::failure("Unexpected input!");
					}

					transformInstance = new TransformInstance(
							type,
							name,
							XML_VALUE_SOURCE_TSPLIB,
							comment,
							n);

					inputLine = "";
					while (true) {
						string partOfMatrixInputLine;
						if (!getline(inputFile, partOfMatrixInputLine)) {
							throw ifstream::failure("Unexpected input!");
						}
						trim(partOfMatrixInputLine);
						if (partOfMatrixInputLine == TAG_EOF) {
							break;
						}
						inputLine.append(" ");
						inputLine.append(partOfMatrixInputLine);
					}
					trim(inputLine);
					istringstream rowIStringstream(inputLine);
					rowIStringstream.exceptions(ifstream::failbit | ifstream::badbit);
					for (vector<vector<double> >::size_type i = 0; i < n; i++) {
						double cost;
						for (vector<double>::size_type j = 0; j < n; j++) {
							rowIStringstream >> cost;
							transformInstance->setAdjacencyMatrixElement(i, j, cost);
						}
					}
					if (!rowIStringstream.eof()) {
						throw ifstream::failure("Unexpected input!");
					}
				}
				else  {
					throw ifstream::failure("Unexpected input!");
				}
			}
			else {
				throw ifstream::failure("Unexpected input!");
			}

			//Rest.
			inputFile.exceptions(ifstream::badbit);
			while (!inputFile.eof()) {
				getline(inputFile, inputLine);
				trim(inputLine);
				if (inputLine != "") {
					throw ifstream::failure("Unexpected input!");
				}
			}
		}
		else {
			throw ifstream::failure("Unexpected input!");
		}

	    inputFile.close();
	}
	catch (InputFileFormatNotSupported &e) {
    	inputFile.close();
    	delete transformInstance;
	    throw;
	}
	catch (ifstream::failure &e) {
	    if (inputFile.is_open()) {
	    	inputFile.close();
	    }
	    if (transformInstance != 0) {
	    	delete transformInstance;
	    }
	    throw;
    }
	catch (bad_alloc &e) {
	    if (inputFile.is_open()) {
	    	inputFile.close();
	    }
	    throw;
	}
	catch (range_error &e) {
	    if (inputFile.is_open()) {
	    	inputFile.close();
	    }
	    if (transformInstance != 0) {
	    	delete transformInstance;
	    }
	    throw;
	}
	catch (...) {
	    if (inputFile.is_open()) {
	    	inputFile.close();
	    }
	    if (transformInstance != 0) {
	    	delete transformInstance;
	    }
	    throw;
	}

	return (transformInstance);
}

/**
 * Trancodes char* data to XMLCh data.
 */
class XMLStringTranscode {
private:
	/**
	 * Unicode XMLCh format of the string.
	 */
    XMLCh *stringUnicodeForm;
public:
    /**
     * Constructor for the class XMLStringTranscode.
     * @param toTranscode String that should be transcoded.
     */
    inline XMLStringTranscode(const string toTranscode) {
        // Call the private transcoding method
    	stringUnicodeForm = XMLString::transcode(toTranscode.c_str());
    }

    /**
     * Destructor for the class XMLString.
     */
    inline ~XMLStringTranscode() {
        XMLString::release(&stringUnicodeForm);
    }

    /**
     * Returns the unicode XMLCh format of the string.
     * @return Unicode XMLCh format of the string.
     */
    inline const XMLCh *unicodeForm() const {
        return (stringUnicodeForm);
    }
};
#define unicodeForm(str) XMLStringTranscode(str).unicodeForm()

void writeOutputFile(const string &outputFileName, const TransformInstance *transformInstance) {
	try {
		XMLPlatformUtils::Initialize();
	}
	catch (const XMLException &e) {
		throw;
	}

	{
		DOMImplementation *dOMImplementation =
				DOMImplementationRegistry::getDOMImplementation(unicodeForm("Core"));
		if (dOMImplementation != 0) {
			try {
				/*
				 * Creating the DOM structure.
				 */
				DOMDocument *document = dOMImplementation->createDocument(
							0,  // root element namespace URI.
							unicodeForm(XML_DOCUMENT_NODE),  // root element name
							0);  // document type object (DTD).

				DOMElement *documentElement = document->getDocumentElement();

				DOMElement *nameElement = document->createElement(unicodeForm(XML_NAME));
				documentElement->appendChild(nameElement);
				DOMText *nameEntry = document->
						createTextNode(unicodeForm(transformInstance->getName()));
				nameElement->appendChild(nameEntry);

				DOMElement *sourceElement = document->createElement(unicodeForm(XML_SOURCE));
				documentElement->appendChild(sourceElement);
				DOMText *sourceEntry = document->
						createTextNode(unicodeForm(transformInstance->getSource()));
				sourceElement->appendChild(sourceEntry);

				DOMElement *descriptionElement = document->createElement(unicodeForm(XML_DESCRIPTION));
				documentElement->appendChild(descriptionElement);
				DOMText *descriptionEntry = document->
						createTextNode(unicodeForm(transformInstance->getDescription()));
				descriptionElement->appendChild(descriptionEntry);

				DOMElement *doublePrecisionElement =
						document->createElement(unicodeForm(XML_DOUBLE_PRECISION));
				documentElement->appendChild(doublePrecisionElement);
				stringstream stringstreamDoublePrecision;
				stringstreamDoublePrecision.setf(DOUBLE_FLOATFIELD, ios::floatfield);
				stringstreamDoublePrecision <<
						setprecision(static_cast<int>(TRANSFORM_DOUBLE_PRECISION));
				stringstreamDoublePrecision << TRANSFORM_DOUBLE_PRECISION;
				DOMText *doublePrecisionEntry = document->
						createTextNode(unicodeForm(stringstreamDoublePrecision.str()));
				doublePrecisionElement->appendChild(doublePrecisionEntry);

				DOMElement *ignoredDigitsElement =
						document->createElement(unicodeForm(XML_IGNORED_DIGITS));
				documentElement->appendChild(ignoredDigitsElement);
				stringstream stringstreamIgnoredDigits;
				stringstreamIgnoredDigits.setf(DOUBLE_FLOATFIELD, ios::floatfield);
				stringstreamIgnoredDigits <<
						setprecision(static_cast<int>(TRANSFORM_DOUBLE_PRECISION));
				stringstreamIgnoredDigits << TRANSFORM_IGNORED_DIGITS;
				DOMText *ignoredDigitsEntry = document->
						createTextNode(unicodeForm(stringstreamIgnoredDigits.str()));
				ignoredDigitsElement->appendChild(ignoredDigitsEntry);

				DOMElement *graphElement = document->createElement(unicodeForm(XML_GRAPH));
				documentElement->appendChild(graphElement);
				for (
						vector<vector<double> >::size_type i = 0;
						i < transformInstance->getN();
						i++) {
					DOMElement *vertexElement = document->createElement(unicodeForm(XML_VERTEX));
					graphElement->appendChild(vertexElement);
					for (
							vector<double>::size_type j = 0;
							j < i;
							j++) {
						DOMElement *edgeElement = document->createElement(unicodeForm(XML_EDGE));
						vertexElement->appendChild(edgeElement);
						stringstream stringstreamCost;
						stringstreamCost.setf(DOUBLE_FLOATFIELD, ios::floatfield);
						stringstreamCost << setprecision(static_cast<int>(TRANSFORM_DOUBLE_PRECISION));
						stringstreamCost << transformInstance->getAdjacencyMatrixElement(i, j);
						edgeElement->setAttribute(
								unicodeForm(XML_EDGE_ATTRIBUTE_COST),
								unicodeForm(stringstreamCost.str()));
						stringstream stringstreamElementEntry;
						stringstreamElementEntry << j;
						DOMText *edgeElementEntry = document->
								createTextNode(unicodeForm(stringstreamElementEntry.str()));
						edgeElement->appendChild(edgeElementEntry);
					}
					if (transformInstance->getType() == VALUE_TYPE_ATSP) {
						DOMElement *edgeElement = document->createElement(unicodeForm(XML_EDGE));
						vertexElement->appendChild(edgeElement);
						stringstream stringstreamCost;
						stringstreamCost.setf(DOUBLE_FLOATFIELD, ios::floatfield);
						stringstreamCost << setprecision(static_cast<int>(TRANSFORM_DOUBLE_PRECISION));
						stringstreamCost << transformInstance->
								getAdjacencyMatrixElement(
										i,
										static_cast<vector<double>::size_type >(i));
						edgeElement->setAttribute(
								unicodeForm(XML_EDGE_ATTRIBUTE_COST),
								unicodeForm(stringstreamCost.str()));
						stringstream stringstreamElementEntry;
						stringstreamElementEntry << i;
						DOMText *edgeElementEntry = document->
								createTextNode(unicodeForm(stringstreamElementEntry.str()));
						edgeElement->appendChild(edgeElementEntry);
					}
					for (
							vector<double>::size_type j = i + 1;
							j < transformInstance->getN();
							j++) {
						DOMElement *edgeElement = document->createElement(unicodeForm(XML_EDGE));
						vertexElement->appendChild(edgeElement);
						stringstream stringstreamCost;
						stringstreamCost.setf(DOUBLE_FLOATFIELD, ios::floatfield);
						stringstreamCost << setprecision(static_cast<int>(TRANSFORM_DOUBLE_PRECISION));
						stringstreamCost << transformInstance->getAdjacencyMatrixElement(i, j);
						edgeElement->setAttribute(
								unicodeForm(XML_EDGE_ATTRIBUTE_COST),
								unicodeForm(stringstreamCost.str()));
						stringstream stringstreamElementEntry;
						stringstreamElementEntry << j;
						DOMText *edgeElementEntry = document->
								createTextNode(unicodeForm(stringstreamElementEntry.str()));
						edgeElement->appendChild(edgeElementEntry);
					}
				}


				/*
				 * Writing to the output file.
				 */
				XMLCh *outputEncoding = XMLString::transcode(XML_ENCODING.c_str());
				const bool splitCDataSections    = true;
				const bool discardDefaultContent = true;
				const bool formatPrettyPrint = XML_FORMAT_PRETTY_PRINT;
				const bool writeBOM = false;

				DOMErrorHandler *myErrorHandler = 0;
				XMLFormatTarget *myFormTarget = 0;
				try
				{
					//Gets a serializer, an instance of DOMLSSerializer.
					XMLCh tempStr[3] = {chLatin_L, chLatin_S, chNull};
					DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(tempStr);
					DOMLSSerializer *theSerializer = ((DOMImplementationLS *) impl)->
							createLSSerializer();
					DOMLSOutput *theOutputDesc = ((DOMImplementationLS *) impl)->
							createLSOutput();

					//Sets the output encoding.
					theOutputDesc->setEncoding(outputEncoding);

					myErrorHandler = new TransformDOMErrorHandler();
					DOMConfiguration *serializerConfig=theSerializer->getDomConfig();
					serializerConfig->setParameter(XMLUni::fgDOMErrorHandler, myErrorHandler);

					if (serializerConfig->
							canSetParameter(XMLUni::fgDOMWRTSplitCdataSections, splitCDataSections)) {
						serializerConfig->
						setParameter(XMLUni::fgDOMWRTSplitCdataSections, splitCDataSections);
					}
					if (serializerConfig->
							canSetParameter(
									XMLUni::fgDOMWRTDiscardDefaultContent, discardDefaultContent)) {
						serializerConfig->
						setParameter(XMLUni::fgDOMWRTDiscardDefaultContent, discardDefaultContent);
					}
					if (serializerConfig->
							canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, formatPrettyPrint)) {
						serializerConfig->
						setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, formatPrettyPrint);
					}
					if (serializerConfig->
							canSetParameter(XMLUni::fgDOMWRTBOM, writeBOM)) {
						serializerConfig->
						setParameter(XMLUni::fgDOMWRTBOM, writeBOM);
					}

					const char* outputFileNameCString = outputFileName.c_str();
					myFormTarget = new LocalFileFormatTarget(outputFileNameCString);
					theOutputDesc->setByteStream(myFormTarget);

					theSerializer->write(document, theOutputDesc);

					theOutputDesc->release();
					theSerializer->release();

					delete myFormTarget;
					delete myErrorHandler;

				}
				catch (const OutOfMemoryException &e) {
					XMLString::release(&outputEncoding);
					if (myFormTarget != 0) {
						delete myFormTarget;
					}
					if (myErrorHandler != 0) {
						delete myErrorHandler;
					}
					document->release();
					throw;
				}
				catch (XMLException &e) {
					XMLString::release(&outputEncoding);
					if (myFormTarget != 0) {
						delete myFormTarget;
					}
					if (myErrorHandler != 0) {
						delete myErrorHandler;
					}
					document->release();
					throw;
				}
				catch ( ... ) {
					XMLString::release(&outputEncoding);
					if (myFormTarget != 0) {
						delete myFormTarget;
					}
					if (myErrorHandler != 0) {
						delete myErrorHandler;
					}
					document->release();
					throw;
				}

				XMLString::release(&outputEncoding);

				document->release();
			}
			catch (bad_alloc &e) {
				XMLPlatformUtils::Terminate();
				throw;
			}
			catch (const OutOfMemoryException &e) {
				XMLPlatformUtils::Terminate();
				throw;
			}
			catch (const DOMException &e) {
				XMLPlatformUtils::Terminate();
				throw;
			}
			catch (XMLException &e) {
				XMLPlatformUtils::Terminate();
				throw;
			}
			catch ( ... ) {
				XMLPlatformUtils::Terminate();
				throw;
			}
		}
		else {
			throw DOMException();
		}
	}

	XMLPlatformUtils::Terminate();
}


void writeOutputFileWithoutUsingAParser(
		const std::string &outputFileName,
		const TransformInstance *transformInstance) {
	ofstream outputFile;
	outputFile.exceptions(ifstream::failbit | ifstream::badbit);
	try {
		outputFile.open(outputFileName.c_str());

		outputFile.setf(DOUBLE_FLOATFIELD, ios::floatfield);
		outputFile <<
				setprecision(static_cast<int>(TRANSFORM_DOUBLE_PRECISION));

		outputFile << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>" << endl;
		outputFile << "<travellingSalesmanProblemInstance>" << endl;
		outputFile << endl;
		outputFile << "  <name>" << transformInstance->getName() << "</name>" << endl;
		outputFile << endl;
		outputFile << "  <source>TSPLIB</source>" << endl;
		outputFile << endl;
		outputFile <<
				"  <description>" <<
				transformInstance->getDescription() <<
				"</description>" <<
				endl;
		outputFile << endl;
		outputFile <<
				"  <doublePrecision>" <<
				TRANSFORM_DOUBLE_PRECISION <<
				"</doublePrecision>" <<
				endl;
		outputFile << endl;
		outputFile << "  <ignoredDigits>" << TRANSFORM_IGNORED_DIGITS << "</ignoredDigits>" << endl;
		outputFile << endl;
		outputFile << "  <graph>" << endl;

		for (
				vector<vector<double> >::size_type i = 0;
				i < transformInstance->getN();
				i++) {
			outputFile << "    <vertex>" << endl;
			for (
					vector<double>::size_type j = 0;
					j < i;
					j++) {
				outputFile <<
						"      <edge cost=\"" <<
						transformInstance->getAdjacencyMatrixElement(i, j) <<
						"\">" <<
						j <<
						"</edge>"<<
						endl;
			}
			if (transformInstance->getType() == VALUE_TYPE_ATSP) {
				outputFile <<
						"      <edge cost=\"" <<
						transformInstance->getAdjacencyMatrixElement(
								i,
								static_cast<vector<double>::size_type>(i)) <<
						"\">" <<
						static_cast<vector<double>::size_type>(i) <<
						"</edge>"<<
						endl;
			}
			for (
					std::vector<double>::size_type j = i + 1;
					j < transformInstance->getN();
					j++) {
				outputFile <<
						"      <edge cost=\"" <<
						transformInstance->getAdjacencyMatrixElement(i, j) <<
						"\">" <<
						j <<
						"</edge>"<<
						endl;
			}
			outputFile << "    </vertex>" << endl;
		}

		outputFile << "  </graph>" << endl;
		outputFile << endl;
		outputFile << "</travellingSalesmanProblemInstance>" << endl;

		outputFile.close();
	}
	catch (ofstream::failure &e) {
	    if (outputFile.is_open()) {
	    	outputFile.close();
	    }
	    throw;
    }
	catch (...) {
	    if (outputFile.is_open()) {
	    	outputFile.close();
	    }
	    throw;
	}
}
