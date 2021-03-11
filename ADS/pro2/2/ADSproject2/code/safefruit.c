//ADSproject2 safe fruit
#include <stdio.h>
#include <stdlib.h> 
#define INF 1000000
typedef enum 
{
	false=0,true=1
}bool;
typedef struct fgraph//fruit graph
{
	int v;//quantity of fruits
	bool graph[1000][1000];//since all fruits are expressed by 'xxx',we can use 000-999 to express them  	
}FG;

FG	G;//define a fruit graph

int price[1000];//price[i] to express  the price of fruit i 
int bestprice=INF;// the least price of max fruits set
int curprice=0; //the price of currenr price set
int currentf[1000];//what fruits in the current clique,it is a stack 
int top=0;//the stack top of currentf
int fruits[1000];//these fruits order is not continous,so we need an array to store these fruits order  
int maxf=0;//max quantity of the fruits in clique
int maxclique[1000];//to store the max clique of the fruits 
int maxleft[1000];//to store how many fruits can there exists in clique from fruits[i] to fruits[G.v] 
void graphinitial(int m);//to initialize the graph
void FindMaxFruit();//to find the max clique of the fruits
void dfs(int *leftf,int k,int curn);//use dfs to backtrack,curn means the quantity of the current clique

int cmp(const void *a, const void *b)//the function parameter of qsort() 
{
     return(*(int *)a-*(int *)b);
}
int main()
{
	int i,j,n,m;
	int f1,f2,x;
	scanf("%d %d",&n,&m);
	graphinitial(m);//initialize the graph
	
	for(i=0;i<n;i++)//input the fruits can't be eaten together
	{
		scanf("%d %d",&f1,&f2);
		G.graph[f1][f2]=0;//f1,f2 can't be eaten together
		G.graph[f2][f1]=0;
	}
	for(i=0;i<m;i++)//assign the fruits with their price
	{
		scanf("%d %d",&f1,&x);
		price[f1]=x;
		fruits[i+1]=f1;//from fruits[1] to fruits[G.v] store the order of these fruits
	}
	FindMaxFruit();//to find the clique with max quantity and min price 
	//output 
	printf("%d\n",maxf);//out put the max quantity
	int a1=1,p=0;//a1 used to count£¬p is the total price
	
	qsort(maxclique,maxf,sizeof(int),cmp);//store the fruits order in ascending order 
	
	for(i=0;i<maxf;i++)//print the fruits
	{
		printf("%03d%c",maxclique[i],a1!=maxf?' ':'\n'); 
		a1++;
		p+=price[maxclique[i]];
	}
	printf("%d",p);//print the price
	return 0; 
}

void graphinitial(int m)
{
	G.v=m;
	int i,j; 
	for(i=0;i<=999;i++)//to make every vertex connected with the other vertexs
	{
		for(j=0;j<=999;j++)
		{
			if(i!=j)//the vertex isn't connected with itself. to avoid infinite loop
			{
				G.graph[i][j]=1;
			}
			else
			{
				G.graph[i][j]=0;
			}	
		}
	}
}

void FindMaxFruit()
{
	int i,j,k;
	int leftf[100];//leftf records the fruits which can eat from fruits[i+1] to fruits[G.v] now
	for(i=0;i<=999;i++)
	{
		maxleft[i]=INF;//initialize maxleft
	}
	for(i=G.v;i>=1;i--)//tranverse from G.v can optimize the performance
	{
		k=0;//k is the stack top of leftf 		
		currentf[top++]=fruits[i];//store fruits[i] into currentf
		curprice+=price[fruits[i]];//add the price to curprice
		for(j=i+1;j<=G.v;j++)
		{
			if(G.graph[fruits[i]][fruits[j]])leftf[k++]=fruits[j];//leftf records the fruits form i+1 to G.v which can be eaten together 
		}
		dfs(leftf,k,1);
		curprice-=price[fruits[i]];//minus the price 	
		top--;//pop the fruit from the currentf
		maxleft[fruits[i]]=maxf;//record the fruits can br put in the clique from i to G.v
	}	 
}

void dfs(int *leftf,int k,int curn)//leftf records the fruits form i+1 to G.v which can be eaten together,k is leftf's stack top,curn means the quantity of the current clique
{
	int i;
	if(k!=0&&curn>maxf)//Updating the maxf early can help improve the efficiency
	{
		maxf=curn;
		bestprice=curprice;
	}
	if(k==0) {//This is the last node of the maxclique, it won't have any other edges so k is equal to 0
		if (curn>maxf){
			maxf=curn;
			bestprice=curprice;
			for(i=0;i<top;i++)
			{
				maxclique[i]=currentf[i];//put the fruits into maxclique
			}
		}
		else if(curn==maxf)
		{
			if(curprice<bestprice)
			{
				for(i=0;i<top;i++)
				{
					maxclique[i]=currentf[i];//put the fruits into maxclique	
				}	 
				bestprice=curprice;
			}
		} 
		return;
	}
	int newleftf[100],j,x;
	for(i=0;i<k;i++)//tranverse the leftf
	{
		x=0;
		//pruning
		if(curn+k-i<maxf)return;//if the curn+(fruits left)<maxf,then prun
		if(curn+maxleft[leftf[i]]<maxf)return;//if the curn+(fruits can be in the clique form i)<maxf,then prun
		for(j=i+1;j<k;j++)
		{
			if(G.graph[leftf[i]][leftf[j]])newleftf[x++]=leftf[j];//add the fruits can be eaten together to newleftf
		}
		currentf[top++]=leftf[i];
		curprice+=price[leftf[i]];
		dfs(newleftf,x,curn+1);
		curprice-=price[leftf[i]];
		top--;	
	}
	return;
}
