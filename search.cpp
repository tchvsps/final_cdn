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

#define N 2000


extern unsigned int node_cnt,demand_cnt,edge_cnt,deploy_cost,demand_sum;
extern set<unsigned int> service_set;
extern set<unsigned int> unassigned_demand_set;
extern map<unsigned int,unsigned int> node2edge;
struct Edge
{
    int from,to,cap,flow,cost;
    Edge(int u,int v,int ca,int f,int co):from(u),to(v),cap(ca),flow(f),cost(co){};
};

extern vector<Edge> origal_edges;
extern vector<Edge> edges;
extern vector<int> G[N];

unsigned int search_deepth;

struct {
    set<unsigned int> node_set;
    vector<unsigned int> node_array;
}demand[400];

struct {
    set<unsigned int> demand_set;
    unsigned int degree_sum;
    void sele_action(void)
    {
        set<unsigned int>::iterator set_iter;
        for(set_iter=demand_set.begin(); set_iter!=demand_set.end(); ++set_iter)
        {
            if(unassigned_demand_set.find(*set_iter)!=unassigned_demand_set.end()){
//                unassigned_demand_set.erase(unassigned_demand_set.find(*set_iter));
            }
        }
    }
}service[1000];

extern map<unsigned int, unsigned int> demand2node;

void dfs(unsigned int demand_index,unsigned int node_index, unsigned int deepth){
    if(deepth>search_deepth) return;

    demand[demand_index].node_set.insert(node_index);
    service[node_index].demand_set.insert(demand_index);

    for(unsigned int i=0; i<G[node_index].size();i++){
        dfs(demand_index, origal_edges[G[node_index][i]].to, deepth+origal_edges[G[node_index][i]].cost);
    }
}
void service_search(){
    search_deepth=4;
    set<unsigned int>::iterator demand_iter;
    for(unsigned int i=0; i<demand_cnt; i++){
        dfs(i,demand2node[i],0);
        for(demand_iter=demand[i].node_set.begin();demand_iter!=demand[i].node_set.end();demand_iter++){
            demand[i].node_array.push_back(*demand_iter);
        }
    }
}

void caculate_degree_sum(){
    for(unsigned int i=0; i<node_cnt; i++){
        int degee_sum=0;
        for(unsigned int j=0; j<G[i].size();j++){
            degee_sum+= origal_edges[G[i][j]].cap;
        }
        service[i].degree_sum=degee_sum;
    }
}

void prepare_for_creat(void)
{
    service_search();
    caculate_degree_sum();
}

set<unsigned int> fix_set;
void pre_fix_service(void)
{
//    for(unsigned int i=0; i<demand_cnt; i++)
//    {
//
//        struct node* node=node_vector[demand->attached_node];
//        unsigned int out_bandwidth_sum=0;
//        unsigned int out_cost_sum=0;
//        unsigned int now_bandwidth;
//
//        while(now_edge)
//        {
//            now_bandwidth=now_edge->edge_bandwidth;
//
////            cout<<"edge:"<<start_node<<"->"<<stop_node<<":"<<" bandwidth:"<<now_bandwidth<<" length:"<<now_edge->length<<endl;
//            if(now_bandwidth+out_bandwidth_sum >= demand->demand)
//            {
//                out_cost_sum+=(now_edge->length)*(demand->demand-out_bandwidth_sum);
//                out_bandwidth_sum=demand->demand;
////                cout<<"cost:"<<out_cost_sum<<" bandwidth:"<<out_bandwidth_sum<<endl;
//                break;
//            }
//            else
//            {
//                out_bandwidth_sum+=now_bandwidth;
//                out_cost_sum+=now_bandwidth*now_edge->length;
//            }
////            cout<<"cost:"<<out_cost_sum<<" bandwidth:"<<out_bandwidth_sum<<endl;
//            now_edge=now_edge->next;
//        }
//        if(out_bandwidth_sum < demand->demand || out_cost_sum>deploy_cost)
//        {
//            pre_fix_cnt++;
//            cost_sum+=deploy_cost;
//
//            demand->demand=0;
//            demand->test_demand=0;
//            fix_service_set.insert(demand->attached_node);
//            assigned_demand.insert(i);
//
//        }else{
//            unassigned_demand.insert(i);
//        }
//    }
}

int select_one()
{
    if(unassigned_demand_set.empty())
    {
        for(unsigned int i=0; i<demand_cnt; i++)
        {
            unassigned_demand_set.insert(i);
        }
    }

//    while(true){
        int rand_demand_index=random()%unassigned_demand_set.size();
        set<unsigned int>::iterator set_iter=unassigned_demand_set.begin();
        while(rand_demand_index--)
        {
            set_iter++;
        }
        rand_demand_index=*set_iter;

        int rand_service_index=random()%demand[rand_demand_index].node_array.size();
        int new_service_index=demand[rand_demand_index].node_array[rand_service_index];
        if(service_set.find(new_service_index)==service_set.end())
        {
            unassigned_demand_set.erase(unassigned_demand_set.find(rand_demand_index));
            service[new_service_index].sele_action();
        }
        return new_service_index;
//    }
}

void init_set(int num){
    service_set.clear();
    while(num--)
    {
        int service_index=select_one();
//        cout<<service_index<<endl;
        service_set.insert(service_index);
    }
}

int find_min_service(void)
{
    int min_flow=100000;
    int min_index=0;
    set<unsigned int>::iterator set_iter;
    for(set_iter=service_set.begin(); set_iter!=service_set.end(); set_iter++)
    {
        if(edges[node2edge[*set_iter]].flow < min_flow)
        {
            min_flow=edges[node2edge[*set_iter]].flow;
            min_index=*set_iter;
        }
    }
    return min_index;
}

int degee_sum=0;
bool extern update_big;
set<unsigned int> unassigned_demand_set;
void update_set(){
    if(update_big){
//        for(unsigned int i=0; i<5; i++){
//            service_set.erase(service_set.find(find_min_service()));
//        }
        service_set.erase(service_set.find(find_min_service()));
    }else
    {
        service_set.erase(service_set.find(find_min_service()));
    }
}
