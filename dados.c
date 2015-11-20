#include <string.h>
#include "dados.h"
#include "binario.h"
#include "fs.h"
#include "path.h"

#define MAX_NIVEIS 100

int addressBlock[MAX_NIVEIS];
int numNiveis;

void imprimeDiretorio(int inicioBytes);

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
	qtadeBlocos = (TAM_FS/TAM_BLOCO);
	qtadeBlocosLivres = (TAM_FS/TAM_BLOCO);
	memUsada = 6*TAM_BLOCO + 4;

	iniSuperbloco = 0;
	iniBitmap = TAM_BLOCO;
	iniFat = 2 * TAM_BLOCO; 

	raiz = setStruct("/\0", (int)(2 * TAM_BLOCO + TAM_FAT), 0, 0, 0, -1);
	
	escreveInt(arquivo, 0, 0, TAM_BLOCO);	// preenche com zero				  
	
	escreveInt(arquivo, qtadeBlocos,  0,  	1);		// escreve numero de blocos totais
	escreveInt(arquivo, qtadeBlocosLivres,  4 ,  1);		// escreve # blocos livres
	escreveInt(arquivo, memUsada, 	   8 ,  1);		// escreve mem. utilizada
	escreveInt(arquivo, iniBitmap, 			   12,  1);		// escreve inicio bitmap
	escreveInt(arquivo, iniFat, 		   16,  1);		// escreve inicio FAT
	escreveStruct(arquivo, raiz, 			   20,  1);		// escreve entrada da raiz
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

void escreveRaizEmDisco() {
	int numEntradas = (TAM_BLOCO-4)/(sizeof(Arquivo));

	escreveInt(arquivo, numEntradas, raiz.byteInicio, 1);
	escreveChar(arquivo, '\0', raiz.byteInicio+4, TAM_BLOCO-4);
}

// pra 1 bloco!
void imprimeDiretorio(int inicioBytes) {
	Arquivo entrada;
	int i;

	for (i = inicioBytes+4; i < inicioBytes + TAM_BLOCO; ) {
		if (leChar(arquivo, i) != '\0')
			i++;
		else {
			entrada = leStruct(arquivo, i);
			i += sizeof(Arquivo);

			printf("Nome: %s|\n", entrada.nome);
			printf("Inicio byte: %d|\n", entrada.byteInicio);
			printf("Tamanho: %d|x|\n\n", entrada.tamanho);
		}
	}
}

/**************** MKDIR ****************/ 

int mkdir(char* path) {
	Arquivo novo;
	int i;

	numNiveis = parserPath(path);
	percorreArvoreFS(path);

	novo = setStruct(matrizPath[numNiveis-1], 0, 10, 10, 10, -1); 

	if (insereEntradaEmDiretorio(novo)) {
		alocaDiretorio(novo.byteInicio);
		
		for (i = 0; i < numNiveis; i++)
			imprimeDiretorio(addressBlock[i]);
		
		return 1;
	}
	return 0;
}

// escreve bloco de dados do diretorio no FS
void alocaDiretorio(int byteInicio) {
	int numEntradas = (TAM_BLOCO-4)/(sizeof(Arquivo));

	escreveInt(arquivo, numEntradas, byteInicio, 1);
	escreveChar(arquivo, '\0', byteInicio+4, TAM_BLOCO-4);
}

int posLivreBitmap(int inicioBusca) {
	int i;

	if (inicioBusca != -1) {
		for (i = inicioBusca; i < (iniFat - iniBitmap); i++) {				
			if (leChar(arquivo, iniBitmap+i) == 0) return i;
		}
	}
	return -1;
}

// monta a arvore em addressBlock
void percorreArvoreFS(char* path) {
	int i, endPai = 0;

	endPai = addressBlock[0] = raiz.byteInicio; 
	
	for (i = 1; i < numNiveis; i++) {
		endPai = buscaEntradaEmDiretorio(matrizPath[i-1], mapsBlockToFAT(endPai), endPai);
		addressBlock[i] = endPai; // vai salvando o caminho
	}
}

// busca por endereco da entrada no diretorio endInicioFAT, 
// cujo endereco de bloco eh endInicioBloco 
// => pra montar a arvore :)
int buscaEntradaEmDiretorio(char* entrada, int endInicioFAT, int endInicioBloco) {
	int endPai, endEntrada;

	// percorre a lista ligada do diretorio
	for (endPai = endInicioFAT; endPai != -1; endPai = FAT[endPai]) {
		endEntrada = buscaEnderecoDaEntradaPorBloco(entrada, endInicioBloco);
		
		if (endEntrada != -1)
			return endEntrada;
	}
	return -2; // caminhos sempre serao validos
}

// retorna endereco do bloco de inicio da entrada ou -1 => pra montar a arvore :)
int buscaEnderecoDaEntradaPorBloco(char* entrada, int enderecoBlocoPai) {
	int NEntradas = leInt(arquivo, enderecoBlocoPai);
	int pos;
	Arquivo arq;

	if (NEntradas > 0) {
		// começa a busca da entrada depois do inteiro
		for (pos = enderecoBlocoPai+4; pos < enderecoBlocoPai + TAM_BLOCO; ) {
			if (leChar(arquivo, pos) == '\0') pos++;
			else { 
				arq = leStruct(arquivo, pos);

				if (strcmp(entrada, arq.nome) == 0)
					return arq.byteInicio;

				else pos += sizeof(Arquivo);
			}
		} 
	}
	return -1;
}

int mapsBlockToFAT(int endBloco) {
	return (int)((endBloco - raiz.byteInicio)/TAM_BLOCO);
}

int mapsFATtoBlock(int endFAT) {
	return (int)(raiz.byteInicio + endFAT * TAM_BLOCO);
}

int mapeiaBitmapEmByte(int posBitmap) {
	return (int)(raiz.byteInicio + posBitmap * TAM_BLOCO);
}

// retorna o endereco do bloco da entrada onde deve inserir
int insereEntradaPorBloco(Arquivo entrada, int endInicioBloco) {
	int pos;
	int NEntradas = leInt(arquivo, endInicioBloco);

	if (NEntradas > 0) {
		for (pos = endInicioBloco+4; pos < endInicioBloco + TAM_BLOCO; pos++) {
			if (leChar(arquivo, pos) == '\0') {
				//escreveStruct(arquivo, entrada, pos, 1); // grava entrada no diretorio pai
				escreveInt(arquivo, NEntradas-1, endInicioBloco, 1); 
				return pos;	
			}
		}
	}
	return 0;
}


// insere a entrada em um bloco do diretorio e retorna 1 ou 0 se conseguiu ou nao
// Tb guarda o endereco do bloco do novo diretorio
int insereEntradaEmDiretorio(Arquivo entrada) {
	int i, lastBlock, endInsert;

	if (qtadeBlocosLivres > 1) 
	{
		// percorre os blocos do pai via FAT
		for (i = mapsBlockToFAT(addressBlock[numNiveis-1]); i != -1; i = FAT[i]) 
		{	
			// tenta escrever a entrada em algum bloco do diretorio pai
			endInsert = insereEntradaPorBloco(entrada, mapsFATtoBlock(i));

			if (endInsert) 
			{		
				// aloca bloco do diretorio
				qtadeBlocosLivres--;
				int novoBloco = posLivreBitmap(0);
				escreveChar(arquivo, 1, iniBitmap + novoBloco, 1); // seta 1 no bitmap
				FAT[novoBloco] = -1; // inicio do diretorio	

				entrada.byteInicio = mapeiaBitmapEmByte(novoBloco);
				return 1;
			}
			lastBlock = i;	
		}

		// não tem entrada disponivel: + 1 bloco pra entrada!
		if (qtadeBlocosLivres > 2) 
		{
			// algo a mais ...
			qtadeBlocosLivres -= 2;

			int novoBlocoPai = posLivreBitmap(1);	
			// seta 1 no bitmap (bloco do pai para alocar entrada)
			escreveChar(arquivo, 1, iniBitmap + novoBlocoPai, 1); 
			expandeFAT(lastBlock, novoBlocoPai); //atualiza o FAT da posPai 
			

			int novoBloco = posLivreBitmap(novoBlocoPai+1); 
			// seta 1 no bitmap (bloco do filho) como antes
			escreveChar(arquivo, 1, iniBitmap + novoBloco, 1); 
			FAT[novoBloco] = -1; // primeiro bloco do diretorio

			entrada.byteInicio = mapeiaBitmapEmByte(novoBloco);
			return 1;
		}
	}
	return 0;
}


void expandeFAT(int posicaoInicio, int novaPos) {
	int i;

	for(i = posicaoInicio; FAT[i] != -1; i = FAT[i]);

	FAT[i] = novaPos;
	FAT[novaPos] = -1;
}


/**************** LS ****************/ 

// void ls(char* nome, int tempoModificado, int tamanho) {
// 	if(tamanho == -1)
// 		printf("%-20s    	%-10d    	      (DIR)\n", nome, tempoModificado);
// 	else
// 		printf("%-20s    	%-10d    	%10d KB\n", nome, tempoModificado, tamanho);
// }
/*
int alocaArquivo(char* nomeDir) {
	int entradasLivres = leInt(arquivo, iniRaiz);
	int posPai = 0, posDir = 0;
	Arquivo novoDir;

	// aloca o pai que esta cheio e o novo diretorio
	if (entradasLivres <= 0){
		posPai = posLivreBitmap(0);
		posDir = posLivreBitmap(posPai); 
		
		if (posPai != -1 && posDir != -1) {
			escreveChar(arquivo, 1, iniBitmap + posPai, 1);
			escreveChar(arquivo, 1, iniBitmap + posDir, 1);
			expandeFAT(iniRaiz, posPai);			//atualiza o FAT da posPai 
			FAT[posDir] = -1;
			
			novoDir = setStruct(nomeDir, mapeiaBitmapEmByte(posDir), 9, 8, 7, -1);
			escreveStruct(arquivo, novoDir, mapeiaBitmapEmByte(posPai)+4, 1);

			return posDir; 
		}
		else {
			printf("Nao foi possivel criar o diretorio %s: Memoria cheia!!!\n", nomeDir);
		} 
	} 
	
	// aloca o novo diretorio
	else {
		posDir = posLivreBitmap(0); 
	
		if(posDir != -1) {
			escreveChar(arquivo, 1, iniBitmap + posDir, 1);
			FAT[posDir] = -1;

			novoDir = setStruct(nomeDir, mapeiaBitmapEmByte(posDir), 9, 8, 7, -1);
			escreveStruct(arquivo, novoDir, mapeiaBitmapEmByte(posPai)+4, 1);
			return posDir;
		}
		else{
			printf("Nao foi possivel criar o diretorio %s: Memoria cheia!!!\n", nomeDir);
		} 
	}

	return -1;
}
*/
