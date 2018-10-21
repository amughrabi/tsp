/**
 * @file TransformConstantsClassesAndFunctions.hpp
 * Defines the constants, classes and some functions necessary for the
 * transform programs.
 *
 * @brief Defines the constants, types and some functions necessary for
 *   the transform programs.
 *
 * @author Ulrich Pferschy and Rostislav Stanek
 * (Institut fuer Statistik und Operations Research, Universitaet Graz)
 */

#ifndef _TransformConstantsClassesAndFunctions_HPP_
#define _TransformConstantsClassesAndFunctions_HPP_

#include <cmath>
#include <string>
#include <vector>
#include <iomanip>

#include <xercesc/util/PlatformUtils.hpp>


/**
 * Filename extension for the symmetric travelling salesman problem files from the TSPLIB.
 */
const std::string INPUT_FILE_FILENAME_EXTENSION_TSP = ".tsp";

/**
 * Filename extension for the asymmetric travelling salesman problem files from the TSPLIB.
 */
const std::string INPUT_FILE_FILENAME_EXTENSION_ATSP = ".atsp";

/**
 * Tag for the TSPLIB: "NAME":.
 */
const std::string TAG_NAME = "NAME:";

/**
 * Tag for the TSPLIB: "TYPE:".
 */
const std::string TAG_TYPE = "TYPE:";

/**
 * Tag for the TSPLIB: "COMMENT:".
 */
const std::string TAG_COMMENT = "COMMENT:";

/**
 * Tag for the TSPLIB: "DIMENSION:".
 */
const std::string TAG_DIMENSION = "DIMENSION:";

/**
 * Tag for the TSPLIB: "EDGE_WEIGHT_TYPE:".
 */
const std::string TAG_EDGE_WEIGHT_TYPE = "EDGE_WEIGHT_TYPE:";

/**
 * Tag for the TSPLIB: "EDGE_WEIGHT_FORMAT:".
 */
const std::string TAG_EDGE_WEIGHT_FORMAT = "EDGE_WEIGHT_FORMAT:";

/**
 * Tag for the TSPLIB: "DISPLAY_DATA_TYPE:".
 */
const std::string TAG_DISPLAY_DATA_TYPE = "DISPLAY_DATA_TYPE:";

/**
 * Tag for the TSPLIB: "NODE_COORD_SECTION".
 */
const std::string TAG_NODE_COORD_SECTION = "NODE_COORD_SECTION";

/**
 * Tag for the TSPLIB: "EDGE_WEIGHT_SECTION".
 */
const std::string EDGE_WEIGHT_SECTION = "EDGE_WEIGHT_SECTION";

/**
 * Tag for the TSPLIB: "DISPLAY_DATA_SECTION".
 */
const std::string DISPLAY_DATA_SECTION = "DISPLAY_DATA_SECTION";

/**
 * Tag for the TSPLIB: "EOF:".
 */
const std::string TAG_EOF = "EOF";

/**
 * Value for the tag "TYPE:" for the symmetric travelling salesman problem.
 */
const std::string VALUE_TYPE_TSP = "TSP";

/**
 * Value for the tag "TYPE:" for the asymmetric travelling salesman problem.
 */
const std::string VALUE_TYPE_ATSP = "ATSP";

/**
 * Idealized sphere radius of the earth.
 */
const double RRR = 6378.388;

/**
 * Value "GEO" for the tag "EDGE_WEIGHT_TYPE:" for the TSPLIB.
 */
const std::string VALUE_EDGE_WEIGHT_TYPE_GEO = "GEO";

/**
 * Value "EUC_2D" for the tag "EDGE_WEIGHT_TYPE:" for the TSPLIB.
 */
const std::string VALUE_EDGE_WEIGHT_TYPE_EUC_2D = "EUC_2D";

/**
 * Value "CEIL_2D" for the tag "EDGE_WEIGHT_TYPE:" for the TSPLIB.
 */
const std::string VALUE_EDGE_WEIGHT_TYPE_CEIL_2D = "CEIL_2D";

/**
 * Value "GEO" for the tag "EDGE_WEIGHT_TYPE:" for the TSPLIB.
 */
const std::string VALUE_EDGE_WEIGHT_TYPE_ATT = "ATT";

/**
 * Value "EXPLICIT" for the tag "EDGE_WEIGHT_TYPE:" for the TSPLIB.
 */
const std::string VALUE_EDGE_WEIGHT_TYPE_EXPLICIT = "EXPLICIT";

/**
 * Value "FUNCTION" for the tag "EDGE_WEIGHT_FORMAT:" for the TSPLIB.
 */
const std::string VALUE_EDGE_WEIGHT_FORMAT_FUNCTION = "FUNCTION";

/**
 * Value "FUNCTION" for the tag "EDGE_WEIGHT_FORMAX:" for the TSPLIB.
 */
const std::string VALUE_EDGE_WEIGHT_FORMAT_FULL_MATRIX = "FULL_MATRIX";

/**
 * Value "FUNCTION" for the tag "EDGE_WEIGHT_FORMAX:" for the TSPLIB.
 */
const std::string VALUE_EDGE_WEIGHT_FORMAT_LOWER_DIAG_ROW = "LOWER_DIAG_ROW";

/**
 * Value "FUNCTION" for the tag "EDGE_WEIGHT_FORMAX:" for the TSPLIB.
 */
const std::string VALUE_EDGE_WEIGHT_FORMAT_UPPER_DIAG_ROW = "UPPER_DIAG_ROW";

/**
 * Value "FUNCTION" for the tag "EDGE_WEIGHT_FORMAX:" for the TSPLIB.
 */
const std::string VALUE_EDGE_WEIGHT_FORMAT_UPPER_ROW = "UPPER_ROW";

/**
 * Value "COORD_DISPLAY" for the tag "VALUE_DISPLAY_DATA_TYPE:" for the TSPLIB.
 */
const std::string VALUE_DISPLAY_DATA_TYPE_COORD_DISPLAY = "COORD_DISPLAY";

/**
 * Value "COORD_DISPLAY" for the tag "VALUE_DISPLAY_DATA_TYPE:" for the TSPLIB.
 */
const std::string VALUE_DISPLAY_DATA_TYPE_TWOD_DISPLAY = "TWOD_DISPLAY";


/**
 * Filename extension for xml files.
 */
const std::string OUTPUT_FILE_FILENAME_EXTENSION = ".xml";

/**
 * The output will be pretty printed
 */
const bool XML_FORMAT_PRETTY_PRINT = true;

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
 * Encoding of the xml file
 */
const std::string XML_ENCODING = "UTF-8";

/**
 * Value "TSPLIB" for the xml tag "source"
 */
const std::string XML_VALUE_SOURCE_TSPLIB = "TSPLIB";

/**
 * Floatfield flag of doubles writed to stringstreams.
 */
const std::ios::fmtflags DOUBLE_FLOATFIELD = std::ios::scientific;

/**
 * Precision of doubles writed to the .xml-files.
 */
const std::streamsize TRANSFORM_DOUBLE_PRECISION = std::numeric_limits<double>::digits10;

/**
 * Number of ignored digits of double types. (The deviation of double
 * values can be at most 1e-(DoublePrecision - IgnoredDigits).)
 */
const std::streamsize TRANSFORM_IGNORED_DIGITS = 5;

/**
 * Maximum of an absolute value considered as zero used by "Transform"-classes.
 */
const double TRANSFORM_DOUBLE_ZERO =
		pow(10.0, -1.0 * static_cast<double>(TRANSFORM_DOUBLE_PRECISION - TRANSFORM_IGNORED_DIGITS));


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
 * Rounds the double to the TRANSFORM_DOUBLE_PRECISION. The floatfield is set to
 * DOUBLE_FLOATFIELD (see IOConstantsBasicFunctionsAndBasicClasses.hpp).
 * @param d Number which should be rounded.
 * @return Rounded number d.
 */
inline double roundToDoublePrecisionAndDoubleFloatField(double d) {
	std::stringstream dStringstream;
	dStringstream.setf(DOUBLE_FLOATFIELD, std::ios::floatfield);
	dStringstream << std::setprecision(static_cast<int>(TRANSFORM_DOUBLE_PRECISION));
	dStringstream << d;
	std::string dString;
	dString = dStringstream.str();
	std::istringstream dIstringstream(dString);
	double result;
	dIstringstream >> result;
	return (result);
}


/**
 * Exception class used if the input format of the tsp file is not supported.
 */
class InputFileFormatNotSupported : public std::exception {
public:
	/**
	 * Constructor for the class FormatNotSupported.
	 */
	inline InputFileFormatNotSupported() {
	}
};

/**
 * Saves one instance with the graph in the form of an adjacency matrix.
 * If the instance is symmetric the program saves only the lower
 * part of the adjacency matrix.
 * This class provides no checks of validity or ranges.
 */
class TransformInstance {
private:
	/**
	 * Type of the instance.
	 */
	std::string type;

	/**
	 * Name of the instance.
	 */
	std::string name;

	/**
	 * Source of the instance.
	 */
	std::string source;

	/**
	 * Description of the instance.
	 */
	std::string description;

	/**
	 * Number of vertices in the graph of the instance.
	 */
	std::vector<std::vector<double> >::size_type n;

	/**
	 * Adjacency matrix of the instance. If the instance is
	 * symmetric the program saves only the lower part of
	 * the adjacency matrix.
	 */
	std::vector<std::vector<double> > adjacencyMatrix;

	/**
	 * Initializes the name, the source, the description and n
	 * @param type Type of the instance.
	 * @param name Name of the instance.
	 * @param source Source of the instance.
	 * @param description Description of the instance.
	 * @param n Number of vertices in the graph of the instance.
	 */
	void init(
			const std::string &type,
			const std::string &name,
			const std::string &source,
			const std::string &description,
			const std::vector<std::vector<double> >::size_type n);

public:
	/**
	 * Constructor for the class TransformInstance.
	 * @param type Type of the instance.
	 * @param name Name of the instance.
	 * @param source Source of the instance.
	 * @param description Description of the instance.
	 * @param n Number of vertices in the graph of the instance.
	 */
	TransformInstance(
			const std::string &type,
			const std::string &name,
			const std::string &source,
			const std::string &description,
			const std::vector<std::vector<double> >::size_type n);

	/**
	 * Copy constructor for the class TransformInstance.
	 * @param transformInstance Instance to be copied.
	 */
	TransformInstance(const TransformInstance &transformInstance);

	/**
	 * Implements the operator "=".
	 * @param transformInstance Right side of the operator.
	 * @return Left side of the operator.
	 */
	TransformInstance &operator=(const TransformInstance &transformInstance);

	/**
	 * Returns the type of the instance.
	 * @return Type of the instance.
	 */
	inline std::string getType() const {
		return (type);
	}

	/**
	 * Returns the name of the instance.
	 * @return Name of the instance.
	 */
	inline std::string getName() const {
		return (name);
	}

	/**
	 * Returns the source of the instance.
	 * @return Source of the instance.
	 */
	inline std::string getSource() const {
		return (source);
	}

	/**
	 * Returns the description of the instance.
	 * @return Description of the instance.
	 */
	inline std::string getDescription() const {
		return (description);
	}

	/**
	 * Returns the number of vertices in the graph of the instance.
	 * @return Number of vertices in the graph of the instance.
	 */
	inline std::vector<std::vector<double> >::size_type getN() const {
		return (n);
	}

	/**
	 * Sets one element in the adjacency matrix. The parameters are not checked.
	 * The new value will be rounded according to the constants DOUBLE_PRECISION
	 * and IGNORED_DIGITS.
	 * @param i Row.
	 * @param j Column.
	 * @param value New value of the element on the i-th row and j-th column.
	 */
	inline void setAdjacencyMatrixElement(
			const std::vector<std::vector<double> >::size_type i,
			const std::vector<double>::size_type j,
			const double value) {
		double roundedValue = roundToDoublePrecisionAndDoubleFloatField(value);
		if (type == VALUE_TYPE_TSP) {
			if (i < j) {
				adjacencyMatrix.at(
						static_cast<std::vector<std::vector<double> >::size_type>(j)).at(
								static_cast<std::vector<double>::size_type>(i)) = roundedValue;
			}
			else {
				adjacencyMatrix.at(i).at(j) = roundedValue;
			}
		}
		else {  //if (type == VALUE_TYPE_ATSP)
			adjacencyMatrix.at(i).at(j) = roundedValue;
		}
	}

	/**
	 * Returns one element in the adjacency matrix. The parameters are not checked.
	 * @param i Row.
	 * @param j Column.
	 * @return Value of the element on the i-th row and j-th column.
	 */
	inline double getAdjacencyMatrixElement(
			const std::vector<std::vector<double> >::size_type i,
			const std::vector<double>::size_type j) const {
		if (type == VALUE_TYPE_TSP) {
			if (i < j) {
				return (adjacencyMatrix.at(
						static_cast<std::vector<std::vector<double> >::size_type>(j)).at(
								static_cast<std::vector<double>::size_type>(i)));
			}
			else {
				return (adjacencyMatrix.at(i).at(j));
			}
		}
		else {  //if (type == VALUE_TYPE_ATSP)
			return (adjacencyMatrix.at(i).at(j));
		}
	}
};


/**
 * Reads the input file and creates an instance of the class TransformInstance. The parameters
 * are not checked.
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
 * @param inputFileName Name of the input file.
 * @return Instance of the class TransformInstance.
 */
TransformInstance *readInputFileTSPLIB(const std::string &inputFileName);

/**
 * Writes one instance of the class TransformInstance to an output file. The parameters are
 * not checked.
 * @param outputFileName Name of the output file.
 * @param transformInstance Instance of the class TransformInstance.
 */
void writeOutputFile(const std::string &outputFileName, const TransformInstance *transformInstance);

/**
 * Writes one instance of the class TransformInstance to an output file without using a parser.
 * This method is quicker than the previous one but it provides no parser guarantee that the output
 * file is a valid xml file. The parameters are not checked.
 * @param outputFileName Name of the output file.
 * @param transformInstance Instance of the class TransformInstance.
 */
void writeOutputFileWithoutUsingAParser(
		const std::string &outputFileName,
		const TransformInstance *transformInstance);


#endif
