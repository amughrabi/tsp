package jo.ju.edu.tsp.core;

public class Vertex {
    private int id;
    private double cost;

    public Vertex() {
    }

    public Vertex(int id, double cost) {
        this.id = id;
        this.cost = cost;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public double getCost() {
        return cost;
    }

    public void setCost(double cost) {
        this.cost = cost;
    }
}