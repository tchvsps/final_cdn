
#include "zkw.h"
#include<map>

#include<iostream>
#include<string>
#include<sstream>


//http://blog.sina.com.cn/s/blog_76f6777d0101bbc8.html
using namespace std;


#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

extern unsigned int node_cnt;
extern unsigned int demand_cnt;


void MCMF_ZKW::dw(int &a,int b){ if (b< a) a=b; }

void MCMF_ZKW:: add(int a,int b,int up,int co)
{
    ++tot;
    V[tot]=b;G[tot]=up;C[tot]=co;
    N[tot]=F[a];  F[a]=tot;
}

void MCMF_ZKW::add_two(int a,int b,int up,int co)
{
    ++tot;
    V[tot]=b;G[tot]=up;C[tot]=co;
    N[tot]=F[a];  F[a]=tot;

    ++tot;
    V[tot]=a;G[tot]=up;C[tot]=co;
    N[tot]=F[b];  F[b]=tot;

    B[tot]=tot-1; B[tot-1]=tot;
}

void MCMF_ZKW::delete_service(void)
{
    int _tmp_node_index;
    for(unsigned int i=0; i<this->zkw_last_service_size; i++)
    {
        _tmp_node_index=this->last_service_vector[i];
        F[_tmp_node_index]=N[F[_tmp_node_index]];
    }
    tot-=this->zkw_last_service_size;
    memcpy(G,origal_G,sizeof(G[0])*(tot+1));
    memcpy(C,origal_C,sizeof(C[0])*(tot+1));
}

extern unsigned int edge_cnt;

int MCMF_ZKW:: aug(int u,int f)
{
    int p,t,left=f;
    if (u==T) {
        all_demand+=f; ans+=f*d[S];
//        cout<<"reach"<<u<<" bandwidth:"<<f<<" deep:"<<d[S]<<endl;
        return f;
    }

//    if(u==16||u==0)
//    {
//        cout<<"key:"<<endl;
//    }

    v[u]=1;
//    cout<<"visit in "<<u<<endl;
    for (p=F[u];p;p=N[p])
    {
        if (G[p]>0&&!v[V[p]])
        {
            t=d[V[p]]+C[p]-d[u];
            if (t==0)
            {
                int delt=aug(V[p],G[p]< left? G[p] : left);
                if (delt>0) {
//                    cout<<"update:"<<u<<"->"<<V[p]<<": "<<delt<<endl;
                    if(p<=edge_cnt*2){
                        if(G[p]==origal_G[p]&&G[B[p]]==origal_G[p])
                        {
                            C[B[p]]=-C[B[p]];
                            G[B[p]]=0;
                        }
                        G[p]-=delt,G[B[p]]+=delt,left-=delt;
                        if(C[p]<0 && G[p]==0)
                        {
                            G[p]=G[B[p]];
                            C[p]=-C[p];
                        }
                    }else{
                        G[p]-=delt;left-=delt;
                    }
                }

                if (left==0){
//                cout<<"back from "<<u<<endl;
                return f;
                }
            }else dw(slk[V[p]],t);
        }
    }

//    cout<<"back from "<<u<<endl;
    return f-left;
}

bool MCMF_ZKW:: modlabel()
{
    int delt=INF,i;
    for (i=0;i<=T;i++)
      if (!v[i]) { dw(delt,slk[i]);slk[i]=INF;}
    if (delt==INF) return true;
    for (i=0;i<=T;i++)
      if (v[i]) d[i]+=delt;
    return false;
}

extern unsigned int deploy_cost;
extern set<unsigned int> service_set;
extern unsigned int demand_sum;

unsigned int thing_in_node[2000];

void MCMF_ZKW:: flow_test()
{
    memset(thing_in_node,sizeof(thing_in_node),0);
    thing_in_node[node_cnt]=demand_sum;
    queue<unsigned int> bfs_queue;
    bfs_queue.push(node_cnt);

    unsigned int current_node;
    unsigned int node_demand_out;
    unsigned int current_edge;
    while(!bfs_queue.empty()){
        current_node=bfs_queue.front();
        bfs_queue.pop();
//        cout<<"node:"<<current_node<<endl;
        if(current_node==22)
        {
            cout<<"key:"<<endl;
        }

        for(current_edge=F[current_node];current_edge;current_edge=N[current_edge])
        {
            if(C[current_edge]>=0){
                int _tmp_origal_G;
                if(current_edge<=edge_cnt*2+demand_cnt) {_tmp_origal_G=origal_G[current_edge];}
                else{_tmp_origal_G=INF;}
                int _tmp_flow=min((int)thing_in_node[current_node],_tmp_origal_G-G[current_edge]);
                if(_tmp_flow>0)
                {
                    cout<<current_node<<"->"<<V[current_edge]<<":"<<_tmp_flow<<endl;
                    thing_in_node[current_node]-=_tmp_flow;
                    thing_in_node[V[current_edge]]+=_tmp_flow;
                    G[current_edge]+=_tmp_flow;
                    bfs_queue.push(V[current_edge]);
                }
            };
        }
    }

    cout<<"end of tran:";
    for(unsigned int i=0; i<node_cnt+2;i++){
        if(thing_in_node[i]==0){cout<<" "<<thing_in_node[i];}
        else{cout<<" "<<thing_in_node[i]<<"("<<i<<")";}
    }

    cout<<endl;
}

map<unsigned int, unsigned int> node2level;

void MCMF_ZKW:: Zkw_Flow()
{
     int i;ans=0;
     all_demand=0;

     for (i=0;i<=T;i++) d[i]=0,slk[i]=INF;
     do{
//        cout<<endl<<"new dist"<<d[S]<<endl;
         do {
            memset(v,0,sizeof(v[0])*(node_cnt+1));
//            cout<<"new route"<<endl;
         }while (aug(S,INF));

     }while (!modlabel());

//    cout<<ans<<endl;
    if(all_demand<demand_sum)
    {
        ans=-1;
    }else
    {
         ans+=zkw_last_service_size*deploy_cost;
          node2level.clear();
         for(unsigned int i=0; i<this->zkw_last_service_size; i++)
         {
            node2level[this->last_service_vector[i]]=0;
         }
    }
//    cout<<ans<<endl;

}

void MCMF_ZKW:: Init (int _s,int _t)    //算法初始化
{
  S=_s;T=_t;
}

void MCMF_ZKW:: init_graph(char * topo[], int line_num)
{

    memset(G,sizeof(G),0);
    memset(V,sizeof(V),0);
    memset(C,sizeof(C),0);
    memset(N,sizeof(N),0);
    memset(F,sizeof(F),0);
    memset(B,sizeof(B),0);
    tot=0;
    int _node_cnt,_edge_cnt,_demand_cnt;
    sscanf(topo[0],"%d %d %d",&_node_cnt,&_edge_cnt,&_demand_cnt);


    unsigned int start_node,stop_node,bandwidth,length;

    for(unsigned int i=4; i<4+_edge_cnt; i++)
    {
        sscanf(topo[i],"%d %d %d %d",&start_node,&stop_node,&bandwidth,&length);
        add_two(start_node,stop_node,bandwidth,length);
    }

    unsigned int demand_index,service_index,demand;
    for(unsigned int i=5+_edge_cnt; i<5+_edge_cnt+_demand_cnt; i++)
    {
        sscanf(topo[i],"%d %d %d",&demand_index,&service_index,&demand);
        add(_node_cnt,service_index,demand,0);
    }

    memcpy(origal_G,G,sizeof(G[0])*(tot+1));
    memcpy(origal_V,V,sizeof(V[0])*(tot+1));
    memcpy(origal_C,C,sizeof(C[0])*(tot+1));
    memcpy(origal_N,N,sizeof(N[0])*(tot+1));
    memcpy(origal_B,B,sizeof(B[0])*(tot+1));

    memcpy(origal_F,F,sizeof(F[0])*(node_cnt+1));
    origal_tot=tot;
    zkw_last_service_size=0;
}


void MCMF_ZKW:: add_service(set<unsigned int> ser_set)
{
    delete_service();

//    memcpy(G,origal_G,sizeof(G));
//    memcpy(V,origal_V,sizeof(V));
//    memcpy(C,origal_C,sizeof(C));
//    memcpy(N,origal_N,sizeof(N));
//    memcpy(F,origal_F,sizeof(F));
//    memcpy(B,origal_B,sizeof(B));
//    tot=origal_tot;

    this->zkw_last_service_size=0;
    set<unsigned int>::iterator set_iter;
    for(set_iter=ser_set.begin(); set_iter!=ser_set.end(); set_iter++)
    {
        add(*set_iter,node_cnt+1,INF,0);
        last_service_vector[this->zkw_last_service_size++]=(*set_iter);
    }
}


string route_string;
unsigned int service_level;
unsigned int route_cnt=0;

extern map<unsigned int,unsigned int> node2demand;

int MCMF_ZKW::dfs_for_route(unsigned int _cur_node, unsigned int in_flow)
{
    if(_cur_node==this->T){
        route_string="\n";
        return in_flow;
    }

    unsigned int _cur_edge;
    unsigned int _tmp_cap;

    for(_cur_edge=F[_cur_node];_cur_edge;_cur_edge=N[_cur_edge])
    {
        if(C[_cur_edge]>=0){
            _tmp_cap=origal_G[_cur_edge]-G[_cur_edge];
            if(_tmp_cap>0)
            {
                if(V[_cur_edge]==this->T){service_level=node2level[_cur_node];}
                unsigned int _tmp_flow=dfs_for_route(V[_cur_edge],_tmp_cap<in_flow?_tmp_cap:in_flow);
                if(_tmp_flow>0){
                    G[_cur_edge]+=_tmp_flow;
                    if(_cur_node==this->S){
                        route_string = route_string.append(SSTR(node2demand[V[_cur_edge]]));
                        route_string = route_string.append(" ");
                        route_string = route_string.append(SSTR(_tmp_flow));
                        route_string = route_string.append(" ");
                        route_string = route_string.append(SSTR(service_level));
                    }else{
                        route_string=route_string.append(SSTR(_cur_node));
                        route_string=route_string.append(" ");
                    }
                }
                return _tmp_flow;
            }
        }
    }
}

string MCMF_ZKW::flow_2_route(void)
{
    string _out_string="";
    route_cnt=0;
    route_string="";

    while(dfs_for_route(this->S,INF))
    {
        route_cnt++;
        _out_string=_out_string.append(route_string);
    }
    string _cnt_string=SSTR(route_cnt);
    _cnt_string=_cnt_string.append("\n");
    _out_string=_cnt_string.append(_out_string);
    return _out_string;
}






