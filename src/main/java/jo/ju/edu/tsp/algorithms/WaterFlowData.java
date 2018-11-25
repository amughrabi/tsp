package jo.ju.edu.tsp.algorithms;

import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;

/**
 * Created by sondo on 24/11/2018.
 */
class WaterFlowData implements Comparable<WaterFlowData> {
    private ArrayList<Integer> nodes;
    private double cost;
    private double mass;
    private double velocity;
    private int location;

    public WaterFlowData() {
        this.nodes = new ArrayList<Integer>();
        this.cost = 0;
        location = -1;
    }

    public WaterFlowData(ArrayList<Integer> nodes, double cost, double mass, double velocity) {
        this.nodes = new ArrayList<Integer>(nodes);
        this.cost = cost;
        this.velocity = velocity;
        this.mass = mass;
        this.location = nodes.get(nodes.size() - 1);
    }

    public ArrayList<Integer> getNodes() {
        return nodes;
    }

    public void setNodes(ArrayList<Integer> nodes) {
        this.nodes = nodes;
        this.location = nodes.get(nodes.size() - 1);
    }

    public double getCost() {
        return cost;
    }

    public void setCost(double cost) {
        this.cost = cost;
    }

    public double getMass() {
        return mass;
    }

    public void setMass(double mass) {
        this.mass = mass;
    }

    public double getVelocity() {
        return velocity;
    }

    public void setVelocity(double velocity) {
        this.velocity = velocity;
    }

    public void addCost(double cost) {
        this.cost += cost;
    }

    public void addNodeToFlow(int node) {
        this.nodes.add(node);
        this.location = node;
    }

    public int compareTo(@NotNull WaterFlowData o) {
        return Double.compare(this.cost, o.cost);
    }

    public int getLocation() {
        return location;
    }

    public void setLocation(int location) {
        this.location = location;
    }

    @Override
    public boolean equals(Object obj) {
        return obj instanceof WaterFlowData && this.location == ((WaterFlowData) obj).location && this.nodes.size() == ((WaterFlowData) obj)
                .getNodes().size();

    }
}