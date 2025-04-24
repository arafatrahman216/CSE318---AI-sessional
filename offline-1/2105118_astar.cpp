#include<iostream>
#include<vector>
#include<queue>
#include<cmath>

using namespace std;

int n;
float heuristicValue; // h(n) value
int expandedNodes = 0; 
int exploredNodes = 0;


class Grid{
    public :
        vector<vector<int>> boardConfiguration ;
        pair<int,int> zeroPosition ;

        Grid(){
            boardConfiguration = vector<vector<int>>(n, vector<int>(n, 0));
            zeroPosition = {0,0};
        }

        Grid(int N){
            boardConfiguration = vector<vector<int>>(N, vector<int>(N, 0));
            zeroPosition = {0,0};
        }

        void printBoard(){
            for (auto x : boardConfiguration){
                for (auto y : x){
                    cout<< y<< " ";
                }
                cout<<endl;
            }
        }

        vector<Grid> generateValidMoves(){
            vector<Grid> moves ;
            pair<int,int> zeroPos = getBlankPosition();
            int x = zeroPos.first;
            int y = zeroPos.second;
            vector<pair<int,int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // up, down, left, right
            for (auto dir : directions){
                int newX = x + dir.first;
                int newY = y + dir.second;
                if (newX >= 0 && newX < n && newY >= 0 && newY < n){
                    Grid newGrid = *this; // create a copy of the current grid
                    swap(newGrid.boardConfiguration[x][y], newGrid.boardConfiguration[newX][newY]); // swap the blank with the adjacent tile
                    moves.push_back(newGrid);
                }
            }
            return moves;

        }

        pair<int,int> getBlankPosition(){
            for (int i = 0; i < boardConfiguration.size(); i++) {
            for (int j = 0; j < boardConfiguration[i].size(); j++) {
                if (boardConfiguration[i][j] == 0) {
                zeroPosition = {i,j};
                return {i, j};
                }
            }
            }
            return zeroPosition; 
        }

        void setBlankPosition(pair<int,int> zeroPos){
            zeroPosition= zeroPosition ;
        }
};


class Node
{
private:
    float priorityValue ; // used in priority queue 
    int moves ; // g(n)
    Node * parent ; // parent node
    Grid grid ; // grid configuration
    pair<int, int> zeroPosition ;

public:
    void incrementMoves(){
        moves++;
    }
    Grid getGrid(){
        return grid;
    }

    int getMoves(){
        return moves;
    }

    Node(float _priority , int _moves , Node * _parent , Grid _grid, pair<int, int> _zeroPosition = pair<int, int>{0,0} ){
        priorityValue = _priority;
        moves = _moves;
        parent = _parent;
        grid = _grid;
        zeroPosition = _zeroPosition;
    }

    float getPriorityValue() const{
        return priorityValue;
    }

    void setPriorityValue(float _priority){
        priorityValue = _priority;
    }

    Node * getParent(){
        return parent;
    }

    ~Node(){

    }
};

struct ComparePriority {
    bool operator()( Node& a, Node& b) const {
        // if a's priority value is less than b's, return true
        // this will make the priority queue a max-heap
        // so that the node with the lowest priority value is at the top
        return a.getPriorityValue() > b.getPriorityValue();
    }
};

priority_queue<Node, vector<Node>, ComparePriority> pq;


float findHammingDistance(Grid g){
    int hammingDistance = 0;
    for (int i = 0; i < n; i++) {
          for (int j = 0; j < n; j++) {
            if ( g.boardConfiguration[i][j] != 0 && g.boardConfiguration[i][j] != i * n + j + 1) {
                hammingDistance++;
            }
        }
    }
    return hammingDistance*1.0;
}

float findManhattanDistance(Grid g){
    int manhattanDistance = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (g.boardConfiguration[i][j] != 0) {
                int value = g.boardConfiguration[i][j];
                int targetRow = (value - 1) / n;
                int targetCol = (value - 1) % n;
                manhattanDistance += (abs(i - targetRow) + abs(j - targetCol));
            }
        }
    }
    return manhattanDistance*1.0;
}

float euclideanDistance(Grid g){
    float euclideanDistance = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (g.boardConfiguration[i][j] != 0) {
                int value = g.boardConfiguration[i][j];
                int targetRow = (value - 1) / n;
                int targetCol = (value - 1) % n;
                euclideanDistance += sqrt(pow(i - targetRow, 2) + pow(j - targetCol, 2));
            }
        }
    }
    return euclideanDistance;
}

int linearConflict(Grid g){
    int linearConflict = 0;
    // Check for row conflicts
    for (int row = 0; row < n; row++) {
        for (int col1 = 0; col1 < n; col1++) {
            if (g.boardConfiguration[row][col1] == 0) continue;
            int value1 = g.boardConfiguration[row][col1];
            int targetRow1 = (value1 - 1) / n;
            int targetCol1 = (value1 - 1) % n;

            if (targetRow1 == row) { // Tile is in its goal row
                for (int col2 = col1 + 1; col2 < n; col2++) {
                    if (g.boardConfiguration[row][col2] == 0) continue;
                    int value2 = g.boardConfiguration[row][col2];
                    int targetRow2 = (value2 - 1) / n;
                    int targetCol2 = (value2 - 1) % n;

                    if (targetRow2 == row && targetCol1 > targetCol2) { // Conflict in the same row
                        linearConflict += 1;
                    }

                }
            }
        }
    }

    // Check for column conflicts
    for (int col = 0; col < n; col++) {
        for (int row1 = 0; row1 < n; row1++) {
            if (g.boardConfiguration[row1][col] == 0) continue;
            int value1 = g.boardConfiguration[row1][col];
            int targetRow1 = (value1 - 1) / n;
            int targetCol1 = (value1 - 1) % n;

            if (targetCol1 == col) { // Tile is in its goal column
                for (int row2 = row1 + 1; row2 < n; row2++) {
                    if (g.boardConfiguration[row2][col] == 0) continue;
                    int value2 = g.boardConfiguration[row2][col];
                    int targetRow2 = (value2 - 1) / n;
                    int targetCol2 = (value2 - 1) % n;

                    if (targetCol2 == col && targetRow1 > targetRow2) { // Conflict in the same column
                        linearConflict += 2;
                    }
                }
            }
        }
    }
    return linearConflict;
}


int inversionCount(Grid g){
    vector<int> flatGrid;
    // pushing so that we can count inversions in row major order
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (g.boardConfiguration[i][j] == 0) continue; // skip the blank tile
            flatGrid.push_back(g.boardConfiguration[i][j]);
        }
    }

    int inversions = 0;
    for (int i = 0; i < flatGrid.size(); i++) {
        for (int j = i + 1; j < flatGrid.size(); j++) {
            if ( flatGrid[i] > flatGrid[j]) {
                inversions++;
            }
        }
    }
    return inversions;
}

bool isSolvable(Grid g){
    int inversions = inversionCount(g);
    if (n % 2 != 0) { 
        return (inversions % 2 == 0); // odd grid size
    } else {
        int blankRow = g.getBlankPosition().first;
        int blankRowFromBottom = n - blankRow; // blank row from bottom
        return (inversions + blankRowFromBottom) & 1; // even row odd inv or even row even inv // sum odd means solvable
    }
    
}

void aStarSearch(Grid &startGrid, float (*heuristicFunction)(Grid) = findManhattanDistance){
    pq.push(Node(0, 0, nullptr, startGrid));
    exploredNodes++;
    if (!isSolvable(startGrid)){
        cout << "Unsolvable puzzle" << endl;
        return;
    }
    startGrid.getBlankPosition(); // set the blank position and returns it
    while (!pq.empty()){
        Node currentNode = pq.top();
        pq.pop();
        expandedNodes++; 
        Grid currentGrid = currentNode.getGrid();
        cout<< "Parent Node: " << currentNode.getParent() << endl;
        currentGrid.printBoard();
        if (currentGrid.getBlankPosition() == pair<int,int>{n-1,n-1}){ // goal state
            cout << "Goal State Reached!" << endl;
            return;
        }

        vector<Grid> validMoves = currentGrid.generateValidMoves();
        for (auto move : validMoves){
            float heuristic_Hn = findManhattanDistance(move);
            float moves_Gn = currentNode.getMoves() + 1; // g(n)
            float priorityValue = moves_Gn + heuristic_Hn; // f(n) = g(n) + h(n)
            pq.push( Node(priorityValue, moves_Gn, &currentNode, move));
            exploredNodes++;

        }
        cout << "----------------------------------------" << endl;
    }
}