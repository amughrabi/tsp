package jo.ju.edu.tsp.algorithms;

import jo.ju.edu.tsp.core.Graph;
import jo.ju.edu.tsp.core.xml.Transformer;
import jo.ju.edu.tsp.set.SetDetails;
import jo.ju.edu.tsp.set.SetInstance;
import org.jetbrains.annotations.NotNull;
import org.xml.sax.SAXException;

import javax.xml.parsers.ParserConfigurationException;
import java.io.IOException;

public abstract class TSP {

    public TSP() {
    }

    public @NotNull Graph solve()
            throws ParserConfigurationException, SAXException, IOException {
        return solve(SetInstance.SIMPLE);
    }

    /**
     * TSP container to have a uniform implementation for all algorithms that belong to the TSP.
     * @param set the dataset that need to run the TSP algorithm on it.
     * @return a hamiltonian graph https://en.wikipedia.org/wiki/Hamiltonian_path
     * @throws IOException in case the dataset (XML) has not been created by/resources/transformer/Debug/TransformTSPLIB tool.
     * @throws SAXException in case the XML has a parsing problem.
     * @throws ParserConfigurationException in case the XML has a parsing problem for hugeXML files.
     */
    public @NotNull Graph solve(@NotNull SetDetails set)
            throws IOException, SAXException, ParserConfigurationException {
        return solve(Transformer.constructGraph(set));
    }

    public abstract @NotNull Graph solve(@NotNull Graph graph);
}
