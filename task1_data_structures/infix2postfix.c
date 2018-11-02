#include<stdio.h>
#include<stdlib.h>
#include<string.h>

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
void swap(char * input);

Stack build_stack(void)
{
		Stack S;
		S=malloc(sizeof(struct Stack_info));
		S->top_index=-1;
		S->Array=malloc(sizeof(int)*10);
		return S;
}

void push(Stack S,int x)
{
		S->top_index++;
		S->Array[S->top_index]=x;
}

int pop(Stack S)
{
		int tem=S->Array[S->top_index];
		S->top_index--;
		return tem;
}

int isEmpty(Stack S)
{
		return S->top_index==-1;
}

void swap(char*input)
{
		Stack S;
		S=build_stack();
		int i=0;
		while (i<=strlen(input)&&input[i]!='\0')
		{
				if (input[i]=='+'||input[i]=='-')
				{
						if (isEmpty(S))
						{
								push(S,input[i]);
						}
						else if (S->Array[S->top_index]=='*'||S->Array[S->top_index]=='/')
						{
								while(!isEmpty(S))
								{
										printf("%c",pop(S));
								}
								push(S,input[i]);
						}
						else//栈顶为加或减
						{
								printf("%c",pop(S));
								push(S,input[i]);
						}
				}
				else if (input[i]=='*'||input[i]=='/')
				{
						if (isEmpty(S))
						{
								push(S,input[i]);
						}
						else if (S->Array[S->top_index]=='*'||S->Array[S->top_index]=='/')
						{
								//此时栈内有+(-)*(/)或只有*(/)
								//只需将*(/)弹出并压入新元素
								printf("%c",pop(S));
								push(S,input[i]);
						}
						else
						{
								push(S,input[i]);
						}
				}
				else //为数字
				{
						printf("%c ",input[i]);
				}
				i++;
				putchar(' ');
		}
		while(!isEmpty(S))//将栈内所有符号全部推出
		{
				printf("%c",pop(S));
		}
		free(S->Array);
		free(S);
		putchar('\n');
}

void deal_plus(Stack S)
{
		if (isEmpty(S))
		{
				push(S,'+');
		}
		else if (S->Array[S->top_index]=='+'||S->Array[S->top_index]=='-')
		{
				printf("+ ");
				push(S,'+');
		}
		else
		{
				printf("%c + ",pop(S));
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
		char infix1[]="3+4*5-6+7-8/9";
		swap(infix1);
		printf("now we have another infix:\n13*15*14+1-5+3/6");
		printf("now I change it to postfix\n");
		char infix2[]=("13*15+14/1-5+3/6");
		swap(infix2);
		return 0;
}
