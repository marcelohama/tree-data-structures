/********************|
| Marcelo Tomio Hama |
|********************/

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#define T 2
#define maxChaves T*2
#define maxFilhos T*2+1
#define tipoChave int

/*************************************\
| DEFINI��O DA �RVORE B E DO REGISTRO |
|************************************************************************\
| Defini��o geral de �rvore b, tipos e ponteiros.                        |
\************************************************************************/
typedef struct
{
	tipoChave chave;
	char info1[30];
	int info2;
	float info3;
} tRegistro;
typedef struct Pagina *tArvoreB;
typedef struct Pagina
{
	int numChaves;
  	tRegistro registro[maxChaves];
  	tArvoreB filhos[maxFilhos];
} tPagina;
typedef tArvoreB TipoDicionario;

/***************************\
| N�MERO DE CHAVES INCLUSAS |
|************************************************************************\
| Percorre a �rvore b e retorna o n�mero de chaves inclusas nela.        |
\************************************************************************/
int DADOS_numeroChavesArvoreB(tArvoreB R)
{
	//Inteiros para percurss�o e retorno
	int i,nChaves=0;
	//se n�o for n� nulo...
	if(R != NULL)
	{
		//acumula o n�mero de chaves para:
 		for(i=0;i<R->numChaves;i++)
		{
			//o n�
 			nChaves++;
 			//os filhos
			if(R->filhos[i] != NULL) nChaves+=DADOS_numeroChavesArvoreB(R->filhos[i]);
		}
		//o filho direito(se existir)
		if(R->filhos[i] != NULL) nChaves+=DADOS_numeroChavesArvoreB(R->filhos[i]);
	}
	//retorna o n�mero acumulado
	return nChaves;
}

/********************\
| ALTURA DA �RVORE B |
|************************************************************************\
| Percorre a �rvore b e retorna a altura m�xima alcan�ada na percurss�o  |
\************************************************************************/
int DADOS_alturaArvoreB(tArvoreB R)
{
	//Inteiros para posi��o e altura acumulada
	int i,altura=-2;
	//se a �rvore n�o for nula...
	if(R != NULL)
	{
		//verifica a altura dos filhos e incrementa a maior altura encontrada em 1
		for(i=0;i<=R->numChaves;i++)
			if(altura < DADOS_alturaArvoreB(R->filhos[i]))
   				altura=DADOS_alturaArvoreB(R->filhos[i])+1;
		//retorna a altura
		return altura;
	}
	//sen�o retorna -1
	else return -1;
}

/*********************\
| PERCURSS�O EM ORDEM |
|************************************************************************\
| Recebe uma ra�z de �rvore b e percorre-a em ordem.                     |
\************************************************************************/
void DADOS_emOrdemArvoreB(tArvoreB R)
{
	//inteiro para guardar as posi��es dos elementos do vetor
	int i;
	//se o n� n�o for nulo...
	if(R != NULL)
	{
		//imprime todos os seus elementos e chama a fun��o novamente em seus filhos
		for(i=0;i<R->numChaves;i++)
		{
			DADOS_emOrdemArvoreB(R->filhos[i]);
			printf("%d ",R->registro[i].chave);
		}
		//chama a fun��o novamente no filho que faltou
		DADOS_emOrdemArvoreB(R->filhos[i]);
	}
}

/*******************\
| DESENHO DA ARVORE |
|************************************************************************\
| Recebe uma ra�z de �rvore b e desenha-a em tela.                       |
\************************************************************************/
void DADOS_desenhoArvoreB(tArvoreB R, int espaco)
{
	//inteiro para guardar as posi��es dos elementos do vetor
	int i, j;
	//se o n� n�o for nulo...
	if(R != NULL)
	{
		//imprime todos os seus elementos
		printf("< ");
		for(i=0;i<R->numChaves;i++)
		{
         printf("%d ",R->registro[i].chave);
		}
		printf(">");
		
		//chama a fun��o novamente nos filhos
		for(i=0;i<=R->numChaves;i++) {
         if(R->filhos[i] != NULL)
         {
            if(R->filhos[i] != NULL) printf("\n");
            for(j=0;j<espaco+3;j++) printf(" ");
      		DADOS_desenhoArvoreB(R->filhos[i],espaco+3);
         }
      }
	}
}

/******************************\
| FUN��O DE PESQUISA DE CHAVES |
|************************************************************************\
| Procura em uma ra�z passada como par�metro, a p�gina contendo a chave  |
| passada como par�metro, retornando a p�gina caso exista a chave e nulo |
| caso n�o exista a chave.                                               |
\************************************************************************/
tArvoreB DADOS_pesquisaArvoreB(tRegistro Reg, tArvoreB R)
{
	//inteiro para percorrer a p�gina
	int i = 1;
	//se a ra�z for nula, a chave n�o existe e retorna nulo
	if(R == NULL)
  	{
    	printf("\n=== REGISTRO INEXISTENTE ===");
    	getch();
    	return NULL;
  	}
  	//procura na p�gina pela posi��o correta de verifica��o
  	while(i < R->numChaves && Reg.chave > R->registro[i - 1].chave)
		i++;
	//se encontrar a chave, retorna a p�gina
  	if(Reg.chave == R->registro[i - 1].chave)
  	{
    	Reg.chave = R->registro[i - 1].chave;
    	return R;
  	}
  	//sen�o, retorna o resultado de uma nova chamada em um dos filhos, dependendo do valor da chave
  	if(Reg.chave < R->registro[i - 1].chave)
    	return DADOS_pesquisaArvoreB(Reg, R->filhos[i - 1]);
	else
    	return DADOS_pesquisaArvoreB(Reg, R->filhos[i]);
}

/*********************************\
| INSER��O ORDENADA EM UMA P�GINA |
|************************************************************************\
| Insere de forma ordenada uma chave em uma p�gina, remanejando filhos   |
| e chaves caso necess�rio.                                              |
\************************************************************************/
void INSERCAO_paginaArvoreB(tArvoreB r, tRegistro Reg, tArvoreB filhoDireito)
{
	//inteiros para percorrer e verifica��o de posi��o
	int k = r->numChaves,
  	    NaoAchouPosicao = k > 0;
	//enquanto n�o for encontrada a posi��o...
	while(NaoAchouPosicao)
  	{
  		//procurando pela posi��o
    	if(Reg.chave >= r->registro[k - 1].chave)
    	{
      		NaoAchouPosicao = 0;
      		break;
    	}
    	//remanejando as chaves e filhos
    	r->registro[k] = r->registro[k - 1];
    	r->filhos[k + 1] = r->filhos[k];
    	k--;
    	if(k < 1)
			NaoAchouPosicao = 0;
  	}
  	//inser��o do novo elemento e atualiza��o do n� (de forma organizada)
  	r->registro[k] = Reg;
  	r->filhos[k + 1] = filhoDireito;
	r->numChaves++;
}

/*********************************\
| INSER��O ORDENADA EM UMA P�GINA |
|************************************************************************\
| Esta fun��o serve de auxilio para a inser��o de chaves na p�gina.      |
| [CHAVE] � a chave a ser inclusa, [R] � a ra�z da �rvore b, [h] � um    |
| valor booleano de retorno verdadeiro caso [R] seja folha, [m] � a      |
| chave m�dia retornada caso haja divis�o e novaPagina � a p�gina a ser  |
| retornada caso haja divis�o da p�gina.                                 |
\************************************************************************/
void INSERCAO_insereChaveArvoreB(tRegistro Reg, tArvoreB r, bool *h, tRegistro *regRetorno, tArvoreB *filhoRetorno)
{
	//ponteiro tempor�rio para guardar poss�vel novo n� alocado
  	tArvoreB ptAux;
  	//inteiros para percorer as p�ginas
  	int i = 1, j;
	//se a �rvore for nula, retorna para inserir o pr�prio valor recebido
  	if(r == NULL)
  	{
    	*h = true;
    	*regRetorno = Reg;
    	*filhoRetorno = NULL;
    	return;
  	}
  	//percorrendo a p�gina at� encontrar-se a posi��o de inser��o
  	while(i < r->numChaves && Reg.chave > r->registro[i - 1].chave)
    	i++;
	//tratando o caso da chave j� existir na �rvore
  	if(Reg.chave == r->registro[i - 1].chave)
  	{
	    printf("=== REGISTRO JA INCLUSA ===");
    	getch();
    	*h = false;
    	return;
  	}
  	//procurando de forma recursiva a p�gina folha e a poss�vel chave de retorno no:
  	if(Reg.chave < r->registro[i - 1].chave)
  		//filho esquerdo, caso seja uma chave menor da de verifica��o
    	INSERCAO_insereChaveArvoreB(Reg, r->filhos[i - 1], h, regRetorno, filhoRetorno);
  	else
  		//filho direito, caso seja uma chave maior da de verifica��o
    	INSERCAO_insereChaveArvoreB(Reg, r->filhos[i], h, regRetorno, filhoRetorno);
	//se a condi��o de folha for verdadeira, retorna
  	if(!*h)
    	return;
	//caso a p�gina n�o esteja completa, insere-se normalmente e retorna
  	if(r->numChaves < maxChaves)
  	{
    	INSERCAO_paginaArvoreB(r, *regRetorno, *filhoRetorno);
    	*h = false;
    	return;
  	}
  	//sen�o, haver� a split e a aloca��o de nova p�gina
  	ptAux = (tArvoreB) malloc(sizeof(tPagina));
  	ptAux->numChaves = 0;
  	ptAux->filhos[0] = NULL;
  	//caso a inser��o seja antes do meio da p�gina...
  	if(i <= T + 1)
  	{
  		//insere-se a chave ordenadamente e busca a chave m�dia depois do meio da p�gina
    	INSERCAO_paginaArvoreB(ptAux, r->registro[maxChaves - 1], r->filhos[maxChaves]);
    	r->numChaves--;
    	INSERCAO_paginaArvoreB(r, *regRetorno, *filhoRetorno);
  	}
	//caso a inser��o seja depois do meio da p�gina...
  	else
  		//insere-se a chave e busca a chave m�dia antes do meio da p�gina
    	INSERCAO_paginaArvoreB(ptAux, *regRetorno, *filhoRetorno);
	//coloca na nova p�gina metade dos valores da outra p�gina
  	for(j = T + 2; j <= maxChaves; j++)
    	INSERCAO_paginaArvoreB(ptAux, r->registro[j - 1], r->filhos[j]);
	//atualizando o n�mero de chaves da p�gina antiga
  	r->numChaves = T;
  	//passando a p�gina-filha da antiga p�gina para a nova p�gina
  	ptAux->filhos[0] = r->filhos[T + 1];
  	//setando o valor m�dio e a p�gina a serem retornadas
  	*regRetorno = r->registro[T];
  	*filhoRetorno = ptAux;
}

/**************************\
| INSER��O EM UMA �RVORE B |
|************************************************************************\
| Esta fun��o recebe uma ra�z de �rvore b e uma chave. Insere a chave    |
| recebida na ra�z utilizando as rotinas de aux�lio de inser��o acima.   |
\************************************************************************/
void INSERCAO_insereArvoreB(tRegistro Reg, tArvoreB *r)
{
	//valor booleano para verifica��o de folha
  	bool h;
  	//chave para valor m�dio de retorno
  	tRegistro regRetorno;
  	//ponteiros para retorno de p�gina nova e para aux�lio
  	tArvoreB filhoRetorno;
  	//inserindo-se e preenchendo as vari�veis de verifica��o [m] e [novaPagina]
  	tArvoreB ptAux;
  	//se o n� for folha...
  	INSERCAO_insereChaveArvoreB(Reg, *r, &h, &regRetorno, &filhoRetorno);
  	if(h)
   	{
   		//aloca��es e configura��es para a cria��o da �rvore
    	ptAux = (tArvoreB)malloc(sizeof(tPagina));
    	ptAux->numChaves = 1;
    	ptAux->registro[0] = regRetorno;
    	ptAux->filhos[1] = filhoRetorno;
    	ptAux->filhos[0] = *r;
    	//passando a �rvore para a ra�z recebida
    	*r = ptAux;
	}
}

/****************************\
| RECONSTITUI��O DE �RVORE B |
|************************************************************************\
| Esta fun��o � an�loga � [INSERCAO_splitArvoreB] pois serve de aux�lio  |
| para a remo��o de chaves. [R] � a ra�z da �rvore b, [ptPai] � a p�gina |
| precedente de [R], [posPai] � a posi��o dos poss�veis irm�o de [R] e   |
| [h] � uma valor booleano para verifica��o de folha.                    |
\************************************************************************/
void REMOCAO_remontaArvoreB(tArvoreB r, tArvoreB ptPai, int posPai, bool *d)
{
	//ponteiro auxiliar
  	tArvoreB ptAux;
  	//inteiros para aux�lio e percurss�o
  	int DispAux, j;
  	if(posPai < ptPai->numChaves)
   	{
    	ptAux = ptPai->filhos[posPai + 1];
    	DispAux = (ptAux->numChaves - T + 1) / 2;
    	r->registro[r->numChaves] = ptPai->registro[posPai];
    	r->filhos[r->numChaves + 1] = ptAux->filhos[0];
    	r->numChaves++;
    	if(DispAux > 0)
     	{
      		for(j = 1; j < DispAux; j++)
        		INSERCAO_paginaArvoreB(r, ptAux->registro[j - 1], ptAux->filhos[j]);
      		ptPai->registro[posPai] = ptAux->registro[DispAux - 1];
      		ptAux->numChaves -= DispAux;
      		for(j = 0; j < ptAux->numChaves; j++)
        		ptAux->registro[j] = ptAux->registro[j + DispAux];
      		for(j = 0; j <= ptAux->numChaves; j++)
        		ptAux->filhos[j] = ptAux->filhos[j + DispAux];
      		*d = false;
		}
    	else
   	 	{
  			for(j = 1; j <= T; j++)
    			INSERCAO_paginaArvoreB(r, ptAux->registro[j - 1], ptAux->filhos[j]);
      		free(ptAux);
      		for(j = posPai + 1; j < ptPai->numChaves; j++)
      		{
        		ptPai->registro[j - 1] = ptPai->registro[j];
        		ptPai->filhos[j] = ptPai->filhos[j + 1];
      		}
      		ptPai->numChaves--;
      		if(ptPai->numChaves >= T)
        		*d = false;
		}
  	}
  	else
  	{
    	ptAux = ptPai->filhos[posPai - 1];
    	DispAux = (ptAux->numChaves - T + 1) / 2;
    	for(j = r->numChaves; j >= 1; j--)
      		r->registro[j] = r->registro[j - 1];
    	r->registro[0] = ptPai->registro[posPai - 1];
    	for(j = r->numChaves; j >= 0; j--)
      		r->filhos[j + 1] = r->filhos[j];
    	r->numChaves++;
    	if(DispAux > 0)
     	{
      		for(j = 1; j < DispAux; j++)
        		INSERCAO_paginaArvoreB(r, ptAux->registro[ptAux->numChaves - j], ptAux->filhos[ptAux->numChaves - j + 1]);
      		r->filhos[0] = ptAux->filhos[ptAux->numChaves - DispAux + 1];
 		 	ptPai->registro[posPai - 1] = ptAux->registro[ptAux->numChaves - DispAux];
      		ptAux->numChaves -= DispAux;
      		*d = false;
    	}
    	else
    	{
      		for(j = 1; j <= T; j++)
        		INSERCAO_paginaArvoreB(ptAux, r->registro[j - 1], r->filhos[j]);
      		free(r);
      		ptPai->numChaves--;
      		if(ptPai->numChaves >= T)
        	*d = false;
    	}
  	}
}

void REMOCAO_buscaPaiArvoreB(tArvoreB r, int Ind, tArvoreB ptPai, bool *d)
{
  	if(ptPai->filhos[ptPai->numChaves] != NULL)
	{
    	REMOCAO_buscaPaiArvoreB(r, Ind, ptPai->filhos[ptPai->numChaves], d);
    	if(*d)
      		REMOCAO_remontaArvoreB(ptPai->filhos[ptPai->numChaves], ptPai, ptPai->numChaves, d);
    	return;
  	}
  	r->registro[Ind - 1] = ptPai->registro[ptPai->numChaves - 1];
  	ptPai->numChaves--;
  	*d = ptPai->numChaves < T;
}

void REMOCAO_mergeArvoreB(tipoChave Ch, tArvoreB *r, bool *d)
{
	int Ind = 1, j;
  	tArvoreB ptAux = *r;
  	if(*r == NULL)
  	{
  		printf("=== REGISTRO INEXISTENTE ===");
    	getch();
    	*d = false;
    	return;
  	}
  	while(Ind < ptAux->numChaves && Ch > ptAux->registro[Ind - 1].chave)
    Ind++;
  	if(Ch == ptAux->registro[Ind - 1].chave)
  	{
    	if(ptAux->filhos[Ind - 1] == NULL)
     	{
      		ptAux->numChaves--;
      		*d = ptAux->numChaves < T;
      		for(j = Ind; j <= ptAux->numChaves; j++)
			{
        		ptAux->registro[j - 1] = ptAux->registro[j];
        		ptAux->filhos[j] = ptAux->filhos[j + 1];
			}
  			return;
    	}
    	REMOCAO_buscaPaiArvoreB(*r, Ind, ptAux->filhos[Ind - 1], d);
    	if(*d)
  			REMOCAO_remontaArvoreB(ptAux->filhos[Ind - 1], *r, Ind - 1, d);
    	return;
  	}
	if(Ch > ptAux->registro[Ind - 1].chave)
    	Ind++;
  	REMOCAO_mergeArvoreB(Ch, &ptAux->filhos[Ind - 1], d);
	if(*d)
    	REMOCAO_remontaArvoreB(ptAux->filhos[Ind - 1], *r, Ind - 1, d);
}

/*************************\
| REMO��O EM UMA �RVORE B |
|************************************************************************\
| Esta fun��o recebe uma ra�z de �rvore b e uma chave. Remove da �rvore  |
| recebida a chave recebida caso ambas existam.                          |
\************************************************************************/
void REMOCAO_removeArvoreB(tipoChave Ch, tArvoreB *r)
{
	//booleano para verifica��o de folha
	bool d;
	//ponteiro auxiliar
  	tArvoreB ptAux;
  	//removendo a chave...
  	REMOCAO_mergeArvoreB(Ch, r, &d);
  	//caso o valor de [h] seja verdadeiro (folha) e o n�mero de chaves na p�gina seja 0...
  	if(d && (*r)->numChaves == 0)
   	{
   		//libera a �rvore b
   		ptAux = *r;
		*r = ptAux->filhos[0];
    	free(ptAux);
  	}
}

/*******************\
| MENU DE INTERFACE |
|************************************************************************\
| Este menu possibilita a manipula��o da �rvore b.                       |
\************************************************************************/
void MENU_arvoreB()
{
    tArvoreB R=NULL, ptAux;
    tRegistro reg;
    char resp;
    while(1)
    {
        system("cls");
        
        printf("|--------------------------------------------------------------------|\n");
        printf("| ARVORE B                                                           |\n");
        printf("|--------------------------------------------------------------------|\n");
        printf("| Implementacao em console de arvore B. O programa foi feito para    |\n");
        printf("| testes com numeros, e deve ser modificado em caso de troca de tipo |\n");
        printf("| de dado.                                                           |\n");
        printf("|--------------------------------------------------------------------|\n");
        printf("| Marcelo Tomio Hama - trabalho de estrutura de dados                |\n");
        printf("|--------------------------------------------------------------------|\n|\n");
		
        printf("| <1> INSERIR\n| <2> REMOVER\n| <3> BUSCAR\n| <4> DADOS\n| <5> ESTRUTURA\n| <6> SAIR\n|\n");
        printf("|--------------------------------------------------------------------|\n| ");
        
        resp=getch();
        switch(resp)
        {
            case '1': printf("Insira o elemento da insercao --- ");
                      scanf("%d",&reg.chave);
                      INSERCAO_insereArvoreB(reg,&R);
                      break;
            case '2': printf("Insira o elemento a remover --- ");
                      scanf("%d",&reg.chave);
                      REMOCAO_removeArvoreB(reg.chave,&R);
                      break;
            case '3': printf("Insira o elemento a se pesquisar --- ");
                      scanf("%d",&reg.chave);
                      ptAux=DADOS_pesquisaArvoreB(reg,R);
                      if(ptAux != NULL)
                      {
  				              printf("\n=== CHAVE %d ENCONTRADA! ===",reg.chave);
                      	  getch();
                  	 }
                      break;
            case '4': printf("\nElementos em ordem - ");
             		    DADOS_emOrdemArvoreB(R);
   					    printf("\nNumero de chaves incluidas - %d",DADOS_numeroChavesArvoreB(R));
   					    printf("\nAltura da arvore - %d",DADOS_alturaArvoreB(R));
                  	 getch();
                      break;
            case '5': printf("Desenho da arvore:\n\n");
                      DADOS_desenhoArvoreB(R,0);
                      getch();
                      break;
            case '6': exit(0);
                      break;
            default : ;
        }
    }
	
}

int main()
{
	MENU_arvoreB();
	return 0;
}
