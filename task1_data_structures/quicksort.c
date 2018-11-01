#include<stdio.h>

void quicksort(int a[], int first_index, int last_index)
{
		if (first_index<last_index)
		{
				int partition_point;
				partition_point=divide_array(a,first_index,last_index);
				quicksort(a,first_index,partition_point-1);
				quicksort(a,partition_point+1,last_index);
		}
}

int divide_array(int a[], int first_index, int last_index)
{
		int temporary_storage;
		temporary_storage=a[first_index];
		int smaller=last_index;//从后开始找比挖出来的数小的数
		int bigger=first_index;//从前开始找比挖出来的数大的数
		while(smaller!=bigger)
		{
				while(smaller>bigger&&a[smaller]>=temporary_storage)
				{					
						smaller--;
				}
				if (smaller>bigger)
				{
						a[bigger]=a[smaller];//将小的数填入之前挖的坑
				}
				while(smaller>bigger&&a[bigger]<=temporary_storage)
				{
						bigger++;
				}
				if (smaller>bigger)
				{
						a[smaller]=a[bigger];
				}
		}
		a[smaller]=temporary_storage;//填上坑
		return smaller;//返回分开的位置
}
int main(void)
{
		int a[9]={4,2,9,5,1,7,3,6,8};
		int n=9;
		printf("now we have an array with %d integer.\n",n);
		printf("it hasn't been sorted yet. \n");
		int i;
		for (i=0;i<n;i++)
		{
				printf("%d ",a[i]);
		}
		putchar('\n');
		printf("now let's sort it with quicksort\n");
		quicksort(a,0,n);
		printf("the array has been sorted\n");

		for ( i=0;i<n;i++)
		{
				printf("%d ",a[i]);
		}
		putchar('\n');
}
