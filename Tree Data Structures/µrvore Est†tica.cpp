/********************|
| Marcelo Tomio Hama |
|********************/

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#define Nos 100

typedef struct ArvoreEstatica
{
	int esq;
	int dir;
	int info;
} tStaticTree;
tStaticTree A[Nos];

void inicializaArvore(tStaticTree r[])
{
	int i;
	for(i=0;i<Nos;i++)
	{
		r[i].esq = -1;
		r[i].dir = -1;
		r[i].info = '\0';
	}
}

void insereNo(tStaticTree r[], int elem)
{
	int i=0;
	while(r[i].info != '\0')
	{
		if(elem < r[i].info) i=2*i+1;
		else i=2*i+2;
		if(i >= Nos)
		{
			printf("\nlimite da arvore excedido");
			return;
		}
	}
	r[i].info = elem;
	r[i].esq = 2*i+1;
	r[i].dir = 2*i+2;
	return;
}

void mostraArvore(tStaticTree r[])
{
	int i;
	for(i=0;i<Nos;i++)
		if(r[i].info != '\0')
		{
			printf("\nindice - %d",i);
  			printf("\ninfo - %d",r[i].info);
			printf("\nesq - %d",r[i].esq);
			printf("\ndir - %d\n",r[i].dir);
		}
	return;
}

int main()
{
	int i;
	inicializaArvore(A);
	for(i=0;i<5;i++) insereNo(A,i);
	mostraArvore(A);
    getch();
    return 0;
}
