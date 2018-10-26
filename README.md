# TSP  
Metaheuristic Algorithms for Solving Symmetric Traveling Salesperson Problem (TSP). This project is an evaluation project for several metaheuristic algorithms on [swarm intelligence](https://en.wikipedia.org/wiki/Swarm_intelligence). The selected algorithims are:

 - Hydrological Cycle Algorithm (HCA).
 - Water Flow-Like Algorithm.
 - Intelligent Water Drops Algorithm (IWD).
 - Rain Water Algorithm.
 - River Formation Dynamics.

## How to use?
The following code demonstrates an example of using the **HCA** algorithm on **RL1304** dataset. **To know more about the Graph and the dataset details**, you should read [this page](https://github.com/amughrabi/tsp/tree/master/src/main/java/jo/ju/edu/tsp/core/xml) first.

    package jo.ju.edu.tsp;  
      
    import jo.ju.edu.tsp.algorithms.HCA;  
    import jo.ju.edu.tsp.algorithms.TSP;  
    import jo.ju.edu.tsp.set.SetInstance;  
    import org.xml.sax.SAXException;  
      
    import javax.xml.parsers.ParserConfigurationException;  
    import java.io.IOException;  
      
    public class Main {  
        public static void main(String[] args) {  
            TSP tsp = new HCA();  
            try {  
                tsp.solve(SetInstance.RL1304);  
            } catch (ParserConfigurationException e) {  
                e.printStackTrace();  
            } catch (SAXException e) {  
                e.printStackTrace();  
            } catch (IOException e) {  
                e.printStackTrace();  
            }  
        }  
    }
## Measurements
TBD
## The Team 

 - Ahmad Al Mughrabi.
 - Raneem abuzayed.
 - Ali
 - Nancy
