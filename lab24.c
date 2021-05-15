#include <stdio.h>
#include <string.h>
#include "tree.h"


void swap1(int *a,int *b)
{
	int t1=*a;
	*a=*b;
	*b=t1;
}


int find_from1(char *a,int alen,int from,char b,int step)
{
	for(int i=from;i>=0 && i<alen;i+=step)
		if(a[i]==b)
			return i;
	return -1;
}

int get_operator_priority1(char a)
{
	if(a=='+' || a=='-')
		return 1;
	if(a=='*' || a=='/')
		return 2;
	return -1;
}


char *get_priority1(char *a)
{
	int len1=strlen(a);
	char na1[len1];
	memcpy(na1,a,len1);
	char *ret1=calloc(len1,1);
	int curnum1=1;
	while(true)
	{
		int to1=find_from1(na1,len1,0,')',1);
		int from1=0;
		if(to1==-1)
			to1=len1;
		else
			from1=find_from1(na1,len1,to1,'(',-1);
		if(na1[from1+1]=='-')
		{
			na1[from1]=' ';
			na1[to1]=' ';
			na1[from1+1]='_';
			continue;
		}
		int op1=-1;
		int op2=-1;
		for(int i=from1;i<to1;i++)
		{
			if(ret1[i]!=0)
				continue;
			int pr1=get_operator_priority1(na1[i]);
			if(pr1==-1)
				continue;
			if(op1==-1)
				op1=i;
			else if(op2==-1)
				op2=i;
			else
				break;
		}
		if(op1==-1)
		{
			if(na1[from1]=='(')
			{
				na1[from1]='[';
				na1[to1]=']';
				continue;
			}
			else
				break;
		}
		if(op2!=-1 && get_operator_priority1(na1[op1])<get_operator_priority1(na1[op2]))
			swap1(&op1,&op2);
		ret1[op1]=curnum1;
		curnum1++;
	}
	return ret1;
}


char *str_in_heap1(char *a)
{
	int l1=strlen(a);
	char *ret1=malloc(l1+1);
	memcpy(ret1,a,l1);
	ret1[l1]=0;
	return ret1;
}

char *substr1(char *a,int fr,int to)
{
	char *ret1=malloc(to-fr+1);
	memcpy(ret1,a+fr,to-fr);
	ret1[to-fr]=0;
	return ret1;
}


tnode *create_operator1(char *str,char *pri,int len)
{
	int mx1=0;
	int ttl1=0;
	for(int i=1;i<len;i++)
	{
		if(pri[i])
			ttl1++;
		if(pri[i]>pri[mx1])
			mx1=i;
	}
	tnode *ret1;
	if(ttl1==0)
		ret1=tree_create(substr1(str,0,len));
	else
	{
		char opn1[2] = {str[mx1],0};
		ret1=tree_create(str_in_heap1(opn1));
		tree_add_node(ret1,create_operator1(str+mx1+1,pri+mx1+1,len-(mx1+1)));
		tree_add_node(ret1,create_operator1(str,pri,mx1));
	}
	return ret1;
}


tnode *convert_to_tree1(char *a)
{
	int len1=strlen(a);
    char *prior1=get_priority1(a);
    //for(int i=0;i<len1;i++)
    //	printf("%d",prior1[i]);
    //printf("\n");
    tnode *ret1=create_operator1(a,prior1,len1);
    free(prior1);
    return ret1;
}


void neg_cnt1(tnode *a,int *b)
{
	if(a==NULL)
		return;
	if(a->data[0]=='(')
	{
		(*b)++;
		a->data=substr1(a->data,2,strlen(a->data)-1);
	}
	neg_cnt1(a->childs,b);
	neg_cnt1(a->next,b);
}

// вынести из произведения унарные минусы
tnode *solve_task(tnode *a)
{
	int cnt1=0;
	neg_cnt1(a,&cnt1);
	if(cnt1%2==1)
	{
		tnode *nn1=tree_create(str_in_heap1("-"));
		tree_add_node(nn1,a);
		a=nn1;
	}
	return a;
}

// собрать дерево в выражение
char *tree_to_expression1(tnode *a)
{
	if(a->childs!=NULL && a->childs->next==NULL)
	{
		char *rgh1=tree_to_expression1(a->childs);
		char con1[strlen(a->data)+3+strlen(rgh1)];
		strcpy(con1,a->data);
		strcat(con1,"(");
		strcat(con1,rgh1);
		strcat(con1,")");
		return str_in_heap1(con1);
	}
	if((a->childs)!=NULL)
	{
		char *lft1=tree_to_expression1(a->childs);
		char *rgh1=tree_to_expression1(a->childs->next);
		char con1[strlen(lft1)+strlen(rgh1)+strlen(a->data)+1];
		strcpy(con1,lft1);
		strcat(con1,a->data);
		strcat(con1,rgh1);
		con1[strlen(lft1)+strlen(rgh1)+strlen(a->data)]=0;
		free(lft1);
		free(rgh1);
		return str_in_heap1(con1);
	}
	else
		return str_in_heap1(a->data);
}

int main(int argc,char *argv[])
{
	char exp1[256];
    printf("*введите выражение:\n");
    scanf("%s",exp1);
    tnode *tr1=convert_to_tree1(exp1);
    printf("*дерево до преобразования:\n");
    tree_print(tr1);
    tr1=solve_task(tr1);
    printf("*дерево после преобразования:\n");
    tree_print(tr1);
    char *result1=tree_to_expression1(tr1);
    printf("*конечное выражение:\n%s\n",result1);
    free(result1);
    tree_destroy(tr1);
    return 0;
}

