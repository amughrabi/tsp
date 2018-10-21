/**
 * @file ValidateGraph.cpp
 * Defines the class Graph which saves one weighted complete graph.
 * The defined class provides no checks of ranges.
 *
 * @brief Defines the class Graph.
 *
 * @author Ulrich Pferschy and Rostislav Stanek
 * (Institut fuer Statistik und Operations Research, Universitaet Graz)
 */

#include "ValidateGraph.hpp"

using namespace std;


Graph::Graph(const std::vector<std::vector<double> > &adjacencyMatrix) {
	if (adjacencyMatrix.size() < 1) {
		throw GraphNotValid();
	}

	this->adjacencyMatrix = adjacencyMatrix;
	n = adjacencyMatrix.size();

	if ((adjacencyMatrix.at(0).size() == 1) || (adjacencyMatrix.at(0).size() < n)) {
		isUndirected = true;

		//size check
		for (vector<vector<double> >::size_type i = 0; i < n; i++) {
			if (adjacencyMatrix.at(i).size() != i + 1) {
				throw GraphNotValid();
			}
		}
	}
	else {
		isUndirected = false;

		//size check
		for (vector<vector<double> >::size_type i = 0; i < n; i++) {
			if (adjacencyMatrix.at(i).size() != n) {
				throw GraphNotValid();
			}
		}
	}

}

Graph::Graph(const Graph &graph) {
	isUndirected = graph.getIsUndirected();

	n = graph.getN();

	adjacencyMatrix.resize(n);

	if (isUndirected) {
		for (vector<vector<double> >::size_type i = 0; i < n; i++) {
			adjacencyMatrix.at(i).resize(i + 1);
			for (
					vector<double>::size_type j = 0;
					j <= static_cast<vector<double>::size_type>(i);
					j++) {
				setAdjacencyMatrixElement(i, j, graph.getAdjacencyMatrixElement(i, j));
			}
		}
	}
	else {
		for (vector<vector<double> >::size_type i = 0; i < n; i++) {
			adjacencyMatrix.at(i).resize(n);
			for (
					vector<double>::size_type j = 0;
					j < static_cast<vector<double>::size_type>(n);
					j++) {
				setAdjacencyMatrixElement(i, j, graph.getAdjacencyMatrixElement(i, j));
			}
		}
	}
}

Graph &Graph::operator=(const Graph &graph) {
	if (this == &graph) {
		return (*this);
	}
	else {
		isUndirected = graph.getIsUndirected();

		n = graph.getN();

		adjacencyMatrix.resize(n);

		if (isUndirected) {
			for (vector<vector<double> >::size_type i = 0; i < n; i++) {
				adjacencyMatrix.at(i).resize(i + 1);
				for (
						vector<double>::size_type j = 0;
						j <= static_cast<vector<double>::size_type>(i);
						j++) {
					setAdjacencyMatrixElement(i, j, graph.getAdjacencyMatrixElement(i, j));
				}
			}
		}
		else {
			for (vector<vector<double> >::size_type i = 0; i < n; i++) {
				adjacencyMatrix.at(i).resize(n);
				for (
						vector<double>::size_type j = 0;
						j < static_cast<vector<double>::size_type>(n);
						j++) {
					setAdjacencyMatrixElement(i, j, graph.getAdjacencyMatrixElement(i, j));
				}
			}
		}

		return (*this);
	}
}

std::ostream &operator<<(std::ostream &os, Graph &graph) {
	os << "<";
	os << "isUndirected: " << (graph.getIsUndirected() ? "true" : "false") << endl;
	os << "n: " << graph.getN() << endl;
	os << "adjacencyMatrix: ";
	if (graph.getIsUndirected()) {
		for (vector<vector<double> >::size_type i = 0; i < graph.getN(); i++) {
			string s = "[";
			for (
					vector<double>::size_type j = 0;
					j <= static_cast<vector<double>::size_type>(i);
					j++) {
				ostringstream costOStringstream;
				costOStringstream << graph.getEdgeCost(i, j);
				s.append(costOStringstream.str());
				if (j < static_cast<vector<double>::size_type>(i)) {
					s.append(", ");
				}
				else {
					s.append("]");
				}
			}
			os << s;
			if (i < graph.getN() - 1) {
				os << endl;
			}
		}
	}
	else {
		for (vector<vector<double> >::size_type i = 0; i < graph.getN(); i++) {
			string s = "[";
			for (
					vector<double>::size_type j = 0;
					j < static_cast<vector<double>::size_type>(graph.getN());
					j++) {
				ostringstream costOStringstream;
				costOStringstream << graph.getEdgeCost(i, j);
				s.append(costOStringstream.str());
				if (j < static_cast<vector<double>::size_type>(graph.getN() - 1)) {
					s.append(", ");
				}
				else {
					s.append("]");
				}
			}
			os << s;
			if (i < graph.getN() - 1) {
				os << endl;
			}
		}
	}
	os << ">" << endl;
	return (os);
}
