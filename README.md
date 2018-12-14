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
## Systems Prerequisites
* [JDK](http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html) 1.8+
* [Maven](https://maven.apache.org/)
* [Git](https://git-scm.com/) - Optional in case of download the project
## Run the project
* Download or clone the project
* Open the source code in an IDE; for example, [IntelliJ](https://www.jetbrains.com/idea/)
* Go to any algorithm; for example, to run the HCA, you want */tsp/src/main/java/jo/ju/edu/tsp/algorithms/HCA.java*
* Right click in the HCA class -> Click on "Run'HCA.main()'" (Same thing for IWD and WFA java files)

![](https://raw.githubusercontent.com/amughrabi/tsp/master/src/test/resources/ezgif-1-96b68ef0ced3.gif)

## The Team 
 - Ahmad Al Mughrabi.
 - Raneem abuzayed.
 - Ali Al Sarhan
 - Nancy
