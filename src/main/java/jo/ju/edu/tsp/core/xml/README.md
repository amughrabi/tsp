# An XML Conversion for TSPLIB Specification

A TSPLIB is a library of sample instances for the TSP (and related problems) from various sources and of various types. These instances has been written in .tsp and .atsp files format. The .atsp file contains **asymmetric** traveling salesman problem which means the distance from node *i* to node *j* and the distance from node *j* to node *i* may be different. The .tsp file contains **symmetric** traveling salesman problem which means the distance from node *i* to node *j* is the same as from node *j* to node *i*. However, we took a clone from the original [source code](https://www.iwr.uni-heidelberg.de/groups/comopt/software/TSPLIB95/XML-TSPLIB/Sample/). 

The reason behind this clone is that the tsp and atsp files are out of sync; for example, not all tsp or atsp file can be found as XML files. Moreover, We cannot download all files because these files have a big size (if exists). On the other hand, these big files should be paginated because we cannot parse these files at once (limitation on memory); a lazy load mechanism should be provided.

# Specifications
 - The specification of tsp and atsp formats has been provided [here](https://www.iwr.uni-heidelberg.de/groups/comopt/software/TSPLIB95/tsp95.pdf)
 - The specification of the XML structure has been provided [here](https://www.iwr.uni-heidelberg.de/groups/comopt/software/TSPLIB95/XML-TSPLIB/Description.pdf).
 - The structure of the cloned code has been provided [here](https://www.iwr.uni-heidelberg.de/groups/comopt/software/TSPLIB95/XML-TSPLIB/Sample/Readme.pdf).

## SAMPLE.xml
The following figure demonstrates a simple weighted undirected graph with five nodes. The graph cited from doi [10.4236/ajor.2018.83010](http://doi.org/10.4236/ajor.2018.83010)

![SAMPLE graph](http://html.scirp.org/file/1-1040612x9.png)

**Figure 1.** A TSP instance with five nodes. 

Based on the [XML specification](https://www.iwr.uni-heidelberg.de/groups/comopt/software/TSPLIB95/XML-TSPLIB/Description.pdf), the simple graph can be written as the following:

    <?xml version="1.0" encoding="UTF-8" standalone="no" ?>  
    <travellingSalesmanProblemInstance>  
        <name>simple5</name>  
        <source>DOI: 10.4236/ajor.2018.83010</source>  
        <description>  
            Solving the Traveling Salesman Problem Using Hydrological Cycle Algorithm  
            Figure 1. TSP instance with five nodes.  
        </description>  
        <author>Ahmad Al Mughrabi</author>  
        <doublePrecision>15</doublePrecision>  
        <ignoredDigits>5</ignoredDigits>  
        <graph>  
            <vertex>  
                <edge cost="31.000000000000000e+00">1</edge>  
                <edge cost="33.000000000000000e+00">2</edge>  
                <edge cost="37.000000000000000e+00">3</edge>  
                <edge cost="29.000000000000000e+00">4</edge>  
            </vertex>  
            <vertex>  
                <edge cost="31.000000000000000e+00">0</edge>  
                <edge cost="40.000000000000000e+00">2</edge>  
                <edge cost="61.000000000000000e+00">3</edge>  
                <edge cost="46.000000000000000e+00">4</edge>  
            </vertex>  
            <vertex>  
                <edge cost="33.000000000000000e+00">0</edge>  
                <edge cost="40.000000000000000e+00">1</edge>  
                <edge cost="42.000000000000000e+00">3</edge>  
                <edge cost="49.000000000000000e+00">4</edge>  
            </vertex>  
            <vertex>  
                <edge cost="37.000000000000000e+00">0</edge>  
                <edge cost="61.000000000000000e+00">1</edge>  
                <edge cost="42.000000000000000e+00">2</edge>  
                <edge cost="48.000000000000000e+00">4</edge>  
            </vertex>  
            <vertex>  
                <edge cost="29.000000000000000e+00">0</edge>  
                <edge cost="46.000000000000000e+00">1</edge>  
                <edge cost="49.000000000000000e+00">2</edge>  
                <edge cost="48.000000000000000e+00">3</edge>  
            </vertex>  
        </graph>  
    </travellingSalesmanProblemInstance>

## Create new XML files

You can create the XML files from .tsp or .atsp files based on the following instructions. Please note that the following instructions has been tested on **Ubuntu 18.04.1 LTS** OS.  

 - Make sure the [Xerces-C++](http://xerces.apache.org/xerces-c/) version 3.1.1 library or later version has been installed on your machine.
 - After download the source code, go to the directory `/src/main/resources/transformer/Debug`, and run the `make` command.
 - Choose a .tsp/.atsp file path and convert it by using the following command structure in your terminal: 

       ./Debug/TransformTSPLIB $PATH_TO_PROJECT/tsp/src/main/resources/elearning/xmc10150.tsp $PATH_TO_PROJECT/tsp/src/main/resources/dataset/must/xmc10150.xml

 - After the conversion, you will find the new XMLs files has been add to `$PATH_TO_PROJECT/tsp/src/main/resources/dataset/must/` directory. As demoestrate in Figure 2.
 
![Intellij viewer](https://raw.githubusercontent.com/amughrabi/tsp/master/src/main/resources/images/datasets.png)

**Figure 2.** A snapshot from Intellij.

A second option, you can create this file manually as we did in SAMPLE.xml file.
## Using the API
We wrap the MUST dataset by its names to easy find and use. After clone the project, the only thing that you need to do is the following:

    package jo.ju.edu.tsp;  
    import jo.ju.edu.tsp.core.Graph;  
    import jo.ju.edu.tsp.core.xml.Transformer;  
    import jo.ju.edu.tsp.set.SetInstance;  
    import org.xml.sax.SAXException;  
      
    import javax.xml.parsers.ParserConfigurationException;  
    import java.io.IOException;  
      
    public class Main {  
        public static void main(String[] args) {  
            try {  
                Graph graph = Transformer.constructGraph(SetInstance.SIMPLE);  
                // You are ready to use the graph :)
                Transformer.print(graph); // print the graph.
            } catch (ParserConfigurationException e) {  
                e.printStackTrace();  
            } catch (SAXException e) {  
                e.printStackTrace();  
            } catch (IOException e) {  
                e.printStackTrace();  
            }  
        }  
    }

### Graph Details
The graph here is an [adjacency list](https://en.wikipedia.org/wiki/Adjacency_list) graph based on [Java collections](https://docs.oracle.com/javase/8/docs/api/java/util/Collection.html). The used Java version is Java 9. We consider the code is compatible with Java 8 too, but we are not tested the implementation on Java 8.
### Available sets
In Table 1, the description about the **mandatory sets** that need to be tested. Currently, There is no support for the optional tests. Hopefully, in the future, we will provide the fill datasets.

**Table 1.** MUST datasets.

|ID|Name|Description|
|--|--|--|
|`jo.ju.edu.tsp.set.SetInstance#SIMPLE`| simple5.xml | A symetric weighted undirected graph with **five** nodes|
|`jo.ju.edu.tsp.set.SetInstance#RL1304`| rl1304.xml | A symetric weighted undirected graph with **1304** nodes|
|`jo.ju.edu.tsp.set.SetInstance#DJB2036`| djb2036.xml | A symetric weighted undirected graph with **2036** nodes|
|`jo.ju.edu.tsp.set.SetInstance#PCB3038`| pcb3038.xml | A symetric weighted undirected graph with **3038** nodes|
|`jo.ju.edu.tsp.set.SetInstance#RL5934`| rl5934.xml | A symetric weighted undirected graph with **5934** nodes|
|`jo.ju.edu.tsp.set.SetInstance#XSC6880`| xsc6880.xml | A symetric weighted undirected graph with **6880** nodes|
|`jo.ju.edu.tsp.set.SetInstance#IDA8197`| ida8197.xml | A symetric weighted undirected graph with **8197** nodes|
|`jo.ju.edu.tsp.set.SetInstance#XMC10150`| xmc10150.xml | A symetric weighted undirected graph with **10150** nodes|
