#include<stdio.h>

void heapsort(int a[], int n)
{
		int i;
		for (i=0;i<n;i++)
		{
				int j;
				for (j=(n-i)/2;j>=1;j--)
				{
						change_in_triangle(a,j);
				}
				swap(a,1,n-i);//最终为swap(a,1,1)：不移动。
		}
}

void change_in_triangle(int a[], int summit)//将每个三角中的最大值调整至上面的顶点
{
		if (a[summit]<a[2*summit])
		{
				swap(a,summit,2*summit);
		}
		if (a[summit]<a[2*summit+1])
		{
				swap(a,summit,2*summit+1);
		}
}

void swap(int a[], int index1, int index2)
{
		int tem;
		tem=a[index1];
		a[index1]=a[index2];
		a[index2]=tem;
}

int main(void)
{
		int a[9]={4,2,9,5,1,7,3,6,8};
		int n=9;
		int b[n+1];
		int t;
		for (t=0;t<n;t++)
		{
				b[t+1]=a[t];
		}
		b[0]=-1;
		printf("now we have an array with %d integer.\n",n);
		printf("it hasn't been sorted yet. \n");
		int i;
		for (i=0;i<n;i++)
		{
				printf("%d ",a[i]);
		}
		putchar('\n');
		printf("now let's sort it with heapsort\n");
		heapsort(b,n+1);
		printf("the array has been sorted\n");

		for ( i=1;i<n;i++)
		{
				printf("%d ",a[i]);
		}
		putchar('\n');
}


