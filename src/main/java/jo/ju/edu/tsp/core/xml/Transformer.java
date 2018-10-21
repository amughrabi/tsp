package jo.ju.edu.tsp.core.xml;

import jo.ju.edu.tsp.core.Graph;
import jo.ju.edu.tsp.core.Vertex;
import jo.ju.edu.tsp.set.SetDetails;
import org.jetbrains.annotations.NotNull;
import org.xml.sax.SAXException;

import javax.xml.parsers.*;
import java.io.File;
import java.io.IOException;
import java.util.List;

// We can handle HUGE XML
public class Transformer {

    public static @NotNull Graph constructGraph(@NotNull SetDetails details) throws ParserConfigurationException, SAXException, IOException {
        return constructGraph(details.getFile(), details.getName(), details.getSize());
    }

    public static @NotNull Graph constructGraph(@NotNull File fullPath, @NotNull String name, int size) throws ParserConfigurationException, SAXException, IOException {
        SAXParserFactory spf = SAXParserFactory.newInstance();
        spf.setNamespaceAware(true);

        SAXParser saxParser = spf.newSAXParser();
        org.xml.sax.XMLReader reader = saxParser.getXMLReader();
        VertexHandler handler = new VertexHandler(size, name);
        reader.setContentHandler(handler);
        saxParser.parse(fullPath, handler);

        return handler.getGraph();
    }

    public static @NotNull Graph constructGraph(@NotNull String fullPath, @NotNull String name, int size) throws ParserConfigurationException, SAXException, IOException {
        return constructGraph(new File(fullPath), name, size);
    }


    // DON'T USE IT FOR BIG GRAPHS
    public static void print(@NotNull Graph graph) {
        List<Vertex> vertices;
        for (int i = 0; i < graph.getSize(); i++) {
            vertices = graph.adjacentOf(i);
            System.out.print(i);
            for(Vertex v : vertices) {
                System.out.print(" -> " + v.getId() + "(" + v.getCost() + ")");
            }
            System.out.println();
        }
    }

    public static void main(String[] args) {
        try {

            Graph graph = Transformer.constructGraph(Transformer.class.getClassLoader().getResource("dataset/must/simple5.xml").getFile(), "Simple5", 5);
            Transformer.print(graph);
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }
}
