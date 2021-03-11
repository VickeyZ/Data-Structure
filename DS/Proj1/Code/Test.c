#include<stdio.h>
#include<time.h>



extern double AL1(double x,int n);           /*Function of algorithm1*/
extern double AL2Iteration(double x,int n);  /*Algorithm2 iterative version*/
extern double AL2Recursion(double x,int n);  /*Algorithm2 recursive version*/  
clock_t start,stop;                          /*clock_t is a built-in type for processor time(ticks)*/
double duration;                             /*records the run time (seconds) of a function*/

int main(){
    double x;
    double result;                           /*Use result to record x to the n and initialize it*/
    double runtime,duration,total_time;
    int n;
    int k;
    int c;
	int Ticks;

    //The following three sections only call different functions with the same logic, so our group takes the first section as an example to annotate the idea in detail
    //algorithm 1
    printf("please enter the base x and the exponent n for algorithm1\n");
    scanf("%lf%d",&x,&n);                    /*Read in the unknowns and indices*/
    printf("please enter the iterative time k:\n");
    scanf("%d",&k);                          /*we may repeat the function calls for K times to obtain a total run time*/
    start = clock();                         /*records the ticks at the beginning of the function call*/
    for(c=1;c<=k;c++){                       /*repeat the function calls for K times*/ 
        result=AL1(x, n);
    }
    stop=clock();                            /*records the ticks at the end of the function call*/
	Ticks = stop - start;                    /*Calculate the ticks*/ 
    total_time = ((double)(stop - start)) / CLK_TCK; /*Calculate the total_time*/ 
    duration = total_time / k;                      /*Calculate the duration*/ 
    printf("result = %lf\n", result);         /*Output the result*/ 
    printf("Ticks = %d\n", Ticks);     /*Output the Ticks*/
    printf("total_time = %lf\n", total_time);       /*Output the total_time*/
    printf("duration = %lf\n", duration);           /*Output the duration*/

    //algorithm 2 iterative version
    printf("please enter the base x and the exponent n for algorithm2 iterative version\n");
    scanf("%lf%d",&x,&n);
    printf("please enter the iterative time k:\n");
    scanf("%d",&k);
    start = clock();
    for(c=1;c<=k;c++){
        result=AL2Iteration(x, n);
    }
    stop=clock();
	Ticks = stop - start;
    total_time = ((double)(stop - start)) / CLK_TCK; /*Calculate the total_time*/ 
    duration = total_time / k;                      /*Calculate the duration*/ 
    printf("result = %lf\n", result);         /*Output the result*/ 
    printf("Ticks = %d\n", Ticks);     /*Output the Ticks*/
    printf("total_time = %lf\n", total_time);       /*Output the total_time*/
    printf("duration = %lf\n", duration);           /*Output the duration*/

    //algorithm 2 recursive version
    printf("please enter the base x and the exponent n for algorithm2 recursive version\n");
    scanf("%lf%d",&x,&n);
    printf("please enter the iterative time k:\n");
    scanf("%d",&k);
    start = clock();
    for(c=1;c<=k;c++){
        result=AL2Recursion( x,n);
    }
    stop=clock();
	Ticks = stop - start;
        total_time = ((double)(stop - start)) / CLK_TCK; /*Calculate the total_time*/ 
    duration = total_time / k;                      /*Calculate the duration*/ 
    printf("result = %lf\n", result);         /*Output the result*/ 
    printf("Ticks = %d\n", Ticks);     /*Output the Ticks*/
    printf("total_time = %lf\n", total_time);       /*Output the total_time*/
    printf("duration = %lf\n", duration);           /*Output the duration*/
	getchar();
    return 0;
}
