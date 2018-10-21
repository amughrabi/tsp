/**
 * @file ValidateSAX2ErrorHandler.hpp
 * Defines the ErrorHandler for the SAX2 parser.
 *
 * @brief Defines the ErrorHandler for the SAX2 parser.
 *
 * @author Ulrich Pferschy and Rostislav Stanek
 * (Institut fuer Statistik und Operations Research, Universitaet Graz)
 */

#ifndef _ValidateSAX2ErrorHandler_HPP_
#define _ValidateSAX2ErrorHandler_HPP_

#include <xercesc/sax/ErrorHandler.hpp>

using namespace xercesc;

XERCES_CPP_NAMESPACE_USE


/**
 * Implements an ErrorHandler
 */
class SAX2ErrorHandler : public ErrorHandler {
private:
	/**
	 * True if a validation fails.
	 */
	bool failed;

	/**
	 * Not implemented copy constructor for the class SAX2ErrorHandler.
	 * @param sAXErrorHandler Instance to be copied.
	 */
	SAX2ErrorHandler(const SAX2ErrorHandler &sAXErrorHandler);

	/**
	 * Not implemented operator "=".
	 * @param sAXErrorHandler Right side of the operator.
	 * @return Left side of the operator.
	 */
	SAX2ErrorHandler &operator=(const SAX2ErrorHandler &sAXErrorHandler);

public:
    /**
	 * Constructor for the class SAXErrorHandler.
     */
	SAX2ErrorHandler();

	/**
	 * Destructor for the class SAXErrorHandler.
	 */
    ~SAX2ErrorHandler();

    /**
     * Returns true if the last validation failed
     * @return
     *   <ul>
     *     <li>true if the last validation failed</li>
     *     <li>false if the last validation failed</li>
     *   </ul>
     */
    inline bool getFailed() {
    	return (failed);
    }

    /**
     * Error handling interface for warnings. The method does
     * nothing.
     * @param exc Warning.
     */
    void warning(const SAXParseException &exc);

    /**
     * Error handling interface for errors. The method does
     * nothing.
     * @param exc Error.
     */
    void error(const SAXParseException &exc);

    /**
     * Error handling interface for fatal errors. The method
     * does nothing
     * @param exc Fatal error.
     */
    void fatalError(const SAXParseException &exc);

    /**
     * Reset the Error handler object on its reuse. The method
     * does nothing.
     */
    void resetErrors();
};


#endif
