package jo.ju.edu.tsp.algorithms;

import jo.ju.edu.tsp.core.Graph;
import jo.ju.edu.tsp.core.Vertex;
import jo.ju.edu.tsp.set.SetDetails;
import jo.ju.edu.tsp.set.SetInstance;
import org.jetbrains.annotations.NotNull;
import org.xml.sax.SAXException;

import javax.xml.parsers.ParserConfigurationException;
import java.io.IOException;
import java.util.*;

public class HCA extends TSP {
    private static int maxIterations;
    private final double initSoil = 10000, epsilon = 0.01, alpha = 2, PN = 0.99, beta = 10; // 10k
    private double temperature = 50, maxTemperature = 1000;

    public @NotNull Graph solve(@NotNull Graph graph) {
        System.out.println( "started: " + (new Date()));
        // print the graph
       // Transformer.print(graph);
        // Maximum number of iterations is a triple the number of vertices
        maxIterations = 3 * graph.getNumberOfVertices();

        // A graph of water drops solutions, each index represent a water drop.
        Graph solutions;
        // Number of water drops equals to number of vertices
        List<WaterDrop> waterDrops;
        WaterDrop waterDrop;
        double gDepth, fSoil, probability, sum, theChosenOneValue;
        Vertex theChosenOne;
        HashMap<Integer, List<Vertex>> bestGlobalSolution = null;
        int cycle = 0;
        temperature = 50;
        //maxTemperature = maxTemperature * graph.getNumberOfVertices();
        while (temperature < maxTemperature) {
            // distribute the initial amount of soil and depth on the vertices.
            initParams(graph, bestGlobalSolution);
            waterDrops = getWDs(graph);
            solutions = new Graph(graph.getNumberOfVertices(), "Solutions");
            for(int iteration = 0; iteration < maxIterations; iteration++) {
                for(int i = 0; i < waterDrops.size(); i++) {
                    waterDrop = waterDrops.get(i);
                    theChosenOne = null;
                    theChosenOneValue = 0;
                    // summation for each water drop
                    sum = 0;
                    for(Vertex vc : graph.adjacentOf(waterDrop.getCurrentVertexId())) {
                        if(!waterDrop.isVisited(vc.getId()))
                            sum += (Math.pow(fSoil(vc), 2) * gDepth(vc));
                    }

                    if(Double.isNaN(sum))
                        sum = 1;

                    for(Vertex vertex : graph.adjacentOf(waterDrop.getCurrentVertexId())) {
                        if(!waterDrop.isVisited(vertex.getId())) {
                            gDepth = gDepth(vertex);
                            fSoil = fSoil(vertex);
                            probability = (Math.pow(fSoil, 2) * gDepth) / sum;
                            if(theChosenOneValue < probability) {
                                theChosenOneValue = probability;
                                theChosenOne = vertex;
                            }
                        }
                    }

                    if(theChosenOne == null) break;
                    // Mark the current vertex as visited
                    waterDrop.markAsVisited(theChosenOne.getId());
                    // 3.3.1. Velocity Update
                    updateVelocity(waterDrop, theChosenOne);
                    // 3.3.2. Soil Update
                    updateSoil(waterDrop, theChosenOne, waterDrops, graph);
                    //3.3.3. Soil Transportation Update
                    updateCarrySoil(waterDrop, theChosenOne);
                    // update the pointer
                    waterDrop.setCurrentVertexId(theChosenOne.getId());
                    // save the solution
                    solutions.put(i, theChosenOne);

                    //System.out.println("depth: " + theChosenOne.getCharacteristic("depth") + ", water drop: " + (waterDrop.getWaterDropId() + 1) + ", vertex id : " + (theChosenOne.getId() + 1) + ", soil: " + theChosenOne.getCharacteristic("soil") + ", Velocity: " + waterDrop.getVelocity() + " carry soil: " + waterDrop.getCarriedSoil());
                }
                // 3.3.4. Temperature Update
                updateTemperature(waterDrops);
                // check if the evaporation
                // When the temperature increases and reaches a specified value, the evaporation stage is invoked.
                //if(temperature > maxTemperature) break;
            }
            System.out.println("Done for " + graph.getName());

            // 3.4. Evaporation Stage
            List<WaterDrop> evaporatedWaterDrops = evaporation(waterDrops);
            // 3.5. Condensation Stage
            WaterDrop theCollector = condensation(evaporatedWaterDrops, solutions);
            // 3.6. Precipitation Stage
            if(theCollector != null) {
                bestGlobalSolution = new HashMap<Integer, List<Vertex>>();
                bestGlobalSolution.put(theCollector.getWaterDropId(), solutions.adjacentOf(theCollector.getWaterDropId()));
            }

            cycle ++;
            System.out.println("cycle" + cycle + " tempreture: " + temperature);
            //break;
        }
        // Print the best global solution details
        if(bestGlobalSolution != null) {
            double cost = 0;
            for(int i : bestGlobalSolution.keySet()) {
               // System.out.print("---\nHamiltonian path: ");
                System.out.print( "Size: " + bestGlobalSolution.get(i).size() );
                for(Vertex v : bestGlobalSolution.get(i)) {
                    cost += v.getCost();
                    //System.out.print("  ->  " + (v.getId()+1));
                }
                //System.out.print("  ->  " +  (i + 1) );
            }
            System.out.println("   | cost (" + cost + ") cycles: " + cycle);
        }

        System.out.println( "ended: " + (new Date()));
        return new Graph(0, "");
    }

    private WaterDrop condensation(List<WaterDrop> wds, Graph solutions) {
        double similarity;
        for(int i = 0; i < wds.size(); i++) {
            if(wds.get(i) != null) {
                for(int j = 0; j < wds.size(); j++) {
                    if(wds.get(j) != null && isCollideWaterDrops(solutions.adjacentOf(i), solutions.adjacentOf(j))) {
                        similarity = similarity(solutions.adjacentOf(i), solutions.adjacentOf(j));
                        if(similarity >= 50) {// Merge
                            // When two water drops collide and merge, one water drop (i.e., the collector) will
                            // become more powerful by eliminating the other one in the process also acquiring the
                            // characteristics of the eliminated water drop (i.e., its velocity)
                            WaterDrop wd1 = wds.get(i); // the collector
                            WaterDrop wd2 = wds.get(j);
                            wd1.setVelocity(wd1.getVelocity() + wd2.getVelocity());
                            wds.set(j, null); // eliminating
                        } else {
                            // Bounce - for now, leave it empty. The weight is NOT used in Node selection in the
                            // flow stage. We only depends on the soil, velocity, carried soil, and temperature
                        }
                    }
                }
            }
        }
        // In addition, at this stage, the temperature is reduced by 50°C. The lowering and raising of the temperature
        // helps to prevent the water drops from sticking with the same solution every iteration.
        temperature -= 50;

        return findTheCollector(wds);
    }

    // Higher velocity, higher probability to be the collector.
    private WaterDrop findTheCollector(List<WaterDrop> wds) {
        double max = -1;
        WaterDrop collector = null;
        for (WaterDrop wd : wds) {
            if (wd != null && max < wd.getVelocity()) {
                max = wd.getVelocity();
                collector = wd;
            }
        }
        return collector;
    }

    private boolean isCollideWaterDrops(List<Vertex> solution1, List<Vertex> solution2) {
        return solution1.size() == solution2.size();
    }

    /* NOT USED FOR NOW */
    private HashMap<Integer, Double> calcOccurrences(List<Vertex> solution1, List<Vertex> solution2) {
        HashMap<Integer, Double> occurrences = new HashMap<Integer, Double>(solution1.size());
        for(int i = 0; i < solution1.size(); i++) {
            occurrences.put(solution1.get(i).getId(), occurrences.get(solution1.get(i).getId()) + 1);
            occurrences.put(solution2.get(i).getId(), occurrences.get(solution2.get(i).getId()) + 1);
        }
        return occurrences;
    }

    private List<WaterDrop> evaporation(List<WaterDrop> wds) {
        return rouletteWheelSelection(wds);
    }

    private void updateTemperature(List<WaterDrop> wds) {
        Pair<Double, Double> boundaries = getBoundariesSolutionQuality(wds);
        double dD = boundaries.getV() - boundaries.getK(); // max - min
        double dTemp;
        if(dD > 0) {
            dTemp = beta * (temperature/dD);
        } else {
            dTemp = temperature / 10;
        }

        temperature += dTemp;
    }

    private Pair<Double, Double> getBoundariesSolutionQuality(List<WaterDrop> wds) {
        double max = 0, min = Integer.MAX_VALUE;
        for(WaterDrop wd : wds) {
            if(max < wd.getSolutionQuality()) {
                max = wd.getSolutionQuality();
            }

            if(min > wd.getSolutionQuality()) {
                min = wd.getSolutionQuality();
            }
        }

        return new Pair<Double, Double>(min, max);
    }

    private void updateCarrySoil(WaterDrop wd, Vertex v) {
        double timeWD = v.getCost() / wd.getVelocity();
        double dSoil = 1 / timeWD;
        double carriedSoil = wd.getCarriedSoil() + (dSoil / wd.getSolutionQuality());
        wd.setCarriedSoil(carriedSoil);
        wd.setSolutionQuality(carriedSoil);
    }

    private void updateSoil(WaterDrop wd, Vertex v, List<WaterDrop> wds, Graph graph) {
        double timeWD = v.getCost() / wd.getVelocity();
        double dSoil = 1 / timeWD;
        double avgWDs = getAverageVelocityForAllWaterDrops(wds);
        // A water drop can remove (or add) soil from (or to) a path while moving based on its velocity.
        double soil = (PN * v.getCharacteristic("soil"));
        if(wd.getVelocity() >= avgWDs) { // Erosion
            soil = soil - dSoil - Math.sqrt(1 / depth(v));
        } else { // decomposition
            soil = soil + dSoil + Math.sqrt(1 / depth(v));
        }

        v.addCharacteristic("soil", soil);
        // because it's symmetric
        Vertex sv = graph.getVertex(v.getId(), wd.getCurrentVertexId());
        sv.addCharacteristic("soil", soil);
    }

    private double getAverageVelocityForAllWaterDrops(List<WaterDrop> wds) {
        double sum = 0;
        for(WaterDrop w : wds) {
            sum += w.getVelocity();
        }
        return sum / wds.size();
    }

    private void updateVelocity(WaterDrop wd, Vertex v) {
        double K = Math.random(); // refers to the roughness coefficient [0,1]
        // current velocity
        double vt = wd.getVelocity();
        // next one t + 1
        double vt1 = (K * vt) + (alpha * (vt / v.getCharacteristic("soil")) + Math.sqrt(vt / wd.getCarriedSoil())) + (100 / wd.getSolutionQuality()) + Math.sqrt(vt / depth(v));

        wd.setVelocity(vt1);
    }

    private double fSoil(Vertex v) {
        return 1 / (epsilon + v.getCharacteristic("soil"));
    }

    private double gDepth(Vertex v) {
        return 1 / normalize(depth(v));
    }

    private double normalize(double value) {
        return value * 10000;
    }

    private double depth(Vertex v){
        return v.getCost() / v.getCharacteristic("soil");
    }

    private List<WaterDrop> getWDs(Graph graph) {
        List<WaterDrop> waterDrops = new ArrayList<WaterDrop>(graph.getNumberOfVertices());
        for(int i = 0; i < graph.getNumberOfVertices(); i++) {
            // Starting point is the same water drop ID.
            waterDrops.add(i, new WaterDrop(i, i, graph.getNumberOfVertices()));
        }
        return waterDrops;
    }
    // reinitializing all the dynamic variables, such as the amount of the soil on each edge, depth of paths,
    // the velocity of each water drop, and the amount of soil it holds.
    private void initParams(@NotNull Graph graph, HashMap<Integer, List<Vertex>> bestGlobalSolution) {
        List<Vertex> vertices;
        double soil;
        for(int i = 0; i < graph.getNumberOfVertices(); i++) {
            vertices = graph.adjacentOf(i);
            for(Vertex vertex : vertices) {
                soil = initSoil;
                if(bestGlobalSolution != null && bestGlobalSolution.get(i) != null) {
                    if(isBelong(vertex, bestGlobalSolution.get(i))) {
                        soil = 0.9 * initSoil;
                    }
                }
                vertex.addCharacteristic("soil", soil);
                vertex.addCharacteristic("depth", (vertex.getCost() / soil) );
            }
        }
    }

    private boolean isBelong(Vertex v, List<Vertex> vs) {
        for(Vertex vertex : vs) {
            if(vertex.getId() == v.getId())
                return true;
        }
        return false;
    }

    /*
    *  According to the evaporation rate, a specific number of water drops is selected to evaporate. The selection
    *  is done using the roulette-wheel selection method, taking into consideration the solution quality of each water
    *  drop. Only the selected water drops evaporate and go to the next stage.
    * */
    private List<WaterDrop> rouletteWheelSelection(List<WaterDrop> wds) {
        Random random = new Random();
        int popSize = random.nextInt(wds.size()) + 1;

        List<WaterDrop> populationNew = new ArrayList<WaterDrop>(popSize);
        //sum the total fitness of the population
        int totalFitness = 0;
        for (int i = 0; i < popSize; i++) {
            totalFitness +=  wds.get(i).getSolutionQuality();
        }
        /* sum the fitness of each individual in the population again
         * until the running sum is >= to the randomly chosen number.
         */
        for (int i = 0; i < popSize; i++) {
            //pick a random number between 0 and that sum.
            int randomNumber = random.nextInt(totalFitness + 1);
            int runningSum = 0;
            int index = 0;
            int lastAddedIndex = 0;
            while (runningSum < randomNumber) {
                runningSum += wds.get(index).getSolutionQuality();
                lastAddedIndex = index;
                index++;
            }
            populationNew.add(i,  wds.get(lastAddedIndex));
        }
        return populationNew;
    }


    private int similarity(List<Vertex> wd1, List<Vertex> wd2) {
        if(wd1.size() != wd2.size() || wd1.size() == 0) return -1;
        int counter = 0;
        for(int i = 0; i < wd1.size(); i++) {
            if(wd1.get(i).getId() != wd2.get(i).getId()) counter++;
        }
        return (counter / wd1.size()) * 100; // output in percentage. 50%
    }

    public static void main(String[] args) {
        TSP tsp = new HCA();
        try {
            SetDetails ds = SetInstance.SIMPLE;
            tsp.solve(ds);

            System.gc();
            System.out.println("-----------------------------------");

            tsp.solve(ds);

            System.gc();
            System.out.println("-----------------------------------");

            tsp.solve(ds);

            System.gc();
            System.out.println("-----------------------------------");

            tsp.solve(ds);

            System.gc();
            System.out.println("-----------------------------------");

            tsp.solve(ds);

        } catch (ParserConfigurationException | SAXException | IOException e) {
            e.printStackTrace();
        }
    }
}
