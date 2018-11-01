#include<stdio.h>
void mergearray(int a[], int first, int mid, int last, int temp[])
{
	int i = first, j = mid + 1;
	int m = mid,   n = last;
	int k = 0;
	
	while (i <= m && j <= n)
	{
		if (a[i] <= a[j])
			temp[k++] = a[i++];
		else
			temp[k++] = a[j++];
	}
	
	while (i <= m)
		temp[k++] = a[i++];
	
	while (j <= n)
		temp[k++] = a[j++];
	
	for (i = 0; i < k; i++)
		a[first + i] = temp[i];
}
void mergesort(int a[], int first, int last, int temp[])
{
	if (first < last)
	{
		int mid = (first + last) / 2;
		mergesort(a, first, mid, temp);    //左边有序
		mergesort(a, mid + 1, last, temp); //右边有序
		mergearray(a, first, mid, last, temp); //再将二个有序数列合并
	}
}

int main(void)
{
		int a[9]={4,2,9,5,1,7,3,6,8};
		int n=9;
		int temp[9];
		printf("now we have an array with %d integer.\n",n);
		printf("it hasn't been sorted yet. \n");
		int i;
		for (i=0;i<n;i++)
		{
				printf("%d ",a[i]);
		}
		putchar('\n');
		printf("now let's sort it with mergesort\n");
		mergesort(a,0,n-1,temp);
		printf("the array has been sorted\n");

		for ( i=0;i<n;i++)
		{
				printf("%d ",a[i]);
		}
	
}
