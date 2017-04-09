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

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

unsigned int node_cnt,demand_cnt,edge_cnt,deploy_cost,demand_sum;
map<unsigned int,unsigned int> node2demand;
map<unsigned int, unsigned int> demand2node;
extern set<unsigned int> service_set;
extern set<unsigned int> best_service_set;

#define N 2000
#define INF 100000000

struct Edge
{
    int from,to,cap,flow,cost;
    Edge(int u,int v,int ca,int f,int co):from(u),to(v),cap(ca),flow(f),cost(co){};
};


int n,m,s,t;
vector<Edge> origal_edges;
vector<Edge> edges;
vector<int> G[N];
int inq[N];//是否在队列中
int d[N];//距离
int p[N];//上一条弧
int a[N];//可改进量

void init(int node_cnt)//初始化
{
    n=node_cnt;
    for(int i=0;i<n;i++)
        G[i].clear();
    edges.clear();
}

map<unsigned int,unsigned int> node2edge;
void addedge(int from,int to,int cap,int cost)//加边
{
    edges.push_back(Edge(from,to,cap,0,cost));
    // edges.push_back(Edge(to,from,0,0,-cost));
    int m=edges.size();
    G[from].push_back(m-1);
    // G[to].push_back(m-1);
}

void addedge_for_sink(int from,int to,int cap,int cost)//加边
{
    edges.push_back(Edge(from,to,cap,0,cost));
    // edges.push_back(Edge(to,from,0,0,-cost));
    int m=edges.size();
    G[from].push_back(m-1);
    node2edge[from]=m-1;
}

void deleteedge(void)
{
    unsigned int from=edges[edges.size()-1].from;
    unsigned int to=edges[edges.size()-1].to;
    edges.pop_back();
//    edges.pop_back();
    G[from].pop_back();
//    G[to].pop_back();
}

void init_graph(char * topo[], int line_num)
{
    sscanf(topo[0],"%d %d %d",&node_cnt,&edge_cnt,&demand_cnt);
    sscanf(topo[2],"%d",&deploy_cost);

    init(node_cnt+3);

    unsigned int start_node,stop_node,bandwidth,length;

    for(unsigned int i=4; i<4+edge_cnt; i++)
    {
        sscanf(topo[i],"%d %d %d %d",&start_node,&stop_node,&bandwidth,&length);
        addedge(start_node,stop_node,bandwidth,length);
        addedge(stop_node,start_node,bandwidth,length);
    }

    demand_sum=0;
    unsigned int demand_index,service_index,demand;
    for(unsigned int i=5+edge_cnt; i<5+edge_cnt+demand_cnt; i++)
    {
        sscanf(topo[i],"%d %d %d",&demand_index,&service_index,&demand);
        demand_sum+=demand;
        addedge(node_cnt,service_index,demand,0);
        node2demand[service_index]=demand_index;
        demand2node[demand_index]=service_index;
    }
    origal_edges.assign(edges.begin(), edges.end());
}

unsigned int last_service_size=0;

void init_service(set<unsigned int> service_set, unsigned int _last_service_size)
{
    for(unsigned int i=0; i<_last_service_size; i++)
    {
        deleteedge();
    }

    edges.clear();
    edges.assign(origal_edges.begin(),origal_edges.end());

    set<unsigned int>::iterator service_iter;
    for(service_iter=service_set.begin();service_iter!=service_set.end();++service_iter)
    {
        addedge_for_sink(*service_iter,node_cnt+1,INF,0);
    }
    last_service_size=service_set.size();
}

bool SPFA(int s,int t,int &flow,int &cost)//寻找最小费用的增广路，使用引用同时修改原flow,cost
{
    for(int i=0;i<n;i++)
        d[i]=INF;
    memset(inq,0,sizeof(inq));
    d[s]=0;inq[s]=1;p[s]=0;a[s]=INF;
    deque<int> Q;
//    int que_sum=0;
//    int ave_dis=0;
    Q.push_back(s);
//    que_sum+=d[s];

    while(!Q.empty())
    {
//        if(Q.size()>2){
//            ave_dis=que_sum/Q.size();
//            int tmp_n=Q.front();
//            while(d[tmp_n]>ave_dis){
//                Q.pop_front();
//                Q.push_back(tmp_n);
//                tmp_n=Q.front();
//            }
//        }

        int u=Q.front();
        Q.pop_front();
//        que_sum-=d[u];

        inq[u]--;
        for(int i=0;i<G[u].size();i++)
        {
            Edge& e=edges[G[u][i]];
            if(e.cap>e.flow && d[e.to]>d[u]+e.cost)
            {
                d[e.to]=d[u]+e.cost;
                p[e.to]=G[u][i];
                a[e.to]=min(a[u],e.cap-e.flow);
                if(!inq[e.to])
                {
                    inq[e.to]++;

                    if (!Q.empty())
                    {
                        if(d[e.to]>d[Q.front()])
                            Q.push_back(e.to);
                        else
                            Q.push_front(e.to);
                    }
                    else{
                        Q.push_back(e.to);
                    }
//                    que_sum+=d[e.to];
                }
            }
        }
    }
    if(d[t]==INF) return false;//汇点不可达则退出
    flow+=a[t];
    cost+=d[t]*a[t];
    int u=t;
    while(u!=s)//更新正向边和反向边
    {
        if(p[u] < edge_cnt*2){
            if(edges[p[u]].cap==edges[p[u]^1].cap)
            {
                edges[p[u]^1].cost=-edges[p[u]^1].cost;
                edges[p[u]^1].cap=0;
            }
            edges[p[u]].flow+=a[t];
            edges[p[u]^1].flow-=a[t];
            if(edges[p[u]].flow==0){
                edges[p[u]].cost=-edges[p[u]].cost;
                edges[p[u]].cap=edges[p[u]^1].cap;
            }
        }else{
            edges[p[u]].flow+=a[t];
        }
        u=edges[p[u]].from;
    }
    return true;
}

int MincotMaxflow(int s,int t)
{
    int flow=0,cost=0;
    while(SPFA(s,t,flow,cost)){};
//    cout<<flow<<":"<<cost<<endl;
    if(flow==demand_sum){
        cost+=deploy_cost*service_set.size();
        return cost;
    }else{
//        cout<<"INVALID!"<<endl;
        return -1;
    }
}

int MCMF(void)
{
    return MincotMaxflow(node_cnt,node_cnt+1);
}

string flow2string(void)
{
	int remain_deman = demand_sum;

	string out_string = "";
	string route_string = "";
	unsigned int route_cnt;

	stack<Edge*> route;
	int route_flow;

	unsigned int cur_node;
	Edge* e=NULL;

	route_cnt = 0;

	int _tmp_route_cost;
	while (remain_deman){
		route_flow = INF;
		cur_node = node_cnt;
		route_string = "\n";

		_tmp_route_cost=0;
		while (cur_node != node_cnt + 1){
			//find a e
			for (unsigned int i = 0; i<G[cur_node].size(); i++)
			{
				e = &edges[G[cur_node][i]];
				if (e->cost >= 0 && e->flow>0){ break; }
			}
			//push e
			route.push(e);
			cur_node = e->to;
			route_flow = min(route_flow, e->flow);
			//update cur_node

			_tmp_route_cost+=e->cost;
		}


		remain_deman -= route_flow;
		//find a route
		route.pop();
		while (!route.empty())
		{
			e = route.top();
			route.pop();
			route_string = route_string.append(SSTR(e->to));
			route_string = route_string.append(" ");
			e->flow -= route_flow;
		}
		route_string = route_string.append(SSTR(node2demand[e->to]));
		route_string = route_string.append(" ");
		route_string = route_string.append(SSTR(route_flow));

        route_string = route_string.append(" deepth:");
		route_string = route_string.append(SSTR(_tmp_route_cost));

		route_cnt++;
		//        cout<<route_string<<endl;
		out_string.append(route_string);
	}
	string cnt_string = SSTR(route_cnt);
	cnt_string = cnt_string.append("\n");
	out_string = cnt_string.append(out_string);
	return out_string;
}





