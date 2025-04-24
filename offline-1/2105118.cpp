#include"2105118_astar.cpp"


int main(){
    // cin>>n;
    n=3;
    Grid g(n);
    int x=0;
    vector<int> v= {1,2,3,0,4,6,7,5,8};

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            // cin>>x;
            g.boardConfiguration[i][j]=v[x];
            x++;
        }
    }
    g.setBlankPosition(g.getBlankPosition());

    
    aStarSearch(g);

}


