/**
 * @file ValidateSAX2ContentHandler.cpp
 * Defines the ContentHandler for the SAX2 parser.
 *
 * @brief Defines the ContentHandler for the SAX2 parser.
 *
 * @author Ulrich Pferschy and Rostislav Stanek
 * (Institut fuer Statistik und Operations Research, Universitaet Graz)
 */

#include <fstream>
#include <sstream>
#include <cmath>

#include <xercesc/util/XMLString.hpp>
#include <xercesc/sax2/Attributes.hpp>

#include "ValidateConstantsFunctionsAndClasses.hpp"

#include "ValidateSAX2ContentHandler.hpp"

using namespace std;


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

/**
 * Trancodes XMLCh data to string.
 */
class StringTranscode {
private:
	/**
	 * String format of the unicode XMLCh format.
	 */
    string stringStringForm;
public:
    /**
     * Constructor for the class StringTranscode.
     * @param toTranscode String that should be transcoded.
     */
    inline StringTranscode(const XMLCh *toTranscode) {
        // Call the private transcoding method
    	char* cStringForm = XMLString::transcode(toTranscode);
    	stringStringForm = string(cStringForm);
    }

    /**
     * Destructor for the class StringTranscode
     */
    inline ~StringTranscode() {
    }

    /**
     * Returns the string format of the unicode XMLCh format.
     * @return String format of the unicode XMLCh format.
     */
    inline const string stringForm() const {
        return (stringStringForm);
    }
};
#define stringForm(str) StringTranscode(str).stringForm()

void SAX2ContentHandler::init() {
	failed = false;
	name = "";
	source = "";
	description = "";
	doublePrecision = 0;
	doubleZero = 0.0;
	firstVertex = true;
	numberOfParsedVertices = 0;
	n = adjacencyMatrix.size();
	for (std::vector<std::vector<double> >::size_type i = 0; i < n; i++) {
		adjacencyMatrix.at(i).clear();
	}
	adjacencyMatrix.clear();
	n = 0;
	costsOfEdgesDefinedInTheFirstVertex.clear();
	buffer = "";
	cost = 0.0;
}

SAX2ContentHandler::SAX2ContentHandler(): ContentHandler() {
	init();
}

SAX2ContentHandler::~SAX2ContentHandler() {
}

void SAX2ContentHandler::setDocumentLocator(const Locator *const) {
}

void SAX2ContentHandler::startDocument() {
	init();
}

void SAX2ContentHandler::endDocument() {
	//The decision if the graph is undirected. If yes,
	//the matrix will be resized.
	bool isUndirected = true;
	for (vector<vector<double> >::size_type i = 0; i < n; i++) {
		for (
				vector<double>::size_type j = 0;
				j < static_cast<vector<double>::size_type>(i);
				j++) {
			if (!parsedEntries.at(
					static_cast<vector<vector<bool> >::size_type>(i)).at(
							static_cast<vector<bool>::size_type>(j))) {
				failed = true;
				return;
			}
			if (!parsedEntries.at(
					static_cast<vector<vector<bool> >::size_type>(j)).at(
							static_cast<vector<bool>::size_type>(i))) {
				failed = true;
				return;
			}
			if (
					abs(adjacencyMatrix.at(i).at(j) -
							adjacencyMatrix.at(
									static_cast<vector<vector<double> >::size_type>(j)).at(
											static_cast<vector<double>::size_type>(i))) >
					doubleZero) {
				isUndirected = false;
			}
		}
	}

	//Setting 0 on the main diagonal if necessary
	if (isUndirected) {
		for (vector<vector<double> >::size_type i = 0; i < n; i++) {
			adjacencyMatrix.at(i).resize(static_cast<vector<double>::size_type>(i + 1));
			if (!parsedEntries.at(i).at(
					static_cast<vector<bool>::size_type>(i))) {
				adjacencyMatrix.at(i).at(static_cast<vector<double>::size_type>(i)) = 0;
			}
		}
	}
	else {
		for (vector<vector<bool> >::size_type i = 0; i < n; i++) {
			if (!parsedEntries.at(i).at(
					static_cast<vector<bool>::size_type>(i))) {
				adjacencyMatrix.at(i).at(static_cast<vector<double>::size_type>(i)) = 0;
			}
		}
	}
}

void SAX2ContentHandler::startPrefixMapping(const XMLCh *const, const XMLCh *const) {
}

void SAX2ContentHandler::endPrefixMapping(const XMLCh *const) {
}

void SAX2ContentHandler::skippedEntity(const XMLCh *const) {
}

void SAX2ContentHandler::startElement(
		const XMLCh *const,
        const XMLCh *const localname,
        const XMLCh *const,
        const Attributes &attributes) {
	buffer.clear();

	//vertex
	if (XMLString::compareIString(localname, unicodeForm(XML_VERTEX)) == 0) {
		if (firstVertex) {
			n = 1;
		}
	}

	//edge
	else if (XMLString::compareIString(localname, unicodeForm(XML_EDGE)) == 0) {
		if (firstVertex) {
			costsOfEdgesDefinedInTheFirstVertex.resize(
					costsOfEdgesDefinedInTheFirstVertex.size() + 1);
		}
		const XMLCh* costXMLCh = attributes.getValue(static_cast<XMLSize_t>(0));
		string costString = stringForm(costXMLCh);
		trim(costString);
		istringstream costIStringstream(costString);
		costIStringstream.exceptions(ifstream::failbit | ifstream::badbit);
		try {
			costIStringstream >> cost;
		}
		catch (ifstream::failure &e) {
			failed = true;
		}
	}
}

void SAX2ContentHandler::endElement(
		const XMLCh *const,
		const XMLCh *const localname,
		const XMLCh *const) {
	//name
	if (XMLString::compareIString(localname, unicodeForm(XML_NAME)) == 0) {
		name = buffer;
		trim(name);
	}

	//source
	else if (XMLString::compareIString(localname, unicodeForm(XML_SOURCE)) == 0) {
		source = buffer;
		trim(source);
	}

	//descritpion
	else if (XMLString::compareIString(localname, unicodeForm(XML_DESCRIPTION)) == 0) {
		description = buffer;
		trim(description);
	}

	//doublePrecision
	else if (XMLString::compareIString(localname, unicodeForm(XML_DOUBLE_PRECISION)) == 0) {
		string doublePrecisionString = buffer;
		trim(doublePrecisionString);
		istringstream doublePrecisionIStringstream(doublePrecisionString);
		doublePrecisionIStringstream.exceptions(ifstream::failbit | ifstream::badbit);
		try {
			doublePrecisionIStringstream >> doublePrecision;
		}
		catch (ifstream::failure &e) {
			failed = true;
		}
	}

	//ignoredDigits
	else if (XMLString::compareIString(localname, unicodeForm(XML_IGNORED_DIGITS)) == 0) {
		string ignoredDigitsString = buffer;
		trim(ignoredDigitsString);
		istringstream ignoredDigitsIStringstream(ignoredDigitsString);
		ignoredDigitsIStringstream.exceptions(ifstream::failbit | ifstream::badbit);
		try {
			ignoredDigitsIStringstream >> ignoredDigits;
		}
		catch (ifstream::failure &e) {
			failed = true;
		}
		doubleZero = pow(10,
				-1.0 * (static_cast<double>(doublePrecision - ignoredDigits)));
	}

	//graph
	else if (XMLString::compareIString(localname, unicodeForm(XML_GRAPH)) == 0) {
		//We have to check if all vertices are defined.
		if (numberOfParsedVertices != n) {
			failed = true;
		}
	}

	//vertex
	else if (XMLString::compareIString(localname, unicodeForm(XML_VERTEX)) == 0) {
		//If the first vertex is read. We can assume that the graph is complete
		//(with, or without self-loops).
		if (firstVertex) {
			firstVertex = false;
			parsedEntries.resize(static_cast<vector<vector<bool> >::size_type>(n));
			adjacencyMatrix.resize(n);

			//Resizing.
			for (vector<vector<double> >::size_type i = 0; i < n; i++) {
				parsedEntries.at(static_cast<vector<vector<bool> >::size_type>(i)).resize(
						static_cast<vector<bool>::size_type>(n));
				adjacencyMatrix.at(i).resize(
						static_cast<vector<double>::size_type>(n));
			}

			//Initializing
			for (vector<vector<bool> >::size_type i = 0; i < n; i++) {
				for (vector<bool>::size_type j = 0; j < n; j++) {
					parsedEntries.at(i).at(j) = false;
				}
			}

			//Copying of the first row.
			for (
					vector<double>::size_type j = 0;
					j < costsOfEdgesDefinedInTheFirstVertex.size();
					j++) {
				if (edgesDefinedInTheFirstVertex.count(j) > 0) {
					adjacencyMatrix.at(0).at(j) = costsOfEdgesDefinedInTheFirstVertex.at(j);
					parsedEntries.at(
							static_cast<vector<vector<bool> >::size_type>(0)).at(
									static_cast<vector<bool>::size_type>(j)) = true;
				}
			}
		}

		//Vertex counter.
		numberOfParsedVertices++;
	}

	//edge
	else if (XMLString::compareIString(localname, unicodeForm(XML_EDGE)) == 0) {
		vector<double>::size_type edge;
		string edgeString = buffer;
		trim(edgeString);
		istringstream edgeIStringstream(edgeString);
		edgeIStringstream.exceptions(ifstream::failbit | ifstream::badbit);
		try {
			edgeIStringstream >> edge;
		}
		catch (ifstream::failure &e) {
			failed = true;
		}

		//Now, the inequality numberOfParsedEdges >= 0 allways holds.
		//We need special handling of the first row (since we do not know the number
		//of vertices)
		if (firstVertex) {
			//In a complete graph there must be also defined an edge to the last vertex.
			if (edge + 1 > n) {
				n = edge + 1;
			}

			//If necessary, we make the first row longer.
			if (costsOfEdgesDefinedInTheFirstVertex.size() < edge + 1) {
				costsOfEdgesDefinedInTheFirstVertex.resize(edge + 1);
			}

			//The edges are not allowed to be redefined.
			if (edgesDefinedInTheFirstVertex.count(edge) > 0) {
				failed = true;
				return;
			}

			//We mark the edges which have been defined.
			edgesDefinedInTheFirstVertex.insert(edge);
			costsOfEdgesDefinedInTheFirstVertex.at(edge) = cost;
		}
		else {
			//We try to set the matrix entries
			try {
				//The edges are not allowed to be redefined.
				if (parsedEntries.at(
						static_cast<vector<vector<bool> >::size_type>(numberOfParsedVertices)).at(
								static_cast<vector<bool>::size_type>(edge))) {
					failed = true;
					return;
				}

				//We set the cost.
				parsedEntries.at(
						static_cast<vector<vector<bool> >::size_type>(numberOfParsedVertices)).at(
								static_cast<vector<bool>::size_type>(edge)) = true;
				adjacencyMatrix.at(numberOfParsedVertices).at(edge) = cost;
			}
			catch ( ... ) {
				failed = true;
			}
		}
	}
}

void SAX2ContentHandler::characters(const XMLCh *const chars, const XMLSize_t) {
	string toAppend = stringForm(chars);
	trim(toAppend);
	string::size_type i = 0;
	while (i < toAppend.size()) {
		switch (toAppend.at(i)) {
		case '\t':
		case '\r':
		case '\n':
		{
			toAppend.erase(i, static_cast<string::size_type>(1));
		}
		break;
		default:
		{
			i++;
		}
		break;
		}
	}

	buffer.append(toAppend);
}

void SAX2ContentHandler::ignorableWhitespace(const XMLCh *const, const XMLSize_t) {
}

void SAX2ContentHandler::processingInstruction(const XMLCh *const, const XMLCh *const) {
}

void SAX2ContentHandler::resetDocument() {
	init();
}
