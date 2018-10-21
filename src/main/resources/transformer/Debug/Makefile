################################################################################
# Makefile for the TSPLIB 2013 project.                                        #
#                                                                              #
# Authors:                                                                     #
#  - Ulrich Pferschy and                                                       #
#  - Rostislav Stanek                                                          #
#                                                                              #
#    Institut fuer Statistik und Operations Research,                          #
#    Karl-Franzens-Universitaet Graz                                           #
#    Universitaetsstraße 15, Bauteil E/III,                                    #
#    A-8010, Graz                                                              #
#    Republik Oesterreich                                                      #
#                                                                              #
#                                                                              #
# Testet with gcc version 4.6.3 (Ubuntu/Linaro 4.6.3-1ubuntu5) for             #
# x86_64-linux-gnu.                                                            #
#                                                                              #
# Recommended tools and libraries:                                             #
#  - Xerces-C++ version 3.1.1.                                                 #
#                                                                              #
#                                                                              #
# Just type "make" to compile this project. If you want to use some compiler   #
# options, use the directive "options" - e.g. "make options=-O3". Maybe it     #
# will be necessary to update the Xerces includes and library links.           #
#                                                                              #
################################################################################

#@file Makefile
#Makefile for the TSPLIB 2013 project.
#
#@brief Makefile for the TSPLIB 2013 project.
#
#@author Ulrich Pferschy and Rostislav Stanek
#(Institut fuer Statistik und Operations Research, Karl-Franzens-Universitaet Graz)

ifdef options
CC=g++ -ansi -pedantic -pedantic-errors -std=c++0x -Wall -Wextra -Werror -Wconversion $(options)
else
CC=g++ -ansi -pedantic -pedantic-errors -std=c++0x -Wall -Wextra -Werror -Wconversion
endif

VPATH=../src

all: TransformTSPLIBLink ValidateLink
	
TransformTSPLIBLink: TransformTSPLIB
	$(CC) -o TransformTSPLIB TransformTSPLIB.o TransformConstantsClassesAndFunctions.o TransformDOMErrorHandler.o -lxerces-c -lpthread
	
ValidateLink: Validate
	$(CC) -o Validate Validate.o ValidateIO.o ValidateSAX2ErrorHandler.o ValidateSAX2ContentHandler.o ValidateInstance.o ValidateGraph.o -lxerces-c -lpthread

TransformTSPLIB: TransformTSPLIB.cpp TransformConstantsClassesAndFunctions TransformConstantsClassesAndFunctions.hpp
	$(CC) -c -o TransformTSPLIB.o $(CPPFLAGS)$<
	
Validate:  Validate.cpp ValidateIO ValidateInstance ValidateConstantsFunctionsAndClasses.hpp ValidateIO.hpp ValidateInstance.hpp
	$(CC) -c -o Validate.o $(CPPFLAGS)$<
	
TransformConstantsClassesAndFunctions: TransformConstantsClassesAndFunctions.cpp TransformDOMErrorHandler TransformConstantsClassesAndFunctions.hpp TransformDOMErrorHandler.hpp
	$(CC) -c -o TransformConstantsClassesAndFunctions.o $(CPPFLAGS)$<
	
TransformDOMErrorHandler: TransformDOMErrorHandler.cpp TransformDOMErrorHandler.hpp
	$(CC) -c -o TransformDOMErrorHandler.o $(CPPFLAGS)$<
	
ValidateIO: ValidateIO.cpp ValidateInstance ValidateGraph ValidateSAX2ErrorHandler ValidateSAX2ContentHandler ValidateIO.hpp ValidateConstantsFunctionsAndClasses.hpp ValidateInstance.hpp ValidateGraph.hpp ValidateSAX2ErrorHandler.hpp ValidateSAX2ContentHandler.hpp
	$(CC) -c -o ValidateIO.o $(CPPFLAGS)$<
		
ValidateSAX2ErrorHandler: ValidateSAX2ErrorHandler.cpp ValidateSAX2ErrorHandler.hpp
	$(CC) -c -o ValidateSAX2ErrorHandler.o $(CPPFLAGS)$<
	
ValidateSAX2ContentHandler: ValidateSAX2ContentHandler.cpp ValidateSAX2ContentHandler.hpp ValidateConstantsFunctionsAndClasses.hpp
	$(CC) -c -o ValidateSAX2ContentHandler.o $(CPPFLAGS)$<

ValidateInstance: ValidateInstance.cpp ValidateGraph ValidateInstance.hpp ValidateGraph.hpp
	$(CC) -c -o ValidateInstance.o $(CPPFLAGS)$<

ValidateGraph: ValidateGraph.cpp ValidateGraph.hpp
	$(CC) -c -o ValidateGraph.o $(CPPFLAGS)$<
	

clean:
	rm -f TransformTSPLIB Validate *.o
#End
