#include <bits/stdc++.h>
#include <random>
using namespace std;

class Edge {
public:
    int u, v, w;
    Edge(int u, int v, int w) : u(u), v(v), w(w) {}
};

class Graph {
    int n, m;
    vector<Edge> edges;
    vector<vector<pair<int, int>>> adj;

public:
    Graph(int n, int m) : n(n), m(m) {
        adj.resize(n);
    }

    void addEdge(int u, int v, int w) {
        edges.emplace_back(u, v, w);
        adj[u].emplace_back(v, w);
        adj[v].emplace_back(u, w);
    }

    const vector<Edge>& getEdges() const { return edges; }
    const vector<vector<pair<int, int>>>& getAdj() const { return adj; }
    int getN() const { return n; }

    friend Graph readFromFile(const string& filename);
};

Graph readFromFile(const string& filename) {
    ifstream file(filename);
    int n, m;
    file >> n >> m;
    Graph g(n, m);
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        file >> u >> v >> w;
        g.addEdge(u - 1, v - 1, w);  // Convert to 0-based indexing
    }
    return g;
}

// ------------------ Randomized Max-Cut ------------------

// int randomizedMaxCut(const Graph& g, int iterations = 100) {
    
//     int totalCutWeight = 0;
//     int n = g.getN();
//     const auto& edges = g.getEdges();

//     for (int i = 0; i < iterations; ++i) {
//         vector<bool> inSet(n);
//         for (int v = 0; v < n; ++v)
//             inSet[v] = rand() % 2;

//         int cutWeight = 0;
//         for (const auto& edge : edges) {
//             if (inSet[edge.u] != inSet[edge.v])
//                 cutWeight += edge.w;
//         }

//         totalCutWeight += cutWeight;
//     }

//     return totalCutWeight / iterations;
// }




int randomizedMaxCut(const Graph& g, int iterations = 100) {
    int totalCutWeight = 0;
    int n = g.getN();
    const auto& edges = g.getEdges();

    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, 1);

    for (int i = 0; i < iterations; ++i) {
        vector<bool> inSet(n);
        for (int v = 0; v < n; ++v)
            inSet[v] = dist(rng);  // modern RNG

        int cutWeight = 0;
        for (const auto& edge : edges) {
            if (inSet[edge.u] != inSet[edge.v])
                cutWeight += edge.w;
        }

        totalCutWeight += cutWeight;
    }

    int avg = (iterations > 1) ? totalCutWeight / iterations : totalCutWeight;
    return max(0, avg);  // ensure non-negative
}

// ------------------ Greedy Max-Cut ------------------

int greedyMaxCut(const Graph& g) {
    int n = g.getN();
    const auto& edges = g.getEdges();
    const auto& adj = g.getAdj();

    vector<int> partition(n, -1);

    int maxWeight = -1, startU = -1, startV = -1;
    for (const auto& e : edges) {
        if (e.w > maxWeight) {
            maxWeight = e.w;
            startU = e.u;
            startV = e.v;
        }
    }
    partition[startU] = 0;
    partition[startV] = 1;

    for (int v = 0; v < n; ++v) {
        if (partition[v] != -1) continue;

        int scoreA = 0, scoreB = 0;
        for (const auto& p : adj[v]) {
            int nei = p.first;
            int w = p.second;
            if (partition[nei] == 0) scoreB += w;
            else if (partition[nei] == 1) scoreA += w;
        }

        partition[v] = (scoreA > scoreB) ? 0 : 1;
    }

    int cutWeight = 0;
    for (const auto& e : edges) {
        if (partition[e.u] != partition[e.v])
            cutWeight += e.w;
    }

    return cutWeight;
}




int semiGreedyMaxCut(const Graph& g, double alpha) {
    int n = g.getN();
    const auto& adj = g.getAdj();
    vector<int> partition(n, -1);

    const auto& edges = g.getEdges();
    int maxWeight = -1, startU = -1, startV = -1;
    for (const auto& e : edges) {
        if (e.w > maxWeight) {
            maxWeight = e.w;
            startU = e.u;
            startV = e.v;
        }
    }
    partition[startU] = 0;
    partition[startV] = 1;

    set<int> unassigned;
    for (int i = 0; i < n; ++i) {
        if (partition[i] == -1) unassigned.insert(i);
    }

    while (!unassigned.empty()) {
        vector<pair<int, int>> scores;

        int wmin = INT_MAX, wmax = INT_MIN;
        for (int v : unassigned) {
            int scoreA = 0, scoreB = 0;
            for (const auto& p : adj[v]) {
                int nei = p.first;
                int w = p.second;
                if (partition[nei] == 0) scoreB += w;
                else if (partition[nei] == 1) scoreA += w;
            }
            int val = max(scoreA, scoreB);
            scores.emplace_back(val, v);
            wmin = min(wmin, val);
            wmax = max(wmax, val);
        }

        int mu = wmin + alpha * (wmax - wmin);
        vector<int> rcl;
        for (const auto& p : scores) {
            int val = p.first;
            int v = p.second;
            if (val >= mu) rcl.push_back(v);
        }

        if (rcl.empty()) rcl.push_back(scores[0].second);

        int chosen = rcl[rand() % rcl.size()];

        int scoreA = 0, scoreB = 0;
        for (const auto& p : adj[chosen]) {
            int nei = p.first;
            int w = p.second;
            if (partition[nei] == 0) scoreB += w;
            else if (partition[nei] == 1) scoreA += w;
        }
        partition[chosen] = (scoreA > scoreB) ? 0 : 1;
        unassigned.erase(chosen);
    }

    int cutWeight = 0;
    for (const auto& e : edges) {
        if (partition[e.u] != partition[e.v])
            cutWeight += e.w;
    }
    cout<<"Cut weight: " << cutWeight << endl;
    return cutWeight;
}



pair<int, int> localSearchMaxCut(const Graph& g, vector<int>& partition) {
    const auto& adj = g.getAdj();
    int n = g.getN();
    bool improved;
    int iterationCount = 0;

    do {
        improved = false;
        for (int v = 0; v < n; ++v) {
            int gain = 0;
            for (const auto& p : adj[v]) {
                int nei = p.first, w = p.second;
                if (partition[v] == partition[nei]) gain += w;
                else gain -= w;
            }
            if (gain > 0) {
                partition[v] ^= 1;
                improved = true;
            }
        }
        if (improved) iterationCount++;
    } while (improved);

    int cutWeight = 0;
    const auto& edges = g.getEdges();
    for (const auto& e : edges) {
        if (partition[e.u] != partition[e.v])
            cutWeight += e.w;
    }

    return {cutWeight, iterationCount};
}


// ------------------ GRASP Max-Cut ------------------

pair<int,int> graspMaxCut(const Graph& g, double alpha, int iterations = 100) {
    int bestCut = 0;
    int totallocalIterations = 0;
    vector<int> bestPartition;
    for (int i = 0; i < iterations; ++i) {
        // Constructive: Semi-Greedy
        int n = g.getN();
        const auto& adj = g.getAdj();
        vector<int> partition(n, -1);

        const auto& edges = g.getEdges();
        int maxWeight = -1, startU = -1, startV = -1;
        for (const auto& e : edges) {
            if (e.w > maxWeight) {
                maxWeight = e.w;
                startU = e.u;
                startV = e.v;
            }
        }
        partition[startU] = 0;
        partition[startV] = 1;

        set<int> unassigned;
        for (int i = 0; i < n; ++i) {
            if (partition[i] == -1) unassigned.insert(i);
        }

        while (!unassigned.empty()) {
            vector<pair<int, int>> scores;
            int wmin = INT_MAX, wmax = INT_MIN;

            for (int v : unassigned) {
                int scoreA = 0, scoreB = 0;
                for (const auto& p : adj[v]) {
                    int nei = p.first;
                    int w = p.second;
                    if (partition[nei] == 0) scoreB += w;
                    else if (partition[nei] == 1) scoreA += w;
                }
                int val = max(scoreA, scoreB);
                scores.emplace_back(val, v);
                wmin = min(wmin, val);
                wmax = max(wmax, val);
            }

            int mu = wmin + alpha * (wmax - wmin);
            vector<int> rcl;
            for (const auto& p : scores) {
                if (p.first >= mu) rcl.push_back(p.second);
            }

            if (rcl.empty()) rcl.push_back(scores[0].second);

            int chosen = rcl[rand() % rcl.size()];
            int scoreA = 0, scoreB = 0;
            for (const auto& p : adj[chosen]) {
                int nei = p.first;
                int w = p.second;
                if (partition[nei] == 0) scoreB += w;
                else if (partition[nei] == 1) scoreA += w;
            }

            partition[chosen] = (scoreA > scoreB) ? 0 : 1;
            unassigned.erase(chosen);
        }

        // Local search refinement
        pair<int,int>localresult = localSearchMaxCut(g, partition);
        int cut = localresult.first;
        int localIterations = localresult.second;
        totallocalIterations += localIterations;
        if (cut > bestCut) {
            bestCut = cut;
            bestPartition = partition;
        }
    }
    totallocalIterations=totallocalIterations/iterations;
    return {bestCut, totallocalIterations};  // Return best cut and total local iterations  
}



// ------------------ main function ------------------

void writeResultsToCSV(const string& filename, const vector<vector<string>>& data) {
    ofstream out(filename);
    out << "Name,|V|,|E|,Randomized-1,Greedy-1,Semi-greedy-1,Local-1 avg,Local-1 iterations,GRASP-1 best,GRASP-1 iterations,Known best\n";
    for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            out << row[i];
            if (i != row.size() - 1) out << ",";
        }
        out << "\n";
    }
    out.close();
}

int main() {
    srand(time(0));
    vector<vector<string>> results;

    // Known bests (partial list from assignment)
    map<int, int> knownBest = {
        {1, 12078},{2,12084},{3,12077},{11,627},{12,621},{13,645},{14,3187},{15,3169},{16,3172},{22,14123},{23,14129},{24,14131},{32,1560},{33,1537},{34,1541},{35,8000},{36,7996},{37,8009},{43,7027},{44,7022},{45,7020},{48,6000},{49,6000},{50,5988}, {22, 14123}, {43, 7027}
    };

    for (int g = 1; g <= 54; ++g) {
        string filename = "set1/g" + to_string(g) + ".rud";
        ifstream test(filename);
        if (!test) {
            cerr << "⚠️ File not found: " << filename << endl;
            continue;
        }

        Graph graph = readFromFile(filename);

        int n = graph.getN();
        int m = graph.getEdges().size();

        // Run Randomized (1 iteration)
        int randomized = randomizedMaxCut(graph, 1);

        // Run Greedy
        int greedy = greedyMaxCut(graph);

        // Semi-Greedy
        double alpha = 0.75;
        int semiGreedy = semiGreedyMaxCut(graph, alpha);

        // Local Search from Greedy partition
        vector<int> partition(n, -1);
        const auto& edges = graph.getEdges();
        const auto& adj = graph.getAdj();
        int maxWeight = -1, startU = -1, startV = -1;
        for (const auto& e : edges) {
            if (e.w > maxWeight) {
                maxWeight = e.w;
                startU = e.u;
                startV = e.v;
            }
        }
        partition[startU] = 0;
        partition[startV] = 1;
        for (int v = 0; v < n; ++v) {
            if (partition[v] != -1) continue;
            int scoreA = 0, scoreB = 0;
            for (const auto& p : adj[v]) {
                int nei = p.first;
                int w = p.second;
                if (partition[nei] == 0) scoreB += w;
                else if (partition[nei] == 1) scoreA += w;
            }
            partition[v] = (scoreA > scoreB) ? 0 : 1;
        }
        pair<int, int> localResult = localSearchMaxCut(graph, partition);
        int localCut = localResult.first;
        int localIters = localResult.second;


        // GRASP (1 iteration)
        pair<int,int> grasp = graspMaxCut(graph, alpha, 50);
        int graspCut = grasp.first;
        int graspIters = grasp.second;

        // Known best or 0
        int bestKnown = knownBest.count(g) ? knownBest[g] : 0;

        results.push_back({
            "G" + to_string(g),
            to_string(n),
            to_string(m),
            to_string(randomized),
            to_string(greedy),
            to_string(semiGreedy),
            to_string(localCut),
            to_string(localIters),

            to_string(graspCut),
            to_string(graspIters),  // GRASP iterations
            to_string(bestKnown)
        });

        cout << "✅ Processed G" << g << endl;
    }

    writeResultsToCSV("2105099.csv", results); // Replace with your actual ID
    cout << "🎉 CSV written to 2105099.csv" << endl;

    return 0;
}

