/**
 * @file ValidateGraph.hpp
 * Defines the class Graph which saves one weighted complete graph.
 * The defined class provides no checks of ranges.
 *
 * @brief Defines the class Graph.
 *
 * @author Ulrich Pferschy and Rostislav Stanek
 * (Institut fuer Statistik und Operations Research, Universitaet Graz)
 */

#ifndef _ValidateGraph_HPP_
#define _ValidateGraph_HPP_

#include <vector>
#include <sstream>


/**
 * Exception class used if the graph is not valid.
 */
class GraphNotValid {
public:
	/**
	 * Constructor for the class GraphNotValid.
	 */
	inline GraphNotValid() {
	}
};

/**
 * Saves one weighted complete graph.
 * This class provides no checks of ranges.
 */
class Graph {
private:
	/**
	 * Indicator indicating if the graph is undirected.
	 * <ul>
	 *   <li>true The Graph is undirected.</li>
	 *   <li>false The Graph is directed.</li>
	 * </ul>
	 */
	bool isUndirected;

	/**
	 * Number of vertices in the graph of the instance.
	 */
	std::vector<std::vector<double> >::size_type n;

	/**
	 * Adjacency matrix of a weighted undirected complete graph.
	 * The elements in the matrix are the costs. The elements
	 * in the matrix are the costs. The matrix can be either only
	 * a lower triangular matrix for undirected graphs or it can
	 * be a square matrix for the directed graph.
	 */
	std::vector<std::vector<double> > adjacencyMatrix;

protected:
	/**
	 * Sets one element in the adjacency matrix. The parameters are not checked.
	 * @param i Row.
	 * @param j Column.
	 * @param value New value of the element on the i-th row and j-th column.
	 */
	inline void setAdjacencyMatrixElement(
			const std::vector<std::vector<double> >::size_type i,
			const std::vector<double>::size_type j,
			const double value) {
		adjacencyMatrix.at(i).at(j) = value;
	}

	/**
	 * Returns one element in the adjacency matrix. The parameters are not checked.
	 * @param i Row.
	 * @param j Column.
	 * @return Value of the element on the i-th row and j-th column.
	 */
	inline double getAdjacencyMatrixElement(
			const std::vector<std::vector<double> >::size_type i,
			const std::vector<double>::size_type j) const {
		return (adjacencyMatrix.at(i).at(j));
	}

public:
	/**
	 * Constructor for the class Graph.
	 * @param adjacencyMatrix Adjacency matrix of a weighted undirected
	 * complete graph. The elements in the matrix are the costs.
	 * The matrix can be either only a lower triangular matrix for
	 * undirected graphs or it can be a square matrix for the directed
	 * graph. In both cases on the main diagonale there must be 0.
	 */
	Graph(const std::vector<std::vector<double> > &adjacencyMatrix);

	/**
	 * Copy constructor for the class Graph.
	 * @param graph Instance to be copied.
	 */
	Graph(const Graph &graph);

	/**
	 * Implements the operator "=".
	 * @param graph Right side of the operator.
	 * @return Left side of the operator.
	 */
	Graph &operator=(const Graph &graph);

	/**
	 * Implements the operator "<<".
	 * @param os Ostream.
	 * @param graph Graph.
	 * @return Ostream with the information about the graph instance.
	 */
	friend std::ostream &operator<<(std::ostream &os, Graph &graph);

	/**
	 * Returns the indicator indicating if the graph is undirected.
	 * @return Indicator indicating if the graph is undirected.
	 *   <ul>
	 *     <li>true The Graph is undirected.</li>
	 *     <li>false The Graph is directed.</li>
	 *   </ul>
	 */
	inline bool getIsUndirected() const {
		return (isUndirected);
	}

	/**
	 * Returns the number of vertices in the graph of the instance.
	 * @return Number of vertices in the graph of the instance.
	 */
	inline std::vector<std::vector<double> >::size_type getN() const {
		return (n);
	}

	/**
	 * Returns the cost of the edge from i to j. The parameters are not checked.
	 * @param i Start vertex.
	 * @param j End vertex.
	 * @return Weight of the edge from i to j.
	 */
	inline double getEdgeCost(
			const std::vector<std::vector<double> >::size_type i,
			const std::vector<double>::size_type j) const {
		if (isUndirected) {
			if (i < j) {
				return (getAdjacencyMatrixElement(j, i));
			}
			else {
				return (getAdjacencyMatrixElement(i, j));
			}
		}
		else {
			return (getAdjacencyMatrixElement(i, j));
		}
	}
};


#endif
