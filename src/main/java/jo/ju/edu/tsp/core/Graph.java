package jo.ju.edu.tsp.core;

import java.util.ArrayList;
import java.util.List;

public class Graph {
    private int size;
    private List<List<Vertex>> graph;
    private String name;
    
    public Graph(int size, String name) {
        this.size = size;
        this.name = name;
        graph = new ArrayList<List<Vertex>>(size);
        for(int i = 0; i < size; i++) {
            // initialize all items with empty lists.
            graph.add(i, new ArrayList<Vertex>());
        }
    }

    public void put(int idx, Vertex vertex) {
        if(idx >= size) return; // false information

        if(graph.get(idx) == null) {
            graph.add(idx, new ArrayList<Vertex>());
        }

        graph.get(idx).add(vertex);
    }

    public List<Vertex> adjacentOf(int idx) {
        List<Vertex> vertices = new ArrayList<Vertex>();
        if(idx < size) {
            vertices = graph.get(idx); // never return empty
        }
        return vertices;
    }

    public int getSize() {
        return size;
    }

    public List<List<Vertex>> getGraph() {
        return graph;
    }

    public String getName() {
        return name;
    }
}
