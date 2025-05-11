#include"2105118_algo.cpp"

int main() {
    int n, m;
    cin >> n >> m;
    Graph g(n, m);
    for (int i = 0; i < m; i++) {
        int u,v,w ;
        cin>>u >>v >> w;
        g.addEdge(u, v, w);
    }
    cout << "Graph created with " << g.n << " nodes and " << g.m << " edges." << endl;
    
    pair<int, pair<unordered_set<int>, unordered_set<int>>> result = semiGreedyMaxCut(g, 0.5);
    int cutWeight = result.first;
    cout << "Max Cut Weight: " << cutWeight << endl;

    // priority_queue<Edge, vector<Edge>, decltype(&compareEdges)> pq = g.getEdgePQ();

    return 0;
}