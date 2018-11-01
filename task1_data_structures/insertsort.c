#include<stdio.h>

void insertsort(int a[], int n)
{
		int index;
		for (index=1;index<n;index++)
		{
				int search_index,x=a[index];
				for(search_index=index-1;search_index>=0;search_index--)
				{
						if (a[search_index]>x)
						{
								a[search_index+1]=a[search_index];
								a[search_index]=x;
						}
						else
						{
								break;
						}
				}
		}
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
		printf("now let's sort it with insertsort\n");
		insertsort(a,n);
		printf("the array has been sorted\n");

		for ( i=0;i<n;i++)
		{
				printf("%d ",a[i]);
		}
		putchar('\n');
}
