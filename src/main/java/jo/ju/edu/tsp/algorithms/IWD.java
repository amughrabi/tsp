package jo.ju.edu.tsp.algorithms;

import jo.ju.edu.tsp.core.Graph;
import jo.ju.edu.tsp.core.Vertex;
import jo.ju.edu.tsp.set.SetInstance;
import org.xml.sax.SAXException;

import javax.xml.parsers.ParserConfigurationException;
import java.io.IOException;
import java.util.*;

public class IWD extends TSP {
    private final double EPSILON = 0.01;

    public Graph solve(Graph graph) {

        // A graph of water drops solutions, each index represent a water drop.
        Graph solutions;
        // Number of water drops equals to number of vertices
        List<WaterDrop> waterDrops = getWDs(graph);
        WaterDrop waterDrop;
        double bestTourCost = 0;
        initParams(graph);

        for (int iteration = 0; iteration < 1000; iteration++) {
            solutions = new Graph(graph.getNumberOfVertices(), "Solutions");

            for (int k = 0; k < waterDrops.size() * waterDrops.size(); k++) {
                double cost;
                Vertex nextNode = null;
                waterDrop = waterDrops.get(k % waterDrops.size());

                int waterDropCurrentVertexId = waterDrop.getCurrentVertexId();
                List<Vertex> adjVertices = graph.adjacentOf(waterDropCurrentVertexId);
                double fSoilSummation = getFSoilSummation(adjVertices, waterDrop);
                double prob = 0;
                for (Vertex vertex : adjVertices) {
                    if (!waterDrop.isVisited(vertex.getId())) {
                        double maxProb = fSoil(vertex, adjVertices, waterDrop) / fSoilSummation;
                        if (maxProb >= prob) {
                            prob = maxProb;
                            nextNode = vertex;
                        }
                    }
                }
                if (nextNode == null) break;

                waterDrop.markAsVisited(nextNode.getId());

                cost = nextNode.getCost() + waterDrop.getSolutionQuality();

                updateVelocity(waterDrop, nextNode.getCharacteristic("soil"));

                updateSoil(waterDrop, nextNode, graph);

                waterDrop.setCurrentVertexId(nextNode.getId());

                waterDrop.setSolutionQuality(cost);

                solutions.put(k % waterDrops.size(), nextNode);
            }
            for (WaterDrop waterDrop1 :
                    waterDrops) {
                double cost = graph.getVertex(waterDrop1.getFirstVertexId(), waterDrop1.getCurrentVertexId()).getCost();
                waterDrop1.setSolutionQuality(waterDrop1.getSolutionQuality() + cost);

            }
            WaterDrop bestWaterDrop = getBestWaterDrop(waterDrops);
            if (bestTourCost == 0 || bestTourCost >= bestWaterDrop.getSolutionQuality()) {
                bestTourCost = bestWaterDrop.getSolutionQuality();
            }            List<Vertex> vertices = solutions.adjacentOf(bestWaterDrop.getWaterDropId());

            if (bestTourCost != 0 && bestTourCost <= bestWaterDrop.getSolutionQuality()) {
                waterDrops = getWDs(graph);

                continue;
            }

            initParams(graph, vertices, bestWaterDrop);

            waterDrops = getWDs(graph);

        }
        System.out.println("cost" + bestTourCost);

        return new Graph(0, "");
    }

    private WaterDrop getBestWaterDrop(List<WaterDrop> waterDrops) {
        WaterDrop best = waterDrops.get(0);
        for (WaterDrop waterdrop : waterDrops) {
            if (waterdrop.getSolutionQuality() <= best.getSolutionQuality()) {
                System.out.println(waterdrop.getSolutionQuality());
                best = waterdrop;
            }
        }
        return best;
    }

    // update IWD velocity
    private void updateVelocity(WaterDrop wd, double pathSoil) {
        // current velocity
        double vt = wd.getVelocity();
        // next one t + 1
        double vt1 = vt + ((100) / (0.01 + pathSoil));
        wd.setVelocity(vt1);
    }

    // update IWD soil + pathSoil
    private void updateSoil(WaterDrop wd, Vertex v, Graph graph) {
        // current velocity
        double soil = wd.getCarriedSoil();
        double time = v.getCost() / Math.max(0.0001, wd.getVelocity());
        double deltaSoil = ((1000) / (0.01 + time));
        // next one t + 1
        double soil1 = soil + deltaSoil;
        wd.setCarriedSoil(soil1);
        v.addCharacteristic("soil", 0.1 * v.getCharacteristic("soil") - 0.9 * deltaSoil);
        // because it's symmetric
        Vertex sv = graph.getVertex(v.getId(), wd.getCurrentVertexId());
        sv.addCharacteristic("soil", 0.1 * v.getCharacteristic("soil") - 0.9 * deltaSoil);
    }

    private double getFSoilSummation(List<Vertex> uniqueAdjVertices, WaterDrop waterdrop) {
        double soil_i_K_sum = 0;
        for (Vertex vertex : uniqueAdjVertices) {
            if (!waterdrop.isVisited(vertex.getId())) {
                double v = fSoil(vertex, uniqueAdjVertices, waterdrop);
                soil_i_K_sum += v;
            }
        }
        return soil_i_K_sum;
    }

    private double fSoil(Vertex j, List<Vertex> adjVertices, WaterDrop waterDrop) {
        return 1 / (EPSILON + gSoil(j, adjVertices, waterDrop));
    }

    private double gSoil(Vertex j, List<Vertex> adjVertices, WaterDrop waterDrop) {
        double soil_i_j = j.getCharacteristic("soil");
        double minSoil = soil_i_j;
        for (Vertex vertex : adjVertices) {
            if (!waterDrop.isVisited(vertex.getId())) {
                if (vertex.getCharacteristic("soil") < minSoil) {
                    minSoil = vertex.getCharacteristic("soil");
                }
            }
        }
        if (minSoil >= 0) {
            return soil_i_j;
        } else return soil_i_j - minSoil;
    }

    private List<WaterDrop> getWDs(Graph graph) {
        List<WaterDrop> waterDrops = new ArrayList<>(graph.getNumberOfVertices());

        for (int i = 0; i < graph.getNumberOfVertices(); i++) {
            int random = getRandomNumberInts(0, graph.getNumberOfVertices() - 1);

            // Starting point is the same water drop ID.
            waterDrops.add(i, new WaterDrop(i, random, graph.getNumberOfVertices(), 100, 0, random));
        }
        return waterDrops;
    }

    public static int getRandomNumberInts(int min, int max) {
        Random random = new Random();
        return random.ints(min, (max + 1)).findFirst().getAsInt();
    }

    private void initParams(Graph graph, List<Vertex> verticesBEst, WaterDrop bestWaterDrop) {
        List<Vertex> vertices;
        for (int i = 0; i < graph.getNumberOfVertices(); i++) {
            vertices = graph.adjacentOf(i);
            for (Vertex vertex : vertices) {
                vertex.addCharacteristic("soil", (double) 1000);
            }}
            int from = bestWaterDrop.getFirstVertexId();
            for (Vertex v : verticesBEst) {
                double soil = (0.1 * v.getCharacteristic("soil")) + (0.9 * ((2 * bestWaterDrop.getCarriedSoil()) / (graph.getNumberOfVertices() * (graph.getNumberOfVertices() - 1))));
                graph.getVertex(from, v.getId()).addCharacteristic("soil", soil);
                // because it's symmetric? how can we solve this issue in this case?
                graph.getVertex(v.getId(), from).addCharacteristic("soil", soil);
                from = v.getId();
            }

    }
    private void initParams(Graph graph) {
        List<Vertex> vertices;
        for (int i = 0; i < graph.getNumberOfVertices(); i++) {
            vertices = graph.adjacentOf(i);
            for (Vertex vertex : vertices) {
                vertex.addCharacteristic("soil", (double) 1000);
            }
        }
    }

    public static void main(String[] args) {
        long startTime = System.nanoTime();
        TSP iwd = new IWD();
        try {
            iwd.solve(SetInstance.SIMPLE);
        } catch (ParserConfigurationException e) {
            e.printStackTrace();
        } catch (SAXException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        long endTime = System.nanoTime();
        long totalTime = endTime - startTime;
        System.out.println("total time =" + totalTime);
    }
}