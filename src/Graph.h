#ifndef GRAPH_H
#define GRAPH_H


#include <iostream>
#include <vector>
#include <list>
#include <cmath>
#include <iomanip> 
#include <limits>
#include "Coords.h"  
#include "PriorityQueue.h" 



/*
 * Class to store edges in the adjacency list.
 * The starting vertex is the index in the adjacency list vector (adj).
 * The Edge object only contains the destination vertex and the edge cost (weight).
 */
class Edge {
public:
    Edge() {}
    Edge(int v, double cost){
        this->v = v;
        this->cost = cost;
    }

    int getV() const { 
        return this->v;
    }
    
    double getCost() const { 
        return this->cost;
    }

private:
    int v;       // Destination vertex
    double cost; // Edge cost (distance)
};

class Graph {
public:
    // Constructor: Initializes the graph
    Graph(int numSatellites, int numVertices, const std::vector<Coords>& outpostCoords);

    // Adds an edge to the graph (used internally by constructor)
    // NOTE: This implementation assumes edges are added only once for pairs (i, j) where i < j.
    void addEdge(int v, int u, double cost);

    // Prints the adjacency list representation of the graph (for debugging)
    void printGraph() const;

    // Computes the Minimum Spanning Tree using Prim's algorithm
    void computePrimMST();

    // Calculates the minimum radio distance 'D' required after placing satellites
    double determineMinRadioRange(); 

private:
    // Runs the core Prim's algorithm starting from vertex s
    void prim(int s);
    // Scans vertex v during Prim's algorithm execution
    void scan(int v);

    std::vector<std::list<Edge>> adj_list; // Adjacency list
    int V; // Number of vertices (outposts)
    int S; // Number of available satellite channels

    // --- Data structures for Prim's Algorithm ---
    std::vector<int> edgeTo;     // Vector containing the starting vertices of the edges of the MST
    std::vector<double> distTo;  // Stores the weight of the edge stored in edgeTo[]
    std::vector<bool> marked;    // marked[v] = true if v is in the , avoids the creation of cycles
    PriorityQueue pq;            // Min-priority queue of vertices not yet in MST, ordered by distTo[]
    PriorityQueue maxEdgeHeap;   // Max-priority queue for maximum cost arc extraction
                                 
};

/*
 * Constructor: Initializes a graph with V nodes (outposts).
 * It calculates Euclidean distances and builds an adjacency list representing a complete graph.
 */
Graph::Graph(int numSatellites, int numVertices, const std::vector<Coords>& outpostCoords)
    : V(numVertices), S(numSatellites), adj_list(numVertices),
      pq(), maxEdgeHeap()
{
    // Calculate Euclidean distances and build the graph representation
    double distance;
    for (int i = 0; i < V; ++i) {
        for (int j = i + 1; j < V; ++j) { // Iterate only for j > i
            // Calculate distance between outpost i and outpost j
            distance = std::sqrt(std::pow((outpostCoords[i].getX() - outpostCoords[j].getX()), 2) +
                                 std::pow((outpostCoords[i].getY() - outpostCoords[j].getY()), 2));
            this->addEdge(i, j, distance);
        }
    }
}

// Adds edge v -> u with given cost to v's adjacency list
void Graph::addEdge(int v_start, int u_end, double cost) {
    Edge edge(u_end, cost);
    this->adj_list[v_start].push_front(edge);
}



void Graph::printGraph() const {
    std::cout << std::endl;
    std::cout << "Adjacency List:" << std::endl;
    for (int i = 0; i < this->adj_list.size(); ++i) {
        std::cout << i << ": ";
        for (std::list<Edge>::const_iterator pos = this->adj_list[i].begin(); pos != this->adj_list[i].end(); ++pos) {
            // Output format: DestVertex(Cost)
            std::cout << "  " << pos->getV() << "->" << std::fixed << std::setprecision(2) << pos->getCost();;
        }
        std::cout << std::endl;
    }
}

void Graph::computePrimMST() {
    this->edgeTo.resize(V);
    this->distTo.resize(V);
    this->marked.resize(V);
    this->pq = PriorityQueue(V, false); 
    this->maxEdgeHeap = PriorityQueue(V, true); 

    //Initialization of the vector of distances to inf
    for(int i=0; i<V;i++){
        this->distTo[i]=numeric_limits<double>::infinity();
    }
    //invocation of the prim algorithm on the first node
    prim(0);
}


void Graph::prim(int s) {
    this->distTo[s] = 0.0;
    this->edgeTo[s] = 0; 
    this->pq.insert(s, this->distTo[s]);

    //Scan done before the while to not insert the arc from the source vertex to itself in the priority queue
    scan(this->pq.deleteM()); 

    while(!this->pq.isEmpty()){
        //Extraction of the arrival node of the minimum weight arc
        int v= this->pq.deleteM();
        //keeps track of MST arcs sorted in ascending order of cost
        this->maxEdgeHeap.insert(v,this->distTo[v]);

        //cout <<edgeTo[v] << " -> " << v << " c: " << distTo[v] << endl;

        scan(v);
    } 
}

//visit the vertices adjacent to v
void Graph::scan(int v) {
    this->marked[v] = true;

    for (std::list<Edge>::const_iterator adj = this->adj_list[v].begin(); adj != this->adj_list[v].end(); ++adj) {
        int w = adj->getV();        
        double cost = adj->getCost(); 

        // If w is already in the MST, skip this edge (prevents cycles)
        if (this->marked[w]) continue;

        // Relaxation step: If found a shorter path to w via v
        if (cost < this->distTo[w]) {
            this->distTo[w] = cost;   
            this->edgeTo[w] = v;      

            // Update w's priority in the min-priority queue
            if (this->pq.contains(w)) {
                // Assuming changeKey exists and works correctly
                this->pq.decreaseKey(w, this->distTo[w]);
            } else {
                // Otherwise, insert w into the priority queue
                this->pq.insert(w, this->distTo[w]);
            }
        }
    }
}



// Calculates the minimum radio distance D required after placing S satellites
// strategically on the endpoints of the heaviest MST edges.
double Graph::determineMinRadioRange(){
    //vector to mark the vertices on which we arranged the satellite channels
    vector<bool> hasSatellite(V);
    //as long as we can assign satellite channels we take the extremes of the MST's maximum weight arc
    while(this->S != 0){
        int w=this->maxEdgeHeap.getRoot(); //arrival vertex
        int v=this->edgeTo[w];  //starting vertex
        
        /*if v does not have a satellite channel, it is assigned by marking it as true 
        and also the number of remaining satellite channels is decreased*/
        if(!hasSatellite[v]){
            hasSatellite[v]=true;
            this->S--;
        }
        /*same reasoning for w, further considering that if the number of satellite channels is exhausted, 
        we cannot assign it to w*/
        if(!hasSatellite[w] && this->S>=1){
            hasSatellite[w]=true;
            this->S--;
        }
        /*once both ends are marked, we can extract the edge from the tail at priority 
        to consider the next highest weight edge at the next interaction*/
        if(hasSatellite[v] && hasSatellite[w]){
            this->maxEdgeHeap.deleteM();
        }
    }
    //Returns the weight of the remaining maximum cost arc, corresponding to the minimum D required to connect the network
    return this->maxEdgeHeap.getValue(this->maxEdgeHeap.getRoot()); 
}

#endif // GRAPH_H