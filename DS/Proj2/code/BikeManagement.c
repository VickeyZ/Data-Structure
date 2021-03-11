#include<stdio.h>
#include<stdlib.h>

#define INFINITY 1000000
#define MaxNum 500//Number of stations
typedef int Vertex;//Index of stations

typedef struct GraphRecord *PtrToGraph;
struct GraphRecord{
    int Nv;
    int Ne;
    int Edge[MaxNum+1][MaxNum+1];           //Time cost between two stations
};
typedef PtrToGraph Graph;                   //Define the structure of the graph
Graph ReadGraph();                          //Handle the input files
int ShortestTime();                  //Get the shorteset time
void FindPath();                    //Store every path within shortest time in Path[PathNum][]
void CntBike();                             //Count the number of bikes sent and taken back along the path
int SortBike();                             //Find out the path with the least number of bikes sent and taken back

Vertex P;//The index of the problem station
Graph G;
int cap;//Record the capibility of each station
int BikeNum[MaxNum];//Record the number of bikes in each station

int MinTime, time[MaxNum], CntTime = 0;//Record the least time, the time to reach PBMC, the time spent along one selected way
int Path[MaxNum][MaxNum], PathNum = 0;//Store each path, and the number of vertex along the path is stored in Path[PathNum][0]
int Stack[MaxNum], top = -1;//Store index along the path in process
static int SentBike[MaxNum], TakeBack[MaxNum];//Record the number of bikes sent or taken back along the way

int main()
{
    G = ReadGraph();//Read the graph
    MinTime = ShortestTime();//Get the shortest time spent
    FindPath();//Find each path from PBMC to the problem station
    CntBike();//Get the number of bikes sent or taken back along each path
    int num = SortBike();//Get the index of the path with the least number of sent bikes
    printf("%d ", SentBike[num]);
    for(int i = 1; i <= Path[num][0]; i++)
    {
        if(i == 1) printf("0");
        else printf("->%d", Path[num][i]);
    }
    printf(" %d", TakeBack[num]);
    return 0;
}

Graph ReadGraph()
{
    int i, j;
    Graph G = (Graph)malloc(sizeof(struct GraphRecord));
    scanf("%d %d %d %d", &cap, &G->Nv, &P, &G->Ne);
    for(i = 1; i <= G->Nv; i++)
        scanf("%d", &BikeNum[i]);
    for(i = 0; i <= MaxNum; i++)
        for(j = 0; j <= MaxNum; j++)
            G->Edge[i][j] = -1;//Initialize
               
    for(i = 0; i < G->Ne; i++)
    {
        int m, n, tmp;
        scanf("%d %d", &m, &n);
        scanf("%d", &G->Edge[m][n]);
        G->Edge[n][m] = G->Edge[m][n];
    }
    return G;
}

int ShortestTime()
{
    int i;
    static int known[MaxNum];
    for(i = 1; i <= G->Nv; i++)
    {
        if(G->Edge[0][i] > 0)
            time[i] = G->Edge[0][i];
        else    time[i] = INFINITY;            
    }
    time[0] = 0;
    known[0] = 1;//Initialize
    Vertex V;

    for(;;)
    {
        for(i = 1; i <= G->Nv; i++)
            if(!known[i]) break;
        V = i;
        for(i = 1; i <= G->Nv; i++)
            if(!known[i] && time[i] < time[V])
                V = i;//Find the unknown vertex with the least time
        if(V == P) break;//Reach the problem station within the least time, end the loop

        known[V] = 1;
        for(i = 0; i <= G->Nv; i++)
        {
            if(G->Edge[V][i] > 0)
            {
                if(time[V] + G->Edge[V][i] < time[i])
                    time[i] = time[V] + G->Edge[V][i];//Update the time spent along the path
            }
        }
    }
    return time[V];
}

void FindPath()
{
    int i, j, cnt;
    static int visit[MaxNum][MaxNum];//Whether Edge[i][j] is visited 
    Stack[++top] = 0;//Push PBMC into the stack
    while(top != -1)
    {
        cnt = 0;
        Vertex a = Stack[top];
        for(i = 1; i <= G->Nv; i++)  
        {
            for(j = 0; j <= top; j++)
                if(i == Stack[j])
                    break;            
            if(G->Edge[a][i] > 0 && !visit[a][i] && j > top)//For each adjacent and unvisited edge
            {
                visit[a][i] = 1;
                CntTime += G->Edge[a][i];
                if(CntTime > MinTime)//Invalid vertex
                {
                    CntTime -= G->Edge[a][i];
                    cnt++;
                    continue;
                }
                else 
                {
                    Stack[++top] = i;//Push into the stack
                    if(i == P && CntTime == MinTime)//Reach the problem station within the least time
                    {
                        int j;
                        for(j = 0; j <= top; j++)
                        //{
                            Path[PathNum][j+1]= Stack[j];
                        //    printf("%d", Stack[j]);
                        //}
                        //printf("\n");
                        Path[PathNum++][0] = j;//The number of vertex along the path
                        CntTime -= G->Edge[a][i];
                        top--;//Pop the top vertex in the stack and continue the loop
                        cnt++;
                        continue;
                    }       
                    else break;               
                }

            }
            else cnt++;
        }
        if(cnt == G->Nv)//Invalid top vertex
        {
            a = Stack[top];
            CntTime -= G->Edge[Stack[top-1]][a];
            for(i = 1; i <= G->Nv; i++)
                visit[a][i] = 0;
            top--;
        }
    }
}

void CntBike()
{
    int BikeRecord, BikeNow;
    int i, j;
    for(i = 0; i < PathNum; i++)//i = the index of the path select
    {
        BikeNow = 0;//The number of bikes taken from stations along the path
        BikeRecord = 0;//The number of bikes taken from the PBMC
        for(j = 2; j <= Path[i][0]; j++)// j = the index of the station along the path, start from the first station
        {
            if(BikeNum[Path[i][j]] >= cap/2)
                BikeNow += BikeNum[Path[i][j]] - cap/2;
            else if(BikeNow >= cap/2 - BikeNum[Path[i][j]])
                BikeNow -= cap/2 - BikeNum[Path[i][j]];
            else
            {
                BikeRecord += cap/2 - BikeNum[Path[i][j]] - BikeNow;
                BikeNow = 0;
            }            
        }
        SentBike[i] = BikeRecord;
        TakeBack[i] = BikeNow;
    }
}


int SortBike()
{
    int i;
    int minsent = INFINITY, mintake = INFINITY;
    int index;
    for(i = 0; i < PathNum; i++)
    {
        if(SentBike[i] == minsent)//Compare the paths with the same number of sent bikes
        {
            if(TakeBack[i] < mintake)
            {
                mintake = TakeBack[i];
                index = i;
            }
        }
        else if(minsent > SentBike[i])
        {
            minsent = SentBike[i];
            mintake = TakeBack[i];
            index = i;
        }
    }
    return index;
}
