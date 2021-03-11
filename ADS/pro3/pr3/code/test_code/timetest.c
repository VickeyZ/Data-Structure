//
//  timetest.c
//  Beautiful_Subsequence
//  used to test the runtime of the program.
//  just input the number n and m, the program will generate the data of sequence randomly. 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <windows.h>

#define Time_init	LARGE_INTEGER lFrequency; \
					QueryPerformanceFrequency(&lFrequency);  
#define Time_begin	LARGE_INTEGER lBeginCount; \
					QueryPerformanceCounter(&lBeginCount);
#define Time_end	LARGE_INTEGER lEndCount; \
					QueryPerformanceCounter(&lEndCount);
#define Time_out	double time = (double)(lEndCount.QuadPart - lBeginCount.QuadPart) / (double)lFrequency.QuadPart; \
					printf("runtime:%lfms\n",time*1000);

#define overflow 1000000007

void get_sub(int n,int m);
void sub_check(int i,int m);
long long avoid_overflow(int n);

int sequence[100001];
static long sub[100001];
static long nonsub[100001];
static long sum;

int main(int argc, const char * argv[])
{
    int i;
    int n,m;
    srand((unsigned)time(NULL));
    scanf("%d%d",&n,&m);              
    for(i=1;i<=n;i++)
    {
        sequence[i]=rand();
    }
    Time_init;
    Time_begin;
    get_sub(n,m);
    Time_end;
    printf("%ld\n",sum);
    Time_out;
    system("pause");
}

void get_sub(int n,int m)   //get the total number of beautiful subsquence
{                                    //by adding the number of subsquence from element 1 to element n
    int i;
    nonsub[1]=1;
    for(i=2;i<=n;i++)
    {
        sub_check(i,m); //figure out the number of beautiful subsequences ended with i from the first to the last
    }
}

void sub_check(int i,int m)
{
    int j;
    
    sub[i]=sum; //if there is no matched number from element 1 to element i-1 with element i
                    //then the number of beautiful subsequence stay the same
    for(j=1;j<i;j++)
    {
        if(abs(sequence[j]-sequence[i])<=m) //if there is such a matched number j
        {
            sub[i]=sub[i]%overflow+nonsub[j]%overflow;  //then the nonbeautiful subsequence from element 1 to j
            sub[i]=sub[i]%overflow; //become the beautiful subsequence

        }
    }
    nonsub[i]=avoid_overflow(i-1)%overflow-sub[i]%overflow; // get number nonbeautiful subsquence by subtracting nunber of
    while(nonsub[i]<0)  // beautiful subsquence from the total number of subsequence
    {
        nonsub[i]=nonsub[i]+overflow;   //dealing with the overflow problem in case the number is less than 0 (or underflow)
    }
    nonsub[i]=nonsub[i]%overflow;
    sum=sum%overflow+sub[i]%overflow;
    sum=sum%overflow;   //dealing with the overflow problem in case the number is too large
}
long long avoid_overflow(int n) //this function is to deal with the overflow problem by using Euler's power law
{
    long long result;   //result=(2^n)%overflow
    long long a;
    result=1;
    a=2;
    while(n>0)
    {
        if(n%2==1)  //n&1==1
            result=(result*a)%overflow;
        n=n/2;  //n=n>>1
        a=(a*a)%overflow;
    }
    return result;
}
