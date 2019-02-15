#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//总表
struct str_hashtable;
typedef struct str_hashtable* Hashtable;
typedef unsigned int Index;

//每个块的状态
enum Status { EMPTY, DELETED = 999, OCCUPIED };

//每个入口
struct hash_entry  // block
{
  enum Status status;
  int value;
  unsigned int key_sum;
};

typedef struct hash_entry block;

//总表中包含一个含有每个块的数组和表的大小
struct str_hashtable  // Hashtable
{
  block* block_array;
  int table_size;
};

unsigned int Hash(const char* key);
Index Hash_func(Hashtable H, const char* key);
void delete (Hashtable H, const char* key);
Index find(Hashtable H, const char* key);
Hashtable initialize_table(int size);
void insert(Hashtable H, const char* key, int value);

Hashtable initialize_table(int size) {
  Hashtable H;
  H = malloc(sizeof(struct str_hashtable));
  H->table_size = size;
  H->block_array = malloc(sizeof(struct hash_entry) * size);
  int i;
  for (i = 0; i < size; i++) {
    H->block_array[i].status = EMPTY;
  }
  return H;
}

Index Hash_func(Hashtable H, const char* key)  //未处理冲突的哈希函数
{
  unsigned int key_sum;
  key_sum = Hash(key);
  Index original_index = key_sum % H->table_size;
  return original_index;
}

unsigned int Hash(const char* key) {
  int n;
  n = strlen(key);  // key的总长度
  int i;
  unsigned int key_sum;
  for (i = 0, key_sum = 0; i < n; i++)  //使用秦九韶算法进行散列
  {
    key_sum = (key_sum << 3) + key[i];  //<<3相当于*8
  }
  return key_sum;
}

Index find(Hashtable H, const char* key) {
  Index index;
  index = Hash_func(H, key);
  unsigned int key_sum;
  key_sum = Hash(key);
  int i = 1;
  while (H->block_array[index].status != EMPTY &&
         key_sum != H->block_array[index].key_sum)
  //若不为空且key不对应，则查找下一个
  {
    index += i * i;  //平方探测
    i++;
    if (index >= H->table_size) {
      index -= H->table_size;
    }
  }
  if (H->block_array[index].status == EMPTY) printf("didn't find");
  return index;
}

void insert(Hashtable H, const char* key, int value) {
  Index original_index;
  original_index = Hash_func(H, key);
  int i = 1;
  while (H->block_array[original_index].status == OCCUPIED)
  //由于与find的判断条件不同，不能将处理冲突合并为一个函数，于是就这样写着吧。。
  {
    original_index += i * i;
    i++;
    if (original_index >= H->table_size) {
      original_index -= H->table_size;
    }
  }
  printf("%d\n", original_index);
  H->block_array[original_index].status = OCCUPIED;
  H->block_array[original_index].value = value;
  unsigned int key_sum;
  key_sum = Hash(key);
  H->block_array[original_index].key_sum = key_sum;
}

void delete (Hashtable H, const char* key) {
  Index index;
  index = find(H, key);
  H->block_array[index].status = DELETED;
  H->block_array[index].value = 0;
}

int main(void) {
  printf("initializing hash table...\n");
  Hashtable H;
  H = initialize_table(2017);

  printf("now we have k1:Andy,  v1:1\n");
  const char key1[] = "Andy";
  int value1 = 1;
  printf("            k2:Bob,   v2:2\n");
  const char key2[] = "Bob";
  int value2 = 2;
  printf("            k3:Colin, v3:3\n");
  const char key3[] = "Colin";
  int value3 = 3;

  //实现插入哈希表
  printf("insert them into hashtable\n");
  insert(H, key1, value1);
  insert(H, key2, value2);
  insert(H, key3, value3);

  //实现查找
  printf("let's find them\nhere are their indexs:\n");

  //查看散列地址
  int i1, i2, i3;
  i1 = find(H, key1);
  i2 = find(H, key2);
  i3 = find(H, key3);
  printf("%d\n%d\n%d\n", i1, i2, i3);

  //根据地址查找
  printf("the value for Andy is: %d\n", H->block_array[i1].value);
  printf("the value for Bob is: %d\n", H->block_array[i2].value);
  printf("the value for Colin is: %d\n", H->block_array[i3].value);
  printf("now delete Colin:change value to 0,change stauts to 999\n");

  //实现删除
  delete (H, key3);
  printf("now try to find Colin\n");
  Index index3;
  index3 = find(H, key3);
  printf("the index of Colin:%d\n", index3);
  printf("the status of Colin:%d\n", H->block_array[index3].status);
  printf("the value of Colin is:%d\n", H->block_array[index3].value);
  printf("deleting hash table...\n");
  //释放动态生成的内存
  free(H->block_array);
  free(H);
}
