package jo.ju.edu.tsp.algorithms;

// The HCA uses a set of artificial water drops to generate solutions,
// where each water drop has three properties:
// 1) velocity, 2) amount of carried soil, and 3) solution quality.
public class WaterDrop {
    private int waterDropId;
    private int firstVertexId;

    // Current vertex
    private int currentVertexId;
    // properties
    private double velocity, carriedSoil, solutionQuality;
    // The visited map for each water drop to minimize the duplicate efforts.
    private boolean[] visited;
    // The number of visited nodes.
    private int numberOfVisitedNodes;

    public WaterDrop(int waterDropId, int currentVertexId, int numberOfVertices) {
        this(waterDropId, currentVertexId, numberOfVertices, 100, 1);
        // In HCA, the carrying soil is encoded with the solution quality; more carrying soil indicates a better solution.
        // In the HCA, we consider the fact that the amount of soil a water drop carries reflects its solution quality.
        // This can be done by associating the quality of the water drop’s solution with its carrying soil value.
        this.solutionQuality = carriedSoil;
    }
    public WaterDrop(int waterDropId, int currentVertexId, int numberOfVertices, double velocity, double carriedSoil, int firstVertexId) {
        this.waterDropId = waterDropId;
        this.firstVertexId = firstVertexId;
        this.velocity = velocity;
        this.carriedSoil = carriedSoil;
        this.currentVertexId = currentVertexId;

        visited = new boolean[numberOfVertices];
        markAsVisited(currentVertexId);
    }
    public WaterDrop(int waterDropId, int currentVertexId, int numberOfVertices, double velocity, double carriedSoil) {
        this.waterDropId = waterDropId;
        this.velocity = velocity;
        this.carriedSoil = carriedSoil;
        this.currentVertexId = currentVertexId;
        visited = new boolean[numberOfVertices];
        numberOfVisitedNodes = 0;
        markAsVisited(currentVertexId);
    }
    public WaterDrop(int waterDropId, int currentVertexId, int numberOfVertices, double solutionQuality) {
        this(waterDropId, currentVertexId, numberOfVertices, 100, 1, solutionQuality);
        // In HCA, the carrying soil is encoded with the solution quality; more carrying soil indicates a better solution.
        // In the HCA, we consider the fact that the amount of soil a water drop carries reflects its solution quality.
        // This can be done by associating the quality of the water drop’s solution with its carrying soil value.

    }

    public WaterDrop(int waterDropId, int currentVertexId, int numberOfVertices, double velocity, double carriedSoil, double solutionQuality
    ) {
        this.waterDropId = waterDropId;
        this.velocity = velocity;
        this.carriedSoil = carriedSoil;
        this.currentVertexId = currentVertexId;
        this.solutionQuality = solutionQuality;
        visited = new boolean[numberOfVertices];
        markAsVisited(currentVertexId);
    }

    public void setSolutionQuality(double solutionQuality) {
        this.solutionQuality = solutionQuality;
    }

    public int getCurrentVertexId() {
        return currentVertexId;
    }

    public void setCurrentVertexId(int currentVertexId) {
        this.currentVertexId = currentVertexId;
    }

    public double getVelocity() {
        return velocity;
    }

    public void setVelocity(double velocity) {
        this.velocity = velocity;
    }

    public double getCarriedSoil() {
        return carriedSoil;
    }

    public void setCarriedSoil(double carriedSoil) {
        this.carriedSoil = carriedSoil;
    }

    public double getSolutionQuality() {
        return solutionQuality;
    }

    public boolean isVisited(int vertexId) {
        return vertexId < visited.length && visited[vertexId];
    }

    public void markAsVisited(int vertexId) {
        if(vertexId < visited.length) {
            visited[vertexId] = true;
            numberOfVisitedNodes++;
        }
    }

    public int getWaterDropId() {
        return waterDropId;
    }

    public int getNumberOfVisitedNodes() {
        return numberOfVisitedNodes;
    }

    public int getFirstVertexId() {
        return firstVertexId;
    }
}
