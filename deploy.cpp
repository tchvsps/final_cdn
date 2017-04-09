#include "deploy.h"
#include <stdio.h>

#include<iostream>
#include<algorithm>
#include<string>
#include<sstream>
#include<set>
#include<vector>
#include<stack>
#include<map>
#include<queue>
#include<deque>
#include<cstdlib>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<ctime>
#include<functional>
using namespace std;



//你要完成的功能总入口
int MCMF(void);
void init_graph(char * topo[], int line_num);
void init_service(set<unsigned int> service_set, unsigned int last_service_size);

void init_set(int num);
void dpos(void);

extern unsigned int node_cnt,demand_cnt,edge_cnt,deploy_cost,demand_sum;
set<unsigned int> service_set;
extern unsigned int last_service_size;
set<unsigned int> best_service_set;
int best_cost=10000000;
string flow2string(void);

void prepare_for_creat(void);
void update_set();

unsigned int search_cnt=0;
int valid_cnt=0;
int dyna_service_num;

int init_service_num;
bool update_big=true;

time_t t1;
time_t t_start;

#define dim 1000
extern char gbest[dim];

int array2cost(char* arr);

void zkw_test(char * topo[], int line_num);


#include "zkw.h"
extern unsigned int node_cnt;
extern unsigned int demand_cnt;
void init_set(int num);
extern set<unsigned int> service_set;
void init_set(int num);


set<unsigned int > set_from_chen_hang;
int mini_cost_from_chen_hang=100000;
MCMF_ZKW zkw;
extern map<unsigned int,unsigned int> node2demand;
extern map<unsigned int,unsigned int> demand2node;


void same_eff_test(void)
{
    int last_cost;
    int dif_cnt;
    cout<<"same effect test..."<<endl;
    for(unsigned int i=0 ; i<10000; i++){
        init_set(demand_cnt*2);


        init_service(service_set,last_service_size);
        last_cost=MCMF();
//        if(last_cost<0){continue;}

//        if(i==145)
//        {
//            cout<<"BUG OCCURE:"<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl;
//            cout<<flow2string()<<endl;
//            zkw.add_service(service_set);
//            zkw.Zkw_Flow();
//            zkw.flow_test();
//            return ;
//        }
        zkw.add_service(service_set);
        zkw.Zkw_Flow();

//        cout<<"spfa :"<<last_cost<<" "<<"zkw :"<<zkw.ans<<endl;
        if(last_cost!=zkw.ans)
        {
            cout<<"spfa :"<<last_cost<<" "<<"zkw :"<<zkw.ans<<endl;

//            zkw.flow_test();
            dif_cnt++;
        }
        if(time(NULL)-t1>30){cout<<"all cnt:"<<i<<" dif cnt:"<<dif_cnt<<endl;; break;}
    }
    if(dif_cnt==0){ cout<<"GOOD: zkw spfa is equal effect"<<endl<<endl;}
    else{cout<<"BAD !!!"<<endl<<endl;}
}

void spfa_zkw_time_test(void)
{
    t_start=time(NULL);
    for(unsigned int i=0; true; i++)
    {
        if(time(NULL)-t_start>=1) break;
    }

    cout<<"spfa zkw time test..."<<endl;
    for(unsigned int i=0;true; i++)
    {
        init_set(demand_cnt);
        zkw.add_service(service_set);
        zkw.Zkw_Flow();
        if(time(NULL)-t_start>=6)
        {
            cout<<"5s zkw:"<<i;
            break;
        }
    }
    for(unsigned int i=0 ;true; i++){
        init_set(demand_cnt);

        init_service(service_set,last_service_size);
        MCMF();
        if(time(NULL)-t_start>=11)
        {
            cout<<" spfa:"<<i<<endl<<endl;
            break;
        }
    }
}

void zkw_speed_test(void){
    t_start=time(NULL);
    for(unsigned int i=0; true; i++)
    {
        if(time(NULL)-t_start>=1) break;
    }

    cout<<"zkw speed test..."<<endl;
    for(unsigned int i=0;true; i++)
    {
        init_set(demand_cnt);
        zkw.add_service(service_set);
        zkw.Zkw_Flow();
        if(time(NULL)-t_start>=11)
        {
            cout<<"10s zkw:"<<i<<endl<<endl;
            break;
        }
    }
}

void zkw_opt_test(void)
{
        init_set(demand_cnt*2);
        init_service(service_set,last_service_size);
        int last_cost=MCMF();
        cout<<flow2string()<<endl;

        zkw.add_service(service_set);
        zkw.Zkw_Flow();
        zkw.flow_test();
        if(last_cost!=zkw.ans){
            cout<<"BAD!!"<<endl;
        }
        else{
            cout<<"GOOD THANK GOD"<<endl;
        }
}

void deploy_server(char * topo[MAX_EDGE_NUM], int line_num,char * filename)
{
    t1=time(NULL);

//    srand(time(NULL));
    srand(0);

    last_service_size=0;
    init_graph(topo,line_num);
    prepare_for_creat();

    zkw.Init(node_cnt,node_cnt+1);
    zkw.init_graph(topo,line_num);
//    same_eff_test();
//    zkw_opt_test();
//    spfa_zkw_time_test();
    zkw_speed_test();
//    for(unsigned int i=0; i<demand_cnt*2; i++){
//        set_from_chen_hang.insert(demand2node[i]);
//    }
//    zkw.add_service(set_from_chen_hang);
//    zkw.Zkw_Flow();
//    if(zkw.ans<0){
//        mini_cost_from_chen_hang=100000;
//    }else{
//        mini_cost_from_chen_hang=zkw.ans;
//    }
//
//    dpos();
//
//    int final_best=array2cost(gbest);
//    string out_string=flow2string();
//    cout<<endl<<out_string<<endl;
//    cout<<endl<<endl<<"final best:"<<final_best<<endl;
//    write_result(out_string.c_str(), filename);

}
