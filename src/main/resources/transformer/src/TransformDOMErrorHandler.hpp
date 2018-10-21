/**
 * @file TransformDOMErrorHandler.hpp
 * Defines the ErrorHandler for the DOM parser.
 *
 * @brief Defines the ErrorHandler for the DOM parser.
 *
 * @author Ulrich Pferschy and Rostislav Stanek
 * (Institut fuer Statistik und Operations Research, Universitaet Graz)
 */

#ifndef _TransformDOMErrorHandler_HPP_
#define _TransformDOMErrorHandler_HPP_

#include <xercesc/dom/DOMErrorHandler.hpp>

using namespace xercesc;

XERCES_CPP_NAMESPACE_USE


/**
 * Implements a DOMErrorHandler
 */
class TransformDOMErrorHandler : public DOMErrorHandler {
private:
	/**
	 * Not implemented copy constructor for the class TransformDOMErrorHandler.
	 * @param dOMErrorHandler Instance to be copied.
	 */
    TransformDOMErrorHandler(const DOMErrorHandler &dOMErrorHandler);

	/**
	 * Not implemented operator "=".
	 * @param dOMErrorHandler Right side of the operator.
	 * @return Left side of the operator.
	 */
    DOMErrorHandler &operator=(const DOMErrorHandler &dOMErrorHandler);
public:
    /**
	 * Constructor for the class TransformDOMErrorHandler.
     */
	TransformDOMErrorHandler();

	/**
	 * Destructor for the class TransformDOMErrorHandler.
	 */
    ~TransformDOMErrorHandler();

    /**
     * Error handling interface. The methode returns always
     * false
     * @param dOMError Error.
     * @return
     *   <ul>
     *     <li>true if it should be continued,</li>
     *     <li>false if it should not be continued.</li>
     *   </ul>
     */
    bool handleError(const DOMError &dOMError);
};


#endif
