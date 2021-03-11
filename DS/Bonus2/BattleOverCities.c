#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define maxn 501
#define INFINITY 999999

struct RoadRecord
{
    int C1, C2;
    int Cost;
    int Status;
}Road[maxn]; /* Record all the roads in an array of structure */
int N, M;
int cityseq[maxn], citynum; /* Record connected city sequence */

int cmp(const void* a, const void* b)
{
    return (*(int *)a - *(int *)b);
}

int add(int a) /* Get the minmum cost when city a is added to the connected sequence */ 
{
    int i, j, c1, c2;
    int costrecord = 0;
    int min = INFINITY, idx = -1;
    for(i = 0; i < M; i++) /* Go through all the roads */
    {
        c1 = Road[i].C1;
        c2 = Road[i].C2;
        if(c1 != a && c2 != a) continue;
        for(j = 0; j < citynum; j++) 
        {
            if(c1 == cityseq[j] || c2 == cityseq[j]) /* Find valid roads between a and the connected sequence */
            {
                if(min > Road[i].Cost)
                {
                    min = Road[i].Cost;
                    idx = i;
                }
                break;
            }
        }        
    }
    if(idx != -1)
    {
        costrecord += min;
        cityseq[citynum++] = a;
    }
    return costrecord;

}

int getcost(int a) /* Get the cost of one destroyed city a */
{
    int i,c1, c2;
    int costsum = 0;
    int visit[maxn]; /* Judge wether added to the connected sequence */
    memset(visit, 0, maxn);
    citynum = 0;
    memset(cityseq, 0, maxn);     
    for(i = 0; i < M; i++)
    {
        c1 = Road[i].C1;
        c2 = Road[i].C2;
        if(c1 != a && c2 != a && Road[i].Status) /* Go through all the valid roads and update the connected sequence */
        {
            if(!visit[c1])
            {
                cityseq[citynum++] = c1;
                visit[c1] = 1;
            }
            if(!visit[c2])
            {
                cityseq[citynum++] = c2;
                visit[c2] = 1;
            }
        }
    }
    if(citynum == N-1) return 0; /* Already connected then cost for free */
    qsort(cityseq, citynum, sizeof(int), cmp);
    int cnt = 0, num = citynum; /* Put num to store citynum */
    for(i = 1; i <= N; i++)
    {
        if(i == a) continue; /* Skip the destroyed city */
        if(i < cityseq[0] || i > cityseq[num-1]) /* Out of the sequence */
            costsum += add(i);
        else
        {
            if(i == cityseq[cnt]) /* Within the sequence */
            {
                cnt++;
                continue;
            }
            else    costsum += add(i);
        }
    }
    return costsum;
}

int main()
{
    int i;
    scanf("%d %d", &N, &M);
    for(i = 0; i < M; i++)
        scanf("%d %d %d %d", &Road[i].C1, &Road[i].C2, &Road[i].Cost, &Road[i].Status);
    int num = 0, city[maxn], noweffort, maxeffort = 0;
    for(i = 0; i < N; i++)
    {
        noweffort = getcost(i+1);
        //printf("%d %d\n", i+1, noweffort);
        if(noweffort > maxeffort)
        {
            num = 0; /* reconstruct the cities with the maxeffort */
            city[num++] = i+1;
            maxeffort = noweffort;
        }
        else if(noweffort == maxeffort)
            city[num++] = i+1;
    }   
    qsort(city, num, sizeof(int), cmp);
    if(maxeffort == 0) printf("0");
    else
    {
        printf("%d", city[0]);
        for(i = 1; i < num; i++)
            printf(" %d", city[i]);
    }
    return 0;    
}