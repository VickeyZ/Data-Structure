void creatdataint (int recordnum,int querynum,int carnum)
{
	FILE* fp;
	fp = fopen("I://input.txt", "w");

	fprintf(fp, "%d %d\n",  recordnum, querynum);	//Output the nunber of records and queries
	
	char** carid;
	carid = (char**)malloc(sizeof(char*) * carnum);
	srand(time(NULL));
	for (int i = 0; i < carnum; i++)// Create the plate names randomly
	{
		carid[i] = (char*)malloc(sizeof(char) * 8);
		for (int j = 0; j < 7; j++)
		{
			carid[i][j] = rand() % 26 + 65;
		}
		carid[i][7] = '\0';
	}

	for (int i = 0; i < recordnum; i++)//Create records
	{
		int c = rand() % carnum;//Choose a random car
		fprintf(fp, "%s ", carid[c]);//Output car id
		int t = rand() *100% 86400;	//Create time
		fprintf(fp, "%02d:%02d:%02d ", t / 3600, (t / 60) % 60, t % 60);//Output the time
		int b = rand() % 2;//Create in/out status
		if (b == 0)
			fprintf(fp, "out\n");
		else
			fprintf(fp, "in\n");
	}

	for (int i = 0; i < querynum; i++)
	{
		int t = rand() * 100 % 86400;//Create query time
		fprintf(fp, "%02d:%02d:%02d\n", t / 3600, (t / 60) % 60, t % 60);
	}
}

