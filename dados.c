#include "dados.h"
#include "binario.h"
#include "fs.h"
#include "path.h"

static int FAT[TAM_FAT];
static int iniSuperbloco;
static int iniBitmap;		
static int iniFat;		
static int iniRaiz;
static int qtadeBlocos;

static int posLivreBitmap(int inicioBusca);
static void atualizaFAT(int posicaoInicio, int novaPos);
static int mapeiaBitmapEmByte(int posBitmap);

Arquivo setStruct(char* nome, int byteInicio, int tempoCriado,
			 	   int tempoAcessado, int tempoModificado, int tamanho) 
{
	Arquivo arq;
	arq.nome = nome;
	arq.byteInicio = byteInicio;
	arq.tempoCriado = tempoCriado;
	arq.tempoModificado = tempoModificado;
	arq.tempoAcessado = tempoAcessado;
	arq.tamanho = tamanho;
	return arq;
}	

void criaSuperBloco() {
	qtadeBlocos = TAM_FS/TAM_BLOCO;
	iniSuperbloco = 0;
	iniBitmap = iniSuperbloco + TAM_BLOCO;		
	iniFat = iniBitmap + TAM_BLOCO;		
	iniRaiz = iniFat + TAM_FAT;

	Arquivo raiz;
	raiz = setStruct("/\0", iniRaiz, 0, 0, 0, -1);
	
	escreveInt(arquivo, 0		  , iniSuperbloco	  ,	 TAM_BLOCO);	// preenche com zero				  
	escreveInt(arquivo, qtadeBlocos, iniSuperbloco     ,  1);			// escreve numero de blocos totais
	escreveInt(arquivo, 1    	  , iniSuperbloco + 4 ,  1);		 	// escreve # blocos usados
	escreveInt(arquivo, 0     	  , iniSuperbloco + 8 ,  1);		 	// escreve mem. utilizada
	escreveInt(arquivo, iniBitmap  , iniSuperbloco + 12,  1);			// escreve inicio bitmap
	escreveInt(arquivo, iniFat     , iniSuperbloco + 16,  1);		 	// escreve inicio FAT
	escreveStruct(arquivo, raiz 	  , iniSuperbloco + 20,  1);		// escreve entrada da raiz
}

void criaBitMap() {
	escreveChar(arquivo, 1, iniBitmap, 1); 							// bloco da raiz
	escreveChar(arquivo, 0, iniBitmap+1, iniFat-iniBitmap-1);		// inicializa mapa de bits com 0
}

void criaFAT() {
	escreveInt(arquivo, -1, iniFat, 1);				// inicializa o FAT[0] com -1
	escreveInt(arquivo, -2, iniFat+4, TAM_FAT-1);	// inicializa o FAT com -2
}

void carregaFATnaMemoria(int inicioBytes, int qtdeBlocosDados) {
	int i;

	if (arquivo) {
		int val;

		fseek(arquivo, inicioBytes, SEEK_SET);

		for (i = 0; i < qtdeBlocosDados; i++) {
			if (fread(&val, sizeof(char), 4, arquivo) == 4) {
				FAT[i] = val;
			}
		}
	}
}

void regravaFATnoDisco(int inicioBytes, int nbytes) {
	if (arquivo) {
		int i;

		fseek(arquivo, inicioBytes, SEEK_SET);

		for (i = 0; i < nbytes; i++)
			if (fwrite (&FAT[i], sizeof(int), 1, arquivo) != 1)
				printf("Não escreveu 1 int!\n");
	}
}

void imprimeFAT(int quantidadeBlocos) {
	int i;

	for (i = 0; i < quantidadeBlocos; i++) {
		printf("%d ", FAT[i]);
	}
}

void criaRaiz() {
	int numEntradas = (TAM_BLOCO-4)/(sizeof(Arquivo));

	escreveInt(arquivo, numEntradas, iniRaiz, 1);
	escreveChar(arquivo, '\0', iniRaiz+4, TAM_BLOCO-4);
}

/**************** MKDIR ****************/ 

void mkdir(char* path) {
	int posBitmapDir, byteInicioDir;

	posBitmapDir = alocaEntradaArquivo(matrizPath[0]);
	byteInicioDir = mapeiaBitmapEmByte(posBitmapDir);
	alocaDiretorio(byteInicioDir);
}

void alocaDiretorio(int byteInicio) {
	int numEntradas = (TAM_BLOCO-4)/(sizeof(Arquivo));

	escreveInt(arquivo, numEntradas, byteInicio, 1);
	escreveChar(arquivo, '\0', byteInicio+4, TAM_BLOCO-4);
}

static int posLivreBitmap(int inicioBusca) {
	int i;

	if(inicioBusca != -1) {
		for(i = inicioBusca; i < (iniFat - iniBitmap); i++){				
			if(leChar(arquivo, iniBitmap+i) == 0) return i;
		}
	}

	return -1;
}

int alocaEntradaArquivo(char* nomeDir) {
	int entradasLivres = leInt(arquivo, iniRaiz);
	int posPai = 0, posDir = 0;
	Arquivo novoDir;

	// aloca o pai que esta cheio e o novo diretorio
	if(entradasLivres <= 0){
		posPai = posLivreBitmap(0);
		posDir = posLivreBitmap(posPai); 
		
		if(posPai != -1 && posDir != -1) {
			escreveChar(arquivo, 1, iniBitmap + posPai, 1);
			escreveChar(arquivo, 1, iniBitmap + posDir, 1);
			atualizaFAT(iniRaiz, posPai);			//atualiza o FAT da posPai 
			FAT[posDir] = -1;
			
			novoDir = setStruct(nomeDir, mapeiaBitmapEmByte(posPai), 9, 8, 7, -1);
			escreveStruct(arquivo, novoDir, mapeiaBitmapEmByte(posPai)+4, 1);

			return posDir; 
		}
		else{
			printf("Nao foi possivel criar o diretorio %s: Memoria cheia!!!\n", nomeDir);
		} 
	} 
	
	// aloca o novo diretorio
	else {
		posDir = posLivreBitmap(0); 
	
		if(posDir != -1) {
			escreveChar(arquivo, 1, iniBitmap + posDir, 1);
			FAT[posDir] = -1;

			novoDir = setStruct(nomeDir, mapeiaBitmapEmByte(posPai), 9, 8, 7, -1);
			escreveStruct(arquivo, novoDir, mapeiaBitmapEmByte(posPai)+4, 1);
			return posDir;
		}
		else{
			printf("Nao foi possivel criar o diretorio %s: Memoria cheia!!!\n", nomeDir);
		} 
	}

	return -1;
}

static void atualizaFAT(int posicaoInicio, int novaPos) {
	int i;

	for(i = posicaoInicio; FAT[i] != -1; i = FAT[i]);

	FAT[i] = novaPos;

	FAT[novaPos] = -1;
}

static int mapeiaBitmapEmByte(int posBitmap) {
	return iniRaiz + (posBitmap*TAM_BLOCO);
}

int buscaEnderecoArquivo(int numPrimeiro, int numUltimo){
	//TODO

	return 0;
}

/**************** LS ****************/ 

// void ls(char* nome, int tempoModificado, int tamanho) {
// 	if(tamanho == -1)
// 		printf("%-20s    	%-10d    	      (DIR)\n", nome, tempoModificado);
// 	else
// 		printf("%-20s    	%-10d    	%10d KB\n", nome, tempoModificado, tamanho);
// }
