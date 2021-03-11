#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int INF = 1000000;

void CreatData();
int main()
{
	CreatData();
	return 0;
}

void CreatData()
{
	int vertexnum;    //The number of stations
	int maxnum;           //Maximum parking capacity
	int maxdistance;        //Maximum distance
	double saturation;   //The number of average edges at one vertex
	int problemstation;
	scanf("%d%d%d%d%lf", &vertexnum, &maxnum, &maxdistance,&problemstation, &saturation);
	int** edge;
	edge = (int**)malloc(sizeof(int*) * vertexnum);
	for (int i = 0; i < vertexnum; i++)
	{
		edge[i] = (int*)malloc(sizeof(int) * vertexnum);
		for (int j = 0; j < vertexnum; j++)
			edge[i][j] = INF;
	}
	int* currentnum;
	currentnum = (int*)malloc(sizeof(int) * vertexnum);

	srand(time(NULL));
	for (int i = 0; i < vertexnum; i++)   //Randomly generate the number of existing shared bikes
		currentnum[i] = rand() % (maxnum + 1);

	int n = 0;
	for (int i = 0; i < vertexnum-1; i++)   //Randomly generate the edge
	{
		int a = 0;
		for (int j = i + 1; j < vertexnum; j++)
		{
			if (((double)rand() / RAND_MAX) <= saturation) //The probability of saturation between two adjacent vertexes
			{
				edge[i][j] = rand() % maxdistance+1;
				edge[j][i] = edge[i][j];
				n++;
				a = 1;
			}
		}
		if (a == 0)						//Each vertex has at least one edge. 
		{
			int j = rand() % (vertexnum - i -1) + i+1;
			if (edge[i][j] == INF)
			{
				edge[i][j] = rand() % maxdistance+1;
				edge[j][i] = edge[i][j];
				n++;
			}
		}
	}

	FILE* fp;							//The generated data is exported to E://data.txt. 
	fp = fopen("E://data.txt", "w");
	fprintf(fp, "%d %d %d %d\n", maxnum, vertexnum-1, problemstation, n);
	for (int i = 1; i < vertexnum; i++)  //0 is PBMC
		fprintf(fp, "%d ", currentnum[i]);
	for (int i = 0; i < vertexnum; i++)
	{
		for (int j = i+1; j < vertexnum; j++)
		{
			if (edge[i][j] != INF)
				fprintf(fp, "\n%d %d %d", i, j, edge[i][j]);
		}
	}
	fclose(fp);
}