#include <stdio.h>
#include "sort/heapsort.h"
#include "sort/insertsort.h"
#include "sort/mergesort.h"
#include "sort/quicksort.h"
int main(void) {
  // heapSort();
  // insertSort();
  // mergeSort();
  return 0;
}

void heapSort() {
  int a[9] = {4, 2, 9, 5, 1, 7, 3, 6, 8};
  int n = 9;
  int b[n + 1];
  int t;
  for (t = 0; t < n; t++) {
    b[t + 1] = a[t];
  }
  b[0] = -1;
  printf("now we have an array with %d integer.\n", n);
  printf("it hasn't been sorted yet. \n");
  int i;
  for (i = 0; i < n; i++) {
    printf("%d ", a[i]);
  }
  putchar('\n');
  heapsort(b, n + 1);

  for (i = 1; i < n; i++) {
    printf("%d ", a[i]);
  }
  putchar('\n');
}

void insertSort() {
  int a[9] = {4, 2, 9, 5, 1, 7, 3, 6, 8};
  int n = 9;
  printf("now we have an array with %d integer.\n", n);
  printf("it hasn't been sorted yet. \n");
  int i;
  for (i = 0; i < n; i++) {
    printf("%d ", a[i]);
  }
  putchar('\n');
  printf("now let's sort it with insertsort\n");
  insertSort(a, n);
  printf("the array has been sorted\n");

  for (i = 0; i < n; i++) {
    printf("%d ", a[i]);
  }
  putchar('\n');
}

void mergeSort() {
  int a[9] = {4, 2, 9, 5, 1, 7, 3, 6, 8};
  int n = 9;
  printf("now we have an array with %d integer.\n", n);
  printf("it hasn't been sorted yet. \n");
  int i;
  for (i = 0; i < n; i++) {
    printf("%d ", a[i]);
  }
  putchar('\n');
  printf("now let's sort it with mergesort\n");
  mergesort(a, 0, n - 1);
  printf("the array has been sorted\n");

  for (i = 0; i < n; i++) {
    printf("%d ", a[i]);
  }
  putchar('\n');
}

void quickSort() {
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
