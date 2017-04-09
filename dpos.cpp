#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<set>
#include<cstring>
#include<iostream>
#include<map>

#include"zkw.h"

#define c1 1.49445
#define c2 1.49445
#define maxgen 5000

#define sizepop 100

#define Vmax 4.0
#define Vmin -4.0

#define dim 1000

#define w_start 1.7
#define w_end 0.6

#define PI 3.1415926

using namespace std;

extern unsigned int last_service_size;
void init_service(set<unsigned int> service_set, unsigned int _last_service_size);
extern unsigned int node_cnt;

extern unsigned int node_cnt,demand_cnt,edge_cnt,deploy_cost,demand_sum;
extern set<unsigned int> service_set;
extern unsigned int last_service_size;
extern set<unsigned int> best_service_set;
extern time_t t1;

extern MCMF_ZKW zkw;

char pop[sizepop][dim]={0};
char pbest[sizepop][dim]={{0}};
char gbest[dim]={0};

char V[sizepop][dim]={0};

int p_flow_cost[sizepop]={0};
int g_flow_cost=0;

void init_set(int num);
extern unsigned int demand_cnt;
int MCMF(void);
extern set<unsigned int> service_set;
extern map<unsigned int, unsigned int> demand2node;

// 种群初始化
extern set<unsigned int > set_from_chen_hang;
extern int mini_cost_from_chen_hang;
void pop_init(void)
{
    int _best_index=0;
    int _best_cost=1000000;
    int _tmp_cost;
    g_flow_cost=1000000;

    memset(pop,sizeof(pop),0);
//    cout<<sizeof(pop)<<endl;
    memset(pbest,sizeof(pbest),0);
//    cout<<sizeof(pbest)<<endl;
    memset(gbest,sizeof(gbest),0);
//    cout<<sizeof(gbest)<<endl;
    memset(V,sizeof(V),0);
//    cout<<sizeof(V)<<endl;

    set<unsigned int>::iterator set_iter;
    for(set_iter=set_from_chen_hang.begin(); set_iter!=set_from_chen_hang.end(); ++set_iter)
    {
        pop[0][*set_iter]=1;
    }
    _best_cost=mini_cost_from_chen_hang;
    _best_index=0;

	for(int i=1;i<sizepop;i++)
	{
        init_set(demand_cnt*2);
//
//        init_service(service_set,last_service_size);
//        _tmp_cost=MCMF();
        zkw.add_service(service_set);
        zkw.Zkw_Flow();
        _tmp_cost=zkw.ans;

        if(_tmp_cost<0)
        {
            _tmp_cost=1000000;
        }
        p_flow_cost[i]=_tmp_cost;

        if(_tmp_cost<_best_cost){
            _best_cost=_tmp_cost;
            _best_index=i;
        }

        set<unsigned int>::iterator set_iter;
        for(set_iter=service_set.begin(); set_iter!=service_set.end(); set_iter++)
        {
            pop[i][*set_iter]=1;
        }
	}

    g_flow_cost=_best_cost;
    memcpy(gbest,pop[_best_index],sizeof(gbest));

	for(unsigned int i=0; i<sizepop; i++)
	{
        memcpy(pbest[i],pop[i],sizeof(pbest[i]));
	}
}

float sigmal(float x)
{
    return 1.0/(1+exp(-x));
}

void print_array(char* arr);
int array2cost(char* arr);
bool con_flg=true;

void PSO_func(int n)
{
	pop_init();
	cout<<"init done"<<endl;
	//迭代寻优
	for(int i=0;i<maxgen;i++)
	{
        con_flg=true;

        int _best_index=-1;
        int _tmp_cost;

        float w;
        float Tmax = 1000.0;

        if(i<Tmax)
        {
            w = w_start -(w_start-w_end)*(i/Tmax)*(i/Tmax);
        }else
        {
            w=w_end;
        }

		for(int j=0;j<sizepop;j++)
		{
            service_set.clear();
			//速度更新及粒子更新
			for(unsigned int k=0;k<node_cnt;k++)
			{
				// 速度更新
				float rand1 = (float)rand()/RAND_MAX; //0到1之间的随机数
				float rand2 = (float)rand()/RAND_MAX;
				float rand3 = (float)rand()/RAND_MAX;


				V[j][k] = w*V[j][k] + c1*rand1*(pbest[j][k]-pop[j][k]) + c2*rand2*(gbest[k]-pop[j][k]);

				if(V[j][k] > Vmax)
					V[j][k] = Vmax;
				if(V[j][k] < Vmin)
					V[j][k] = Vmin;
				// 粒子更新
				if(rand3< sigmal(V[j][k]))
				{
				    pop[j][k]=1;
				    service_set.insert(k);
				}
                else
                {
                    pop[j][k]=0;
                }
			}

//            init_service(service_set,last_service_size);
//			_tmp_cost=MCMF();
//
			if(_tmp_cost!=g_flow_cost) {con_flg=false;}
//			cout<<_tmp_cost<<endl;
//            init_service(service_set,last_service_size);
//			_tmp_cost=MCMF();
//			cout<<_tmp_cost<<endl;
            zkw.add_service(service_set);
            zkw.Zkw_Flow();
            _tmp_cost=zkw.ans;

			if(_tmp_cost>0)
			{
                if(_tmp_cost<p_flow_cost[j])
                {
                    // 个体极值更新
                    p_flow_cost[j]=_tmp_cost;
                    memcpy(pbest[j],pop[j],sizeof(pbest[j][0])*node_cnt);
//                    cout<<array2cost(pbest[j])<<endl;
//                    cout<<array2cost(pop[j])<<endl;
                    if(p_flow_cost[j]<g_flow_cost)
                    {
                        g_flow_cost=p_flow_cost[j];
                        _best_index=j;
                    }
                }
			}
		}
        // 群体极值更新
		if(_best_index>=0){
            memcpy(gbest,pbest[_best_index],sizeof(gbest[0])*node_cnt );
            cout<<array2cost(pbest[_best_index])<<endl;
            cout<<array2cost(gbest)<<endl;
        }

        if(con_flg) {break;}
        if(time(NULL)-t1>88) {break;}

        cout<<"min cost:"<<g_flow_cost<<endl;
	}
	cout<<"min cost:"<<g_flow_cost<<endl;

}

void dpos(void)
{
    PSO_func(3);
//	for(int i=1;i<=5;i++)
//	{
//        PSO_func(i);
//	}
}

void print_array(char* arr)
{
    for(unsigned int i=0; i<node_cnt; i++){
        cout<<" "<<(int)*(arr+i);
    }
    cout<<endl;
}

int array2cost(char* arr)
{
    service_set.clear();
    for(unsigned int i=0; i<node_cnt; i++){
        if(arr[i]>0)
            service_set.insert(i);
    }
    init_service(service_set,last_service_size);
    return MCMF();
}
