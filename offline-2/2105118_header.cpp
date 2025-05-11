#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <chrono>
#include<random>
#include<algorithm>

using namespace std;


class Edge{
public:
    int u, v;
    int w;
    Edge(int u=0, int v=0, int w=0) {
        this->u = u;
        this->v = v;
        this->w = w;
    }

    void setEdge(int u, int v, int w) {
        this->u = u;
        this->v = v;
        this->w = w;
    }

    bool operator<(const Edge& other) const {
        return w < other.w; // For max-heap, use > for min-heap
    }
    bool operator>(const Edge& other) const {
        return w > other.w; // For max-heap, use < for min-heap
    }
    bool operator==(const Edge& other) const {
        return (u == other.u && v == other.v && w == other.w);
    }
};

bool compareEdges(const Edge& a, const Edge& b) {
    return a.w < b.w; // Sort in descending order of weight
}

class Graph{
private:
    Edge maxEdge=Edge(0, 0, INT32_MIN);
    Edge minEdge=Edge(0, 0, INT32_MAX);
public:
    int n;
    int m;
    vector <int> nodes;
    // vector<Edge> edges;
    priority_queue<Edge, vector<Edge>, decltype(&compareEdges)> greedyEdges;
    vector<map<int, int>> adjList;  //u(start node)-> vector index, v(finish node)-> map key, w(edge weight)-> map value
    Graph(int n, int m) : greedyEdges(compareEdges) { // Initialize with compareEdges
        this->n = n;
        this->m = m;
        adjList.resize(n + 1); // 1-indexed
        for ( int i=1 ; i<= n; i++) nodes.push_back(i);
    }
    void addEdge(int u, int v, int w=0) {
        adjList[u][v] = w; 
        adjList[v][u] = w; 
        if (w > maxEdge.w) {
            maxEdge.setEdge(u, v, w);
        }
        if (w < minEdge.w) {
            minEdge.setEdge(u, v, w);
        }
        
    }

    int getWeight(int u, int v) {
        auto it = adjList[u].find(v);
        if (it != adjList[u].end()) {
            return it->second;
        }
        return 0; // Return 0 if no edge exists
    }

    vector<int> getAdjNodes(int u) {
        vector<int> adjNodes;
        for (const auto& pair : adjList[u]) {
            adjNodes.push_back(pair.first);
        }
        return adjNodes;
    }

    priority_queue<Edge, vector<Edge>, decltype(&compareEdges)> getEdgePQ() {
        if (!greedyEdges.empty()) {
            return greedyEdges;
        }
        for (int u = 1; u <= n; u++) {
            for (const auto& pair : adjList[u] ) {
                cout << "Greedy algorithm started..." << endl;
                int v = pair.first;
                int w = pair.second;
                cout << "u: " << u << ", v: " << v << ", w: " << w << endl;
                if (u < v) { // Avoid duplicates
                    greedyEdges.push(Edge(u, v, w));
                    cout << "Greedy algorithm ended..." << endl;
                }
            }
        }
        return greedyEdges;
    }

    const Edge& getMaxEdge() const {
        return maxEdge;
    }
    
    const vector<map<int, int>>& getAdjList() const {
        return adjList;
    }

    vector <int > getNodes(){
        return nodes;
    }





    
};

int calculateSigma( int v, const unordered_set<int>& X, Graph graph) {
    int sigma = 0;
    for (int u : X) {
        sigma += graph.getWeight(v, u);
    }
    return sigma;
}