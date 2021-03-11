//  main.c
//  Beautiful_Subsequence

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define overflow 1000000007 //used to deal with the overflow problem

void get_sub(int n,int m);  //used to figure out the number of beautiful sequence of the subsequence
void sub_check(int i,int m);    //used to figure out the number of beautiful sequence of the subsequence which is ended with i
long long avoid_overflow(int n);    //used to solve the overflow problem by Euler's power law

int sequence[100001];   //used to store the elements
static long sub[100001];    //used to store the number of beautiful sequence of the subsequence which is ended with i from the first element to n-th elememt
static long nonsub[100001]; //used to store the number of nonbeautiful sequence of the subsequence which is ended with i from the first element to n-th elememt
static long sum;    //store the number of beautiful subsequences

int main(int argc, const char * argv[])
{
    int i;
    int n,m;
    
    scanf("%d%d",&n,&m);    //read the keynum and the sequence
    for(i=1;i<=n;i++)
    {
        scanf("%d",&sequence[i]);   //get the elements
    }
    get_sub(n,m);   //caculate the number of beautiful subsequences
    
    printf("%ld\n",sum);
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
