package jo.ju.edu.tsp.core;

import java.util.ArrayList;
import java.util.List;

public class Graph {
    private int numberOfVertices;
    private List<List<Vertex>> graph;
    private String name;
    
    public Graph(int numberOfVertices, String name) {
        this.numberOfVertices = numberOfVertices;
        this.name = name;
        graph = new ArrayList<List<Vertex>>(numberOfVertices);
        for(int i = 0; i < numberOfVertices; i++) {
            // initialize all items with empty lists.
            graph.add(i, new ArrayList<Vertex>());
        }
    }

    public void put(int idx, Vertex vertex) {
        if(idx >= numberOfVertices) return; // false information

        if(graph.get(idx) == null) {
            graph.add(idx, new ArrayList<Vertex>());
        }

        graph.get(idx).add(vertex);
    }

    public List<Vertex> adjacentOf(int idx) {
        List<Vertex> vertices = new ArrayList<Vertex>();
        if(idx < numberOfVertices) {
            vertices = graph.get(idx); // never return empty
        }
        return vertices;
    }

    public Vertex getVertex(int from, int to) {
        List<Vertex> vertices = graph.get(from);
        if(vertices != null && !vertices.isEmpty()) {
            for(Vertex v : vertices) {
                if(v.getId() == to) return v;
            }
        }
        return null;
    }

    public int getNumberOfVertices() {
        return numberOfVertices;
    }

    public List<List<Vertex>> getGraph() {
        return graph;
    }

    public String getName() {
        return name;
    }
}
