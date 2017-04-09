
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include <cmath>
#include <queue>
#include <cstdio>
#include <cstring>
#include<set>
#include<iostream>

using namespace std;
#define maxn 2000
#define maxm 15000
#define INF 1<<30

class MCMF_ZKW
{
    public:

    int ans,S,T;
    int tot,V[maxm],G[maxm],C[maxm],N[maxm],F[maxn],B[maxm];
    int origal_tot,origal_V[maxm],origal_G[maxm],origal_C[maxm],origal_N[maxm],origal_F[maxn],origal_B[maxm];

    int v[maxn],slk[maxn],d[maxn];

    int all_demand;

    int zkw_last_service_size;
    int last_service_vector[maxn];

    public:
    MCMF_ZKW(){zkw_last_service_size=0;}
    void dw(int &a,int b);
    void add(int a,int b,int up,int co);
    void add_two(int a,int b,int up,int co);
    int aug(int u,int f);
    bool modlabel();
    void Zkw_Flow();
    void Init (int _s,int _t);    //算法初始化
    void init_graph(char * topo[], int line_num);
    void add_service(set<unsigned int> ser_set);
    void delete_service(void);
    void flow_test();
};




