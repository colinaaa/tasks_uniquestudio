#include <stdio.h>
#include <stdlib.h>

struct Node;
typedef struct Node *Pointer2Node;
typedef Pointer2Node List_Header;
typedef Pointer2Node Position;

int isEmpty(List_Header L);
Position find(int x, List_Header L);
Position find_previous(int x, List_Header L);
void insert(Position P, List_Header L, int x);
void delete_element(int x, List_Header L);

struct Node {
  int element;
  Position Next;
};

int isEmpty(List_Header L) { return L->Next == NULL; }

void insert(Position P, List_Header L, int x)  // insert x *behind* Position P
{
  Position temporary_position;
  temporary_position = malloc(sizeof(struct Node));

  temporary_position->element = x;
  temporary_position->Next = P->Next;
  P->Next = temporary_position;
}

Position find_previous(int x, List_Header L) {
  Position p = L->Next;
  while (p->Next != NULL && p->Next->element != x) {
    p = p->Next;
  }
  return p;
}

Position find(int x, List_Header L) {
  Position p = L->Next;
  while (p != NULL && p->element != x) {
    p = p->Next;
  }
  return p;
}

void delete_element(int x, List_Header L) {
  Position where_is_x, pre_x;
  pre_x = find_previous(x, L);
  where_is_x = pre_x->Next;
  //需判断x是否存在！！！
  if (where_is_x != NULL) {
    pre_x->Next = where_is_x->Next;
    free(where_is_x);
  }
}

List_Header initialize_a_list(int n) {
  List_Header L = malloc(sizeof(struct Node));
  int i;
  for (i = 1; i <= n; i++) {
    insert(L, L, 0);
  }
  return L;
}

void print_the_list(List_Header L) {
  Position P = L->Next;
  while (P != NULL) {
    printf("%d", P->element);
    P = P->Next;
  }
  printf("\n");
}

void delete_list(List_Header L) {
  Position P1, P2;
  P1 = L->Next;
  L->Next = NULL;
  while (P1 != NULL) {
    P2 = P1->Next;
    free(P1);
    P1 = P2;
  }
}

int main(void) {
  int n;
  printf("please enter the node you want to build(>=3): ");
  scanf("%d", &n);
  List_Header L = initialize_a_list(n);
  printf("now print all the elements:\n");
  print_the_list(L);
  printf("now insert 6 behind the second node\n");
  insert(L->Next->Next, L, 6);
  printf("now print all the elements:\n");
  print_the_list(L);
  printf("now delete 6\n");
  delete_element(6, L);
  printf("now print all the elements:\n");
  print_the_list(L);
  printf("now delete all the elements!\n");
  delete_list(L);
  printf("now all the nodes have been deleted!\n");
  print_the_list(L);
}
