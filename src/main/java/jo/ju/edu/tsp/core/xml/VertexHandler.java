package jo.ju.edu.tsp.core.xml;

import jo.ju.edu.tsp.core.Graph;
import jo.ju.edu.tsp.core.Vertex;
import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

public class VertexHandler extends DefaultHandler {
    private Graph graph = null;
    private Vertex vertex = null;
    private int idx = 0, id = 0;

    private boolean isEdge = false;

    public VertexHandler(int size, String name) {
        this.graph = new Graph(size, name);
    }

    @Override
    public void startElement (String uri, String localName, String qName, Attributes attributes) throws SAXException {
        if(qName.equalsIgnoreCase("edge")) {
            vertex = new Vertex();
            vertex.setCost(Double.valueOf(attributes.getValue("cost")));
            isEdge = true;
        }
    }

    public Graph getGraph() {
        return graph;
    }

    @Override
    public void characters(char ch[], int start, int length) throws SAXException {
        if(isEdge) {
            id = Integer.valueOf(new String(ch, start, length));
            isEdge = false;
        }
    }

    @Override
    public void endElement(String uri, String localName, String qName) throws SAXException {
        if(qName.equalsIgnoreCase("edge")) {
            vertex.setId(id);
            id = 0;
            graph.put(idx, vertex);
        } else if(qName.equalsIgnoreCase("vertex")) {
            idx++;
        }
    }
}