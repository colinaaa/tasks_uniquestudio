#include "quicksort.h"
#include <stdio.h>
void quicksort(int a[], int firstIndex, int lastIndex) {
  if (firstIndex < lastIndex) {
    int partition_point;
    partition_point = divideArray(a, firstIndex, lastIndex);
    quicksort(a, firstIndex, partition_point - 1);
    quicksort(a, partition_point + 1, lastIndex);
  }
}

int divideArray(int a[], int firstIndex, int lastIndex) {
  int temp;
  temp = a[firstIndex];
  int smaller = lastIndex;  //从后开始找比挖出来的数小的数
  int bigger = firstIndex;  //从前开始找比挖出来的数大的数
  while (smaller != bigger) {
    while (smaller > bigger && a[smaller] >= temp) {
      smaller--;
    }
    if (smaller > bigger) {
      a[bigger] = a[smaller];  //将小的数填入之前挖的坑
    }
    while (smaller > bigger && a[bigger] <= temp) {
      bigger++;
    }
    if (smaller > bigger) {
      a[smaller] = a[bigger];
    }
  }
  a[smaller] = temp;  //填上坑
  return smaller;     //返回分开的位置
}
int main(void) {
  int a[9] = {4, 2, 9, 5, 1, 7, 3, 6, 8};
  int n = 9;
  printf("now we have an array with %d integer.\n", n);
  printf("it hasn't been sorted yet. \n");
  int i;
  for (i = 0; i < n; i++) {
    printf("%d ", a[i]);
  }
  putchar('\n');
  printf("now let's sort it with quicksort\n");
  quicksort(a, 0, n);
  printf("the array has been sorted\n");

  for (i = 0; i < n; i++) {
    printf("%d ", a[i]);
  }
  putchar('\n');
}
