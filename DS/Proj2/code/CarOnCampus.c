#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Record {
	char id[8];
	int time;
	int state;		  //Record whether the car is in or out
	int valid;        //Record whether its state is valid or not
};
struct Car {
	char id[8];
	int totaltime;    //Record the total parking time
};

int cmp1(const void* a, const void* b)      //Sort by the car's ID, the two with the same ID then sort by the time
{
	int c = strcmp((*(struct Record*)a).id, (*(struct Record*)b).id);
	if (c == 0)
		return (*(struct Record*)a).time - (*(struct Record*)b).time;
	else
		return c;
}  
int cmp2(const void* a, const void* b)		//Sort by the time
{
	return (*(struct Record*)a).time - (*(struct Record*)b).time;
}

int main()
{
	int recordnum, querynum;
	scanf("%d%d", &recordnum, &querynum);
	struct Record* records;
	records = (struct Record*)malloc(sizeof(struct Record) * recordnum);
	for (int i = 0; i < recordnum; i++)
	{
		scanf("%s", records[i].id);
		int h, m, s;
		scanf( "%d:%d:%d", &h, &m, &s);
		records[i].time = 3600 * h + 60 * m + s;	//Record the time in term of seconds
		char state[4];
		scanf( "%s", state);
		if (state[0] == 'o')
			records[i].state = 0;   //Record whether the car is in or out
		else
			records[i].state = 1;
		records[i].valid = 0;		//Initialize
	}

	qsort(records, recordnum, sizeof(struct Record), cmp1); //Sort by the car's ID first

	int carsnum = 1;		//Record the total numbers of the cars
	struct Car* cars = (struct Car*)malloc(sizeof(struct Car) * recordnum);
	strcpy(cars[0].id, records[0].id);	
	cars[0].totaltime = 0;	
	for (int i = 0; i < recordnum - 1; i++)
	{
		if (strcmp(records[i].id, records[i + 1].id) == 0)	//Compare whether two adjacent records are the same
		{
			if (records[i].state == 1 && records[i + 1].state == 0)   //Set valid to 1 if the two adjacent records' state are different
			{
				records[i].valid = records[i + 1].valid = 1;
				cars[carsnum - 1].totaltime += records[i + 1].time - records[i].time;	//Update the total parking time
			}
		}
		else     //Update the record of a new car when different
		{
			strcpy(cars[carsnum].id, records[i + 1].id);
			cars[carsnum].totaltime = 0;
			carsnum++;
		}
	}

	qsort(records, recordnum, sizeof(struct Record), cmp2);//Sort by the time
	
	int coc = 0;   //the number of cars on campus at one test time
	int i = 0;
    int* querytime = (int*)malloc(sizeof(int)*querynum); //Record all the test time
    for(int j = 0; j < querynum; j++)
    {
		int h, m, s;
        scanf("%d:%d:%d", &h, &m, &s);
		querytime[j] = 3600 * h + 60 * m + s;
    }
	for (int j = 0; j < querynum; j++)
	{
		for (; records[i].time <= querytime[j]; i++)
		{
			if (records[i].valid == 1) // Pick valid records
			{
				if (records[i].state == 1)
					coc++;
				else if (records[i].state == 0)
					coc--;
			}
		}
		printf("%d\n", coc);
	}
	
	//Output the maxtime
	int maxtime = 0;
	for (int i = 0; i < carsnum; i++) //Find the maxium parking time
	{
		if (cars[i].totaltime > maxtime)
			maxtime = cars[i].totaltime;
	}
	for (int i = 0; i < carsnum; i++)  //Find cars with the maxium parking time, cars are already sorted by ID
	{
		if (cars[i].totaltime == maxtime)
			printf("%s ", cars[i].id);
	}
	printf("%02d:%02d:%02d", maxtime / 3600, (maxtime / 60) % 60, maxtime % 60);
	return 0;
}
