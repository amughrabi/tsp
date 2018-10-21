# tsp
Metaheuristic Algorithms for Solving Symmetric Traveling Salesperson Problem (TSP)


for f in /home/amughrabi/projects/tsp/src/main/resources/all-tsp/*.tsp; do if [ ! -f ${f:0:-4}.xml ]; then ./Debug/TransformTSPLIB $f ${f:0:-4}.xml; fi done