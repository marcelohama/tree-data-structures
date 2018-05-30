/********************|
| Marcelo Tomio Hama |
|********************/

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

/*****************************\
| ESTRUTURA DA ÁRVORE BINÁRIA |
\*****************************/
struct Arvore
{
    int info;
    struct Arvore *dir;
    struct Arvore *esq;
};
typedef struct Arvore tArvore;
tArvore *T = NULL;

/*******************************\
| ESTRUTURA E FUNÇÕES DE PILHAS |
\*******************************/
struct Pilha
{
    tArvore *info;
    struct Pilha *prox;
};
typedef struct Pilha tPilha;
tPilha *P=NULL;
void pilhaPush(tPilha **p, tArvore *r)
{
    tPilha *pt=(tPilha*)malloc(sizeof(tPilha));
    pt->info=r;
    pt->prox=(*p);
    (*p)=pt;
}
tArvore *pilhaPop(tPilha **p)
{
    if((*p)!=NULL)
    {
        tArvore *retorno=(*p)->info;
        tPilha *pro=(*p)->prox;
        free(*p);
        (*p)=pro;
        return retorno;
    }
}

/*******************************\
| CONTA OS NÓS QUE A ÁRVORE TEM |
\*******************************/
int contaArvore(tArvore *r)
{
    if(r == NULL) return 0;
    return contaArvore(r->esq) + contaArvore(r->dir) + 1;
}

/********************************\
| RETORNA A ALTURA DE UMA ÁRVORE |
\********************************/
int alturaArvore(tArvore *r)
{
    if(r == NULL) return -1;
    else
    {
        int he=alturaArvore(r->esq);
        int hd=alturaArvore(r->dir);
        
        if (he < hd) return hd + 1;
        else return he + 1;
    }
}

/****************************\
| BUSCA UM DADO NÓ NA ÁRVORE |
\****************************/
tArvore *buscaNo(tArvore *r, int elem)
{
    if(r != NULL)
    {
        if(elem == r->info) return r;
        else if(elem > r->info) buscaNo(r->dir, elem);
        else if(elem < r->info) buscaNo(r->esq, elem);
    }
    else
    {
        printf("\nelemento nao encontrado");
        return NULL;
    }
}

/****************************\
| PERCORRE EM ERD UMA ÁRVORE |
\****************************/
void percorreArvore(tArvore *r)
{
    if(r != NULL)
    {
		percorreArvore(r->esq);
		printf("%d ", r->info);
		percorreArvore(r->dir);
    }
}

/********************************\
| DESENHO DA ESTRUTURA DA ÁRVORE |
\********************************/
void desenhaArvore(tArvore *r, int posicao) {
   int i;
   if(r != NULL) {
      printf("< %d >",r->info);
   }
   if(r->esq != NULL) {
      printf("\n");
      for(i=0;i<posicao+3;i++) printf(" ");
      desenhaArvore(r->esq,posicao+3);
   }
   if(r->dir != NULL) {
      printf("\n");
      for(i=0;i<posicao+3;i++) printf(" ");
      desenhaArvore(r->dir,posicao+3);
   }
}

/********************************************\
| PERCORRE EM ERD UMA ÁRVORE - NÃO RECURSIVO |
\********************************************/
void percorreIterativo(tArvore *r)
{
    tArvore *pt=r;
    pilhaPush(&P,pt);
    while(P != NULL)
    {
       pt = pilhaPop(&P);
       printf("%d ", pt->info);
       if (pt->dir != NULL) pilhaPush(&P, pt->dir);
       if (pt->esq != NULL) pilhaPush(&P, pt->esq);
    }
}

/*********************\
| FUNÇÕES DE ROTAÇÕES |
\*********************/
void Rdireita(tArvore **r)
{ 
	tArvore *noEsq;
	noEsq=(*r)->esq;
	(*r)->esq=(*r)->esq->dir;
	noEsq->dir=(*r);
	(*r)=noEsq;
}
void Resquerda(tArvore **r)
{
	tArvore *noDir;
	noDir=(*r)->dir;
	(*r)->dir=(*r)->dir->esq;
	noDir->esq=(*r);
	(*r)=noDir;
}

/*****************************\
| DEVOLVE O PAI DE UM DADO NÓ |
\*****************************/
tArvore *paiArvore(tArvore *r, tArvore *no)
{
	if(r==no) return NULL;
	else if(r->dir!=no && r->esq!=no)
	{
		paiArvore(r->dir,no);
		paiArvore(r->esq,no);
	}
	else return r;
}

/*******************************************\
| DEVOLVE O FATOR DE BALANCEAMENTO DE UM NÓ |
\*******************************************/
int FB(tArvore *r)
{
	if(r != NULL)
	{
		int fb=alturaArvore(r->dir)-alturaArvore(r->esq);
		return fb;
	}
	else return 0;
}

/******************************\
| BALANCEIA UMA ÁRVORE BINÁRIA |
\******************************/
void balanceiaArvore(tArvore **r)
{
	if(FB(*r) > 1 || FB(*r) < -1)
	{
    	int FBraiz=FB(*r);
    	int FBdir=FB((*r)->dir);
    	int FBesq=FB((*r)->esq);
		if(FBraiz > 1)
    	{ 
    	    if((FBesq > 0) || FBdir > 0) Resquerda(&(*r));
		    else if(FBesq < 0) { Rdireita(&(*r)->esq); Resquerda(&(*r)); }
		    else if(FBdir < 0) { Rdireita(&(*r)->dir); Resquerda(&(*r)); }
		}
		if(FBraiz < -1)
    	{
    	    if(FBesq < 0 || FBdir < 0) Rdireita(&(*r));
		    else if(FBesq > 0) { Resquerda(&(*r)->esq); Rdireita(&(*r)); }
    	    else if(FBdir > 0) { Resquerda(&(*r)->dir); Rdireita(&(*r)); }
    	}
	}
}

/*********************************\
| INSERÇÃO ORDENADA EM UMA ÁRVORE |
\*********************************/
void insereArvore(tArvore **r, int elem)
{
  	if(*r == NULL)
  	{
    	*r = (tArvore *) malloc(sizeof(tArvore));
    	(*r)->info = elem;
    	(*r)->esq =  NULL;
    	(*r)->dir = NULL;
    	return;
  	}
  	else if ((*r)->info < elem)
    	insereArvore(&(*r)->dir, elem);
    else if ((*r)->info > elem)
    	insereArvore(&(*r)->esq, elem);
	balanceiaArvore(&(*r));
}

/***************************************\
| EXCLUSÃO DE NÓS DE UMA ÁRVORE BINÁRIA |
\***************************************/
void excluirArvore(tArvore **r, int elem)
{	
	tArvore *aux=*r, *pai=NULL;
	while(aux != NULL && aux->info != elem)
	{
		pai=aux;
		if(elem > aux->info) aux=aux->dir;
		else aux=aux->esq;
	}
	if(aux == NULL) return;
	if(pai == NULL)
	{
	    free(*r);
	    return;
	}
	if(aux->esq == NULL && aux->dir == NULL)
	{
		if(elem > pai->info) pai->dir=NULL;
		else pai->esq=NULL;
		free(aux);
	}
	else if(aux->esq == NULL)
	{
		if(elem > pai->info) pai->dir=aux->dir;
		else pai->esq=aux->dir;
		free(aux);
	}
	else if(aux->dir == NULL)
	{
		if(elem > pai->info) pai->dir=aux->esq;
		else pai->esq=aux->esq;
		free(aux);
	}
	else if(aux->esq != NULL && aux->dir != NULL)
	{
		tArvore *pt_suc=aux->dir, *pt_paiSuc=aux;
		while(pt_suc->esq != NULL)
		{
			pt_paiSuc=pt_suc;
			pt_suc=pt_suc->esq;
		}
		if(elem > pai->info) pai->dir=pt_suc;
		else pai->esq=pt_suc;
		if(pt_suc->info > pt_paiSuc->info) pt_paiSuc->dir=pt_suc->dir;
		else pt_paiSuc->esq=pt_suc->dir;
		pt_suc->dir=aux->dir;
		pt_suc->esq=aux->esq;
		free(aux);
	}
}

/*****************************************\
| IMPRIME OS DADOS RELATIVOS Á UMA ÁRVORE |
\*****************************************/
void dadosArvore(tArvore r)
{
	printf("\nAltura - %d",alturaArvore(&r));
	printf("\nNos - %d",contaArvore(&r));
	printf("\nElementos ERD - "); percorreArvore(&r);
	getch();
}

int main()
{
	int elem;
	char R;
	for(;;)
	{
		system("cls");
		
		printf("|--------------------------------------------------------------------|\n");
		printf("| ARVORE AVL                                                         |\n");
		printf("|--------------------------------------------------------------------|\n");
		printf("| Implementacao em console de arvore avl. O programa foi feito para  |\n");
		printf("| testes com numeros, e deve ser modificado em caso de troca de tipo |\n");
		printf("| de dado.                                                           |\n");
		printf("|--------------------------------------------------------------------|\n");
		printf("| Marcelo Tomio Hama - trabalho de estrutura de dados                |\n");
		printf("|--------------------------------------------------------------------|\n|\n");
		
		printf("| <1> Inserir\n| <2> Excluir\n| <3> Dados\n| <4> Busca\n| <5> Percursao Iterativa\n| <6> Estrutura\n| <7> Sair\n|\n");
		printf("|--------------------------------------------------------------------|\n| ");
		
		fflush(stdin); R=getch();
		switch(R)
		{
			case '1': printf("Insira o elemento a ser incluso --- ");
					    scanf("%d",&elem);
					    insereArvore(&T,elem);
       				 break;
			case '2': printf("Insira o elemento a ser excluso --- ");
   					 scanf("%d",&elem);
   					 excluirArvore(&T,elem);
          			 break;
			case '3': dadosArvore(*T);
   					 break;
  			case '4': printf("Insira o elemento a ser pesquisado --- ");
					    scanf("%d",&elem);
    			       printf("\nEndereco do No procurado: %d",buscaNo(T, elem));
     	     		    getch();
   					 break;
  			case '5': percorreIterativo(T);
                   getch();
					    break;
			case '6': printf("\n\nDesenho da arvore\n");
                   desenhaArvore(T,0);
             		 getch();
                   break;
			case '7': exit(0);
                   break;
			default : ;
			getch();
		}
	}
    return 0;
}
