#define N 1000
#include<stdio.h>
#include<stdlib.h>

void swap(int * p,int * q)
{
int t1,t2;
t1 = *p;
t2 = *q;
*p = t2;
*q = t1;
}
ProduceRandomUserNameArray(int * Array)
{
	int i;
	printf("\n\n");
	for (i=1000;i>=1;i--)
	{
	  Array[i] = i*100+999;
	}
	//srand((int)time(0));
	for (i=1000;i>=1;i--)
	{
	  swap(&Array[i],&Array[rand()%i+1]);
	}	
}