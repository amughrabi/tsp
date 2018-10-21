/**
 * @file TransformDOMErrorHandler.cpp
 * Defines the ErrorHandler for the DOM parser.
 *
 * @brief Defines the tag names in the xml structur, filename
 * extensions etc.
 *
 * @author Ulrich Pferschy and Rostislav Stanek
 * (Institut fuer Statistik und Operations Research, Universitaet Graz)
 */

#include <xercesc/dom/DOMError.hpp>

#include "TransformDOMErrorHandler.hpp"


TransformDOMErrorHandler::TransformDOMErrorHandler(): DOMErrorHandler() {
}

TransformDOMErrorHandler::~TransformDOMErrorHandler() {
}

bool TransformDOMErrorHandler::handleError(const DOMError &dOMError) {
	switch (dOMError.getSeverity()) {
	case DOMError::DOM_SEVERITY_WARNING:
	case DOMError::DOM_SEVERITY_ERROR:
	case DOMError::DOM_SEVERITY_FATAL_ERROR:
		return (false);  // Instructs the serializer to do not continue.
	default:
		return (false);  // Instructs the serializer to do not continue.
	}
}
