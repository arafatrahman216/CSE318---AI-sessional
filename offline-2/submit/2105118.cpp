#include"2105118_algo.cpp"

int main() {
    vector<vector<string>> results;
    int knownBests [54] = {12078, 12084, 12077,0,0,0,0,0,0,0,627,621,645,3187,3169,3172,0,0,0,0,0,14123,14129,14131,0,0,0,0,0,0,0,1560,1537,1541,
        8000,7996,8009,0,0,0,0,0,7027,7022,7020,0,0,6000,6000,5988};

    ofstream out("2105118.csv");
    out << "Name,|V|,|E|,Randomized-1,Greedy-1,Semi-greedy-1,Local-1,GRASP-1,Known best\n";
    // For each benchmark graph (you would loop through all 54)
    for (int g = 1; g <= 54; g++) { // Just testing with G1 for this example
        string filename = "set1/g" + to_string(g) + ".rud";
        cout << "Processing graph: " << filename << endl;
        Graph graph = readGraph(filename);
        
        // Known best solution (from the assignment)
        int knownBest = knownBests[g - 1];
        // Add other known best solutions...
        
        // Run all algorithms
        auto [randomCut, _] = randomizedMaxCut(graph, 1);
        auto [greedyCut, __] =  greedyMaxCut(graph);
        auto [semiGreedyCut, sets] = semiGreedyMaxCut(graph, 0.75);
        auto [localSearchCut, ___] = localSearch(graph, sets.first, sets.second);
        auto [graspCut, ____] =  graspMaxCut(graph, 0.75, 1);
        
        // Store results
        results.push_back({
            "G" + to_string(g),
            to_string(graph.n),
            to_string(graph.m),
            to_string(randomCut),
            to_string(greedyCut),
            to_string(semiGreedyCut),
            to_string(localSearchCut),
            to_string(graspCut),
            to_string(knownBest)
        });
        writeRowToCSV(out, results.back());
        cout << "✅ Processed G" << g << endl;
    }
    
    out.close(); // Close the output file stream
    
    return 0;
}