/**
 * @file ValidateInstance.cpp
 * Defines the class Instance which saves one instance of the
 * travelling salesman problem with the underlying graph in
 * the form of a weighted complete graph.
 * The defined class provides no checks of ranges.
 *
 * @brief Defines the class Instance.
 *
 * @author Ulrich Pferschy and Rostislav Stanek
 * (Institut fuer Statistik und Operations Research, Universitaet Graz)
 */

#include <cmath>

#include "ValidateInstance.hpp"

using namespace std;


void Instance::init(
		const std::string &name,
		const std::string &source,
		const std::string &description,
		const std::streamsize doublePrecision,
		const std::streamsize ignoredDigits,
		const Graph &graph) {
	this->name = name;
	this->source = source;
	this->description = description;
	this->doublePrecision = doublePrecision;
	this->ignoredDigits = ignoredDigits;
	this->doubleZero = pow(10, -1.0 * static_cast<double>(doublePrecision - ignoredDigits));
	this->graph = new Graph(graph);
}

Instance::Instance(
		const std::string &name,
		const std::string &source,
		const std::string &description,
		const std::streamsize doublePrecision,
		const std::streamsize ignoredDigits,
		const Graph &graph) {
	this->graph = 0;
	try {
		init(name, source, description, doublePrecision, ignoredDigits, graph);
	}
	catch (bad_alloc &e) {
		if (this->graph != 0) {
			delete this->graph;
		}
		throw;
	}
}

Instance::Instance(const Instance &instance) {
	graph = 0;
	try {
		init(
				instance.getName(),
				instance.getSource(),
				instance.getDescription(),
				instance.getDoublePrecision(),
				instance.getIgnoredDigits(),
				*instance.getGraph());
	}
	catch (bad_alloc &e) {
		if (graph != 0) {
			delete graph;
		}
		throw;
	}
}

Instance::~Instance() {
	if (graph != 0) {
		delete graph;
	}
}

Instance &Instance::operator=(const Instance &instance) {
	if (this == &instance) {
		return (*this);
	}
	else {
		graph = 0;
		try {
			init(
					instance.getName(),
					instance.getSource(),
					instance.getDescription(),
					instance.getDoublePrecision(),
					instance.getIgnoredDigits(),
					*instance.getGraph());
		}
		catch (bad_alloc &e) {
			if (graph != 0) {
				delete graph;
			}
			throw;
		}

		return (*this);
	}
}

std::ostream &operator<<(std::ostream &os, Instance &instance) {
	os << "<";
	os << "name: " << instance.getName() << endl;
	os << "source: " << instance.getSource() << endl;
	os << "description: " << instance.getDescription() << endl;
	os << "doublePrecision: " << instance.getDoublePrecision() << endl;
	os << "ignoredDigits: " << instance.getIgnoredDigits() << endl;
	os << "doubleZero: " << instance.getDoubleZero() << endl;
	os << "graph: " << *instance.getGraph();
	os << ">" << endl;

	return (os);
}
