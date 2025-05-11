#include"2105118_header.cpp"




int calculateCutWeight(const unordered_set<int>& X, const unordered_set<int>& Y, const Graph& graph) {
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

// Greedy Algorithm
pair<int, pair<unordered_set<int>, unordered_set<int>>> greedyMaxCut(Graph& graph) {
    unordered_set<int> X, Y;
    bool partitionSetX[graph.n + 1] = {false};
    bool partitionSetY[graph.n + 1] = {false};
    cout<<"Greedy algorithm started..."<<endl;
    // Find the edge with maximum weight
    Edge maxEdge = graph.getMaxEdge();
    pair<int, int> maxEdgePair = {maxEdge.u, maxEdge.v};
    int maxWeight = maxEdge.w;
    if (maxWeight == 0) return {0, {X, Y}};
    X.insert(maxEdgePair.first);
    Y.insert(maxEdgePair.second);
    partitionSetX[maxEdgePair.first] = true;
    partitionSetY[maxEdgePair.second] = true;
    
    for (int z = 1; z <= graph.n; z++) {
        int gainX = 0, gainY = 0;
        if (z == maxEdgePair.first || z == maxEdgePair.second) continue;

        if (partitionSetX[z] == false) {
            for (int y : Y) {
                gainX += graph.getWeight(z, y);
            }
        }
        if (partitionSetY[z] == false) {
            for (int x : X) {
                gainY += graph.getWeight(z, x);
            }
        }
        if (gainX >= gainY) {
            X.insert(z);
            partitionSetX[z] = true;
        } else {
            Y.insert(z);
            partitionSetY[z] = true;
        }
    }
    int currentCut = calculateCutWeight(X, Y, graph);

    return { currentCut, {X, Y} };
}



// Local Search
pair<int, pair<unordered_set<int>, unordered_set<int>>> randomizedMaxCut(Graph& graph, int iterations = 1) {
    int bestCut = 0;
    unordered_set<int> bestX, bestY;
    cout << "Randomized algorithm started..." << endl;
    cout << "Iterations: " << iterations << endl;

    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> dist(0, 1);

    for (int i = 0; i < iterations; ++i) {
        unordered_set<int> X, Y;
        for (int v = 1; v <= graph.n; ++v) {
            if (dist(rng)) {
                X.insert(v);
            } else {
                Y.insert(v);
            }
        }
        int currentCut = calculateCutWeight(X, Y, graph);
        if (currentCut > bestCut) {
            bestCut = currentCut;
            bestX = X;
            bestY = Y;
        }
    }

    return {bestCut, {bestX, bestY}};
}

pair<int, pair<unordered_set<int>, unordered_set<int>>> semiGreedyMaxCut(Graph& graph, double alpha= 0.5) {
    unordered_set<int> X, Y;
    cout << "Semi-Greedy algorithm started..." << endl;
    Edge maxEdge = graph.getMaxEdge();
    pair<int, int> maxEdgePair = {maxEdge.u, maxEdge.v};
    int maxWeight = maxEdge.w;
    if (maxWeight == INT32_MIN) return {0, {X, Y}}; 
    X.insert(maxEdgePair.first);  Y.insert(maxEdgePair.second);
    vector<int> U(graph.n );  iota(U.begin(), U.end(), 1);
    unordered_set<int> Uset(U.begin(), U.end());
    
    Uset.erase(maxEdgePair.first);
    Uset.erase(maxEdgePair.second);
    Uset.erase(0);
    vector<int> sigmaXs (graph.n + 1, 0);
    vector<int> sigmaYs (graph.n + 1, 0);
    vector<int> greedyValue (graph.n + 1, 0);

    // Assign remaining vertices
    while (!Uset.empty()) {

        fill(greedyValue.begin(), greedyValue.end(), 0);
        fill(sigmaXs.begin(), sigmaXs.end(), 0);
        fill(sigmaYs.begin(), sigmaYs.end(), 0);

        cout << "Calculating gains for unassigned vertices..." << endl;

        int maxGain = INT32_MIN;
        int minGain = INT32_MAX;

        for (int v : U) {
            int sigmaX = calculateSigma(v, X, graph);
            int sigmaY = calculateSigma(v, Y, graph);
            sigmaXs[v] = sigmaX;
            sigmaYs[v] = sigmaY;
            greedyValue[v] = max (sigmaX, sigmaY);
            maxGain = max(maxGain, max(sigmaX, sigmaY));
            minGain = min(minGain, min(sigmaX, sigmaY));
        }

        int threshold_Mu = minGain + alpha * (maxGain - minGain);


        vector <pair<int, int>> RCL;
        //RCL ← { z in U | greedy_value(z) ≥ μ }
        for (int v : U) {
            if (greedyValue[v] >= threshold_Mu) {
                int partition = (sigmaXs[v] > sigmaYs[v]) ? 0 : 1; // 0 for X, 1 for Y
                RCL.push_back({v, partition}); 
            }
        }
        random_device rd;
        mt19937 rng(rd());
        uniform_int_distribution<int> dist(0, RCL.size() - 1);
        auto selected = RCL[dist(rng)];
        int r = selected.first; 
        int partition = selected.second;
        //Assign r to the partition where it contributes most (X or Y)
        if (sigmaXs[r] > sigmaYs[r]) {
            X.insert(r);
        } else {
            Y.insert(r);
        }
        cout << "Selected vertex: " << r << ", Partition: " << partition << endl;
        Uset.erase(r);
        // Calculate gains for all unassigned vertices
    }

    int cutWeight = calculateCutWeight(X, Y, graph);
    return {cutWeight, {X, Y}};
}

pair<int, pair<unordered_set<int>, unordered_set<int>>> graspMaxCut(Graph& graph, double alpha, int maxIterations) {
    int bestCut = 0;
    unordered_set<int> bestX, bestY;
    cout << "GRASP algorithm started..." << endl;

    for (int i = 0; i < maxIterations; ++i) {
        // Construction phase
        auto [cut1, partitions1] = semiGreedyMaxCut(graph, alpha);
        auto [X, Y] = partitions1;

        // Local search phase
        auto [cut2, partitions2] = randomizedMaxCut(graph, 1);

        if (cut2 > bestCut) {
            bestCut = cut2;
            bestX = partitions2.first;
            bestY = partitions2.second;
        }
    }

    return {bestCut, {bestX, bestY}};
}


