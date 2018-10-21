/**
 * @file TransformTSPLIB.cpp
 * Transforms the instances of the TSPLIB to the xml structure. The
 * instance has to be a symmetric or asymmetric travelling salesman
 * problem instance.
 * Every keyword must be on a new line and is allowed to be used only
 * once. Note that the symbol ":" has to follow the keywords immediately.
 * The order possibilities of the input keywords are:
 * 1) "NAME:", "TYPE:" (= "TSP"), "COMMENT:", "DIMENSION:",
 *   "EDGE_WEIGHT_TYPE:" (= "GEO"), ("EDGE_WEIGHT_FORMAT:"
 *   (= "FUNCTION"),) DISPLAY_DATA_TYPE:" (= "COORD_DISPLAY"),
 *   "NODE_COORD_SECTION", coordinates, "EOF",
 * 2) "NAME:", "TYPE:", "COMMENT:", "DIMENSION:",
 *   "EDGE_WEIGHT_TYPE:" (= "EUC_2D" / "CEIL_2D"),
 *   "NODE_COORD_SECTION", coordinates, "EOF",
 * 3) "NAME:", "TYPE:", "COMMENT:", "DIMENSION:",
 *   "EDGE_WEIGHT_TYPE:" (= "ATT"), "NODE_COORD_SECTION",
 *   coordinates, "EOF",
 * 4) "NAME:", "TYPE:" (= "TSP"), "COMMENT:", "DIMENSION:",
 *   "EDGE_WEIGHT_TYPE:" (= "EXPLICIT"), "EDGE_WEIGHT_FORMAT:"
 *   (= "FULL_MATRIX"), "EDGE_WEIGHT_SECTION", matrix entries, "EOF",
 * 5) "NAME:", "TYPE:" (= "TSP"), "COMMENT:", "DIMENSION:",
 *   "EDGE_WEIGHT_TYPE:" (= "EXPLICIT"), "EDGE_WEIGHT_FORMAT:"
 *   (= "FULL_MATRIX"), "TAG_DISPLAY_DATA_TYPE" (= "TWOD_DISPLAY"),
 *    "EDGE_WEIGHT_SECTION", matrix entries, DISPLAY_DATA_SECTION,
 *    coordinates, "EOF",
 * 6) "NAME:", "TYPE:" (= "TSP"), "COMMENT:", "DIMENSION:",
 *   "EDGE_WEIGHT_TYPE:" (= "EXPLICIT"), "EDGE_WEIGHT_FORMAT:"
 *   (= "LOWER_DIAG_ROW"), "EDGE_WEIGHT_SECTION", matrix entries,
 *   "EOF",
 * 7) "NAME:", "TYPE:" (= "TSP"), "COMMENT:", "DIMENSION:",
 *   "EDGE_WEIGHT_TYPE:" (= "EXPLICIT"), "EDGE_WEIGHT_FORMAT:"
 *   (= "LOWER_DIAG_ROW"), "TAG_DISPLAY_DATA_TYPE"
 *   (= "TWOD_DISPLAY"), "EDGE_WEIGHT_SECTION", matrix entries,
 *   DISPLAY_DATA_SECTION, coordinates, "EOF",
 * 8) "NAME:", "TYPE:" (= "TSP"), "COMMENT:", "DIMENSION:",
 *   "EDGE_WEIGHT_TYPE:" (= "EXPLICIT"), "EDGE_WEIGHT_FORMAT:"
 *   (= "UPPER_ROW"), "EDGE_WEIGHT_SECTION", matrix entries, "EOF",
 * 9) "NAME:", "TYPE:" (= "TSP"), "COMMENT:", "DIMENSION:",
 *   "EDGE_WEIGHT_TYPE:" (= "EXPLICIT"), "EDGE_WEIGHT_FORMAT:"
 *   (= "UPPER_ROW"), "TAG_DISPLAY_DATA_TYPE" (= "TWOD_DISPLAY"),
 *    "EDGE_WEIGHT_SECTION", matrix entries, DISPLAY_DATA_SECTION,
 *    coordinates, "EOF",
 * 10) "NAME:", "TYPE:" (= "ATSP"), "COMMENT:", "DIMENSION:",
 *   "EDGE_WEIGHT_TYPE:" (= "EXPLICIT"), "EDGE_WEIGHT_FORMAT:"
 *   (= "FULL_MATRIX"), "NODE_COORD_SECTION", matrix entries, "EOF".
 * The program uses 2 different strategies to create the XML file.
 * The choice which one will be used is determined by the value
 * of the N_THRESHOLD constant.
 *
 * @brief Transforms the instances of the TSPLIB to the xml structure.
 *
 * @author Ulrich Pferschy and Rostislav Stanek
 * (Institut fuer Statistik und Operations Research, Universitaet Graz)
 */

#include <iostream>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>

#include <xercesc/util/XMLException.hpp>
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

#include "TransformConstantsClassesAndFunctions.hpp"

using namespace std;
using namespace xercesc;

XERCES_CPP_NAMESPACE_USE


/**
 * A threshold value of the size of the graph (i.e. of the number of vertices) which determines
 * if the program uses the safe method to create the xml file or the quicker method which
 * provides no guarantee that the output file is a valid XML file.
 */
const vector<vector<double> >::size_type N_THRESHOLD = 4000;


/**
 * The main function.
 * @param argc Number of elements in the array argv, in particular 1 or 4.
 * @param argv Name of the running program or name of the running program,
 *   the name of the input file and the name of the output file.
 * @return
 *   <ul>
 *     <li>0 if succeeded,</li>
 *     <li>1 otherwise.</li>
 *   </ul>
 */
int main(int argc, char* argv[]) {
	/*
	 * Initialling.
	 */
	cout << "TransformTSPLIB.cpp:" << endl;
	cout << "----------------------------------------------------------------" << endl;

	string inputFileName = "";
	string outputFileName = "";
	switch (argc) {
	case 1:
	{
		try {
			cout <<
					"The name of input file (the filename extension has to be \"tsp\" or \"atsp\"): " <<
					flush;
			getline(cin, inputFileName);

			cout << "The name of output file (the filename extension has to be \"xml\"): " << flush;
			getline(cin, outputFileName);
		}
		catch (ios::failure &e) {
			cerr << "The command-line arguments are invalid!" << endl;
		    return (1);
	    }
	}
	break;
	case 3:
	{
		inputFileName = argv[1];

		outputFileName = argv[2];
	}
	break;
	default:
	{
		cerr << "The command-line arguments are invalid!" << endl;
		return (1);
	}
	}

	//Checking of validity of the inputFileName and the ouptuFileName
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

	string outputFileFilenameExtension;
	if (outputFileName.size() >= OUTPUT_FILE_FILENAME_EXTENSION.size() + 1) {
		outputFileFilenameExtension =
				outputFileName.substr(
						outputFileName.size() - OUTPUT_FILE_FILENAME_EXTENSION.size(),
						OUTPUT_FILE_FILENAME_EXTENSION.size());
	}
	else {
		cerr << "The filename extension of the output file has to be  \"xml\"!" << endl;
		return (1);
	}

	transform(
			inputFileFilenameExtensionTSP.begin(),
			inputFileFilenameExtensionTSP.end(),
			inputFileFilenameExtensionTSP.begin(), ::tolower);
	transform(
			inputFileFilenameExtensionATSP.begin(),
			inputFileFilenameExtensionATSP.end(),
			inputFileFilenameExtensionATSP.begin(), ::tolower);
	transform(
			outputFileFilenameExtension.begin(),
			outputFileFilenameExtension.end(),
			outputFileFilenameExtension.begin(), ::tolower);

	if (
			(inputFileFilenameExtensionTSP != INPUT_FILE_FILENAME_EXTENSION_TSP) &&
			(inputFileFilenameExtensionATSP != INPUT_FILE_FILENAME_EXTENSION_ATSP)) {
		cerr << "The filename extension of the input file has to be  \"tsp\" or \"atsp\"!" << endl;
		return (1);
	}
	if (outputFileFilenameExtension != OUTPUT_FILE_FILENAME_EXTENSION) {
		cerr << "The filename extension of the output file has to be  \"xml\"!" << endl;
		return (1);
	}

	//Writing the used filenames on the standard output
	cout << "Input file: " << inputFileName << endl;
	cout << "Output file: " << outputFileName << endl;

	cout << endl;


	/*
	 * Reading of the input file.
	 */
	cout << "Reading the file \"" << inputFileName << "\" ... " << flush;

	TransformInstance *transformInstance = 0;
	try {
		transformInstance = readInputFileTSPLIB(inputFileName);
	}
	catch (InputFileFormatNotSupported &e) {
	    cerr << "the file format of the file \"" << inputFileName << "\" is not supported!" << endl;
	    return (1);
	}
	catch (ifstream::failure &e) {
	    cerr << "the file \"" << inputFileName << "\" does not exists or is not valid" <<
	    		" or is damaged!" << e.what() <<endl;
	    return (1);
    }
	catch (bad_alloc &e) {
		cerr << "the input graph is too large!" << endl;
	    return (1);
	}
	catch (range_error &e) {
		cerr << "the range error occurs!" << endl;
	    return (1);
	}
	catch ( ... ) {
		cerr << "an exception occurs!" << endl;
	    return (1);
	}

	cout << "OK" << endl;


	/*
	 * Writing of the input file.
	 */
	cout << "Writing to the file \"" << outputFileName << "\" ... " << flush;

	try {
		//If the graph is small enough, use the safe method.
		if (transformInstance->getN() <= N_THRESHOLD) {
			writeOutputFile(outputFileName, transformInstance);
		}  //If the graph is too big, create the XML file directly.
		else {
			writeOutputFileWithoutUsingAParser(outputFileName, transformInstance);
		}
	}
	catch (bad_alloc &e) {
		cerr << "the input graph is too large!" << endl;
	    return (1);
	}
    catch (const OutOfMemoryException &e) {
		cerr << "out of memory problem!" << endl;
		delete transformInstance;
		return (1);
    }
    catch (const DOMException &e) {
		cerr << "Xerces failed!" << endl;
		delete transformInstance;
		return (1);
    }
	catch (const XMLException &e) {
		cerr << "Xerces failed!" << endl;
		delete transformInstance;
		return (1);
	}
	catch ( ... ) {
		cerr << "an exception occurs!" << endl;
		delete transformInstance;
		return (1);
	}

	cout << "OK" << endl;


	/*
	 * Releasing of the memory.
	 */
	delete transformInstance;

	return (0);
}
