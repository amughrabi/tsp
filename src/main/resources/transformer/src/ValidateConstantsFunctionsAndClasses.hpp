/**
 * @file ValidateConstantsFunctionsAndClasses.hpp
 * Defines the constants, some basic functions and some basic
 * functions necessary for the IO.
 *
 * @brief Defines the constants, some basic functions and some basic
 *   classes necessary for the IO.
 *
 * @author Ulrich Pferschy and Rostislav Stanek
 * (Institut fuer Statistik und Operations Research, Universitaet Graz)
 */

#ifndef _ValidateConstantsFunctionsAndClasses_HPP_
#define _ValidateConstantsFunctionsAndClasses_HPP_

#include <limits>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>


/**
 * Filename extension for xml files.
 */
const std::string INPUT_FILE_FILENAME_EXTENSION = ".xml";

/**
 * Validation schema for the xml files.
 */
const std::string VALIDATION_SCHEMA = "TSPConfiguration.xsd";

/**
 * Encoding of the xml file
 */
const std::string XML_ENCODING = "UTF-8";

/**
 * Floatfield flag of doubles writed to stringstreams.
 */
const std::ios::fmtflags DOUBLE_FLOATFIELD = std::ios::scientific;

/**
 * Xml tag: Name of the document node.
 */
const std::string XML_DOCUMENT_NODE = "travellingSalesmanProblemInstance";

/**
 * Xml tag: Name of the instance.
 */
const std::string XML_NAME = "name";

/**
 * Xml tag: Source of the instance.
 */
const std::string XML_SOURCE = "source";

/**
 * Xml tag: Description of the instance.
 */
const std::string XML_DESCRIPTION = "description";

/**
 * Precision of doubles.
 */
const std::string XML_DOUBLE_PRECISION = "doublePrecision";

/**
 * Number of ignored digits of double types. (The deviation of
 * double values can be at most 1e-(DoublePrecision - IgnoredDigits).)
 */
const std::string XML_IGNORED_DIGITS = "ignoredDigits";

/**
 * Xml tag: Underlying graph.
 */
const std::string XML_GRAPH = "graph";

/**
 * Xml tag: One vertex.
 */
const std::string XML_VERTEX = "vertex";

/**
 * Xml tag: One edge
 */
const std::string XML_EDGE = "edge";

/**
 * Xml tag - attribute: Cost of the edge.
 */
const std::string XML_EDGE_ATTRIBUTE_COST = "cost";


/**
 * Trims the string from left.
 * @param s The string.
 * @param t The trimmed characters.
 */
inline void trimLeft (std::string &s, const std::string &t = " \t\r\n") {
	s.erase(0, s.find_first_not_of(t));
}

/**
 * Trims the string from right.
 * @param s The string.
 * @param t The trimmed characters.
 */
inline void trimRight (std::string &s, const std::string &t = " \t\r\n") {
	std::string::size_type i = s.find_last_not_of(t);
	if (i == std::string::npos) {
		s = "";
	}
	else {
		s.erase(s.find_last_not_of(t) + 1);
	}
}

/**
 * Trims the string from left and from right.
 * @param s The string.
 * @param t The trimmed characters.
 */
inline void trim (std::string &s, const std::string &t = " \t\r\n") {
	trimLeft(s, t);
	trimRight(s, t);
}


/**
 * Exception class used if the command-line arguments are invalid.
 */
class CommandLineArgumentsInvalid : public std::exception {
public:
	/**
	 * Constructor for the class CommandLineArgumentsInvalid.
	 */
	inline CommandLineArgumentsInvalid() {
	}
};

/**
 * Exception class used if a validation schema does not exist.
 */
class ValidationSchemaDoesNotExist : public std::exception {
public:
	/**
	 * Constructor for the class ValidationSchemaDoesNotExist.
	 */
	inline ValidationSchemaDoesNotExist() {
	}
};

/**
 * Exception class used if a validation fails.
 */
class ValidationFailed : public std::exception {
public:
	/**
	 * Constructor for the class ValidationFailed.
	 */
	inline ValidationFailed() {
	}
};


#endif
