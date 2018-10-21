/**
 * @file ValidateSAX2ContentHandler.hpp
 * Defines the ContentHandler for the SAX2 parser.
 *
 * @brief Defines the ContentHandler for the SAX2 parser.
 *
 * @author Ulrich Pferschy and Rostislav Stanek
 * (Institut fuer Statistik und Operations Research, Universitaet Graz)
 */

#ifndef _ValidateSAX2ContentHandler_HPP_
#define _ValidateSAX2ContentHandler_HPP_

#include <xercesc/sax2/ContentHandler.hpp>

#include <ios>
#include <vector>
#include <set>

using namespace xercesc;

XERCES_CPP_NAMESPACE_USE


class SAX2ContentHandler : public ContentHandler {
private:
	/**
	 * True if the last internal validation fails.
	 */
	bool failed;

	/**
	 * Indicator indicating if the graph is undirected.
	 * <ul>
	 *   <li>true The Graph is undirected.</li>
	 *   <li>false The Graph is directed.</li>
	 * </ul>
	 */
	bool isUndirected;

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
	 * Precision of doubles.
	 */
	std::streamsize doublePrecision;

	/**
	 * Number of ignored digits of double types. (The deviation of
	 * double values can be at most 1e-(DoublePrecision - IgnoredDigits).)
	 */
	std::streamsize ignoredDigits;

	/**
	 * Maximum of an absolute value considered as zero.
	 */
	double doubleZero;

	/**
	 * true during parsing the first vertex
	 */
	bool firstVertex;

	/**
	 * Number of parsed vertices.
	 */
	std::vector<std::vector<double> >::size_type numberOfParsedVertices;

	/**
	 * Edges defined in the first vertex
	 */
	std::set<std::vector<double>::size_type> edgesDefinedInTheFirstVertex;

	/**
	 * Cost of edges incident with the first vertes.
	 */
	std::vector<double> costsOfEdgesDefinedInTheFirstVertex;

	/**
	 * Number of vertices in the graph of the instance.
	 */
	std::vector<std::vector<double> >::size_type n;

	/**
	 * Matrix indicating which vertices were parsed.
	 */
	std::vector<std::vector<bool> > parsedEntries;

	/**
	 * Adjacency matrix of a weighted undirected complete graph.
	 * The elements in the matrix are the costs. The elements
	 * in the matrix are the costs. The matrix can be either only
	 * a lower triangular matrix for undirected graphs or it can be
	 * a square matrix for the directed graph.
	 */
	std::vector<std::vector<double> > adjacencyMatrix;

	/**
	 * Buffer to parse character data.
	 */
	std::string buffer;

	/**
	 * Cost of the current edge.
	 */
	double cost;

	/**
	 * Not implemented copy constructor for the class SAX2ContentHandler.
	 * @param sAX2ContentHandler Instance to be copied.
	 */
	SAX2ContentHandler(const SAX2ContentHandler &sAX2ContentHandler);

	/**
	 * Not implemented operator "=".
	 * @param sAX2ContentHandler Right side of the operator.
	 * @return Left side of the operator.
	 */
	SAX2ContentHandler &operator=(const SAX2ContentHandler &sAX2ContentHandler);

	/**
	 * Initates the class variables.
	 */
	void init();

public:
    /**
	 * Constructor for the class SAXContentHandler.
     */
	SAX2ContentHandler();

	/**
	 * Destructor for the class SAXContentHandler.
	 */
    ~SAX2ContentHandler();

    /**
     * Returns true if the last validation failed
     * @return
     *   <ul>
     *     <li>true if the last validation failed</li>
     *     <li>false if the last validation failed</li>
     *   </ul>
     */
    inline bool getFailed() const {
    	return (failed);
    }

	/**
	 * Returns the name.
	 * @return Name.
	 */
	inline std::string getName() const {
		return (name);
	}

	/**
	 * Returns the source.
	 * @return Source.
	 */
	inline std::string getSource() const {
		return (source);
	}

	/**
	 * Returns the description.
	 * @return Description.
	 */
	inline std::string getDescription() const {
		return (description);
	}

	/**
	 * Returns the precision of doubles.
	 * @return Precision of doubles.
	 */
	inline std::streamsize getDoublePrecision() const {
		return (doublePrecision);
	}

	/**
	 * Returns the number of ignored digits of double types. (The
	 * deviation of double values can be at most
	 * 1e-(DoublePrecision - IgnoredDigits).)
	 * @return Number of ignored digits of double types. (The
	 *   deviation of double values can be at most
	 *   1e-(DoublePrecision - IgnoredDigits).)
	 */
	inline std::streamsize getIgnoredDigits() const {
		return (ignoredDigits);
	}

	/**
	 * Returns the maximum of an absolute value considered as zero.
	 * @return Maximum of an absolute value considered as zero.
	 */
	inline double getDoubleZero() const {
		return (doubleZero);
	}

	/**
	 * Returns the adjacency matrix of a weighted undirected
	 * complete graph. The elements in the matrix are the costs.
	 * @return Adjacency matrix of a weighted undirected complete
	 * graph. The elements in the matrix are the costs.
	 */
	inline std::vector<std::vector<double> > getAdjacencyMatrix() {
		return (adjacencyMatrix);
	}

	/**
	 * Receive a Locator object for document events. The method
	 * does nothing.
	 * @param locator Object that can return the location of
     *   any SAX document event.
	 */
	void setDocumentLocator(const Locator *const locator);

	/**
	 * Reset the Document object on its reuse. The method does
     * nothing.
	 */
	void resetDocument();

    /**
     * Called if the document starts.
     */
	void startDocument();

    /**
     * Called if the document ends.
     */
	void endDocument();

	/**
	 * Receive notification of the start of an namespace prefix mapping.
	 * The method does nothing
	 * @param prefix namespace prefix used.
	 * @param uri Namespace URI used.
	 */
	void startPrefixMapping(const XMLCh *const prefix, const XMLCh *const uri);

	/**
	 * Receive notification of the end of an namespace prefix mapping.
	 * @param prefix Namespace prefix used.
	 */
	void endPrefixMapping(const XMLCh *const prefix);

	/**
	 * Receive notification of a skipped entity. The method does nothing.
	 * @param name Name of the skipped entity.
	 */
	void skippedEntity(const XMLCh *const name);

	/**
	 * Called if an element starts.
	 * @param uri URI of the associated namespace for this element.
	 * @param localname Local part of the element name
	 * @param qname QName of this element.
	 * @param attributes Attributes attached to the element, if any.
	 */
	void startElement(
			const XMLCh *const uri,
	        const XMLCh *const localname,
	        const XMLCh *const qname,
	        const Attributes &attributes);

	/**
	 * Called if an element ends.
	 * @param uri URI of the associated namespace for this element.
	 * @param localname Local part of the element name.
	 * @param qname QName of this element.
	 */
	void endElement(
			const XMLCh *const uri,
			const XMLCh *const localname,
			const XMLCh *const qname);

	/**
	 * Called if character data occurs.
	 * @param chars Character data.
	 * @param length Length of the Character data.
	 */
	void characters(const XMLCh *const chars, const XMLSize_t length);

	/**
	 * Called if whitespaces occurs. The method does
     * nothing.
     * @param chars Whitespace data.
	 * @param length Length of the Character data.
	 */
	void ignorableWhitespace(const XMLCh *const chars, const XMLSize_t length);

	/**
	 * Called if processing instructions occurs. The method does
     * nothing.
     * @param target Processing instruction target.
     * @param data Processing instruction data.
	 */
	void processingInstruction(const XMLCh *const target, const XMLCh *const data);
};


#endif
