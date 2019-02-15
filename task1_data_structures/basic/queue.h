#ifndef QUEUE_H_
#define QUEUE_H_

struct queue_str;
typedef struct queue_str* Queue;
typedef unsigned int Index;

struct queue_str {
  Index ahead;
  Index behind;
  int* array;
  int size;
};

void in_queue(Queue Q, int x) {
  int tem_index;
  tem_index = ++Q->behind;
  if (tem_index >= Q->size) {
    tem_index -= Q->size;
  }
  Q->array[tem_index] = x;
}

#endif
