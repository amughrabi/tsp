package jo.ju.edu.tsp.algorithms;

import jo.ju.edu.tsp.core.Graph;
import jo.ju.edu.tsp.core.Vertex;
import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class HCA extends TSP {
    private static int maxIterations;
    private final double initSoil = 10000, maxTemperature = 100, epsilon = 0.01, alpha = 2, PN = 0.99, beta = 10; // 10k
    private double temperature = 50;

    public @NotNull Graph solve(@NotNull Graph graph) {
        // distribute the initial amount of soil and depth on the vertices.
        initParams(graph);

        // A graph of water drops solutions, each index represent a water drop.
        Graph solutions = new Graph(graph.getNumberOfVertices(), "Solutions");
        // Number of water drops equals to number of vertices
        List<WaterDrop> waterDrops = getWDs(graph);
        WaterDrop waterDrop;
        double gDepth, fSoil, probability, sum, theChosenOneValue;
        Vertex theChosenOne;
        while (temperature < maxTemperature) {
            for(int iteration = 0; iteration < maxIterations; iteration++) {
                for(int i = 0; i < waterDrops.size(); i++) {
                    waterDrop = waterDrops.get(i);
                    theChosenOne = null;
                    theChosenOneValue = 0;
                    for(Vertex vertex : graph.adjacentOf(waterDrop.getCurrentVertexId())) {
                        gDepth = gDepth(vertex);
                        fSoil = fSoil(vertex);
                        sum = 0;
                        if(solutions.adjacentOf(i) == null) {
                            sum = Math.pow(fSoil, 2) * gDepth;
                        } else {
                            for(Vertex vc : solutions.adjacentOf(i)) {
                                sum += (Math.pow(fSoil(vc), 2) * gDepth(vc));
                            }
                        }

                        probability = (Math.pow(fSoil, 2) * gDepth) / sum;
                        if(theChosenOneValue < probability) {
                            theChosenOneValue = probability;
                            theChosenOne = vertex;
                        }
                    }
                    // Mark the current vertex as visited
                    waterDrop.markAsVisited(theChosenOne.getId());
                    // 3.3.1. Velocity Update
                    updateVelocity(waterDrop, theChosenOne);
                    // 3.3.2. Soil Update
                    updateSoil(waterDrop, theChosenOne, waterDrops);
                    //3.3.3. Soil Transportation Update
                    updateCarrySoil(waterDrop, theChosenOne);
                    // 3.3.4. Temperature Update
                    updateTemperature(waterDrops);
                    solutions.put(i, theChosenOne);
                }
                // check if the evaporation
                // When the temperature increases and reaches a specified value, the evaporation stage is invoked.
                if(temperature > maxTemperature) break;
            }
            // 3.4. Evaporation Stage
            List<WaterDrop> evaporatedWaterDrops = evaporation(waterDrops);
            // 3.5. Condensation Stage
            List<WaterDrop> condensationWaterDrops = condensation(evaporatedWaterDrops, solutions);

        }

        return null;
    }

    private List<WaterDrop> condensation(List<WaterDrop> wds, Graph solutions) {
        double similarity;
        for(int i = 1; i < wds.size(); i++) {
            similarity = similarity(solutions.adjacentOf(i - 1), solutions.adjacentOf(i));
            if(similarity < 50) { // Bounce

            } else { // Merge

            }
        }
        return null;
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
                max = wd.getCurrentVertexId();
            }

            if(min > wd.getSolutionQuality()) {
                max = wd.getSolutionQuality();
            }
        }

        return new Pair<Double, Double>(min, max);
    }



    private void updateCarrySoil(WaterDrop wd, Vertex v) {
        double timeWD = v.getCost() / wd.getVelocity();
        double dSoil = 1 / timeWD;
        wd.setCarriedSoil(wd.getCarriedSoil() + (dSoil / wd.getSolutionQuality()));
    }

    private void updateSoil(WaterDrop wd, Vertex v, List<WaterDrop> wds) {
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
        return 1 / depth(v);
    }

    private double depth(Vertex v){
        return v.getCost() / v.getCharacteristic("soil");
    }

    private List<WaterDrop> getWDs(Graph graph) {
        List<WaterDrop> waterDrops = new ArrayList<WaterDrop>(graph.getNumberOfVertices());
        for(int i = 0; i < waterDrops.size(); i++) {
            // Starting point is the same water drop ID.
            waterDrops.add(i, new WaterDrop(i, i, waterDrops.size()));
        }
        return waterDrops;
    }

    private void initParams(@NotNull Graph graph) {
        // Maximum number of iterations is a triple the number of vertices
        maxIterations = 3 * graph.getNumberOfVertices();

        List<Vertex> vertices;
        for(int i = 0; i < graph.getNumberOfVertices(); i++) {
            vertices = graph.adjacentOf(i);
            for(Vertex vertex : vertices) {
                vertex.addCharacteristic("soil", initSoil);
                vertex.addCharacteristic("depth", (vertex.getCost() / initSoil) );
            }
        }
    }

    /*
    *  According to the evaporation rate, a specific number of water drops is selected to evaporate. The selection
    *  is done using the roulette-wheel selection method, taking into consideration the solution quality of each water
    *  drop. Only the selected water drops evaporate and go to the next stage.
    * */
    private List<WaterDrop> rouletteWheelSelection(List<WaterDrop> wds) {
        Random random = new Random();
        int popSize = random.nextInt(wds.size());

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
        if(wd1.size() != wd1.size()) return -1;
        int counter = 0;
        for(int i = 0; i < wd1.size(); i++) {
            if(wd1.get(i).getId() != wd2.get(i).getId()) counter++;
        }
        return (counter / wd1.size()) * 100; // output in percentage. 50%
    }
}
