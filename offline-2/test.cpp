#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <random>
#include <chrono>
#include <numeric>
#include <iomanip>
#include <sstream>

using namespace std;

class Graph {
private:
    int V; // Number of vertices
    int E; // Number of edges
    vector<map<int, int>> adj; // Adjacency list with weights

public:
    Graph(int vertices) : V(vertices), E(0), adj(vertices + 1) {}

    void addEdge(int u, int v, int weight) {
        adj[u][v] = weight;
        adj[v][u] = weight;
        E++;
    }

    int getVertices() const { return V; }
    int getEdges() const { return E; }
    const vector<map<int, int>>& getAdjList() const { return adj; }

    int getWeight(int u, int v) const {
        auto it = adj[u].find(v);
        return it != adj[u].end() ? it->second : 0;
    }
};

class MaxCutSolver {
private:
    mt19937 rng;
    Graph graph;

public:
    MaxCutSolver(const Graph& g) : graph(g) {
        rng.seed(chrono::steady_clock::now().time_since_epoch().count());
    }

    // Randomized Algorithm
    pair<int, pair<set<int>, set<int>>> randomizedMaxCut(int iterations = 1) {
        int bestCut = 0;
        set<int> bestX, bestY;
        cout<<"Randomized algorithm started..."<<endl;
        cout<<"Iterations: "<<iterations<<endl;

        for (int i = 0; i < iterations; ++i) {
            set<int> X, Y;
            for (int v = 1; v <= graph.getVertices(); ++v) {
                if (uniform_int_distribution<int>(0, 1)(rng)) {
                    X.insert(v);
                } else {
                    Y.insert(v);
                }
            }

            int currentCut = calculateCutWeight(X, Y);
            if (currentCut > bestCut) {
                bestCut = currentCut;
                bestX = X;
                bestY = Y;
            }
        }

        return {bestCut, {bestX, bestY}};
    }

    // Greedy Algorithm
    pair<int, pair<set<int>, set<int>>> greedyMaxCut() {
        set<int> X, Y;
        cout<<"Greedy algorithm started..."<<endl;

        // Find the edge with maximum weight
        int maxWeight = 0;
        pair<int, int> maxEdge = {0, 0};
        
        for (int u = 1; u <= graph.getVertices(); ++u) {
            for (const auto& neighbor : graph.getAdjList()[u]) {
                int v = neighbor.first;
                int weight = neighbor.second;
                if (weight > maxWeight && u < v) {
                    maxWeight = weight;
                    maxEdge = {u, v};
                }
            }
        }
        
        if (maxEdge.first == 0) return {0, {X, Y}}; // empty graph
        
        X.insert(maxEdge.first);
        Y.insert(maxEdge.second);
        
        // Assign remaining vertices
        for (int v = 1; v <= graph.getVertices(); ++v) {
            if (v == maxEdge.first || v == maxEdge.second) continue;
            
            int gainX = 0, gainY = 0;
            
            // Calculate gain if added to X
            for (int neighbor : Y) {
                gainX += graph.getWeight(v, neighbor);
            }
            
            // Calculate gain if added to Y
            for (int neighbor : X) {
                gainY += graph.getWeight(v, neighbor);
            }
            
            if (gainX >= gainY) {
                X.insert(v);
            } else {
                Y.insert(v);
            }
        }
        
        int cutWeight = calculateCutWeight(X, Y);
        return {cutWeight, {X, Y}};
    }

    // Semi-Greedy Algorithm
    pair<int, pair<set<int>, set<int>>> semiGreedyMaxCut(double alpha) {
        set<int> X, Y;
        cout<<"Semi-Greedy algorithm started..."<<endl;
        // Find the edge with maximum weight
        int maxWeight = 0;
        pair<int, int> maxEdge = {0, 0};
        
        for (int u = 1; u <= graph.getVertices(); ++u) {
            for (const auto& neighbor : graph.getAdjList()[u]) {
                int v = neighbor.first;
                int weight = neighbor.second;
                if (weight > maxWeight && u < v) {
                    maxWeight = weight;
                    maxEdge = {u, v};
                }
            }
        }
        
        if (maxEdge.first == 0) return {0, {X, Y}}; // empty graph
        
        X.insert(maxEdge.first);
        Y.insert(maxEdge.second);
        
        // Assign remaining vertices
        while (X.size() + Y.size() < graph.getVertices()) {
            vector<pair<int, pair<int, int>>> candidates; // (gain, (v, partition))
            
            // Calculate gains for all unassigned vertices
            for (int v = 1; v <= graph.getVertices(); ++v) {
                if (X.count(v) || Y.count(v)) continue;
                
                int gainX = 0, gainY = 0;
                
                // Calculate gain if added to X
                for (int neighbor : Y) {
                    gainX += graph.getWeight(v, neighbor);
                }
                
                // Calculate gain if added to Y
                for (int neighbor : X) {
                    gainY += graph.getWeight(v, neighbor);
                }
                
                candidates.emplace_back(gainX, make_pair(v, 0)); // 0 for X
                candidates.emplace_back(gainY, make_pair(v, 1)); // 1 for Y
            }
            
            if (candidates.empty()) break;
            
            // Sort candidates by gain in descending order
            sort(candidates.begin(), candidates.end(), 
                [](const pair<int, pair<int, int>>& a, const pair<int, pair<int, int>>& b) {
                    return a.first > b.first;
                });
            
            // Calculate RCL
            int minGain = candidates.back().first;
            int maxGain = candidates.front().first;
            int threshold = minGain + alpha * (maxGain - minGain);
            
            vector<pair<int, pair<int, int>>> RCL;
            for (const auto& candidate : candidates) {
                if (candidate.first >= threshold) {
                    RCL.push_back(candidate);
                }
            }
            
            if (RCL.empty()) {
                // Fallback to random assignment if RCL is empty
                for (int v = 1; v <= graph.getVertices(); ++v) {
                    if (!X.count(v) && !Y.count(v)) {
                        if (uniform_int_distribution<int>(0, 1)(rng)) {
                            X.insert(v);
                        } else {
                            Y.insert(v);
                        }
                    }
                }
                break;
            }
            
            // Randomly select from RCL
            uniform_int_distribution<int> dist(0, RCL.size() - 1);
            auto selected = RCL[dist(rng)];
            int v = selected.second.first;
            int partition = selected.second.second;
            
            if (partition == 0) {
                X.insert(v);
            } else {
                Y.insert(v);
            }
        }
        
        int cutWeight = calculateCutWeight(X, Y);
        return {cutWeight, {X, Y}};
    }

    // Local Search
    pair<int, pair<set<int>, set<int>>> localSearch(set<int> X, set<int> Y) {
        bool improved = true;
        int currentCut = calculateCutWeight(X, Y);
        cout<<"Local Search algorithm started..."<<endl;
        while (improved) {
            improved = false;
            
            // Try moving each vertex to the opposite partition
            for (int v = 1; v <= graph.getVertices(); ++v) {
                int delta = 0;
                
                if (X.count(v)) {
                    // Calculate delta if moved to Y
                    for (int neighbor : X) {
                        if (neighbor != v) delta -= graph.getWeight(v, neighbor);
                    }
                    for (int neighbor : Y) {
                        delta += graph.getWeight(v, neighbor);
                    }
                    
                    if (delta > 0) {
                        X.erase(v);
                        Y.insert(v);
                        currentCut += delta;
                        improved = true;
                    }
                } else {
                    // Calculate delta if moved to X
                    for (int neighbor : Y) {
                        if (neighbor != v) delta -= graph.getWeight(v, neighbor);
                    }
                    for (int neighbor : X) {
                        delta += graph.getWeight(v, neighbor);
                    }
                    
                    if (delta > 0) {
                        Y.erase(v);
                        X.insert(v);
                        currentCut += delta;
                        improved = true;
                    }
                }
            }
        }
        
        return {currentCut, {X, Y}};
    }

    // GRASP Algorithm
    pair<int, pair<set<int>, set<int>>> graspMaxCut(double alpha, int maxIterations) {
        int bestCut = 0;
        set<int> bestX, bestY;
        cout<<"GRASP algorithm started..."<<endl;
        for (int i = 0; i < maxIterations; ++i) {
            // Construction phase
            auto [cut1, partitions1] = semiGreedyMaxCut(alpha);
            auto [X, Y] = partitions1;
            
            // Local search phase
            auto [cut2, partitions2] = localSearch(X, Y);
            
            if (cut2 > bestCut) {
                bestCut = cut2;
                bestX = partitions2.first;
                bestY = partitions2.second;
            }
        }
        
        return {bestCut, {bestX, bestY}};
    }

private:
    int calculateCutWeight(const set<int>& X, const set<int>& Y) const {
        int weight = 0;
        for (int u : X) {
            for (const auto& neighbor : graph.getAdjList()[u]) {
                int v = neighbor.first;
                if (Y.count(v)) {
                    weight += neighbor.second;
                }
            }
        }
        return weight;
    }
};

Graph readGraph(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }
    
    int V, E;
    file >> V >> E;
    
    Graph graph(V);
    
    for (int i = 0; i < E; ++i) {
        int u, v, w;
        file >> u >> v >> w;
        graph.addEdge(u, v, w);
    }
    
    file.close();
    return graph;
}

void writeResultsToCSV(const string& filename, const vector<vector<string>>& results) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }
    
    // Header
    file << "Problem,|V|,|E|,Randomized,Greedy,Semi-Greedy (α=0.5),Local Search (Greedy),GRASP (α=0.5 & iter=100),Known Best Solution" << endl;
    
    // Data
    for (const auto& row : results) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            if (i != row.size() - 1) file << ",";
        }
        file << endl;
    }
    
    file.close();
}

int main() {
    // Example usage
    vector<vector<string>> results;
    int knownBests [54] = {12078, 12084, 12077,0,0,0,0,0,0,0,627,621,645,3187,3169,3172,0,0,0,0,0,14123,14129,14131,0,0,0,0,0,0,0,1560,1537,1541,
        8000,7996,8009,0,0,0,0,0,7027,7022,7020,0,0,6000,6000,5988};
    // For each benchmark graph (you would loop through all 54)
    for (int g = 1; g <= 54; g++) { // Just testing with G1 for this example
        string filename = "set1/g" + to_string(g) + ".rud";
        cout << "Processing graph: " << filename << endl;
        Graph graph = readGraph(filename);
        MaxCutSolver solver(graph);
        
        // Known best solution (from the assignment)
        int knownBest = knownBests[g - 1];
        // Add other known best solutions...
        
        // Run all algorithms
        auto [randomCut, _] = solver.randomizedMaxCut(1);
        auto [greedyCut, __] = solver.greedyMaxCut();
        auto [semiGreedyCut, ___] = solver.semiGreedyMaxCut(0.75);
        auto [localSearchCut, ____] = solver.localSearch(solver.greedyMaxCut().second.first, solver.greedyMaxCut().second.second);
        auto [graspCut, _____] = solver.graspMaxCut(0.75, 1);
        
        // Store results
        results.push_back({
            "G" + to_string(g),
            to_string(graph.getVertices()),
            to_string(graph.getEdges()),
            to_string(randomCut),
            to_string(greedyCut),
            to_string(semiGreedyCut),
            to_string(localSearchCut),
            to_string(graspCut),
            to_string(knownBest)
        });
    }
    
    // Write results to CSV
    writeResultsToCSV("2105118.csv", results);
    
    return 0;
}