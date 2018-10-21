/**
 * @file ValidateIO.hpp
 * Defines the functions and classes necessary for the IO.
 *
 * @brief Defines the functions necessary for the IO.
 *
 * @author Ulrich Pferschy and Rostislav Stanek
 * (Institut fuer Statistik und Operations Research, Universitaet Graz)
 */

#ifndef _ValidateIO_HPP_
#define _ValidateIO_HPP_

#include <string>
#include <stdexcept>

#include "ValidateInstance.hpp"
#include "ValidateGraph.hpp"


/**
 * Parses the command-line arguments.
 * @param argc Number of arguments.
 * @param argv Arguments.
 * @return Name of the input file.
 */
std::string parseCommandLineArguments(int argc, char* argv[]);

/**
 * Reads one travelling salesman problem instance from the file inputFileName.
 * @param inputFileName Name of the input file.
 * @return Instance of the travelling salesman problem.
 */
Instance *instanceIn(const std::string &inputFileName);


#endif
