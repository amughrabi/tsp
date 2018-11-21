package jo.ju.edu.tsp.algorithms;

import jo.ju.edu.tsp.core.Graph;
import jo.ju.edu.tsp.core.Vertex;
import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by sondo on 19/11/2018.
 */
public class WFA extends TSP {

    private double G; //Maximum generation G
    private double[] flow; //initial flow solution flowi,
    private double[] W; //initial mass W0
    private double[] V; //initial velocity V0
    private double T; //the base momentum T for splitting // momentum : 8wwet el dafe3
    private double un; //the upper bound of subflows spited from single flow
    private double g; //The gravity
    private double t; //periodical precipitation generations t : 3dd dwrat trassob el meyah G/20
    private double numberOfSubFlows; // could be 2 or 3
    private double N; //number of water flows in the current iteration
    private double[] n; //subflows
    private double W0;
    private double V0;
    private boolean stopCreation;


    public @NotNull Graph solve(@NotNull Graph graph) {
        step1(graph);
        while (!stopCreation) {
            // calculate number of subflows
            List<Vertex> rootGraph = graph.getGraph().get(0);
            for(int i = 0 ; i < rootGraph.size(); i++) {

            }
        }
        return new Graph(5, "test");
    }


    private void step1(Graph graph) {
        int roadCost = generateInitialSolutionUsingNN(graph);
        G = 0;
        W0 = 0;
        V0 = 0;
        stopCreation = false;
    }

    private void flowSplittingAndMovingStep() {

    }

    private void flowMergingStep() {

    }

    private void waterEvaporationStep() {

    }

    private void precipitationStep() {

    }

    private void bestSolutionUpdateStep() {

    }

    private boolean isRainFallRequired() {
        return false;
    }


    private double valueOfM0V0() {
        return 2 * T - 3 * T;
    }

    private double calculateSubFlowsInIteration(int i) {
        return Math.min(Math.max(1, ((double) (W[i] * V[i] / T))), un);
    }

    private double calculateMassOfSubflowK(int i, double k) {
        return ((n[i] + 1 - k) / sumOfR(i)) * W[i];
    }

    private double sumOfR(int i) {
        double sum = 0;
        for (double r = 1; r <= n[i]; r++) {
            sum += r;
        }
        return sum;
    }

    private double calculateVelocityOfSubflowK(int i, double k) {
        double temp = (V[i] * V[i]) + (2 * g * calculateImprovementInTheObjectiveValue(i, k));
        if (temp > 0) {
            return Math.sqrt(temp);
        }
        return 0;

    }

    private double calculateImprovementInTheObjectiveValue(int i, double k) {
        return 5;
    }


    private void recalculateMassOfI(int i, int j) {
        W[i] = W[i] + W[j];
    }

    private void recalculateVelocityOfI(int i, int j) {
        V[i] = (W[i] * V[i] + W[j] * V[j]) / (W[i] + W[j]);
    }

    private void updateWaterMassAtI(int i) {
        W[i] = (1 - 1.0 / t) * W[i];
    }

    private double distributedInitialMassValueToI(int i) {
        return (W[i] / calculateSumOfW()) * W[0];
    }

    private double calculateSumOfW() {
        int sum = 0;
        for (int k = 1; k <= N; k++) {
            sum += W[k];
        }
        return sum;
    }

    private void calculateAccumulatedMassOfEvaporatedWater(int i) {
        W[i] = distributedInitialMassValueToI(i) - calculateSumOfW();
    }


    private int generateInitialSolutionUsingNN(Graph graph) {
        int roadCost = 0;
        Map<Integer, Boolean> visited;
        Integer initialNode;
        Integer nextNode;
        double minCost;
        int minNode;
        ArrayList<Integer> path = new ArrayList<Integer>();

        for (int i = 0; i < graph.getNumberOfVertices(); i++) {
            minCost = Integer.MAX_VALUE;
            minNode = Integer.MAX_VALUE;
            nextNode = i;
            initialNode = i;
            roadCost = 0;
            visited = new HashMap<Integer, Boolean>();
            visited.put(initialNode, true);
            path.add(initialNode);
            while (nextNode != -1) {
                minCost = Integer.MAX_VALUE;
                minNode = Integer.MAX_VALUE;
                for (Vertex vertex : graph.getGraph().get(nextNode)) {
                    if (minCost > vertex.getCost() && visited.get(vertex.getId()) == null) {
                        minNode = vertex.getId();
                        minCost = vertex.getCost();
                    }
                }
                roadCost += minCost;

                if (visited.get(minNode) != null) {
                    break;
                }
                nextNode = minNode;
                visited.put(minNode, true);
                path.add(minNode);

                if (visited.size() == graph.getNumberOfVertices()) {
                    break;
                }

            }
            if (visited.size() == graph.getNumberOfVertices()) {
                Vertex backNode = graph.getGraph().get(minNode).get(initialNode);
                if (backNode != null) {
                    roadCost += backNode.getCost();
                    break;
                }
            }

        }

        return roadCost;
    }
}
