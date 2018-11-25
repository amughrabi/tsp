package jo.ju.edu.tsp.algorithms;

import jo.ju.edu.tsp.core.Graph;
import jo.ju.edu.tsp.core.Vertex;
import jo.ju.edu.tsp.core.xml.Transformer;
import org.jetbrains.annotations.NotNull;
import org.xml.sax.SAXException;

import javax.xml.parsers.ParserConfigurationException;
import java.io.IOException;
import java.util.*;

public class WFA extends TSP {
    private double G; //Maximum generation G
    private double[] W; //initial mass W0
    private double[] V; //initial velocity V0
    private double T; //the base momentum T for splitting // momentum : 8wwet el dafe3
    private double un; //the upper bound of subflows spited from single flow
    private double g; //The gravity
    private double t; //periodical precipitation generations t : 3dd dwrat trassob el meyah G/20
    private double numberOfSubFlows; // could be 2 or 3
    private double[] n; // number of subflows subflows
    private boolean stopCreation;
    private ArrayList<Integer> initialPath;
    private ArrayList<Integer> bestPath;
    private double bestCost;
    private Random randomGenerator;
    private ArrayList<WaterFlowData> waterFlows;

    public @NotNull Graph solve(@NotNull Graph graph) {
        Transformer.print(graph);
        initializingStep(graph);
        bestCost = generateInitialSolutionUsingNN(graph);
        applyOPT2VersionTwo(initialPath, graph, bestCost);

        bestCost += graph.getGraph().get(initialPath.get(initialPath.size() - 1)).get(initialPath.get(0)).getCost();
        initialPath.add(initialPath.get(0));
        while (!stopCreation) {
            // calculate number of subflows
            n[0] = calculateSubFlowsInIteration(0);
            for (int i = 0; i < G; i++) {
                WaterFlowData mainFlow = new WaterFlowData();
                mainFlow.addCost(0);
                mainFlow.addNodeToFlow(i);
                mainFlow.setMass(W[0]);
                mainFlow.setVelocity(V[0]);
                waterFlows.add(mainFlow);

                generateSubFlows(graph, mainFlow, i);
            }

            printTour(bestPath);
            System.out.println("COST: " + bestCost);
            stopCreation = true;
        }

        return graph;
    }

    private void generateSubFlows(Graph graph, WaterFlowData subMainFlow, int i) {
        double n = Math.ceil(calculateSubFlowsInIteration(subMainFlow.getMass(), subMainFlow.getVelocity()));
        double Wik[] = new double[(int) n];
        double Vik[] = new double[(int) n];

        ArrayList<Vertex> bestNodes = new ArrayList<Vertex>(graph.getGraph().get(subMainFlow.getNodes().get(subMainFlow.getNodes().size() - 1)));
        Collections.sort(bestNodes);

        for (Integer v : subMainFlow.getNodes()) {
            bestNodes.remove(new Vertex(v, 0));
        }

        if (bestNodes.isEmpty())
            return;
        for (int k = 0; k < n; k++) {
            Wik[k] = calculateMassOfSubflowK(i, k, subMainFlow.getMass(), n);
            Vik[k] = calculateVelocityOfSubflowK(i, k, subMainFlow.getVelocity());

            WaterFlowData flow = new WaterFlowData(subMainFlow.getNodes(), subMainFlow.getCost(), Wik[k], Vik[k]);

            if (bestNodes.size() - 1 < k || bestNodes.get(k) == null) continue;
            flow.addNodeToFlow(bestNodes.get(k).getId());
            flow.addCost(bestNodes.get(k).getCost());
            waterFlows.add(flow);

            if (flow.getNodes().size() == graph.getNumberOfVertices()) {
                flow.addNodeToFlow(flow.getNodes().get(0));
                flow.addCost(graph.getGraph()
                        .get(flow.getNodes().get(flow.getNodes().size() - 2))
                        .get(flow.getNodes().get(0) > flow.getNodes().get(flow.getNodes().size() - 2) ? flow.getNodes().get(0) - 1 : flow.getNodes().get(0)).getCost());
                bestPath = flow.getCost() < bestCost ? flow.getNodes() : initialPath;
                bestCost = flow.getCost() < bestCost ? flow.getCost() : bestCost;

            }
            generateSubFlows(graph, flow, k);

        }


    }

    private void initializingStep(Graph graph) {
        G = graph.getNumberOfVertices();
        W = new double[graph.getNumberOfVertices()];
        V = new double[graph.getNumberOfVertices()];
        n = new double[graph.getNumberOfVertices()];
        W[0] = 8;
        V[0] = 5;
        T = 20;
        un = 3;
        randomGenerator = new Random();
        stopCreation = false;
        waterFlows = new ArrayList<WaterFlowData>();
        bestPath = new ArrayList<Integer>();
        bestCost = 0;
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

    private double calculateSubFlowsInIteration(double mass, double velocity) {
        return Math.min(Math.max(1, ((double) (mass * velocity / T))), un);
    }

    private double calculateMassOfSubflowK(int i, double k) {
        return ((n[i] + 1 - k) / sumOfR(i)) * W[i];
    }

    private double calculateMassOfSubflowK(int i, double k, double mass, double numberOfSubFlows) {
        return ((numberOfSubFlows + 1 - k) / sumOfR(i)) * mass;
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

    private double calculateVelocityOfSubflowK(int i, double k, double velocity) {
        double temp = (velocity * velocity) + (2 * g * calculateImprovementInTheObjectiveValue(i, k));
        if (temp > 0) {
            return Math.sqrt(temp);
        }
        return 0;

    }

    private double calculateImprovementInTheObjectiveValue(int i, double k) {
        return randomGenerator.nextInt((int) G) - k;
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
        return (W[i] / calculateSumOfW(n[i])) * W[0];
    }

    private double calculateSumOfW(double numberOfFlows) {
        int sum = 0;
        for (int k = 1; k <= numberOfFlows; k++) {
            sum += W[k];
        }
        return sum;
    }

    private void calculateAccumulatedMassOfEvaporatedWater(int i) {
        W[i] = distributedInitialMassValueToI(i) - calculateSumOfW(n[i]);
    }

    private int generateInitialSolutionUsingNN(Graph graph) {
        int roadCost = 0;
        Map<Integer, Boolean> visited;
        Integer initialNode;
        Integer nextNode;
        double minCost;
        int minNode;
        initialPath = new ArrayList<Integer>();

        for (int i = 0; i < graph.getNumberOfVertices(); i++) {
            minCost = Integer.MAX_VALUE;
            minNode = Integer.MAX_VALUE;
            nextNode = i;
            initialNode = i;
            roadCost = 0;
            visited = new HashMap<Integer, Boolean>();
            visited.put(initialNode, true);
            initialPath.add(initialNode);
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
                initialPath.add(minNode);

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

    private void applyOPT2VersionTwo(ArrayList<Integer> initialPath, Graph graph, double initialDist) {
        double bestDist = initialDist;
        double newDist;
        int swaps = 1;
        ArrayList<Integer> newTour;


        while (swaps != 0) { //loop until no improvements are made.
            swaps = 0;
            //initialise inner/outer loops avoiding adjacent calculations and making use of problem symmetry to half total comparisons.

            for (int i = 1; i < initialPath.size() - 2; i++) {
                for (int j = i + 1; j < initialPath.size() - 1; j++) {
                    //check distance of line A,B + line C,D against A,C + B,D if there is improvement, call swap method.
                    if ((graph.getGraph().get(initialPath.get(i)).get(initialPath.get(i - 1) > initialPath.get(i) ? initialPath.get(i - 1) - 1 : initialPath.get(i - 1)).getCost()
                            + graph.getGraph().get(initialPath.get(j + 1)).get(initialPath.get(j) > initialPath.get(j + 1) ? initialPath.get(j) - 1 : initialPath.get(j)).getCost())
                            >= (graph.getGraph().get(initialPath.get(i)).get(initialPath.get(j + 1) > initialPath.get(i) ? initialPath.get(j + 1) - 1 : initialPath.get(j + 1)).getCost()
                            + graph.getGraph().get(initialPath.get(i - 1)).get(initialPath.get(j) > initialPath.get(i - 1) ? initialPath.get(j) - 1 : initialPath.get(j)).getCost())) {


                        Double newTourCost = 0.0;
                        newTour = swap(initialPath, i, j, newTourCost, graph);

                        if (newTourCost < bestDist) { //if the swap results in an improved distance, increment counters and update distance/tour
                            initialPath = newTour;
                            bestDist = newTourCost;
                            swaps++;
                        }
                    }
                }
            }
        }
    }

    private static ArrayList<Integer> swap(ArrayList<Integer> cities, int i, int j, Double newTourDistance, Graph graph) {
        //conducts a 2 opt swap by inverting the order of the points between i and j
        ArrayList<Integer> newTour = new ArrayList<Integer>();
        int size = cities.size();

        for (int c = 0; c <= i - 1; c++) {
            newTour.add(cities.get(c));
        }

        //invert order between 2 passed points i and j and add to newTour
        int dec = 0;
        for (int c = i; c <= j; c++) {
            newTour.add(cities.get(j - dec));
            dec++;
        }

        //append array from point j to end to newTour
        for (int c = j + 1; c < size; c++) {
            newTour.add(cities.get(c));
        }

        for (int c = 0; c < cities.size() - 1; c++) {
            newTourDistance += graph.getGraph().get(cities.get(c)).get(cities.get(c + 1)).getCost();
        }
        newTourDistance += graph.getGraph().get(cities.get(cities.size() - 1)).get(0).getCost();
        return newTour;
    }

    private void printTour(ArrayList<Integer> path) {
        System.out.println("******************************");
        for (Integer node : path) {
            System.out.println(node + 1);
        }
        System.out.println("******************************");
    }

    public static void main(String[] args) {
        WFA wfa = new WFA();
        try {
            wfa.solve();
        } catch (ParserConfigurationException e) {
            e.printStackTrace();
        } catch (SAXException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }
}
