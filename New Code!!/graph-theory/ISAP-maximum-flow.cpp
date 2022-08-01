﻿//Improved Shortest Augment Path Algorighm 最大流（ISAP版本） O(n^2 m)
//By ysf
//注意ISAP适用于一般稀疏图，对于二分图或分层图情况Dinic比较优，稠密图则HLPP更优

//边的定义
//这里没有记录起点和反向边，因为反向边即为正向边xor 1，起点即为反向边的终点
struct edge{int to,cap,prev;}e[maxe<<1];

//全局变量和数组定义
int last[maxn],cnte=0,d[maxn],p[maxn],c[maxn],cur[maxn],q[maxn];
int n,m,s,t;//s,t一定要开成全局变量

//重要！！！
//main函数最前面一定要加上如下初始化
memset(last,-1,sizeof(last));

//加边函数 O(1)
//包装了加反向边的过程，方便调用
//需要调用AddEdge
void addedge(int x,int y,int z){
	AddEdge(x,y,z);
	AddEdge(y,x,0);
}

//真·加边函数 O(1)
void AddEdge(int x,int y,int z){
	e[cnte].to=y;
	e[cnte].cap=z;
	e[cnte].prev=last[x];
	last[x]=cnte++;
}

//主过程 O(n^2 m)
//返回最大流的流量
//需要调用bfs、augment
//注意这里的n是编号最大值，在这个值不为n的时候一定要开个变量记录下来并修改代码
//非递归
int ISAP(){
	bfs();
	memcpy(cur,last,sizeof(cur));
	int x=s,flow=0;
	while(d[s]<n){
		if(x==t){//如果走到了t就增广一次，并返回s重新找增广路
			flow+=augment();
			x=s;
		}
		bool ok=false;
		for(int &i=cur[x];~i;i=e[i].prev)
			if(e[i].cap&&d[x]==d[e[i].to]+1){
				p[e[i].to]=i;
				x=e[i].to;
				ok=true;
				break;
			}
		if(!ok){//修改距离标号
			int tmp=n-1;
			for(int i=last[x];~i;i=e[i].prev)
				if(e[i].cap)tmp=min(tmp,d[e[i].to]+1);
			if(!--c[d[x]])break;//gap优化，一定要加上
			c[d[x]=tmp]++;
			cur[x]=last[x];
			if(x!=s)x=e[p[x]^1].to;
		}
	}
	return flow;
}

//bfs函数 O(n+m)
//预处理到t的距离标号
//在测试数据组数较少时可以省略，把所有距离标号初始化为0
void bfs(){
	memset(d,-1,sizeof(d));
	int head=0,tail=0;
	d[t]=0;
	q[tail++]=t;
	while(head!=tail){
		int x=q[head++];
		c[d[x]]++;
		for(int i=last[x];~i;i=e[i].prev)
			if(e[i^1].cap&&d[e[i].to]==-1){
				d[e[i].to]=d[x]+1;
				q[tail++]=e[i].to;
			}
	}
}

//augment函数 O(n)
//沿增广路增广一次，返回增广的流量
int augment(){
	int a=(~0u)>>1;
	for(int x=t;x!=s;x=e[p[x]^1].to)a=min(a,e[p[x]].cap);
	for(int x=t;x!=s;x=e[p[x]^1].to){
		e[p[x]].cap-=a;
		e[p[x]^1].cap+=a;
	}
	return a;
}
