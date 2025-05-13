#include"2105118_header.cpp"




int calculateCutWeight( unordered_set<int>& X, unordered_set<int>& Y, Graph& graph) {
    int weight = 0;
    for (int u : X) {
        for ( auto& neighbor : graph.getAdjList()[u]) {
            int v = neighbor.first;
            if (Y.count(v)) {
                weight += neighbor.second;
            }
        }
    }
    return weight;
}



pair<int, pair<unordered_set<int>, unordered_set<int>>> semiGreedyMaxCut(Graph& graph, double alpha = 0.5) {
    unordered_set<int> X, Y;
    cout << "Semi-Greedy algorithm started..." << endl;

    Edge maxEdge = graph.getMaxEdge();
    pair<int, int> maxEdgePair = {maxEdge.u, maxEdge.v};
    int maxWeight = maxEdge.w;

    if (maxWeight == INT32_MIN) return {0, {X, Y}}; 

    X.insert(maxEdgePair.first);
    Y.insert(maxEdgePair.second);

    while (X.size() + Y.size() < graph.n) {
        vector<pair<int, pair<int, int>>> candidates; // (gain, (v, partition))

        for (int v = 1; v <= graph.n; ++v) {
            if (X.count(v) || Y.count(v)) continue;

            int gainX = 0, gainY = 0;

            for (int neighbor : Y) {
                gainX += graph.getWeight(v, neighbor);
            }
            for (int neighbor : X) {
                gainY += graph.getWeight(v, neighbor);
            }

            candidates.emplace_back(gainX, make_pair(v, 0)); // 0 for X
            candidates.emplace_back(gainY, make_pair(v, 1)); // 1 for Y
        }

        if (candidates.empty()) break;

        sort(candidates.begin(), candidates.end(),
             [](const pair<int, pair<int, int>>& a, const pair<int, pair<int, int>>& b) {
                 return a.first > b.first;
             });

        int minGain = candidates.back().first;
        int maxGain = candidates.front().first;
        int threshold = minGain + alpha * (maxGain - minGain);

        vector<pair<int, pair<int, int>>> RCL;
        for (const auto& candidate : candidates) {
            if (candidate.first >= threshold) {
                RCL.push_back(candidate);
            }
        }

        if (RCL.empty()) RCL.push_back(candidates[0]);
        


        // Randomly select from RCL
        random_device rd;
        mt19937 rng(rd());
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

    int cutWeight = calculateCutWeight(X, Y, graph);
    return {cutWeight, {X, Y}};
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
    int bestCut = INT32_MIN;
    unordered_set<int> bestX, bestY;
    cout << "Randomized algorithm started..." << endl;
    cout << "Iterations: " << iterations << endl;

    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> dist(0, 1);

    for (int i = 0; i < iterations; i++) {
        unordered_set<int> X, Y;
        for (int v = 1; v <= graph.n; ++v) {
            int randomNum = dist(rng);
            if (randomNum % 2 == 0) {
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

pair<int, pair<unordered_set<int>, unordered_set<int>>> localSearch(Graph& graph, unordered_set<int> X, unordered_set<int> Y) {
    vector<int> partition(graph.n + 1, 0);
    for (int x : X) partition[x] = 0;
    for (int y : Y) partition[y] = 1;

    const auto& adj = graph.getAdjList();
    bool improved;

    do {
        improved = false;
        for (int v = 1; v <= graph.n; ++v) {
            int same = 0, diff = 0;
            for (auto& neighbor : adj[v]) {
                int u = neighbor.first, w = neighbor.second;
                if (partition[v] == partition[u]) same += w;
                else diff += w;
            }
            int delta = same - diff;
            if (delta > 0) {
                partition[v] ^= 1;
                improved = true;
            }

        }
    } while (improved);

    unordered_set<int> newX, newY;
    for (int v = 1; v <= graph.n; ++v) {
        if (partition[v] == 0) newX.insert(v);
        else newY.insert(v);
    }

    int cutWeight = calculateCutWeight(newX, newY, graph);
    return {cutWeight, {newX, newY}};
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