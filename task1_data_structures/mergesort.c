#include<stdio.h>

void merge_2_array(int a[],int first_index1, int last_index1, int last_index2, int tem[]);

void mergesort(int a[], int first, int last, int tem[])
{
		if (first<last)
		{
				int last_index1=(first+last)/2,first_index2=last_index1+1;
				mergesort(a,first,last_index1,tem);
				mergesort(a,first_index2,last,tem);
				merge_2_array(a,first,last_index1,last,tem);
		}
}

void merge_2_array(int a[], int first_index1,int last_index1, int last_index2, int tem[])
{
		//我意识到错误了。。。first_index2=last_index1+1。。。但是懒得改了。。。
		int index_t=0;
		int n=last_index2-first_index1+1;//两个数组的总个数
		int first_index2=last_index1+1;//我还是改过来了。。
		int tem_first_index1=first_index1;
		int i;

		while(first_index1<=last_index1&&first_index2<=last_index2)//合并两个数组
		{
				if(a[first_index1]>=a[first_index2])
				{
						tem[index_t]=a[first_index2];
						first_index2++;
						index_t++;
				}
				else
				{
						tem[index_t]=a[first_index1];
						first_index1++;
						index_t++;
				}
		}
		//此时一个数组被取空，有可能另一个数组中还有数
		while(first_index1<=last_index1)
		{
				tem[index_t]=a[first_index1];
				first_index1++;
				index_t++;
		}
		while(first_index2<=last_index2)
		{
				tem[index_t]=a[first_index2];
				first_index2++;
				index_t++;
		}

		for(i=0;i<index_t;i++)//将排序好的数组copy到原数组中
		{
				a[tem_first_index1+i]=tem[i];
		}
}

int main(void)
{
		int a[9]={4,2,9,5,1,7,3,6,8};
		int n=9;
		int tem[9];
		printf("now we have an array with %d integer.\n",n);
		printf("it hasn't been sorted yet. \n");
		int i;
		for (i=0;i<n;i++)
		{
				printf("%d ",a[i]);
		}
		putchar('\n');
		printf("now let's sort it with mergesort\n");
		mergesort(a,0,n-1,tem);
		printf("the array has been sorted\n");

		for ( i=0;i<n;i++)
		{
				printf("%d ",a[i]);
		}
		putchar('\n');
}
