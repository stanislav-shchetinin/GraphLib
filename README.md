# GraphLib
Library for working with graphs
# How to use
To use the library, add the files `FunctionForGraph.cpp` and `graphlib.h` to the same folder with the executable file and write in your program: 
```C++
#include "graphlib.h"
```
# Library description
## Structures:
  * `Edge` structure. Stores the vertex numbers of the edge and the weight of the edge. Contains 3 constructors (empty, unweighted edge input, weighted edge input). 5 operators overloaded: equality (checks for equality of vertices and weight), input (first vertex, second, weight respectively), output (format {first vertex number, second vertex number, edge weight}), decrement (reduces vertex numbers by 1) , “less than” (needed to sort edges).
  * A structure for the vertex that the edge goes to in the adjacency list (`SecondVert`). Stores the vertex number and edge weight. Contains 3 constructors (empty, unweighted edge input, weighted edge input). 1 operator overloaded: “less than” (needed to sort vertices)
  * A structure that stores the answer for functions on the shortest paths (`ResultShortWay`). Stores the shortest path between the start and end vertices, an array of shortest paths from the start to the rest, the shortest path from the start to the end vertex itself.
  * A structure for storing an ancestor in the dynamic programming traveling salesman problem (`ParentSalesman`). Stores in itself a mask, and the last peak in a way. The "not equal" operator is overloaded.
## Algorithms:
  * Vertex compression (`vertCompression`). If you need to convert the graph to an adjacency list and if there are vertex numbers > 10^6, then you need to compress the vertices. The input is a list of edges and the converted list of edges is returned.
  * Convert from list of edges to adjacency list (`fromListOfEdgesToListOfAdjacencies`). It is required to specify a directed graph or not (default is not).
  * Convert from list of adjacencies to list of edges (`fromListOfAdjacenciesToListOfEdges`).
  * Convert from list of edges to adjacency matrix (`fromListOfEdgesToAdjacencyMatrix`). It is required to specify the value that will be at the intersection of the column and row if there is no edge between them (default 10 ^ 9) and the orientation of the graph.
  * Convert from adjacency matrix to list of edges (`fromAdjacencyMatrixToListOfEdges`).
  * Convert adjacency list to adjacency matrix (`fromListOfAdjacenciesToAdjacencyMatrix`).
  * Convert from adjacency matrix to adjacency list (`fromAdjacencyMatrixToListOfAdjacencies`)
  * `shortWayDijkstra`. Finding the shortest path between two vertices using Dijkstra's algorithm (edges must be of non-negative weight). The input is a start vertex, an end vertex, and a graph in the form of an adjacency matrix. Returns ResultShortWay.
  * `shortWayFordBellman`. Finding the shortest path between two vertices using the Ford-Bellman algorithm (no limit on all edges, but if there are no negative edges, then Dijkstra's algorithm is better). The input is a start vertex, an end vertex, and a graph in the form of an adjacency matrix. Returns ResultShortWay.
  * shortWayFloyd. The input is an adjacency matrix, the matrix of the shortest paths from each vertex to each is returned (there are no restrictions on the weight of edges).
  * `shortWayBFS`. Finding the shortest path between two BFS vertices (edges must be unweighted, i.e. equal to 1). The input is a start vertex, an end vertex, and a graph in the form of an adjacency matrix. Returns ResultShortWay.
about salesman Exact. More accurate solution of the traveling salesman problem by means of dynamic programming (if the number of vertices is more than 20, you should not use it)
about salesman fast. Faster solution of the traveling salesman problem using simulated annealing.
The code contains the constants `NO_WAY` and `NEGATIVE_CYCLE` equal to 9 * 10^18 and -9 * 10^18 mean the absence of a path or edge and the inclusion of a vertex in a negative cycle, respectively.

In order to make sure that the codes are correct, they were tested on the following resources: codeforces.com + informatics.msk.ru + algocode.ru.
The code design was made according to the recommendations of Stanford University.
