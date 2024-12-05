#include <unordered_map>
#include <stdio.h>
#include <vector>
#include <stack>
#include <set>




  std::vector<int> tempo, low;
  std::vector<bool> visita;

  std::set<std::set<int>> vset;
  std::set<int> cutp;

  std::stack<int> pilha;
  int N, M, timer, cutp_count, clust_count;
  std::unordered_map<int, std::vector<int>> hmap;

void IS_CUTPOINT(int v){
  int is = 0;
  for(int b : cutp)
    if(b==v)
      is = 1;
  if(is == 0){
    cutp_count++;
    cutp.insert(v);
  }
  std::set<int> set;
  while(pilha.top() != v){
    set.insert(pilha.top());
    pilha.pop();
  }
  set.insert(pilha.top());
  clust_count++;
  vset.insert(set);
};

void dfs(int v, int p =-1){
  visita[v] = true;
  tempo[v] = low[v] = timer++;
  int children = 0;
  if(hmap.find(v) != hmap.end()){
    for (int to : hmap.at(v)){
      if(to == p) continue;
      if (visita[to]){
        low[v] = std::min(low[v], tempo[to]);
      }else {
        ++children;
        pilha.push(v);
        dfs(to,v);
        low[v] = std::min(low[v], low[to]);
        if ((low[to] >= tempo[v] && p != -1) || (p == -1 && children > 1) )
            IS_CUTPOINT(v);
      }
    }
    if(children==0)
      pilha.push(v);
  }
};

void cortes(){
  timer = 0;
  visita.assign(N+1,false);
  tempo.assign(N+1, -1);
  low.assign(N+1,-1);
  for(int i =1; i <= N; i++){
    if (!visita[i]){
      dfs(i);
    
      std::set<int> set;
      int j = 0;
      while(!pilha.empty()){
        j = 1;
        set.insert(pilha.top());
        pilha.pop();
      }
      if(j==1){
        clust_count++;
        vset.insert(set);
      }
      
      }
  }
  int check = 0;
  std::set<int> set;
  while (pilha.size() > 1) {
    check = 1;
    set.insert(pilha.top());
    pilha.pop();
  }
  if(check == 1){
    set.insert(pilha.top());
    vset.insert(set);
    clust_count++;
    }
};

/* void check_adj(){
  for(int v = 1 ; v<=N;v++){
    if(hmap.find(v) != hmap.end()){
      printf("sou %d e estou conectado a: ",v);
      for (int to : hmap.at(v)){
        printf("%d ",to);
      }
      printf("\n");
    }
    }
  }
 */

int main(){
  if(!scanf("%d %d", &N, &M))
    printf("\nErro na leitura de variavel\n");
  


  if(N==1 || M==0){
    printf("%d\n",0);
    for(int v=1;v<=N;v++)
      printf("%d\n",v);
    printf("%d\n",0);
    printf("%d %d",N,0);
    return 0;
  }

  for(int i=1;i<=N;i++){
    visita.assign(i,false);
  }
  for(int i = 0;i<M;i++){
    int a,b;
    if(!scanf("%d %d", &a, &b))
      printf("\nErro na leitura de variavel %d\n",i);
    if(hmap.find(a) == hmap.end()){
      std::vector<int> v;
      hmap.insert(std::make_pair(a,v));
    }
    hmap[a].push_back(b);


    if(hmap.find(b) == hmap.end()){
      std::vector<int> v;
      hmap.insert(std::make_pair(b,v));
    }
    hmap[b].push_back(a);

  }

  //check_adj();
  cortes();

  //print 4.2.1
  printf("%d\n",cutp_count);
  for(int v : cutp)
    printf("%d\n",v);
  printf("%d\n",clust_count);

  //print 4.2.2
  int aux = 0;
  std::unordered_map<int, std::set<int>> fmap;   
  int co;

  for(auto s: vset ){
    aux++;
    printf("%d %lu ",aux+N,s.size());
    for(int v : s){
      printf("%d ",v);
      for(int c : cutp)
        if(c==v){
          if(fmap.find(c) == fmap.end()){
            std::set<int> v;
            fmap.insert(std::make_pair(c,v));
          }
          fmap[c].insert(aux+N);
          co++;
        }
      }
    printf("\n");
  }

  //print 4.2.3
  printf("%d %d\n",cutp_count+clust_count,co);
  for (int c : cutp){
    for(int m : fmap.at(c)){
      printf("%d %d\n",c,m);
    }
  }


  return 0;
}