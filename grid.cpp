#include <bits/stdc++.h>

using namespace std;

#define N 1001
#define pi pair<double,pair<int,int>>
#define mp make_pair

int n,startX,startY,finalX,finalY,explored,expanded;
double freq;
char grid[N][N],finalGrid[N][N];
map<pair<int,int>,int>Map;
double dist[N][N];
map<pair<int,int>,pair<int,int>>par;
int X[] = {1,-1,0,0},Y[] = {0,0,1,-1};
vector<pair<int,int>>sol;
bool vis[N][N];

bool isEqual(int x,int y)
{
    return (x == startX && y == startY) || (x == finalX && y == finalY);
}

void generateGrid()
{
    double curFreq = 0;
    double cnt = 0,tot = 1.0*n*n;

    startX = 0,startY =0;
    finalX = n-1,finalY = n-1;



    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            grid[i][j] = '.';
        }
    }



    while(curFreq <= freq){
        int row = rand()%n;
        int col = rand()%n;

        if(Map.find(make_pair(row,col)) == Map.end() && !isEqual(row,col)){
            Map[make_pair(row,col)] = 1;
            grid[row][col] ='*';
            cnt++;
            curFreq = (cnt/tot)*100;
        }

    }
}

bool boundaryCheck(int x,int y)
{
    return x >= 0 && x < n && y >= 0 && y < n;
}

bool isValid(int x,int y)
{
    return grid[x][y] != '*';
}

double h1(int x,int y)
{
    ///euclidean heuristic
    double res = sqrt((x - finalX)*(x - finalX)+(y - finalY)*(y - finalY));
    return res;
}

double h2(int x,int y)
{
    ///manhattan heuristic
    double res = abs(x - finalX) + abs(y - finalY);
    return res;
}

double h3(int x,int y)
{
    ///min max heuristic
    double res = max(abs(x - finalX),abs(y - finalY));
    return res;
}

void solve(int heuristicNumber)
{
    priority_queue<pi,vector<pi>,greater<pi>>pq;

    pq.push(mp(0,mp(startX,startY)));
    dist[startX][startY] = 0;


    while(!pq.empty()){
        pi u = pq.top();
        int fromX = u.second.first;
        int fromY = u.second.second;
        pq.pop();
        if(vis[fromX][fromY] == 0){
            expanded++;
            vis[fromX][fromY] = 1;
        }

        if(fromX == finalX && fromY == finalY){
            break;
        }

        for(int k = 0 ; k < 4; k++){
            int newX = fromX + X[k],newY = fromY + Y[k];
            if(boundaryCheck(newX,newY) && isValid(newX,newY)){
                double d = dist[fromX][fromY] + 1.0;
                if(par.find(mp(newX,newY)) == par.end() || d < dist[newX][newY]){
                    if(par.find(mp(newX,newY)) == par.end()) explored++;
                    par[mp(newX,newY)] = {fromX,fromY};
                    dist[newX][newY] = d;
                    grid[newX][newY] = 'T';
                    if(heuristicNumber == 1) d += h1(newX,newY);
                    else if(heuristicNumber == 2) d += h2(newX,newY);
                    else d += h3(newX,newY);
                    //d += euclideanHeuristic(newX,newY);

                    pq.push({d,{newX,newY}});

                }

            }
        }


    }

}

void showGrid()
{
    cout << endl;
    for(int i = 0; i < n;i++){
        for(int j = 0; j < n ;j++){
            cout << grid[i][j];
        }
        cout << endl;
    }
    cout << endl;
}


bool printPath()
{
    sol.clear();
    int x = finalX,y = finalY;
    bool canTraverse = true;
    map<pair<int,int>,pair<int,int>>::iterator it;
    while(true){
        //cout << x << " " << y << endl;
        //grid[x][y] = 'T';
        sol.push_back(mp(x,y));
        if(x == startX && y == startY){
            //cout << "here" << endl;

            break;
        }
        else if(par.find(mp(x,y)) == par.end()){
            canTraverse = false;
            break;
        }
        it = par.find(mp(x,y));
        pair<int,int>u = it->second;
        x = u.first;
        y = u.second;
        //cnt++;
    }
    if(canTraverse){
        for(int k = 0; k < sol.size(); k++){
            grid[sol[k].first][sol[k].second] = 'P';
        }
        return true;
    }
    else{
        cout << "No path exists" << endl;
        return false;
    }
    //showGrid();
    //finalGrid[x][y] = 'p';
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


void init()
{
    explored = 0;
    expanded = 0;
    Map.clear();
    sol.clear();
    par.clear();
    for(int i = 0; i < N; i++ ){
        for(int j = 0; j < N; j++){
            dist[i][j] = 0;
            finalGrid[i][j] = '.';
            vis[i][j] = false;
        }
    }
}
int main()
{


    cin >> n >> freq;

    generateGrid();
    FILE *fp;
    fp = freopen("1405029_grid.txt","w",stdout);
    cout << "Initial Grid"<< endl;

    showGrid();

    for(int i = 1; i <= 3; i++){
        init();
        solve(i);
        bool is = printPath();
        if(i == 1) cout << "\nEuclidean Heuristic"<< endl;
        else if(i == 2) cout << "\nManhattan Heuristic"<< endl;
        else cout << "\nMin Max Heuristic"<< endl;
        showGrid();
        if(is){
            cout << "Solution Depth : "<< dist[n-1][n-1] << endl;
            cout << "Number of explored Nodes : " << explored + 1 << endl;
            cout << "Branching Factor : " << branchingFactor(1.0*(explored),dist[n-1][n-1]) << endl;
        }
        else{
            cout << "Number of explored Nodes : " << explored + 1  << endl;
        }
    }

    //showGrid();
    //cout << dist[n-1][n-1] << " " << expanded << " " << explored+1 << endl;

    fclose(fp);





    return 0;
}
