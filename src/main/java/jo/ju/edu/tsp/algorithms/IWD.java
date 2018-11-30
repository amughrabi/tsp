package jo.ju.edu.tsp.algorithms;

import com.sun.istack.internal.NotNull;
import jo.ju.edu.tsp.core.Graph;
import jo.ju.edu.tsp.core.Vertex;
import jo.ju.edu.tsp.core.xml.Transformer;
import org.xml.sax.SAXException;

import javax.xml.parsers.ParserConfigurationException;
import java.io.IOException;
import java.util.*;

public class IWD extends TSP {
    private static int maxIterations;
    static final int INIT_SOIL = 1000;
    static final int INIT_VEL = 100;
    private final double EPSILON = 0.01;

    public Graph solve(Graph graph) {
        // print the graph
        Transformer.print(graph);
        // Maximum number of iterations is a triple the number of vertices
        maxIterations = 3 * graph.getNumberOfVertices();

        // A graph of water drops solutions, each index represent a water drop.
        Graph solutions;
        // Number of water drops equals to number of vertices
        List<WaterDrop> waterDrops = getWDs(graph);
        WaterDrop waterDrop;
        int numberOfCities = graph.getNumberOfVertices();
        double bestTourCost = 0;
        HashMap<Integer, List<Vertex>> bestGlobalSolution = null;
        initParams(graph);

        for (int iteration = 0; iteration < maxIterations; iteration++) {
            solutions = new Graph(graph.getNumberOfVertices(), "Solutions");
            for (int i = 0; i < waterDrops.size()*waterDrops.size(); i++) {
                double cost = 0;
                Vertex nextNode = null;
                waterDrop = waterDrops.get(i%waterDrops.size());
                // Graph graph1 = new Graph(graph.getNumberOfVertices(),"Solution");

                int waterDropCurrentVertexId = waterDrop.getCurrentVertexId();
                List<Vertex> adjVertices = graph.adjacentOf(waterDropCurrentVertexId);
                //List<Vertex> uniqueAdjVertices = getUniqueAdjVertices(waterDrop, adjVertices);
                double fSoilSummation = getFSoilSummation(adjVertices, waterDrop);
                double prob = 0;
                for (Vertex vertex : adjVertices) {
                    if (!waterDrop.isVisited(vertex.getId())) {
                        {
                            double maxProb = fSoil(vertex, adjVertices, waterDrop) / fSoilSummation;
                            if (maxProb >= prob) {
                                prob = maxProb;
                                nextNode = vertex;
                            }
                        }
                    }
                }
                if (nextNode == null) break;

                // graph1.put(waterDrop.getCurrentVertexId(),nextNode);

                waterDrop.markAsVisited(nextNode.getId());
                cost = graph.getVertex(waterDropCurrentVertexId,nextNode.getId()).getCharacteristic("cost") + waterDrop.getSolutionQuality();
                waterDrop.setSolutionQuality(cost);
                updateVelocity(waterDrop, nextNode.getCharacteristic("soil"));

                updateSoil(waterDrop, nextNode, graph);
                waterDrop.setCurrentVertexId(nextNode.getId());

                // save the solution
                solutions.put(i%waterDrops.size(), nextNode);
            }
            for (WaterDrop waterDrop1:
                 waterDrops) {
                Vertex firstVertex = graph.getVertex(waterDrop1.getCurrentVertexId(), waterDrop1.getWaterDropId());
                solutions.put(waterDrop1.getWaterDropId(), firstVertex);
                waterDrop1.setSolutionQuality(firstVertex.getCharacteristic("cost")+ waterDrop1.getSolutionQuality());
            }

            WaterDrop bestWaterDrop = getBestWaterDrop(waterDrops);
            if(bestWaterDrop != null) {
                bestGlobalSolution = new HashMap<Integer, List<Vertex>>();
                bestGlobalSolution.put(bestWaterDrop.getWaterDropId(), solutions.adjacentOf(bestWaterDrop.getWaterDropId()));
            }

            /////////////////////
            List<Vertex> vertices = solutions.adjacentOf(bestWaterDrop.getWaterDropId());

            int to = bestWaterDrop.getWaterDropId();

            for (Vertex v : vertices) {
                double soil = (0.1 * v.getCharacteristic("soil")) + (0.9 * ((2 * bestWaterDrop.getCarriedSoil()) / numberOfCities * (numberOfCities - 1)));
                v.addCharacteristic("soil", soil);
                // because it's symmetric? how can we solve this issue in this case?
                Vertex sv = graph.getVertex(v.getId(), to);
                to = v.getId();
                sv.addCharacteristic("soil", soil);
            }

            int cost = 0;
            for (Vertex v : vertices) {
                cost += v.getCharacteristic("cost");
            }

            if (bestTourCost == 0 || bestTourCost >= cost) {
                bestTourCost = cost;
            }
            waterDrops = getWDs(graph);

//            waterDrops = getWDs(graph);
        }
        System.out.println(bestTourCost);


//        // add waterdrop to cities randomly
//        List<Integer> tmp = new ArrayList();
//        for (int i=0; i< numberOfCities;i++){
//            tmp.add(i);
//        }
//        waterDrops = new ArrayList();
//        for (int i=0;i<numberOfWaterDrops;i++) {
//            int random = getRandomNumberInts(0, tmp.size()-1);
//            tmp.remove(random);
//            waterDrops.add(new IWD(random));
//        }

        return new Graph(0,"");
    }

    private WaterDrop getBestWaterDrop(List<WaterDrop> waterDrops) {
        WaterDrop best = waterDrops.get(0);
        for (WaterDrop waterdrop : waterDrops) {
            if (waterdrop.getSolutionQuality() <= best.getSolutionQuality()) {
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
        double vt1 = vt + ((1000) / (0.01 + 1 + pathSoil));
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
        sv.addCharacteristic("soil", soil);
    }

    private double getFSoilSummation(List<Vertex> uniqueAdjVertices, WaterDrop waterdrop) {
        double soil_i_K_sum = 0;
        for (Vertex vertex : uniqueAdjVertices) {
            if (!waterdrop.isVisited(vertex.getId())) {
                double v = fSoil(vertex, uniqueAdjVertices, waterdrop);
                soil_i_K_sum = +v;
            }
        }
        return soil_i_K_sum;
    }

    private List<Vertex> getUniqueAdjVertices(WaterDrop waterDrop, List<Vertex> adjVertices) {
        List<Vertex> uniqueAdjVertices = new ArrayList<Vertex>();
        for (Vertex vertex : adjVertices) {
            if (!waterDrop.isVisited(vertex.getId())) {
                uniqueAdjVertices.add(vertex);
            }
        }
        return uniqueAdjVertices;
    }

    private double fSoil(Vertex j, List<Vertex> uniqueAdjVertices, WaterDrop waterDrop) {
        return 1 / (EPSILON + gSoil(j, uniqueAdjVertices, waterDrop));
    }

    private double gSoil(Vertex j, List<Vertex> uniqueAdjVertices, WaterDrop waterDrop) {
        double soil_i_j = j.getCharacteristic("soil");
        double minSoil = j.getCharacteristic("soil");
        for (Vertex vertex : uniqueAdjVertices) {
            if (!waterDrop.isVisited(vertex.getId())) {
                {
                    if (vertex.getCharacteristic("soil") < minSoil) {
                        minSoil = vertex.getCharacteristic("soil");
                    }
                }
            }
        }
        if (minSoil >= 0) {
            return soil_i_j;
        } else return soil_i_j - minSoil;
    }

    private List<WaterDrop> getWDs(Graph graph) {
        List<WaterDrop> waterDrops = new ArrayList<WaterDrop>(graph.getNumberOfVertices());
        for (int i = 0; i < graph.getNumberOfVertices(); i++) {
            // Starting point is the same water drop ID.
            waterDrops.add(i, new WaterDrop(i, i, graph.getNumberOfVertices(), 100, 0));
        }
        return waterDrops;
    }

    private void initParams(@NotNull Graph graph) {
        List<Vertex> vertices;
        for (int i = 0; i < graph.getNumberOfVertices(); i++) {
            vertices = graph.adjacentOf(i);
            for (Vertex vertex : vertices) {
                vertex.addCharacteristic("soil", (double) 1000);
                vertex.addCharacteristic("cost", vertex.getCost());
            }
        }
    }

    public static void main(String[] args) {
        TSP iwd = new IWD();
        try {
            iwd.solve();
        } catch (ParserConfigurationException e) {
            e.printStackTrace();
        } catch (SAXException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
