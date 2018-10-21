/**
 * @file Validate.cpp
 * Validates an travelling salesman problem instance.
 *
 * @brief Validates an travelling salesman problem instance.
 *
 * @author Ulrich Pferschy and Rostislav Stanek
 * (Institut fuer Statistik und Operations Research, Universitaet Graz)
 */

#include <iostream>

#include <xercesc/util/XMLException.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

#include "ValidateConstantsFunctionsAndClasses.hpp"
#include "ValidateIO.hpp"
#include "ValidateInstance.hpp"

using namespace std;
using namespace xercesc;

XERCES_CPP_NAMESPACE_USE


/**
 * The main function.
 * @param argc Number of elements in the array argv, in particular 1 or 2.
 * @param argv Name of the running program or name of the running program,
 *   name of the input file.
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
	cout << "Validate.cpp:" << endl;
	cout << "----------------------------------------------------------------" << endl;

	string inputFileName;
	try{
		inputFileName = parseCommandLineArguments(argc, argv);
	}
	catch (CommandLineArgumentsInvalid &e) {
		cerr << "The command-line arguments are invalid!" << endl;
	    return (1);
	}

	//Writing the used filename on the standard output
	cout << "Input file: " << inputFileName << endl;


	/*
	 * Reading of the input file.
	 */
	cout << "Reading the file \"" << inputFileName << "\" ... " << flush;

	Instance *instance = 0;
	try {
		instance = instanceIn(inputFileName);
	}
	catch (bad_alloc &e) {
		cerr << "the input graph is too large!" << endl;
	    return (1);
	}
    catch (OutOfMemoryException &e) {
		cerr << "the input graph is too large!" << endl;
	    return (1);
    }
	catch (XMLException &e) {
		cerr << "Xerces failed!" << endl;
		return (1);
	}
	catch (ValidationSchemaDoesNotExist &e) {
		cerr << "the validation schema does not exist!" << endl;
		return (1);
	}
    catch (ValidationFailed &e) {
		cerr << "the validation failed!" <<
				" Note that all costs have to be \"double\" type compatible." <<
				" The number of vertices has to be \"unsigned int\" compatible." << endl;
		return (1);
    }
/*	catch ( ... ) {
		cerr << "an exception occurs!" << endl;
		return (1);
	}*/

	cout << "OK" << endl;
	cout << "----------------------------------------------------------------" << endl;


	/*
	 * Printing the results.
	 */
	if (instance->getGraph()->getIsUndirected()) {
		cout <<
				"The file \"" <<
				inputFileName <<
				"\" responds to a valid instance of the symmetric travelling salesman problem." <<
				endl;
	}
	else {
		cout <<
				"The file \"" <<
				inputFileName <<
				"\" responds to a valid instance of the asymmetric travelling salesman problem." <<
				endl;
	}
	cout << "\tName: " << instance->getName() << endl;
	cout << "\tSource: " << instance->getSource() << endl;
	cout << "\tDescription: " << instance->getDescription() << endl;
	cout << "\tNumber of vertices: " << instance->getGraph()->getN() << endl;


//	cout << endl << endl << *instance << endl << endl;


	/*
	 * Releasing of the memory and returning 0.
	 */
	delete instance;

	return (0);
}
