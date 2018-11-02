#include<stdio.h>
#include<stdlib.h>
#define EMPTY -1

struct Stack_info
{
		int top_index;
		int *Array;
};

typedef struct Stack_info *Stack;

Stack build_stack(void);
void push(Stack S, int x);
int pop(Stack S);
int isEmpty(Stack S);
void swap(void);
void deal_plus(Stack S);
void deal_sub(Stack S);
void deal_muti(Stack S);
void deal_div(Stack S);

Stack build_stack(void)
{
		Stack S;
		S=malloc(sizeof(struct Stack_info));
		S->top_index=EMPTY;
		S->Array=sizeof(int);
		return S;
}

void push(Stack S,int x)
{
		S->top_index++;
		S->Array[S->top_index]=x;
}

int pop(Stack S)
{
		if (!isEmpty(S))
		{
				int tem=S->Array[S->top_index];
				S->top_index--;
				return tem;
		}
		else
				return -1;
}

int isEmpty(Stack S)
{
		return S->top_index==EMPTY;
}

void swap(void)
{
		Stack S;
		S=build_stack();
		int x;
		int flag=1;
		while (flag)
		{
				if (scanf("%d",&x)==1)
				{
						printf("%d ",x);
				}
				else
				{
						char c=getchar();
						switch(c)
						{
								case '+':
										deal_plus(S);
										break;
								case '-':
										deal_sub(S);
										break;
								case '*':
										deal_muti(S);
										break;
								case '/':
										deal_div(S);
										break;
								default:
										flag=0;

						}
				}
		}
}

void deal_plus(Stack S)
{
		if (isEmpty(S))
		{
				push(S,'+');
		}
		else if (S->Array[S->top_index]=='+'||S->Array[S->top_index]=='-')
		{
				printf("%c ",pop(S));
				push(S,'+');
		}
		else
		{
				printf("%c +",pop(S));
		}
}

void deal_sub(Stack S)
{
		if (isEmpty(S))
		{
				push(S,'-');
		}
		else if (S->Array[S->top_index]=='+'||S->Array[S->top_index]=='-')
		{
				printf("%c ",pop(S));
				push(S,'-');
		}
		else
		{
				printf("%c - ",pop(S));
		}
}

void deal_muti(Stack S)
{
		if (isEmpty(S))
		{
				push(S,'*');
		}
		else if (S->Array[S->top_index]=='+'||S->Array[S->top_index]=='-')
		{
				push(S,'*');
		}
		else
		{
				printf("%c ",pop(S));
				push(S,'*');
		}
}

void deal_div(Stack S)
{
		if (isEmpty(S))
		{
				push(S,'/');
		}
		else if (S->Array[S->top_index]=='+'||S->Array[S->top_index]=='-')
		{
				push(S,'/');
		}
		else
		{
				printf("%c ",pop(S));
				push(S,'/');
		}
}

int main(void)
{
		printf("we have an infix:\n3+4*5-6+7-8/9");
		printf("now I change it to postfix\n");
		swap();
		return 0;
}
