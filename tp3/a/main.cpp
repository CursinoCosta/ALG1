#include <cstdint>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <unordered_map>


using namespace std;

typedef pair<int,int> ii ;
typedef tuple<int,int,int,int> i4;
typedef tuple<int,int,int> i3;
typedef vector<int> vi ;
typedef vector<vector<int>> vvi ;


struct PairHash
{
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2> &v) const
    {
        return hash<T1>()(v.first) ^ hash<T2>()(v.second) << 1;    
    }
};

struct Tuple3Hash
{
    template <class T1, class T2, class T3>
    std::size_t operator() (const std::tuple<T1, T2, T3> &v) const
    {
        return hash<T1>()(get<0>(v)) ^ hash<T2>()(get<1>(v)) ^ hash<T3>()(get<2>(v)) << 1;    
    }
};

struct Vertice
{
    int brilho_;
    ii adj_;


    Vertice(){}
    Vertice(int brilho, ii adj) :
    brilho_(brilho), 
    adj_(adj){}
    
};

typedef unordered_map< pair<int, int> , Vertice, PairHash> umapV;
typedef unordered_map< i3 , int, Tuple3Hash> umapI;
typedef unordered_map<tuple<int, int, int>, int,Tuple3Hash> umap3i;



int L, C, N;

umapV grid;
umap3i subp;
umapI filhos;



bool is_set(unsigned int number, int x) {
    return (number >> x) & 1;
}


vi Pmasks(int linha){
    vi possiveis;
    for(int i=0;i<pow(2,C);i++){
        int skip = 0;
        for(int j=0;j<C;j++)if(is_set(i, j)){
            if((grid[{linha,C-j}].brilho_) == -1){   
                skip = 1;
            }
            if((grid[{linha,C-j}].adj_.first) == 1){
                if(j==0){
                    if(is_set(i,C-1)) 
                        skip = 1;
                }
                else if(is_set(i,j-1))
                    skip = 1;
            }
        }
        if(skip==0)
            possiveis.emplace_back(i);
        skip = 0;
    }
    return possiveis;
}


int sumBrilho(int linha, int mask){
    int sum=0;
    for(int i=0;i<C;i++){
        if(is_set(mask,i)){
            sum += grid[{linha,C-i}].brilho_;
        }
    }
    return sum;
}

bool compativeis(int linha, int maska, int maskp){
    for(int i=0;i<C;i++){
        if(is_set(maska, i) && ((grid[{linha,C-i}].adj_.second) == 1) && is_set(maskp, i))
                return false;
    }
    return true;
}

int subpsol(int linha, int mask, int mask1){
    if(subp.count({linha,mask,mask1}))
        return subp[{linha,mask,mask1}];

    if(linha == 1){
        if(compativeis(1,mask1,mask))
            return sumBrilho(1, mask1);
        else
            return INT32_MIN;
    }
    
    vi possiveis = Pmasks(linha);

    int nline = linha+1;
    if(linha == L)
        nline = 1;

    int max = 0, select;
    for(int m : possiveis) if(compativeis(linha,m,mask)){
        int ans = subpsol(nline,m,mask1) + sumBrilho(linha, m);
        if(ans > max){
            max = ans;
            select = m;
        }
    }
    subp[{linha,mask,mask1}] = max;
    filhos[{linha-1,mask,mask1}] = select; 
    return max;
}


ii Cifra(){
    vi plinha1 = Pmasks(1);
    int max = 0, escolha;
    for(int mask : plinha1){
        int ans = subpsol(2,mask,mask);
        if(ans > max){
            max = ans;
            escolha = mask;
        }
    }
    subp[{1,escolha,escolha}] = max;
    return {max,escolha};
}





int main(){
    
    scanf("%d %d %d",&L,&C,&N);
    for(int x=1;x<=L;x++){
        for(int y=1;y<=C;y++){
            grid[{x,y}] = Vertice(-1,{0,0});
        }
    }
    int x, y, v, d, c, e ,b;
    for(int i=0;i<N;i++){
        scanf("%d %d %d %d %d %d %d",&x, &y, &v, &d, &c, &e ,&b);
        grid[{x,y}] = Vertice(v, {d,c});
    }

    
    ii par = Cifra();
    vector<ii> cristais;

    int linha = 1, mask = par.second, count = 0;
    while(linha<=L){
        for(int i=0;i<C;i++){
            if(is_set(mask, i)){
                count++;
                x = linha;
                y = C-i;
                cristais.push_back({x,y});
            }
        }
        mask = filhos[{linha,mask,par.second}];
        linha++;
    }

    printf("%d %d\n",count,par.first);
    for(auto c : cristais)
        printf("%d %d\n",c.first,c.second);

 
    return 0;
}