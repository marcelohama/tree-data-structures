/********************|
| Marcelo Tomio Hama |
|********************/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

/*************************** IN�CIO **************************************************************\
\************************* DEFINI��ES ************************************************************/
/************\
| DEFINI��ES |
|*******************************************************************\
| [M] � a ordem da �rvore b; [maxChave] � o n�mero m�ximo de chaves |
| de uma p�gina; NIL � um status para retornos                      |
\*******************************************************************/
#define M 2
#define maxChave M*2
#define NIL -1

/**************************************\
| ENUMERA��O PARA FEEDBACK DAS FUN��ES |
|*******************************************************************\
| Atribui��o de valores sequ�nciais ao status de retorno de fun��es |
\*******************************************************************/
typedef enum
{
	incompleta,
 	sucesso,
  	chaveExistente,
   	underflow,
    inexistente
} status;

/**************************************\
| DEFINI��O DE N� DE �RVORE B (P�GINA) |
|*******************************************************************\
| [numChaves] � o n�mero de chaves inclusas; [chave] � o vetor das  |
| chaves; [filhos] � o vetor de ponteiros para filhos               |
\*******************************************************************/
typedef struct
{
	int numChaves;
 	int chave[maxChave];
	long filhos[maxChave+1];
} tPagina;

/**************************************\
| VARI�VEIS GLOBAIS PARA MANIPULAMENTO |
|*******************************************************************\
| [R] � a p�gina que ficar� em mem�ria RAM (as demais p�ginas ser�o |
| acess�veis dinamicamente via arquivo; [arquivoArvoreB] � o buffer |
| no qual ser� armazenado o arquivo da �rvore inteira               |
\*******************************************************************/
tPagina R;
FILE *arquivoArvoreB;
long start[2],
	 raiz = NIL,
  	 listaLivre = NIL;
/***************************** FIM ***************************************************************\
\************************** DEFINI��ES ***********************************************************/





/**************************** IN�CIO *************************************************************\
\*************************** ARQUIVOS ************************************************************/
/*****************************\
| LENDO UMA P�GINA DO ARQUIVO |
|*******************************************************************\
| [posicaoArquivo] � a posi��o a partir da qual a cabe�a de leitura |
| ir� come�ar a ler a p�gina; [ptPg] � a p�gina da qual ser� lido o |
| conte�do                                                          |
\*******************************************************************/
void leituraPagina(long posicaoArquivo, tPagina *ptPg, tPagina r)
{
	if(posicaoArquivo == raiz)
 	{
		*ptPg = r;
		return;
	}
	if(fseek(arquivoArvoreB,posicaoArquivo,SEEK_SET))
	{
		printf("\n=== ERRO 'FSEEK' NA LEITURA DE ARQUIVO ===");
		getch();
		exit(1);
	}
 	if(fread(ptPg,sizeof(tPagina),1,arquivoArvoreB) == 0)
 	{
 		printf("\n=== ERRO 'FREAD' NA LEITURA DE ARQUIVO ===");
 		getch();
		exit(1);
 	}
}

/**********************************\
| ESCREVENDO UMA P�GINA NO ARQUIVO |
|*******************************************************************\
| [posicaoArquivo] � a posi��o a partir da qual ser� escrita a p�g. |
| [ptPg] passada como par�metro                                     |
\*******************************************************************/
void gravacaoPagina(long posicaoArquivo, tPagina *ptPg)
{
	if(posicaoArquivo == raiz) R = *ptPg;
 	if(fseek(arquivoArvoreB,posicaoArquivo,SEEK_SET))
	{
		printf("\n=== ERRO 'FSEEK' NA GRAVACAO DE ARQUIVO ===");
		getch();
		exit(1);
	}
 	if(fwrite(ptPg,sizeof(tPagina),1,arquivoArvoreB) == 0)
	{
		printf("\n=== ERRO 'FWRITE' NA GRAVACAO DE ARQUIVO ===");
		getch();
		exit(1);
	}
}

/**************************************************\
| ALOCA��O DE ESPA�O PARA INSER��O DE N� NA �RVORE |
|*******************************************************************\
| Esta fun��o procura por um espa�o livre no arquivo, retornando a  |
| posi��o na qual dever� ser feita a inser��o de p�gina             |
\*******************************************************************/
long procuraPagina(void)
{
	long posicaoArquivo;
	tPagina pt;
	if(listaLivre == NIL)
	{
 		if(fseek(arquivoArvoreB,0L,SEEK_END))
		{
			printf("\n=== ERRO 'FSEEK' NA PROCURA DE PAGINA ===");
			getch();
			exit(1);
		}
		posicaoArquivo = ftell(arquivoArvoreB);
		gravacaoPagina(posicaoArquivo,&pt);
	}
	else
 	{
		posicaoArquivo = listaLivre;
		leituraPagina(posicaoArquivo,&pt,R);
		listaLivre = pt.filhos[0];
	}
 	return posicaoArquivo;
}
/***************************** FIM ***************************************************************\
\*************************** ARQUIVOS ************************************************************/





/**************************** IN�CIO *************************************************************\
\*************************** PESQUISAS ***********************************************************/
/****************************************\
| BUSCA BIN�RIA DE UMA CHAVE EM UM VETOR |
|*******************************************************************\
| busca bin�ria de [chave] em [vetor] o qual cont�m [max] chaves    |
\*******************************************************************/
int buscaBinaria(int chave, int *vetor, int max)
{
	int meio,inicio,fim;
 	if(chave <= vetor[0]) return 0;
 	if(chave > vetor[max-1]) return max;
 	inicio = 0; fim = max-1;
 	while(fim-inicio > 1)
  	{
		meio = (inicio+fim)/2;
		if (chave <= vetor[meio]) fim = meio;
		else inicio = meio;
 	}
 	return fim;
}

/******************************\
| PESQUISA DE CHAVES NA �RVORE |
|*******************************************************************\
| Esta fun��o recebe a chave [ch] e procura-a no arquivo de �rvore  |
| cujo in�cio est� em [r]                                           |
\*******************************************************************/
status pesquisaChave(int ch, long r)
{
	int i,nChaves,*vetorChave;
 	tPagina pg;
 	long posicao = r;
 	while(posicao != NIL)
  	{
		leituraPagina(posicao,&pg,R);
		vetorChave = pg.chave;
		nChaves = pg.numChaves;
		i = buscaBinaria (ch, vetorChave, nChaves);
		if (i < nChaves && ch == vetorChave[i])
  		{
	  		printf("\n=== CHAVE %d ENCONTRADA ===",ch);
	  		return sucesso;
		}
		posicao = pg.filhos[i];
	}
 	return inexistente;
}
/***************************** FIM ***************************************************************\
\*************************** PESQUISAS ***********************************************************/





/**************************** IN�CIO *************************************************************\
\*************************** INSER��O ************************************************************/
/******************************************\
| INSER��O DE CHAVE NO ARQUIVO DA �RVORE B |
|*******************************************************************\
| Fun��o auxiliar de inser��o de chaves na p�gina; [ch] � a chave a |
| ser inclusa; [pagina] � a posi��o do arquivo onde ser� inclu�da a |
| chave; [vetor] � um campo de chaves usado no retorno recursivo da |
| fun��o; [arq] � a posi��o de arquivo usado em retorno recursivo   |
| da fun��o                                                         |
\*******************************************************************/
status insercaoAuxiliar(int ch, long pagina, int *vetor, long *arq)
{
	//vari�veis de manipula��o, status de retorno, auxiliares e retorno recursivo
	status st;
 	tPagina pg, novaPagina;
	long novaPg, auxPg, *vetorFilhos;
	int posicao, i, chaveMedia, aux, *nChaves, *vetorChaves;
 	//verifica��o do arquivo, se este � vazio
	if(pagina == NIL)
 	{
		*arq = NIL;
		*vetor = ch;
		return incompleta;
	}
	//atribui��o das vari�veis de manipula��o
	leituraPagina(pagina, &pg,R);
	nChaves = &pg.numChaves;
	vetorChaves = pg.chave;
 	vetorFilhos = pg.filhos;
 	//procurando pela posi��o de inser��o
	posicao = buscaBinaria(ch, vetorChaves, *nChaves);
	//verifica��o de exist�ncia da chave
	if(posicao < *nChaves && ch == vetorChaves[posicao])
 		return chaveExistente;
	st = insercaoAuxiliar(ch,vetorFilhos[posicao],&chaveMedia,&novaPg);
	if(st != incompleta)
 		return st;
	//existe espa�o no vetor, portanto insere-se normalmente
	if(*nChaves < maxChave)
 	{
		posicao = buscaBinaria(chaveMedia,vetorChaves,*nChaves);
		for(i=*nChaves;i>posicao;i--)
 		{
  			vetorChaves[i] = vetorChaves[i-1];
			vetorFilhos[i+1] = vetorFilhos[i];
		}
		vetorChaves[posicao] = chaveMedia;
		vetorFilhos[posicao+1] = novaPg;
		++*nChaves;
		gravacaoPagina (pagina, &pg);
		return sucesso;
	}
	//caso o lugar de inser��o seja a �ltima posi��o
 	if(posicao == maxChave)
  	{
		aux = chaveMedia;
		auxPg = novaPg;
	}
	//caso a posi��o de inser��o seja no meio do vetor, faz os remanejamentos
 	else
  	{
		aux = vetorChaves[maxChave-1];
		auxPg = vetorFilhos[maxChave];
		for(i=maxChave-1;i>posicao;i--)
  		{
	   		vetorChaves[i] = vetorChaves[i-1];
	   		vetorFilhos[i+1] = vetorFilhos[i];
		}
		vetorChaves[posicao] = chaveMedia;
		vetorFilhos[posicao+1] = novaPg;
	}
	//atribui��o das caracter�sticas de p�gina e dos par�metros de retorno
 	*vetor = vetorChaves[M];
	*nChaves = M;
 	*arq = procuraPagina();
 	novaPagina.numChaves = M;
 	//copiando metade do vetor da p�gina recebida para o vetor da nova p�gina
 	for(i=0;i<M-1;i++)
  	{
		novaPagina.chave[i] = vetorChaves[i+M+1];
		novaPagina.filhos[i] = vetorFilhos[i+M+1];
	}
	//atualiza��o dos campos da nova p�gina
 	novaPagina.filhos[M-1] = vetorFilhos[maxChave];
 	novaPagina.chave[M-1] = aux;
 	novaPagina.filhos[M] = auxPg;
 	//gravando as p�ginas no arquivo
 	gravacaoPagina(pagina,&pg);
 	gravacaoPagina(*arq,&novaPagina);
 	return incompleta;
}

/***************************************************************\
| CHAMADA DA FUN��O DE INSER��O DE CHAVE NO ARQUIVO DA �RVORE B |
|*******************************************************************\
| Chamada principal da fun��o de inser��o, utilizando a fun��o de   |
| inser��o [insercaoAuxiliar] para melhor manipulamento recursivo   |
| Insere-se [chave] na p�gina [r]                                   |
\*******************************************************************/
status insereChave(int chave, tPagina *r)
{
	//vari�veis para manipula��o
	long novaPg,auxPg;
 	int chaveMedia;
 	//inser��o da chave
 	status codigo = insercaoAuxiliar(chave,raiz,&chaveMedia,&novaPg);
 	//verifica��o de exist�ncia
 	if(codigo == chaveExistente)
 	{
		printf("\n=== CHAVE %d JA EXISTENTE ===",chave);
		getch();
	}
	//caso a �rvore ainda n�o exista
 	else if(codigo == incompleta)
  	{
   		auxPg = procuraPagina();
        r->numChaves = 1;
        r->chave[0] = chaveMedia;
        r->filhos[0] = raiz;
        r->filhos[1] = novaPg;
        raiz = auxPg;
        gravacaoPagina(auxPg,&R);
        codigo = sucesso;
	}
 	return codigo;
}
/***************************** FIM ***************************************************************\
\*************************** INSER��O ************************************************************/





/*************************** IN�CIO **************************************************************\
\************************* MANIPULA��O ***********************************************************/
/************************************\
| FUN��O AUXILIAR DE LIBERA��O DE N� |
|*******************************************************************\
| Seta a posi��o [posicao] como livre para novas inser��es          |
\*******************************************************************/
void liberaEspaco(long posicao)
{
	tPagina pg;
 	leituraPagina(posicao,&pg,R);
 	pg.filhos[0] = listaLivre;
 	listaLivre = posicao;
 	gravacaoPagina(posicao,&pg);
}

/*************************************************\
| FUN��O DE LEITURA DE ARQUIVO A PARTIR DO IN�CIO |
|*******************************************************************\
| Seta a cabe�a de leitura no in�cio do arquivo [arq] e inicia a    |
| leitura colocando em [descritor] as caracter�sticas da �rvore     |
\*******************************************************************/
void leituraDescritor(FILE *arq, long *descritor)
{
	if(fseek(arq,0L,SEEK_SET))
	{
		printf("\n=== ERRO 'FSEEK' NA LEITURA ===");
		getch();
		exit(1);
	}
	if(fread(descritor,sizeof(long),2,arq) == 0)
	{
		printf("\n=== ERRO 'FREAD' NA LEITURA ===");
		getch();
		exit(1);
	}
 	leituraPagina(descritor[0],&R,R);
 	raiz = descritor[0];
 	listaLivre = descritor[1];
}

/*************************************************\
| FUN��O DE ESCRITA EM ARQUIVO A PARTIR DO IN�CIO |
|*******************************************************************\
| Seta a cabe�a de leitura no in�cio do arquivo [arq] e inicia a    |
| escrita colocando em [descritor] as caracter�sticas da �rvore     |
\*******************************************************************/
void gravacaoDescritor(FILE *arq, long *descritor)
{
 	descritor[0] = raiz;
 	descritor[1] = listaLivre;
 	if(fseek(arq,0L,SEEK_SET))
 	{
		printf("\n=== ERRO 'FSEEK' NA GRAVACAO ===");
		getch();
		exit(1);
	}
 	if(fwrite(start,sizeof(long),2,arq) == 0)
	{
		printf("\n=== ERRO 'FREAD' NA GRAVACAO ===");
		getch();
		exit(1);
	}
 	if(raiz != NIL) gravacaoPagina(raiz,&R);
}
/***************************** FIM ***************************************************************\
\************************* MANIPULA��O ***********************************************************/





/**************************** IN�CIO *************************************************************\
\*************************** EXCLUS�O ************************************************************/
/*****************************************\
| REMO��O DE CHAVE NO ARQUIVO DA �RVORE B |
|*******************************************************************\
| Fun��o auxiliar de remo��o de chaves na p�gina; [ch] � a chave a  |
| ser exclu�da;                                                     |
\*******************************************************************/
status remocaoAuxiliar(int chave, long pagina)
{
	//vari�veis de manipula��o, auxiliares e de c�pia
	int i, posicao,
 		*vetor, *nChaves, *item,
   		*nChavesEsq, *nChavesDir, *chaveEsq, *chaveDir,
		nChavesAux, *addr, emprestaEsq=0;
	//status de retorno
 	status st;
 	//longos para guardar posi��es no arquivo
 	long *vetorFilho, esq, dir,
  		 *filhoEsq, *filhoDir, posicaoAux1, posicaoAux2;
 	//vari�veis de p�gina para manipula��o, c�pias e/ou fus�es
 	tPagina pg, pgAux1, pgAux2, pgEsq, pgDir;
 	//caso a p�gina ou o n� n�o existam
 	if(pagina == NIL)
  		return inexistente;
	//colocando o conte�do de [pagina] em pg
 	leituraPagina(pagina,&pg,R);
	//atribuindo os valores �s vari�veis auxiliares
 	nChaves = &pg.numChaves;
 	vetor = pg.chave;
 	vetorFilho = pg.filhos;
 	//procurando pela posi��o da chave
 	posicao = buscaBinaria(chave,vetor,*nChaves);
 	//caso [pagina] seja um n� folha...
 	if(vetorFilho[0] == NIL)
 	{
 		//se as posi��es forem incompat�veis, chave n�o existe
		if(posicao == *nChaves || chave < vetor[posicao])
  			return inexistente;
		//procura da chave e remanejamento em vetor auxiliar
       	for(i=posicao+1;i<*nChaves;i++)
       	{
	  		vetor[i-1] = vetor[i];
	  		vetorFilho[i] = vetorFilho[i+1];
       	}
       	//diminui��o do n�mero de chaves, grava��o e retorno
       	--*nChaves;
       	gravacaoPagina(pagina,&pg);
       	return *nChaves >= (pagina == raiz ? 1 : M) ? sucesso : underflow;
	}
	//copiando os campos em vari�eis auxiliares
	item = vetor + posicao;
 	esq = vetorFilho[posicao];
	leituraPagina(esq,&pgAux1,R);
 	nChavesEsq = &pgAux1.numChaves;
 	//chave encontrada, iniciando a remo��o...
 	if(posicao < *nChaves && chave == *item)
 	{
 		//atribui��es de valores
  		posicaoAux1 = vetorFilho[posicao];
   		leituraPagina(posicaoAux1,&pgAux1,R);
   		nChavesAux = pgAux1.numChaves;
   		//remanejamento em auxiliares e procura da posi��o
       	while(posicaoAux2 = pgAux1.filhos[nChavesAux],posicaoAux2 != NIL)
        {
	  		posicaoAux1 = posicaoAux2;
	  		leituraPagina(posicaoAux1,&pgAux1,R);
	  		nChavesAux = pgAux1.numChaves;
		}
		//atribui��o das auxiliares de retorno
       	addr = pgAux1.chave + nChavesAux - 1;
       	*item = *addr;
       	*addr = chave;
       	//grava��o da p�gina
       	gravacaoPagina(pagina,&pg);
       	gravacaoPagina(posicaoAux1,&pgAux1);
	}
	//recurss�o da fun��o por ser uma p�gina n�o-folha e verifica��o de underflow
 	st = remocaoAuxiliar(chave,esq);
 	if(st != underflow)
  		return st;
 	//em caso de underflow...
 	if(posicao < *nChaves)
  		leituraPagina(vetorFilho[posicao+1],&pgDir,R);
	//verifica se o irm�o direito pode emprestar
 	if(posicao == *nChaves || pgDir.numChaves == M)
  	{
  		//se a posi��o for compat�vel...
		if(posicao > 0)
 		{
 			//seta as vari�veis para empr�stimo
 			leituraPagina(vetorFilho[posicao-1],&pgEsq,R);
	 		if(posicao == *nChaves || pgEsq.numChaves > M)
    			emprestaEsq = 1;
		}
	}
	//caso [emprestaEsq] seja verdadeira...
 	if(emprestaEsq)
  	{
  		//fazendo o empr�stimo do irm�o esquerdo da p�gina
   		item = vetor + posicao - 1;
       	esq = vetorFilho[posicao - 1];
       	dir = vetorFilho[posicao];
       	pgAux1 = pgEsq;
       	leituraPagina(dir,&pgAux2,R);
       	nChavesEsq = &pgAux1.numChaves;
	}
	//caso [emprestaEsq] seja falsa...
	else
 	{
 		//faz o empr�stimo do irm�o direito
   		dir = vetorFilho[posicao+1];
       	leituraPagina(esq,&pgAux1,R);
       	pgAux2 = pgDir;
	}
	//atribui��o das vari�veis auxiliares para manipulamento da fus�o
 	nChavesDir = &pgAux2.numChaves;
 	chaveEsq = pgAux1.chave;
 	chaveDir = pgAux2.chave;
 	filhoEsq = pgAux1.filhos;
 	filhoDir = pgAux2.filhos;
 	//realiza remanejamento no irm�o esquerdo
 	if(emprestaEsq)
  	{
  		//remanejamento
   		filhoDir[*nChavesDir + 1] = filhoDir[*nChavesDir];
       	for(i=*nChavesDir;i>0;i--)
		{
	 		chaveDir[i] = chaveDir[i-1];
	 		filhoDir[i] = filhoDir[i-1];
       	}
       	//c�pia dos campos e valores
       	++*nChavesDir;
       	chaveDir[0] = *item;
       	filhoDir[0] = filhoEsq[*nChavesEsq];
       	*item = chaveEsq[*nChavesEsq - 1];
       	//caso a p�gina fique certa, grava-a
       	if(--*nChavesEsq >= M)
        {
			gravacaoPagina(pagina,&pg);
	 		gravacaoPagina(esq,&pgAux1);
	 		gravacaoPagina(dir,&pgAux2);
	 		return sucesso;
       	}
 	}
 	//realiza remanejamento no irm�o direito
 	else if(*nChavesDir > M)
  	{
  		//c�pia dos campos e valores
		chaveEsq[M-1] = *item;
       	filhoEsq[M] = filhoDir[0];
       	*item = chaveDir[0];
       	++*nChavesEsq;
       	--*nChavesDir;
       	//remanejamento
       	for(i=0;i<*nChavesDir;i++)
        {
	 		filhoDir[i] = filhoDir[i+1];
	 		chaveDir[i] = chaveDir[i+1];
       	}
       	//grava��o da p�gina
       	filhoDir[*nChavesDir] = filhoDir[*nChavesDir + 1];
       	gravacaoPagina (pagina, &pg);
       	gravacaoPagina (esq, &pgAux1);
       	gravacaoPagina (dir, &pgAux2);
       	return sucesso;
	}
	//iniciando a fus�o de p�ginas...
 	chaveEsq[M-1] = *item;
 	filhoEsq[M] = filhoDir[0];
 	//c�pia do campo das chaves e filhos
 	for(i=0;i<M;i++)
  	{
		chaveEsq[M+i] = chaveDir[i];
       	filhoEsq[M+i+1] = filhoDir[i+1];
	}
	//atribui��o do n�mero de chaves � p�gina criada e libera��o de [dir]
 	*nChavesEsq = maxChave;
 	liberaEspaco(dir);
 	//c�pia para vetor de aux�lio para uso no caso de recurss�o
 	for(i=posicao+1;i<*nChaves;i++)
  	{
   		vetor[i-1] = vetor[i];
       	vetorFilho[i] = vetorFilho[i+1];
 	}
 	//atribui��o do n�mero de chaves
 	--*nChaves;
 	//grava��o das p�ginas
 	gravacaoPagina(pagina,&pg);
 	gravacaoPagina(esq,&pgAux1);
 	//retorno de status
 	return *nChaves >= (pagina == raiz ? 1 : M) ? sucesso : underflow;
}

/**************************************************************\
| CHAMADA DA FUN��O DE REMO��O DE CHAVE NO ARQUIVO DA �RVORE B |
|*******************************************************************\
| Chamada principal da fun��o de exclus�o, utilizando a fun��o de   |
| exclus�o [remocaoAuxiliar] para melhor manipulamento recursivo    |
| Remove-se [chave] na p�gina [r]                                   |
\*******************************************************************/
status removeChave(int chave, long r)
{
	long novaRaiz;
 	status codigo = remocaoAuxiliar(chave,r);
 	//cuida do caso de underflow
 	if(codigo == underflow)
  	{
   		novaRaiz = R.filhos[0];
       	liberaEspaco(r);
       	if(novaRaiz != NIL)
        	leituraPagina(novaRaiz,&R,R);
       	r = novaRaiz;
       	codigo = sucesso;
	}
	return codigo;
}
/***************************** FIM ***************************************************************\
\*************************** INSER��O ************************************************************/





/**************************** IN�CIO *************************************************************\
\************************** IMPRESS�O ************************************************************/
/**********************************************\
| IMPRESS�O ILUSTRATIVA DA �RVORE B DO ARQUIVO |
|*******************************************************************\
| Recebe uma posi��o [posicao] e imprime a �rvore em tela a partir  |
| desta posi��o                                                     |
\*******************************************************************/
void impressaoArvoreB(long pos)
{
	static int posicao = 0;
 	int i, *vetor, nChaves;
 	tPagina pg;
 	//inicia a impress�o caso a �rvore exista
	if(pos != NIL)
 	{
		posicao += 6;
		//lendo a p�gina do arquivo e atribuindo as auxiliares
       	leituraPagina(pos,&pg,R);
       	vetor = pg.chave;
       	nChaves = pg.numChaves;
       	//imprimindo recurcivamente a �rvore b
       	printf("%*s",posicao,"");
       	for(i=0;i<nChaves;i++)
	 		printf(" %3d",vetor[i]);
       	puts("");
       	for(i=0;i<=nChaves;i++)
	 		impressaoArvoreB(pg.filhos[i]);
       	posicao -= 6;
	}
}
/***************************** FIM ***************************************************************\
\************************** IMPRESS�O ************************************************************/





/***********************\
| MAIN E MENU PRINCIPAL |
|*******************************************************************\
| Menu principal e gerenciamento das fun��es da �rvore b            |
\*******************************************************************/
int main()
{
	//vari�veis de chave, op��o de menu e nome de arquivo
	int chave;
 	char opcao, nomeArvoreB[31];
 	//carregando o nome de arquivo recebido
 	printf("NOME DO ARQUIVO DA ARVORE:");
 	scanf("%30s",nomeArvoreB);
 	arquivoArvoreB = fopen(nomeArvoreB,"r+b");
 	if(arquivoArvoreB == NULL)
 	{
		arquivoArvoreB = fopen(nomeArvoreB,"w+b");
       	gravacaoDescritor(arquivoArvoreB, start);
 	}
 	else leituraDescritor(arquivoArvoreB,start);
 	//la�o principal
 	for(;;)
  	{
  		system("cls");
   		printf("<1> INSERCAO\n<2> REMOCAO\n<3> IMPRESSAO\n<4> BUSCA\n<5> SAIR");
   	   	opcao=getch();
       	switch(opcao)
		{
			case 49: printf("\nChave a ser inserida --> ");
					 scanf("%d",&chave);
					 insereChave(chave,&R);
					 break;
					 
			case 50: printf("\nChave a ser removida --> ");
					 scanf("%d",&chave);
					 if(removeChave(chave,raiz) != sucesso)
					 {
					     printf("\n=== CHAVE %d NAO ENCONTRADA ===",chave);
					     getch();
					 }
					 break;
					 
			case 51: printf("\n\n");
   					 impressaoArvoreB(raiz);
					 getch();
					 break;
					 
			case 52: printf("\nChave a ser buscada --> ");
					 scanf("%d",&chave);
   					 if(pesquisaChave(chave,raiz) == inexistente)
   					 {
 			 		     printf("\n=== CHAVE %d NAO ENCONTRADA ===",chave);
 			 		     getch();
 		     		 }
					 getch();
					 break;
					 
			case 53: gravacaoDescritor(arquivoArvoreB, start);
					 exit(0);
	 		default: ;
       	} 
	}
	return (0);
}
