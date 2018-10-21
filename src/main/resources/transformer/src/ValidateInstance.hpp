/**
 * @file ValidateInstance.hpp
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

#ifndef _ValidateInstance_HPP_
#define _ValidateInstance_HPP_

#include <ios>
#include <sstream>
#include <string>

#include "ValidateGraph.hpp"


/**
 * Saves one instance of the orienteering problem with the graph in the
 * form of a weighted complete graph.
 * This class provides no checks of ranges.
 */
class Instance {
private:
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
	 * The underlying weighted complete graph.
	 */
	Graph *graph;

	/**
	 * Initializes the class variables
	 * @param name Name of the instance.
	 * @param source Source of the instance.
	 * @param description Description of the instance.
	 * @param doublePrecision Precision of doubles.
	 * @param ignoredDigits Number of ignored digits of
	 *   double types. (The deviation of double values
	 *   can be at most 1e-(DoublePrecision - IgnoredDigits).)
	 * @param graph The underlying weighted complete graph.
	 */
	void init(
			const std::string &name,
			const std::string &source,
			const std::string &description,
			const std::streamsize doublePrecision,
			const std::streamsize ignoredDigits,
			const Graph &graph);

public:
	/**
	 * Constructor for the class Instance.
	 * @param name Name of the instance.
	 * @param source Source of the instance.
	 * @param description Description of the instance.
	 * @param doublePrecision Precision of doubles.
	 * @param ignoredDigits Number of ignored digits of
	 *   double types. (The deviation of double values
	 *   can be at most 1e-(DoublePrecision - IgnoredDigits).)
	 * @param graph The underlying weighted complete graph.
	 */
	Instance(
			const std::string &name,
			const std::string &source,
			const std::string &description,
			const std::streamsize doublePrecision,
			const std::streamsize ignoredDigits,
			const Graph &graph);

	/**
	 * Copy constructor for the class Instance.
	 * @param instance Instance to be copied.
	 */
	Instance(const Instance &instance);

	/**
	 * Destructor for the class Instance.
	 */
	~Instance();

	/**
	 * Implements the operator "=".
	 * @param instance Right side of the operator.
	 * @return Left side of the operator.
	 */
	Instance &operator=(const Instance &instance);

	/**
	 * Implements the operator "<<".
	 * @param os Ostream.
	 * @param instance Instance.
	 * @return Ostream with the information about the instance instance.
	 */
	friend std::ostream &operator<<(std::ostream &os, Instance &instance);

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
	 * Returns the underlying weighted undirected complete graph.
	 * @return Underlying weighted undirected complete graph.
	 */
	inline Graph *getGraph() const {
		return (graph);
	}
};


#endif
