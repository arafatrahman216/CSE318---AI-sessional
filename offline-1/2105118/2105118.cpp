#include"2105118_astar.cpp"
#include<fstream>

fstream fin;



int main(){
    // cin>>n;
    fin.open("input.txt");
    fin>>n;
    Grid g(n);
    int x=0;
    cout<<"Enter Heuristic function to be used: \n[1] Manhattan Distance \n[2] Euclidean Distance \n[3] Hamming Distance \n[4] Linear Conflict\n";
    int choice;
    cin>>choice;
    
    float (*heuristicFunction)(Grid);
    switch(choice){
        case 1:
            heuristicFunction = findManhattanDistance;
            break;
        case 2:
            heuristicFunction = findEuclideanDistance;
            break;
        case 3:
            heuristicFunction = findHammingDistance;
            break;
        case 4:
            heuristicFunction = findlinearConflictHeuristic;
            break;
        default:
            cout<<"Invalid choice";
            return 0;
    }
    // test data
    // vector<int> v= {1,2,3,4,6,8,7,5,0};
    // vector<int> v= {4,1,3,0,5,6,2,7,8};
    // vector<int> v= {0,1,3,4,2,5,7,8,6};

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            fin>>x;
            g.boardConfiguration[i][j]=x;}
    }
    g.setBlankPosition(g.getBlankPosition());
    aStarSearch(g, heuristicFunction);
    

}


