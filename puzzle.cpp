#include <bits/stdc++.h>

using namespace std;

#define pi pair<double,vector<vector<int>>>

int n;
int dirx[]={-1,1,0,0},diry[]={0,0,-1,1};
map<vector<vector<int>>,vector<vector<int>>>par;
//map<vector<vector<int>>,int>isVisited;
map<vector<vector<int>>,double>cost;
long long generated;

void show(vector<vector<int>>sol)
{
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout << sol[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

double h1(vector<vector<int>>cur,vector<vector<int>>goal)
{
    ///misplaced tiles
    double cnt = 0;
    for(int i = 0; i < n; i++){
        for(int j = 0 ;j < n; j++){
            if(cur[i][j] != 0 && cur[i][j] != goal[i][j]) cnt++;
        }
    }

    return cnt;
}

double h2(vector<vector<int>>cur,vector<vector<int>>goal)
{
    ///manhattan distance

    double dis  = 0;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(cur[i][j] == 0) continue;
            else{
                bool is = 0;
                for(int k = 0; k < n; k++){
                    for(int l = 0; l < n; l++){
                        if(cur[i][j] == goal[k][l]){
                            dis += abs(i-k)+abs(j-l);
                            is = 1;
                            break;
                        }
                    }
                    if(is) break;
                }
            }
        }
    }

    return dis;
}

double h3(vector<vector<int>>cur,vector<vector<int>>goal)
{
    ///euclidean distance

    double dis  = 0;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(cur[i][j] == 0) continue;
            else{
                bool is = 0;
                for(int k = 0; k < n; k++){
                    for(int l = 0; l < n; l++){
                        if(cur[i][j] == goal[k][l]){
                            dis += sqrt((i-k)*(i-k) + (j-l)*(j-l));
                            is = 1;
                            break;
                        }
                    }
                    if(is) break;
                }
            }
        }
    }

    return dis;
}

double h4(vector<vector<int>>cur,vector<vector<int>>goal)
{
    ///misplaced row column

    double cnt = 0;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            int val = cur[i][j];
            for(int k = 0; k < n; k++){
                bool is = 0;
                for(int l = 0; l < n;l++){
                    if(goal[k][l] == val){
                        if(i != k) cnt++;
                        if(j != l) cnt++;
                        is = 1;
                        break;
                    }
                }
                if(is) break;
            }
        }
    }

    return cnt;
}



double h5(vector<vector<int>>cur,vector<vector<int>>goal)
{
    ///N-swap


    double swapCount = 0;
    while(cur != goal){

        for(int i = 0; i < n; i++){
            for(int j = 0; j < n;j++){
                int curVal = cur[i][j];
                int goalVal = goal[i][j];
                if(curVal != goalVal){
                    int newX,newY;
                    //cout <<"yup "<< i << " " << j << " " << curVal << " " << goalVal << endl;
                    for(int k = 0;k < n; k++){
                        bool is = 0;
                        for(int l = 0; l < n; l++){
                            if(cur[k][l] == goalVal){
                                newX = k;
                                newY = l;
                                is = 1;
                                break;
                            }
                        }
                        if(is) break;
                    }
                    swap(cur[i][j],cur[newX][newY]);
                    swapCount++;
                    //show(cur);

                }
            }
        }





    }

    return swapCount;


}



void printSolver(vector<vector<int>>cur,vector<vector<int>>start)
{
    if(cur == start){
        show(cur);
        return;
    }
    else{
        printSolver(par[cur],start);
        show(cur);
        return;
    }
}

bool boundaryCheck(int row,int col)
{
    return row >= 0 && row < n && col >= 0 && col < n;
}

vector<vector<vector<int>>>getNeighbours(vector<vector<int>>cur)
{
    int blankRowNo ,blankColNo;

    vector<vector<vector<int>>>neighbours;
    //blank tile search

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(cur[i][j] == 0){
                blankRowNo = i;
                blankColNo = j;

                break;
            }
        }
    }

    //cout << "b " << blankRowNo << " " << blankColNo << endl;

    for(int k = 0; k < 4; k++){
        int newRow = blankRowNo + dirx[k];
        int newCol = blankColNo + diry[k];
        vector<vector<int>>newNeighbour = cur;
        if(boundaryCheck(newRow,newCol) == false) continue;
        swap(newNeighbour[blankRowNo][blankColNo],newNeighbour[newRow][newCol]);
        neighbours.push_back(newNeighbour);
    }

    return neighbours;




}

void aStarSearch(vector<vector<int>>start,vector<vector<int>>goal,int heuristicNumber)
{
    priority_queue<pi,vector<pi>,greater<pi>>pq;

    pq.push({0,start});
    cost[start] = 0;
    //vector<vector<int>>init(4,vector<int>(4));
    //par[start] = init;

    while(!pq.empty()){
        pi u = pq.top();
        vector<vector<int>>parState = u.second;
        pq.pop();


        if(parState == goal){
            if(heuristicNumber == 1) cout << "\nMisplaced tiles heuristic:\nPath:\n";
            else if(heuristicNumber == 2) cout << "\nManhattan distance heuristic:\nPath:\n";
            else if(heuristicNumber == 3) cout << "\nEuclidean heuristic:\nPath:\n";
            else if(heuristicNumber == 4) cout << "\nMisplaced row column heuristic:\nPath:\n";
            else cout << "\nN-swap heuristic:\nPath:\n";
            printSolver(goal,start);
            break;
        }

        double parCost = cost[parState];
        for(vector<vector<int>>childState : getNeighbours(parState)){

            double newCost = parCost + 1;


            if(cost.find(childState) == cost.end()){
                ///if new state
                generated++;
                cost[childState] = newCost;
                double val;
                if(heuristicNumber == 1) val = newCost + h1(childState,goal);
                else if(heuristicNumber == 2) val = newCost + h2(childState,goal);
                else if(heuristicNumber == 3) val = newCost + h3(childState,goal);
                else if(heuristicNumber == 4) val = newCost + h4(childState,goal);
                else val = newCost + h5(childState,goal);
                par[childState] = parState;
                pq.push({val,childState});
            }
            else{

                if(newCost < cost[childState]){
                    /// if new cost is less than previous cost
                    cost[childState] = newCost;
                    double val;
                    if(heuristicNumber == 1) val = newCost + h1(childState,goal);
                    else if(heuristicNumber == 2) val = newCost + h2(childState,goal);
                    else if(heuristicNumber == 3) val = newCost + h3(childState,goal);
                    else if(heuristicNumber == 4) val = newCost + h4(childState,goal);
                    else val = newCost + h5(childState,goal);
                    par[childState] = parState;
                    pq.push({val,childState});
                }
            }
        }
    }

}

bool can(double x,double gen,double depth)
{
    double p = pow(x,depth+1);
    if((gen+1)*(x-1) < p-1.0) return 1;
    else return 0;
}

double branchingFactor(double gen,double depth)
{
    double low = 1.0;
    double high = 40.0;
    double mid;
    while(high - low > .00001){
        mid = (low + high) / 2;
        if(can(mid,gen,depth)) high = mid;
        else low = mid;
    }
    return mid;

}


int main()
{

    cout << "Enter n:"<< endl;
    cin >>n;
    cout << "Enter start state:"<< endl;
    vector<vector<int>>start(n,vector<int>(n));
    vector<vector<int>>goal(n,vector<int>(n));
    for(int i = 0; i < n;i++){
        for(int j = 0; j < n; j++){
            cin >> start[i][j];
        }
    }

    cout << "Enter goal state:"<< endl;

    for(int i = 0; i < n; i++){
        for(int j = 0;j < n; j++){
            cin >> goal[i][j];
        }
    }

    int h;
    cout << "Enter heuristic :" << endl;
    cin >> h;

    FILE *fp;
    fp = freopen("1405029_puzzle.txt","w",stdout);


    //for(int i = 1; i <= 5; i++){
        generated = 0;
        par.clear();
        cost.clear();
        aStarSearch(start,goal,h);

        cout << "Generated Nodes : " << generated <<endl;
        cout << "Solution Depth : " <<cost[goal] << endl;
        cout << "Branching Factor : " <<  branchingFactor(1.0*generated,cost[goal]) << endl;

    //}

    fclose(fp);









    return 0;
}
/*
5 6 0
7 1 4
3 8 2

8 2 7
5 0 6
4 3 1

8 2 0
5 4 7
3 6 1

1 2 3
4 5 6
7 8 0
*/
/*
12 14 9 2
4 0 15 3
1 10 11 8
13 6 7 5

14 8 12 1
9 6 4 3
7 2 5 11
10 0 13 15

1 3 11 14
9 10 15 4
0 5 12 8
6 13 2 7

1 2 3 4
5 6 7 8
9 10 11 12
13 14 15 0
*/

