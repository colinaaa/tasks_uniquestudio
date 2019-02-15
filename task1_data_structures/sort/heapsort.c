#include "heapsort.h"
#include <stdio.h>
void heapsort(int a[], int n) {
  int i;
  for (i = 0; i < n; i++) {
    int j;
    for (j = (n - i) / 2; j >= 1; j--) {
      changeInTriganle(a, j);
    }
    swap(a, 1, n - i);  //最终为swap(a,1,1)：不移动。
  }
}

void changeInTriganle(int *a, int summit) {
  if (a[summit] < a[2 * summit]) {
    swap(a, summit, 2 * summit);
  }
  if (a[summit] < a[2 * summit + 1]) {
    swap(a, summit, 2 * summit + 1);
  }
}

void swap(int a[], int index1, int index2) {
  int tem;
  tem = a[index1];
  a[index1] = a[index2];
  a[index2] = tem;
}
