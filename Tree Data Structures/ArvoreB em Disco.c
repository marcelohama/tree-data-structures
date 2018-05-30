/********************|
| Marcelo Tomio Hama |
|********************/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

/*************************** INÍCIO **************************************************************\
\************************* DEFINIÇÕES ************************************************************/
/************\
| DEFINIÇÕES |
|*******************************************************************\
| [M] é a ordem da árvore b; [maxChave] é o número máximo de chaves |
| de uma página; NIL é um status para retornos                      |
\*******************************************************************/
#define M 2
#define maxChave M*2
#define NIL -1

/**************************************\
| ENUMERAÇÃO PARA FEEDBACK DAS FUNÇÕES |
|*******************************************************************\
| Atribuição de valores sequênciais ao status de retorno de funções |
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
| DEFINIÇÃO DE NÓ DE ÁRVORE B (PÁGINA) |
|*******************************************************************\
| [numChaves] é o número de chaves inclusas; [chave] é o vetor das  |
| chaves; [filhos] é o vetor de ponteiros para filhos               |
\*******************************************************************/
typedef struct
{
	int numChaves;
 	int chave[maxChave];
	long filhos[maxChave+1];
} tPagina;

/**************************************\
| VARIÁVEIS GLOBAIS PARA MANIPULAMENTO |
|*******************************************************************\
| [R] é a página que ficará em memória RAM (as demais páginas serão |
| acessíveis dinamicamente via arquivo; [arquivoArvoreB] é o buffer |
| no qual será armazenado o arquivo da árvore inteira               |
\*******************************************************************/
tPagina R;
FILE *arquivoArvoreB;
long start[2],
	 raiz = NIL,
  	 listaLivre = NIL;
/***************************** FIM ***************************************************************\
\************************** DEFINIÇÕES ***********************************************************/





/**************************** INÍCIO *************************************************************\
\*************************** ARQUIVOS ************************************************************/
/*****************************\
| LENDO UMA PÁGINA DO ARQUIVO |
|*******************************************************************\
| [posicaoArquivo] é a posição a partir da qual a cabeça de leitura |
| irá começar a ler a página; [ptPg] é a página da qual será lido o |
| conteúdo                                                          |
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
| ESCREVENDO UMA PÁGINA NO ARQUIVO |
|*******************************************************************\
| [posicaoArquivo] é a posição a partir da qual será escrita a pág. |
| [ptPg] passada como parâmetro                                     |
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
| ALOCAÇÃO DE ESPAÇO PARA INSERÇÃO DE NÓ NA ÁRVORE |
|*******************************************************************\
| Esta função procura por um espaço livre no arquivo, retornando a  |
| posição na qual deverá ser feita a inserção de página             |
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





/**************************** INÍCIO *************************************************************\
\*************************** PESQUISAS ***********************************************************/
/****************************************\
| BUSCA BINÁRIA DE UMA CHAVE EM UM VETOR |
|*******************************************************************\
| busca binária de [chave] em [vetor] o qual contém [max] chaves    |
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
| PESQUISA DE CHAVES NA ÁRVORE |
|*******************************************************************\
| Esta função recebe a chave [ch] e procura-a no arquivo de árvore  |
| cujo início está em [r]                                           |
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





/**************************** INÍCIO *************************************************************\
\*************************** INSERÇÃO ************************************************************/
/******************************************\
| INSERÇÃO DE CHAVE NO ARQUIVO DA ÁRVORE B |
|*******************************************************************\
| Função auxiliar de inserção de chaves na página; [ch] é a chave a |
| ser inclusa; [pagina] é a posição do arquivo onde será incluída a |
| chave; [vetor] é um campo de chaves usado no retorno recursivo da |
| função; [arq] é a posição de arquivo usado em retorno recursivo   |
| da função                                                         |
\*******************************************************************/
status insercaoAuxiliar(int ch, long pagina, int *vetor, long *arq)
{
	//variáveis de manipulação, status de retorno, auxiliares e retorno recursivo
	status st;
 	tPagina pg, novaPagina;
	long novaPg, auxPg, *vetorFilhos;
	int posicao, i, chaveMedia, aux, *nChaves, *vetorChaves;
 	//verificação do arquivo, se este é vazio
	if(pagina == NIL)
 	{
		*arq = NIL;
		*vetor = ch;
		return incompleta;
	}
	//atribuição das variáveis de manipulação
	leituraPagina(pagina, &pg,R);
	nChaves = &pg.numChaves;
	vetorChaves = pg.chave;
 	vetorFilhos = pg.filhos;
 	//procurando pela posição de inserção
	posicao = buscaBinaria(ch, vetorChaves, *nChaves);
	//verificação de existência da chave
	if(posicao < *nChaves && ch == vetorChaves[posicao])
 		return chaveExistente;
	st = insercaoAuxiliar(ch,vetorFilhos[posicao],&chaveMedia,&novaPg);
	if(st != incompleta)
 		return st;
	//existe espaço no vetor, portanto insere-se normalmente
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
	//caso o lugar de inserção seja a última posição
 	if(posicao == maxChave)
  	{
		aux = chaveMedia;
		auxPg = novaPg;
	}
	//caso a posição de inserção seja no meio do vetor, faz os remanejamentos
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
	//atribuição das características de página e dos parâmetros de retorno
 	*vetor = vetorChaves[M];
	*nChaves = M;
 	*arq = procuraPagina();
 	novaPagina.numChaves = M;
 	//copiando metade do vetor da página recebida para o vetor da nova página
 	for(i=0;i<M-1;i++)
  	{
		novaPagina.chave[i] = vetorChaves[i+M+1];
		novaPagina.filhos[i] = vetorFilhos[i+M+1];
	}
	//atualização dos campos da nova página
 	novaPagina.filhos[M-1] = vetorFilhos[maxChave];
 	novaPagina.chave[M-1] = aux;
 	novaPagina.filhos[M] = auxPg;
 	//gravando as páginas no arquivo
 	gravacaoPagina(pagina,&pg);
 	gravacaoPagina(*arq,&novaPagina);
 	return incompleta;
}

/***************************************************************\
| CHAMADA DA FUNÇÃO DE INSERÇÃO DE CHAVE NO ARQUIVO DA ÁRVORE B |
|*******************************************************************\
| Chamada principal da função de inserção, utilizando a função de   |
| inserção [insercaoAuxiliar] para melhor manipulamento recursivo   |
| Insere-se [chave] na página [r]                                   |
\*******************************************************************/
status insereChave(int chave, tPagina *r)
{
	//variáveis para manipulação
	long novaPg,auxPg;
 	int chaveMedia;
 	//inserção da chave
 	status codigo = insercaoAuxiliar(chave,raiz,&chaveMedia,&novaPg);
 	//verificação de existência
 	if(codigo == chaveExistente)
 	{
		printf("\n=== CHAVE %d JA EXISTENTE ===",chave);
		getch();
	}
	//caso a árvore ainda não exista
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
\*************************** INSERÇÃO ************************************************************/





/*************************** INÍCIO **************************************************************\
\************************* MANIPULAÇÃO ***********************************************************/
/************************************\
| FUNÇÃO AUXILIAR DE LIBERAÇÃO DE NÓ |
|*******************************************************************\
| Seta a posição [posicao] como livre para novas inserções          |
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
| FUNÇÃO DE LEITURA DE ARQUIVO A PARTIR DO INÍCIO |
|*******************************************************************\
| Seta a cabeça de leitura no início do arquivo [arq] e inicia a    |
| leitura colocando em [descritor] as características da árvore     |
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
| FUNÇÃO DE ESCRITA EM ARQUIVO A PARTIR DO INÍCIO |
|*******************************************************************\
| Seta a cabeça de leitura no início do arquivo [arq] e inicia a    |
| escrita colocando em [descritor] as características da árvore     |
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
\************************* MANIPULAÇÃO ***********************************************************/





/**************************** INÍCIO *************************************************************\
\*************************** EXCLUSÃO ************************************************************/
/*****************************************\
| REMOÇÃO DE CHAVE NO ARQUIVO DA ÁRVORE B |
|*******************************************************************\
| Função auxiliar de remoção de chaves na página; [ch] é a chave a  |
| ser excluída;                                                     |
\*******************************************************************/
status remocaoAuxiliar(int chave, long pagina)
{
	//variáveis de manipulação, auxiliares e de cópia
	int i, posicao,
 		*vetor, *nChaves, *item,
   		*nChavesEsq, *nChavesDir, *chaveEsq, *chaveDir,
		nChavesAux, *addr, emprestaEsq=0;
	//status de retorno
 	status st;
 	//longos para guardar posições no arquivo
 	long *vetorFilho, esq, dir,
  		 *filhoEsq, *filhoDir, posicaoAux1, posicaoAux2;
 	//variáveis de página para manipulação, cópias e/ou fusões
 	tPagina pg, pgAux1, pgAux2, pgEsq, pgDir;
 	//caso a página ou o nó não existam
 	if(pagina == NIL)
  		return inexistente;
	//colocando o conteúdo de [pagina] em pg
 	leituraPagina(pagina,&pg,R);
	//atribuindo os valores às variáveis auxiliares
 	nChaves = &pg.numChaves;
 	vetor = pg.chave;
 	vetorFilho = pg.filhos;
 	//procurando pela posição da chave
 	posicao = buscaBinaria(chave,vetor,*nChaves);
 	//caso [pagina] seja um nó folha...
 	if(vetorFilho[0] == NIL)
 	{
 		//se as posições forem incompatíveis, chave não existe
		if(posicao == *nChaves || chave < vetor[posicao])
  			return inexistente;
		//procura da chave e remanejamento em vetor auxiliar
       	for(i=posicao+1;i<*nChaves;i++)
       	{
	  		vetor[i-1] = vetor[i];
	  		vetorFilho[i] = vetorFilho[i+1];
       	}
       	//diminuição do número de chaves, gravação e retorno
       	--*nChaves;
       	gravacaoPagina(pagina,&pg);
       	return *nChaves >= (pagina == raiz ? 1 : M) ? sucesso : underflow;
	}
	//copiando os campos em variáeis auxiliares
	item = vetor + posicao;
 	esq = vetorFilho[posicao];
	leituraPagina(esq,&pgAux1,R);
 	nChavesEsq = &pgAux1.numChaves;
 	//chave encontrada, iniciando a remoção...
 	if(posicao < *nChaves && chave == *item)
 	{
 		//atribuições de valores
  		posicaoAux1 = vetorFilho[posicao];
   		leituraPagina(posicaoAux1,&pgAux1,R);
   		nChavesAux = pgAux1.numChaves;
   		//remanejamento em auxiliares e procura da posição
       	while(posicaoAux2 = pgAux1.filhos[nChavesAux],posicaoAux2 != NIL)
        {
	  		posicaoAux1 = posicaoAux2;
	  		leituraPagina(posicaoAux1,&pgAux1,R);
	  		nChavesAux = pgAux1.numChaves;
		}
		//atribuição das auxiliares de retorno
       	addr = pgAux1.chave + nChavesAux - 1;
       	*item = *addr;
       	*addr = chave;
       	//gravação da página
       	gravacaoPagina(pagina,&pg);
       	gravacaoPagina(posicaoAux1,&pgAux1);
	}
	//recurssão da função por ser uma página não-folha e verificação de underflow
 	st = remocaoAuxiliar(chave,esq);
 	if(st != underflow)
  		return st;
 	//em caso de underflow...
 	if(posicao < *nChaves)
  		leituraPagina(vetorFilho[posicao+1],&pgDir,R);
	//verifica se o irmão direito pode emprestar
 	if(posicao == *nChaves || pgDir.numChaves == M)
  	{
  		//se a posição for compatível...
		if(posicao > 0)
 		{
 			//seta as variáveis para empréstimo
 			leituraPagina(vetorFilho[posicao-1],&pgEsq,R);
	 		if(posicao == *nChaves || pgEsq.numChaves > M)
    			emprestaEsq = 1;
		}
	}
	//caso [emprestaEsq] seja verdadeira...
 	if(emprestaEsq)
  	{
  		//fazendo o empréstimo do irmão esquerdo da página
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
 		//faz o empréstimo do irmão direito
   		dir = vetorFilho[posicao+1];
       	leituraPagina(esq,&pgAux1,R);
       	pgAux2 = pgDir;
	}
	//atribuição das variáveis auxiliares para manipulamento da fusão
 	nChavesDir = &pgAux2.numChaves;
 	chaveEsq = pgAux1.chave;
 	chaveDir = pgAux2.chave;
 	filhoEsq = pgAux1.filhos;
 	filhoDir = pgAux2.filhos;
 	//realiza remanejamento no irmão esquerdo
 	if(emprestaEsq)
  	{
  		//remanejamento
   		filhoDir[*nChavesDir + 1] = filhoDir[*nChavesDir];
       	for(i=*nChavesDir;i>0;i--)
		{
	 		chaveDir[i] = chaveDir[i-1];
	 		filhoDir[i] = filhoDir[i-1];
       	}
       	//cópia dos campos e valores
       	++*nChavesDir;
       	chaveDir[0] = *item;
       	filhoDir[0] = filhoEsq[*nChavesEsq];
       	*item = chaveEsq[*nChavesEsq - 1];
       	//caso a página fique certa, grava-a
       	if(--*nChavesEsq >= M)
        {
			gravacaoPagina(pagina,&pg);
	 		gravacaoPagina(esq,&pgAux1);
	 		gravacaoPagina(dir,&pgAux2);
	 		return sucesso;
       	}
 	}
 	//realiza remanejamento no irmão direito
 	else if(*nChavesDir > M)
  	{
  		//cópia dos campos e valores
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
       	//gravação da página
       	filhoDir[*nChavesDir] = filhoDir[*nChavesDir + 1];
       	gravacaoPagina (pagina, &pg);
       	gravacaoPagina (esq, &pgAux1);
       	gravacaoPagina (dir, &pgAux2);
       	return sucesso;
	}
	//iniciando a fusão de páginas...
 	chaveEsq[M-1] = *item;
 	filhoEsq[M] = filhoDir[0];
 	//cópia do campo das chaves e filhos
 	for(i=0;i<M;i++)
  	{
		chaveEsq[M+i] = chaveDir[i];
       	filhoEsq[M+i+1] = filhoDir[i+1];
	}
	//atribuição do número de chaves à página criada e liberação de [dir]
 	*nChavesEsq = maxChave;
 	liberaEspaco(dir);
 	//cópia para vetor de auxílio para uso no caso de recurssão
 	for(i=posicao+1;i<*nChaves;i++)
  	{
   		vetor[i-1] = vetor[i];
       	vetorFilho[i] = vetorFilho[i+1];
 	}
 	//atribuição do número de chaves
 	--*nChaves;
 	//gravação das páginas
 	gravacaoPagina(pagina,&pg);
 	gravacaoPagina(esq,&pgAux1);
 	//retorno de status
 	return *nChaves >= (pagina == raiz ? 1 : M) ? sucesso : underflow;
}

/**************************************************************\
| CHAMADA DA FUNÇÃO DE REMOÇÃO DE CHAVE NO ARQUIVO DA ÁRVORE B |
|*******************************************************************\
| Chamada principal da função de exclusão, utilizando a função de   |
| exclusão [remocaoAuxiliar] para melhor manipulamento recursivo    |
| Remove-se [chave] na página [r]                                   |
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
\*************************** INSERÇÃO ************************************************************/





/**************************** INÍCIO *************************************************************\
\************************** IMPRESSÃO ************************************************************/
/**********************************************\
| IMPRESSÃO ILUSTRATIVA DA ÁRVORE B DO ARQUIVO |
|*******************************************************************\
| Recebe uma posição [posicao] e imprime a árvore em tela a partir  |
| desta posição                                                     |
\*******************************************************************/
void impressaoArvoreB(long pos)
{
	static int posicao = 0;
 	int i, *vetor, nChaves;
 	tPagina pg;
 	//inicia a impressão caso a árvore exista
	if(pos != NIL)
 	{
		posicao += 6;
		//lendo a página do arquivo e atribuindo as auxiliares
       	leituraPagina(pos,&pg,R);
       	vetor = pg.chave;
       	nChaves = pg.numChaves;
       	//imprimindo recurcivamente a árvore b
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
\************************** IMPRESSÃO ************************************************************/





/***********************\
| MAIN E MENU PRINCIPAL |
|*******************************************************************\
| Menu principal e gerenciamento das funções da árvore b            |
\*******************************************************************/
int main()
{
	//variáveis de chave, opção de menu e nome de arquivo
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
 	//laço principal
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
