/**
 * @file ValidateIO.cpp
 * Defines the functions and classes necessary for the IO.
 *
 * @brief Defines the functions necessary for the IO.
 *
 * @author Ulrich Pferschy and Rostislav Stanek
 * (Institut fuer Statistik und Operations Research, Universitaet Graz)
 */

#include <iostream>
#include <algorithm>
#include <cmath>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

#include "ValidateConstantsFunctionsAndClasses.hpp"
#include "ValidateSAX2ErrorHandler.hpp"
#include "ValidateSAX2ContentHandler.hpp"

#include "ValidateIO.hpp"

using namespace std;
using namespace xercesc;

XERCES_CPP_NAMESPACE_USE


std::string parseCommandLineArguments(int argc, char* argv[]) {
	/*
	 * Initialling.
	 */
	string inputFileName = "";
	switch (argc) {
	case 1:
	{
		try {
			cout <<
					"The name of input file (the filename extension has to be \"" <<
					INPUT_FILE_FILENAME_EXTENSION.substr(1, INPUT_FILE_FILENAME_EXTENSION.size() - 1) <<
					"\"): ";
			getline(cin, inputFileName);
		}
		catch (ios::failure &e) {
		    throw CommandLineArgumentsInvalid();
	    }
	}
	break;
	case 2:
	{
		inputFileName = argv[1];
	}
	break;
	default:
	{
	    throw CommandLineArgumentsInvalid();
	}
	}

	//Checking of validity of the inputFileName
	if (inputFileName.size() < INPUT_FILE_FILENAME_EXTENSION.size() + 1) {
	    throw CommandLineArgumentsInvalid();
	}

	string inputFileFilenameExtension =
			inputFileName.substr(
					inputFileName.size() - INPUT_FILE_FILENAME_EXTENSION.size(),
					INPUT_FILE_FILENAME_EXTENSION.size());
	transform(
			inputFileFilenameExtension.begin(),
			inputFileFilenameExtension.end(),
			inputFileFilenameExtension.begin(), ::tolower);

	if (inputFileFilenameExtension != INPUT_FILE_FILENAME_EXTENSION) {
	    throw CommandLineArgumentsInvalid();
	}

	return (inputFileName);
}

/**
 * Trancodes char* data to XMLCh data.
 */
class XMLStringTranscode {
private :
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

Instance *instanceIn(const std::string &inputFileName) {
	Instance *instance = 0;

	try {
		XMLPlatformUtils::Initialize();
	}
	catch (const XMLException &e) {
		throw;
	}

	try	{
		//Gets the SAX parser.
		SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();

		parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
		parser->setFeature(XMLUni::fgSAX2CoreNameSpacePrefixes, true);
		parser->setFeature(XMLUni::fgSAX2CoreValidation, true);

		//Validation.
		parser->setFeature(XMLUni::fgXercesSchema, true);
		parser->setFeature(XMLUni::fgXercesSchemaFullChecking, true);
		parser->setFeature(XMLUni::fgXercesValidationErrorAsFatal, true);
		parser->setFeature(XMLUni::fgXercesContinueAfterFatalError, false);

		//Use the loaded grammar during parsing.
		parser->setFeature(XMLUni::fgXercesUseCachedGrammarInParse, true);

		//Don't load schemas from any other source (e.g., from XML document's
		//xsi:schemaLocation attributes).
		parser->setFeature(XMLUni::fgXercesLoadSchema, false);

        if (!parser->loadGrammar (VALIDATION_SCHEMA.c_str(), Grammar::SchemaGrammarType, true)) {
        	throw ValidationSchemaDoesNotExist();
        }

		//ErrorHandler.
	    SAX2ErrorHandler errorHandler;
		parser->setErrorHandler(&errorHandler);

		//DocumentHandler.
		SAX2ContentHandler contentHandler;
	    parser->setContentHandler(&contentHandler);

	    parser->parse(inputFileName.c_str());

	    if (errorHandler.getFailed()) {
	    	throw ValidationFailed();
	    }

	    if (contentHandler.getFailed()) {
	    	throw ValidationFailed();
	    }

	    instance = new Instance(
	    		contentHandler.getName(),
	    		contentHandler.getSource(),
	    		contentHandler.getDescription(),
	    		contentHandler.getDoublePrecision(),
	    		contentHandler.getIgnoredDigits(),
	    		Graph(contentHandler.getAdjacencyMatrix()));
	}
	catch (bad_alloc &e) {
		if (instance != 0) {
			delete instance;
		}
		XMLPlatformUtils::Terminate();
		throw;
	}
    catch (OutOfMemoryException &e) {
		XMLPlatformUtils::Terminate();
		throw;
    }
    catch (XMLException &e) {
		XMLPlatformUtils::Terminate();
		throw;
    }
    catch (ValidationSchemaDoesNotExist &e) {
		XMLPlatformUtils::Terminate();
		throw;
    }
    catch (ValidationFailed &e) {
		XMLPlatformUtils::Terminate();
		throw;
    }
/*	catch ( ... ) {
		if (instance != 0) {
			delete instance;
		}
		XMLPlatformUtils::Terminate();
		throw;
	}*/

	XMLPlatformUtils::Terminate();

	return (instance);
}
