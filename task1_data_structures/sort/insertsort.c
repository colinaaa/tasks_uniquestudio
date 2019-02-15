#include <stdio.h>

void insertsort(int a[], int n) {
  int index;
  for (index = 1; index < n; index++) {
    int search_index, x = a[index];
    for (search_index = index - 1; search_index >= 0; search_index--) {
      if (a[search_index] > x) {
        a[search_index + 1] = a[search_index];
        a[search_index] = x;
      } else {
        break;
      }
    }
  }
}
