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
| DEFINIÇÃO DA ÁRVORE B E DO REGISTRO |
|************************************************************************\
| Definição geral de árvore b, tipos e ponteiros.                        |
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
| NÚMERO DE CHAVES INCLUSAS |
|************************************************************************\
| Percorre a árvore b e retorna o número de chaves inclusas nela.        |
\************************************************************************/
int DADOS_numeroChavesArvoreB(tArvoreB R)
{
	//Inteiros para percurssão e retorno
	int i,nChaves=0;
	//se não for nó nulo...
	if(R != NULL)
	{
		//acumula o número de chaves para:
 		for(i=0;i<R->numChaves;i++)
		{
			//o nó
 			nChaves++;
 			//os filhos
			if(R->filhos[i] != NULL) nChaves+=DADOS_numeroChavesArvoreB(R->filhos[i]);
		}
		//o filho direito(se existir)
		if(R->filhos[i] != NULL) nChaves+=DADOS_numeroChavesArvoreB(R->filhos[i]);
	}
	//retorna o número acumulado
	return nChaves;
}

/********************\
| ALTURA DA ÁRVORE B |
|************************************************************************\
| Percorre a árvore b e retorna a altura máxima alcançada na percurssão  |
\************************************************************************/
int DADOS_alturaArvoreB(tArvoreB R)
{
	//Inteiros para posição e altura acumulada
	int i,altura=-2;
	//se a árvore não for nula...
	if(R != NULL)
	{
		//verifica a altura dos filhos e incrementa a maior altura encontrada em 1
		for(i=0;i<=R->numChaves;i++)
			if(altura < DADOS_alturaArvoreB(R->filhos[i]))
   				altura=DADOS_alturaArvoreB(R->filhos[i])+1;
		//retorna a altura
		return altura;
	}
	//senão retorna -1
	else return -1;
}

/*********************\
| PERCURSSÃO EM ORDEM |
|************************************************************************\
| Recebe uma raíz de árvore b e percorre-a em ordem.                     |
\************************************************************************/
void DADOS_emOrdemArvoreB(tArvoreB R)
{
	//inteiro para guardar as posições dos elementos do vetor
	int i;
	//se o nó não for nulo...
	if(R != NULL)
	{
		//imprime todos os seus elementos e chama a função novamente em seus filhos
		for(i=0;i<R->numChaves;i++)
		{
			DADOS_emOrdemArvoreB(R->filhos[i]);
			printf("%d ",R->registro[i].chave);
		}
		//chama a função novamente no filho que faltou
		DADOS_emOrdemArvoreB(R->filhos[i]);
	}
}

/*******************\
| DESENHO DA ARVORE |
|************************************************************************\
| Recebe uma raíz de árvore b e desenha-a em tela.                       |
\************************************************************************/
void DADOS_desenhoArvoreB(tArvoreB R, int espaco)
{
	//inteiro para guardar as posições dos elementos do vetor
	int i, j;
	//se o nó não for nulo...
	if(R != NULL)
	{
		//imprime todos os seus elementos
		printf("< ");
		for(i=0;i<R->numChaves;i++)
		{
         printf("%d ",R->registro[i].chave);
		}
		printf(">");
		
		//chama a função novamente nos filhos
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
| FUNÇÃO DE PESQUISA DE CHAVES |
|************************************************************************\
| Procura em uma raíz passada como parâmetro, a página contendo a chave  |
| passada como parâmetro, retornando a página caso exista a chave e nulo |
| caso não exista a chave.                                               |
\************************************************************************/
tArvoreB DADOS_pesquisaArvoreB(tRegistro Reg, tArvoreB R)
{
	//inteiro para percorrer a página
	int i = 1;
	//se a raíz for nula, a chave não existe e retorna nulo
	if(R == NULL)
  	{
    	printf("\n=== REGISTRO INEXISTENTE ===");
    	getch();
    	return NULL;
  	}
  	//procura na página pela posição correta de verificação
  	while(i < R->numChaves && Reg.chave > R->registro[i - 1].chave)
		i++;
	//se encontrar a chave, retorna a página
  	if(Reg.chave == R->registro[i - 1].chave)
  	{
    	Reg.chave = R->registro[i - 1].chave;
    	return R;
  	}
  	//senão, retorna o resultado de uma nova chamada em um dos filhos, dependendo do valor da chave
  	if(Reg.chave < R->registro[i - 1].chave)
    	return DADOS_pesquisaArvoreB(Reg, R->filhos[i - 1]);
	else
    	return DADOS_pesquisaArvoreB(Reg, R->filhos[i]);
}

/*********************************\
| INSERÇÃO ORDENADA EM UMA PÁGINA |
|************************************************************************\
| Insere de forma ordenada uma chave em uma página, remanejando filhos   |
| e chaves caso necessário.                                              |
\************************************************************************/
void INSERCAO_paginaArvoreB(tArvoreB r, tRegistro Reg, tArvoreB filhoDireito)
{
	//inteiros para percorrer e verificação de posição
	int k = r->numChaves,
  	    NaoAchouPosicao = k > 0;
	//enquanto não for encontrada a posição...
	while(NaoAchouPosicao)
  	{
  		//procurando pela posição
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
  	//inserção do novo elemento e atualização do nó (de forma organizada)
  	r->registro[k] = Reg;
  	r->filhos[k + 1] = filhoDireito;
	r->numChaves++;
}

/*********************************\
| INSERÇÃO ORDENADA EM UMA PÁGINA |
|************************************************************************\
| Esta função serve de auxilio para a inserção de chaves na página.      |
| [CHAVE] é a chave a ser inclusa, [R] é a raíz da árvore b, [h] é um    |
| valor booleano de retorno verdadeiro caso [R] seja folha, [m] é a      |
| chave média retornada caso haja divisão e novaPagina é a página a ser  |
| retornada caso haja divisão da página.                                 |
\************************************************************************/
void INSERCAO_insereChaveArvoreB(tRegistro Reg, tArvoreB r, bool *h, tRegistro *regRetorno, tArvoreB *filhoRetorno)
{
	//ponteiro temporário para guardar possível novo nó alocado
  	tArvoreB ptAux;
  	//inteiros para percorer as páginas
  	int i = 1, j;
	//se a árvore for nula, retorna para inserir o próprio valor recebido
  	if(r == NULL)
  	{
    	*h = true;
    	*regRetorno = Reg;
    	*filhoRetorno = NULL;
    	return;
  	}
  	//percorrendo a página até encontrar-se a posição de inserção
  	while(i < r->numChaves && Reg.chave > r->registro[i - 1].chave)
    	i++;
	//tratando o caso da chave já existir na árvore
  	if(Reg.chave == r->registro[i - 1].chave)
  	{
	    printf("=== REGISTRO JA INCLUSA ===");
    	getch();
    	*h = false;
    	return;
  	}
  	//procurando de forma recursiva a página folha e a possível chave de retorno no:
  	if(Reg.chave < r->registro[i - 1].chave)
  		//filho esquerdo, caso seja uma chave menor da de verificação
    	INSERCAO_insereChaveArvoreB(Reg, r->filhos[i - 1], h, regRetorno, filhoRetorno);
  	else
  		//filho direito, caso seja uma chave maior da de verificação
    	INSERCAO_insereChaveArvoreB(Reg, r->filhos[i], h, regRetorno, filhoRetorno);
	//se a condição de folha for verdadeira, retorna
  	if(!*h)
    	return;
	//caso a página não esteja completa, insere-se normalmente e retorna
  	if(r->numChaves < maxChaves)
  	{
    	INSERCAO_paginaArvoreB(r, *regRetorno, *filhoRetorno);
    	*h = false;
    	return;
  	}
  	//senão, haverá a split e a alocação de nova página
  	ptAux = (tArvoreB) malloc(sizeof(tPagina));
  	ptAux->numChaves = 0;
  	ptAux->filhos[0] = NULL;
  	//caso a inserção seja antes do meio da página...
  	if(i <= T + 1)
  	{
  		//insere-se a chave ordenadamente e busca a chave média depois do meio da página
    	INSERCAO_paginaArvoreB(ptAux, r->registro[maxChaves - 1], r->filhos[maxChaves]);
    	r->numChaves--;
    	INSERCAO_paginaArvoreB(r, *regRetorno, *filhoRetorno);
  	}
	//caso a inserção seja depois do meio da página...
  	else
  		//insere-se a chave e busca a chave média antes do meio da página
    	INSERCAO_paginaArvoreB(ptAux, *regRetorno, *filhoRetorno);
	//coloca na nova página metade dos valores da outra página
  	for(j = T + 2; j <= maxChaves; j++)
    	INSERCAO_paginaArvoreB(ptAux, r->registro[j - 1], r->filhos[j]);
	//atualizando o número de chaves da página antiga
  	r->numChaves = T;
  	//passando a página-filha da antiga página para a nova página
  	ptAux->filhos[0] = r->filhos[T + 1];
  	//setando o valor médio e a página a serem retornadas
  	*regRetorno = r->registro[T];
  	*filhoRetorno = ptAux;
}

/**************************\
| INSERÇÃO EM UMA ÁRVORE B |
|************************************************************************\
| Esta função recebe uma raíz de árvore b e uma chave. Insere a chave    |
| recebida na raíz utilizando as rotinas de auxílio de inserção acima.   |
\************************************************************************/
void INSERCAO_insereArvoreB(tRegistro Reg, tArvoreB *r)
{
	//valor booleano para verificação de folha
  	bool h;
  	//chave para valor médio de retorno
  	tRegistro regRetorno;
  	//ponteiros para retorno de página nova e para auxílio
  	tArvoreB filhoRetorno;
  	//inserindo-se e preenchendo as variáveis de verificação [m] e [novaPagina]
  	tArvoreB ptAux;
  	//se o nó for folha...
  	INSERCAO_insereChaveArvoreB(Reg, *r, &h, &regRetorno, &filhoRetorno);
  	if(h)
   	{
   		//alocações e configurações para a criação da árvore
    	ptAux = (tArvoreB)malloc(sizeof(tPagina));
    	ptAux->numChaves = 1;
    	ptAux->registro[0] = regRetorno;
    	ptAux->filhos[1] = filhoRetorno;
    	ptAux->filhos[0] = *r;
    	//passando a árvore para a raíz recebida
    	*r = ptAux;
	}
}

/****************************\
| RECONSTITUIÇÃO DE ÁRVORE B |
|************************************************************************\
| Esta função é análoga à [INSERCAO_splitArvoreB] pois serve de auxílio  |
| para a remoção de chaves. [R] é a raíz da árvore b, [ptPai] é a página |
| precedente de [R], [posPai] é a posição dos possíveis irmão de [R] e   |
| [h] é uma valor booleano para verificação de folha.                    |
\************************************************************************/
void REMOCAO_remontaArvoreB(tArvoreB r, tArvoreB ptPai, int posPai, bool *d)
{
	//ponteiro auxiliar
  	tArvoreB ptAux;
  	//inteiros para auxílio e percurssão
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
| REMOÇÃO EM UMA ÁRVORE B |
|************************************************************************\
| Esta função recebe uma raíz de árvore b e uma chave. Remove da árvore  |
| recebida a chave recebida caso ambas existam.                          |
\************************************************************************/
void REMOCAO_removeArvoreB(tipoChave Ch, tArvoreB *r)
{
	//booleano para verificação de folha
	bool d;
	//ponteiro auxiliar
  	tArvoreB ptAux;
  	//removendo a chave...
  	REMOCAO_mergeArvoreB(Ch, r, &d);
  	//caso o valor de [h] seja verdadeiro (folha) e o número de chaves na página seja 0...
  	if(d && (*r)->numChaves == 0)
   	{
   		//libera a árvore b
   		ptAux = *r;
		*r = ptAux->filhos[0];
    	free(ptAux);
  	}
}

/*******************\
| MENU DE INTERFACE |
|************************************************************************\
| Este menu possibilita a manipulação da árvore b.                       |
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
