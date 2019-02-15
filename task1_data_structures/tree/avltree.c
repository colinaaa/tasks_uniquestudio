#include <stdio.h>
#include <stdlib.h>

struct avltree_node;

typedef struct avltree_node* AvlTree;

struct avltree_node {
  int key;
  int height;
  AvlTree left;
  AvlTree right;
};

int Max(int a, int b);
AvlTree create_node(int key, AvlTree left, AvlTree right);
AvlTree insert(AvlTree tree, int key);
AvlTree ll_rotation(AvlTree tree);
AvlTree lr_rotation(AvlTree tree);
AvlTree rr_rotation(AvlTree tree);
AvlTree rl_rotation(AvlTree tree);
void delete_tree(AvlTree tree);
int height(AvlTree tree);

int height(AvlTree tree) {
  if (tree == NULL) {
    return -1;
  } else {
    return tree->height;
  }
}
int Max(int a, int b) { return a > b ? a : b; }

AvlTree create_node(int key, AvlTree left, AvlTree right) {
  AvlTree T;
  T = malloc(sizeof(struct avltree_node));
  T->key = key;
  T->left = left;
  T->right = right;
  T->height = 0;
  return T;
}

AvlTree insert(AvlTree tree, int key) {
  //空树或递归至叶子
  if (tree == NULL) {
    tree = create_node(key, NULL, NULL);
  } else if (key < tree->key) {
    //插入左子树
    tree->left = insert(tree->left, key);
    if (height(tree->left) - height(tree->right) == 2) {
      //当tree指向root时，才有可能出现不平衡，如果失去平衡
      //此时传入旋转函数的tree是树的root指针
      if (key > tree->left->key)
        lr_rotation(tree);
      else
        ll_rotation(tree);
    }
  } else if (key > tree->key) {
    //插入右子树
    tree->right = insert(tree->right, key);
    if (height(tree->right) - height(tree->left) == 2) {
      if (key > tree->right->key) {
        rr_rotation(tree);
      } else
        rl_rotation(tree);
    }
  }
  //树的高度等于左或右子树高度加一（高度也递归上升）
  tree->height = Max(height(tree->left), height(tree->right)) + 1;
  //返回根节点
  return tree;
}

AvlTree ll_rotation(AvlTree tree) {
  // tree是root指针
  AvlTree tem_tree;  //将需要旋转的左子树储存
  //旋转
  tem_tree = tree->left;
  tree->left = tem_tree->right;
  tem_tree->right = tree;

  //改变高度
  //先改变子树高度
  tree->height = Max(height(tree->left), height(tree->right)) + 1;
  tem_tree->height = Max(height(tree), height(tem_tree->left)) + 1;

  return tem_tree;  //返回root指针
}

AvlTree rr_rotation(AvlTree tree) {
  AvlTree tem_tree;

  tem_tree = tree->right;
  tree->right = tem_tree->left;
  tem_tree->left = tree;

  tree->height = Max(height(tree->left), height(tree->right)) + 1;
  tem_tree->height = Max(height(tree), height(tem_tree->right)) + 1;

  return tem_tree;
}

AvlTree lr_rotation(AvlTree tree) {
  tree->left = rr_rotation(tree->left);
  //对root的左子树进行rr旋转，将lr变为ll
  //再进行ll旋转
  return ll_rotation(tree);
}

AvlTree rl_rotation(AvlTree tree) {
  tree->right = ll_rotation(tree->right);

  return rr_rotation(tree);
}

void delete_tree(AvlTree tree) {
  if (tree == NULL) return;
  if (tree->left != NULL) delete_tree(tree->left);
  if (tree->right != NULL) delete_tree(tree->right);

  free(tree);
}

int main(void) {
  printf("initializing an avltree...\n");
  AvlTree T = NULL;
  printf("insert items into it\n");
  T = insert(T, 19);
  T = insert(T, 28);
  T = insert(T, 5);
  T = insert(T, 9);
  T = insert(T, 22);
  T = insert(T, 15);
  //遍历树

  printf("delete the tree");
  delete_tree(T);
}
