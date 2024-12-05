#include <stdio.h>
#include <utility>
#include <vector>
#include <unordered_map>
#include <queue>
#include <set>
#include <algorithm>

using namespace std;

const int INF = 0x3f3f3f3f;


//funcao hash para usar unordered map com pair como chave
struct PairHash
{
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2> &v) const
    {
        return std::hash<T1>()(v.first) ^ hash<T2>()(v.second) << 1;    
    }
};



int N, M, nmons, tmax, nrt; 

vector<int> posImons;
vector<set<pair<int,int>>> adj;
vector<set<int>> adjTrans;

vector<vector<int>> pathmonstro;

set<pair<int,int>> locmons;



// BFS/path monstros
vector<bool> usado;
vector<int> distbfs, pai;

// Dijkstra
unordered_map< pair<int, int> , bool, PairHash>  cor;
unordered_map< pair<int, int> , int, PairHash> paiD;
unordered_map< pair<int, int> , int, PairHash> distD;
int chegada = -1;



int bfs(){
    queue<int> fila;
    fila.push(1);
    usado[1] = true;
    pai[1] = -1;
    distbfs[1] = 0;
    while (!fila.empty()){
        int v = fila.front();
        fila.pop();
        for(int u : adjTrans[v]){
            
            int p = pai[u];
            if(distbfs[v]+1 < distbfs[u] ){
                usado[u] = true;
                fila.push(u);
                distbfs[u] = distbfs[v] + 1;
                pai[u] = v;
            }else if(p > v && distbfs[p] == distbfs[v]){
                pai[u] = v;
            }
        }
    }

    return 0;
}

int pathmonfind(int origem){
    if(distbfs[origem]==-1 || !usado[origem]){
        pathmonstro.emplace_back(vector<int>(1,origem));
        locmons.insert({origem, -2});
    }else{
        int count = 0;
        vector<int> path(0);
        for (int v = origem; v != -1; v = pai[v]){
            locmons.insert({v, count});
            path.emplace_back(v);
            count++;
            if(v==1){
                while(count <= tmax){
                    count++;
                    locmons.insert({1, count});
                }
            }
        }
        pathmonstro.emplace_back(vector<int>(path));
    }
    
    return 0;
}


int Dijkstra(){
    distD[{1, 0}] = 0;
    paiD[{1, 0}] = -1;

    priority_queue<tuple<int,int,int,int>, vector<tuple<int,int,int,int>>,greater<tuple<int,int,int,int>>> f;
    f.push({0,nrt,1,0});
    
    while(!f.empty()){
        auto [d,rec,v,turno] = f.top();
        f.pop();

        if(v == N ){
            chegada = turno;
            return 0;
        }

        if(!cor.count({v,turno})) cor[{v,turno}]=false;
    
        int disV = distD[{v, turno}];

        if(disV < d || cor[{v, turno}] || turno+1 > tmax)
            continue;
        cor[{v, turno}] = true;
        
        for(auto u : adj[v]){
            //printf("aaaa\n");
            int uV = u.first;
            int newdist = u.second; 
            if(!locmons.count({uV,turno}) && !locmons.count({uV,turno+1})){
                if(!paiD.count({uV,turno+1})) paiD[{uV,turno+1}] = -2;
                if(!distD.count({uV,turno+1})) distD[{uV,turno+1}] = INF;

                pair<int,int> parU(uV, turno+1); 
                if(rec >= newdist && distD[parU] > (disV + newdist)){
                    distD[parU] = (disV + newdist);
                    paiD[parU] = v;
                    f.push(make_tuple(distD[parU], rec + nrt - newdist, uV, turno+1));
                }
            }
        }
    }
    return 0;
}

void printPath(unordered_map< pair<int, int> , int, PairHash> pais, int v, int t){
    printf("%d %d\n",distD[{v,t}],t);

    vector<int> path;
    while(v != -1){
        path.push_back(v);
        v = paiD[{v,t}];
        t--;
    }

    for(int i = path.size()-1; i >= 0; i--){
        printf("%d ",path[i]);
    }
    printf("\n");
}


int main(){

    if(!scanf("%d %d %d %d %d",&N, &M, &nmons, &tmax, &nrt))
        printf(" ");
    for(int i = 0; i<nmons; i++){
        int aux;
        if(!scanf("%d", &aux))
            printf(" ");
        posImons.emplace_back(aux);
    }
    usado = vector<bool>(N+1);
    distbfs = vector<int>(N+1,INF);
    pai = vector<int>(N+1,-2);

    adj = vector<set<pair<int,int>>>(M+1, set<pair<int,int>>());
    adjTrans = vector<set<int>>(M+1, set<int>());

    for(int i = 0; i < M; i++){
        int v, u, c;
        if(!scanf("%d %d %d", &v, &u, &c))
            printf(" a\n");
        adj[v].insert({u,c});
        if(adj[v].find({v,v})==adj[v].end())
            adj[v].insert({v,1});
        //printf("%d %d %d\n", v,u,c);
        adjTrans[u].insert(v);
        
    }

    bfs();
    for(int i : posImons){
        if(i==-1) continue;
        pathmonfind(i);
    }

    Dijkstra();
  

    if(chegada != -1){
        printf("1\n");
    }else {
        printf("0\n");
    }

    for(vector<int> vec : pathmonstro){
        printf("%zu ",vec.size());
        for(int v : vec)
            printf("%d ",v);
        printf("\n");
    }

    if(chegada != -1){
        printPath(paiD, N, chegada);
    }else{
        int maxturn = 0, vf = 1;
        for(auto x : distD){
            if(x.second == INF) continue;
            if(x.first.second > maxturn || (x.first.second == maxturn && x.second < distD[{vf,maxturn}])){
                maxturn = x.first.second;
                vf = x.first.first;
            } 
        }
        if(locmons.count({vf,maxturn+1})){
            distD[{vf,maxturn+1}] = distD[{vf,maxturn}] + 1;
            paiD[{vf,maxturn+1}] = vf;
            maxturn++;
        }
        printPath(paiD, vf, maxturn);
    }
}