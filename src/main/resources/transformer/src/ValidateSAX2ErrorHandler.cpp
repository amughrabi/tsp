/**
 * @file ValidateSAX2ErrorHandler.cpp
 * Defines the ErrorHandler for the SAX2 parser.
 *
 * @brief Defines the ErrorHandler for the SAX2 parser.
 *
 * @author Ulrich Pferschy and Rostislav Stanek
 * (Institut fuer Statistik und Operations Research, Universitaet Graz)
 */

#include "ValidateSAX2ErrorHandler.hpp"


SAX2ErrorHandler::SAX2ErrorHandler(): ErrorHandler() {
	failed = false;
}

SAX2ErrorHandler::~SAX2ErrorHandler() {
}

void SAX2ErrorHandler::warning(const SAXParseException &) {
	failed = true;
}

void SAX2ErrorHandler::error(const SAXParseException &) {
	failed = true;
}

void SAX2ErrorHandler::fatalError(const SAXParseException &) {
	failed = true;
}

void SAX2ErrorHandler::resetErrors() {
	failed = false;
}
