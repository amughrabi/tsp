package jo.ju.edu.tsp.core;

import org.jetbrains.annotations.NotNull;

import java.util.HashMap;

public class Vertex {
    private int id;
    private double cost;
    private HashMap<String, Double> characteristics;

    public Vertex() {
        characteristics = new HashMap<String, Double>();
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

    /*
    * Usually there is a need to have extra characteristics on the edges (which is represented as vertex in the current
    * implementation). If the characteristic is in the characteristics, the value will be overridden.
    * */
    public void addCharacteristic(@NotNull String name, Double value) {
        characteristics.put(name, value);
    }

    public Double getCharacteristic(@NotNull String name) {
        return characteristics.get(name);
    }

    public HashMap<String, Double> getCharacteristics() {
        return characteristics;
    }
}