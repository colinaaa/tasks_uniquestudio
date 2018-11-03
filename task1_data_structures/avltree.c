#include<stdio.h>

struct avltree_node;

typedef avltree_node * AvlTree;

struct avltree_node
{
		int key;
		int height;
		AvlTree left;
		AvlTree ight;
}

int Max(int a, int b);
AvlTree reate_node(int key, AvlTree left, AvlTree right);
AvlTree insert (AvlTree tree, int key);
int height(AvlTree tree);
int change_height(AvlTree tree);
AvlTree ll_rotation(AvlTree tree);
AvlTree lr_rotation(AvlTree tree);
AvlTree rr_rotation(AvlTree tree);
AvlTree rl_rotation(AvlTree tree);

int Max(int a, int b)
{
		return a>b?a:b;
}

int height(AvlTree tree)
{
		if (tree==NULL)
		{
				return -1;
		}
		else 
		{
				return tree->height;
		}
}

void change_height(AvlTree tree)
{
		tree->height=max(height(tree->left),height(tree->right))+1;
}

AvlTree create_node(int key, AvlTree left, AvlTree right)
{
		AvlTree T;
		T=malloc(sizeof(struct avltree_node));
		T->key=key;
		T->left=left;
		T->right=right;
		T->height=0;
}

AvlTree insert(AvlTree tree, int key)
{
		if (tree==NULL)//空树或递归至叶子
		{
				AvlTree T;
				T=create_node(key,NULL,NULL);
		}
		else if(key<tree->key)//插入左子树
		{
				tree->left=insert(tree->left,key);
				if (height(tree->left)-height(tree->right)>=2)
						//当tree指向root时，才有可能出现不平衡，如果失去平衡
						//此时传入旋转函数的tree是树的root指针
				{
						if (key>tree->left->key)
								lr_rotation(tree);
						else
								ll_rotation(tree);
				}
		}
		else if (key>tree->key)//插入右子树
		{
				tree->right=insert(tree->right,key);
				if (height(tree->right)-height(tree->left)>=2)
				{
						if (key>tree->right->key)
						{
								rr_rotation(tree);
						}
						else
								rl_rotation(tree);
				}
		}
		//树的高度等于左或右子树高度加一（高度也递归上升）
		change_height(tree);
		//返回根节点
		return tree;
}

AvlTree ll_rotation(AvlTree tree)//tree是root指针
{
		AvlTree tem_tree;//将需要旋转的左子树储存

		//旋转
		tem_tree=tree->left;
		tree->left=tem_tree->right;
		tem_tree->right=tree;

		//改变高度
		change_height(tree);//先改变子树高度
		change_height(tem_tree);

		return tem_tree;//返回root指针
}

AvlTree rr_rotation(AvlTree tree)
{
		AvlTree tem_tree;

		tem_tree=tree->right;
		tree->right=tem_tree->left;
		tem_tree->left=tree;

		change_height(tree);
		change_height(tem_tree);
		
		return tem_tree;
}

AvlTree lr_rotation(AvlTree tree)
{
		tree->left=rr_rotation(tree->left);
		//对root的左子树进行rr旋转，将lr变为ll
		//再进行ll旋转
		return ll_rotation(tree);
}

AvlTree rl_rotation(AvlTree tree)
{
		tree->right=ll_rotation(tree->right);

		return rr_rotation(tree);
}
