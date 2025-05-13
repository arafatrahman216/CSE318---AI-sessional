#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <chrono>
#include<random>
#include<algorithm>
#include <numeric>
#include <iomanip>
#include <sstream>
#include <fstream>


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
    vector<Edge> edges;
public:
    int n;
    int m;
    vector <int> nodes;
    // vector<Edge> edges;
    vector<map<int, int>> adjList;  //u(start node)-> vector index, v(finish node)-> map key, w(edge weight)-> map value
    Graph(int n, int m) { 
        this->n = n;
        this->m = m;
        adjList.resize(n + 1); // 1-indexed
        for ( int i=1 ; i<= n; i++) nodes.push_back(i);
    }
    void addEdge(int u, int v, int w=0) {
        adjList[u][v] = w; 
        adjList[v][u] = w; 
        edges.push_back(Edge(u, v, w));
        if (w > maxEdge.w) {
            maxEdge.setEdge(u, v, w);
        }
        if (w < minEdge.w) {
            minEdge.setEdge(u, v, w);
        }
        
    }

    vector<Edge> getEdges() {
        return edges;
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
    // cout << "sigma: " << sigma << endl;
    return sigma;
}

void writeRowToCSV(ofstream& file, const vector<string>& row) {
    for (size_t i = 0; i < row.size(); ++i) {
        file << row[i];
        if (i != row.size() - 1) file << ",";
    }
    file << endl;
}


void writeResultsToCSV(const string& filename, const vector<vector<string>>& results) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }
    
    // Header
    file << "Problem,|V|,|E|,Randomized-1,Greedy-1,Semi-Greedy-1,Local-1,GRASP-1,Known Best Solution" << endl;
    
    // Data
    for (const auto& row : results) {
        writeRowToCSV(file, row);
    }
    
    file.close();
}



Graph readGraph(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }
    
    int V, E;
    file >> V >> E;
    
    Graph graph(V, E);
    
    for (int i = 0; i < E; ++i) {
        int u, v, w;
        file >> u >> v >> w;
        graph.addEdge(u, v, w);
    }
    
    file.close();
    return graph;
}