#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "dados.h"
#include "binario.h"
#include "fs.h"
#include "path.h"

#define MAX_NIVEIS 100

int addressBlock[MAX_NIVEIS];
int numNiveis;
const int totalArquivo = (TAM_BLOCO - 4)/(sizeof(Arquivo)); 
void imprimeDiretorio(int inicioBytes);


char* data(){
    time_t tempo;
    struct tm *info;

    time(&tempo);
    info = localtime(&tempo);

    return asctime(info);
}

void listDirectory(int iniBytes) {
 	int i, pos;
 	int iniFAT = mapsBlockToFAT(iniBytes);
 	int myIniBytes;

 	do 
 	{
 		myIniBytes = mapsFATtoBlock(iniFAT);

	 	for(pos = 4; pos < TAM_BLOCO;) {

	 		if (leChar(arquivo, myIniBytes+pos) != '\0') {  
		 		Arquivo saida = leStruct(arquivo, myIniBytes+pos);
			 	
			 	if(saida.tamanho == -1) {
			 		printf("%-20s %-10s  \t %-25s", saida.nome, "(DIR)", saida.tempoModificado);
			 	}
			 	else {
			 		printf("%-20s %-10d \t %-25s", saida.nome, saida.tamanho, saida.tempoModificado);
			 	}
		  		
		  		i++;
		  	}
		 	pos += sizeof(Arquivo);
	 	}
	 	
	 	iniFAT = FAT[iniFAT];

 	} while (iniFAT != -1);
}

Arquivo setStruct(char nome[20], int byteInicio, char tempoCriado[30],
			 	   char tempoAcessado[30], char tempoModificado[30], int tamanho) 
{
	Arquivo arq;
	strcpy(arq.nome, nome);
	arq.byteInicio = byteInicio;
	strcpy(arq.tempoCriado, tempoCriado);
	strcpy(arq.tempoModificado, tempoModificado);
	strcpy(arq.tempoAcessado, tempoAcessado);
	arq.tamanho = tamanho;
	return arq;
}	

void criaSuperBloco() {
	iniSuperbloco = 0;
	iniBitmap = TAM_BLOCO;
	iniFat = 2 * TAM_BLOCO;
	raiz = setStruct("/\0", (int)(iniFat + TAM_FAT), data(), data(), data(), -1);
	
	memUsada = 4;

	qtadeBlocos = (TAM_FS - raiz.byteInicio)/TAM_BLOCO;
	qtadeBlocosLivres = qtadeBlocos - 1;
	qteDiretorios = 0;
	qteArquivos = 0;
	espacoLivre = (TAM_FS - raiz.byteInicio) - memUsada;
	espacoDesperd = (qtadeBlocos - qtadeBlocosLivres) * TAM_BLOCO - memUsada; 
	
	escreveInt(arquivo, 0, 0, TAM_BLOCO);	// preenche com zero				  
	
	escreveInt(arquivo, qtadeBlocos,  		0, 	1);					// escreve numero de blocos totais
	escreveInt(arquivo, qtadeBlocosLivres,  4 ,  1);				// escreve # blocos livres
	escreveInt(arquivo, memUsada, 	   		8 ,  1);				// escreve mem. utilizada
	escreveInt(arquivo, qteDiretorios, 		12,  1);				// escreve qte diretorios
	escreveInt(arquivo, qteArquivos,   		16,  1);				// escreve qte arquivos
	escreveInt(arquivo, espacoLivre,  		20,  1);				// escreve espaco livre
	escreveInt(arquivo, espacoDesperd, 		24,  1);				// escreve espaco desperdicado
	escreveInt(arquivo, iniBitmap, 	  		28,  1);				// escreve inicio bitmap
	escreveInt(arquivo, iniFat, 	  		32,  1);				// escreve inicio FAT	
	escreveStruct(arquivo, raiz, 	   		36,  1);				// escreve entrada da raiz
}

void criaBitMap() {
	escreveChar(arquivo, 1, iniBitmap, 1); 							// bloco da raiz
	escreveChar(arquivo, 0, iniBitmap+1, iniFat-iniBitmap-1);		// inicializa mapa de bits com 0
}

void criaFAT() {
	escreveInt(arquivo, -1, iniFat, 1);				// inicializa o FAT[0] com -1
	escreveInt(arquivo, -2, iniFat+4, TAM_FAT-1);	// inicializa o FAT com -2
}

void carregaFATnaMemoria() {
	int i;

	if (arquivo) {
		int val;

		fseek(arquivo, iniFat, SEEK_SET);

		for (i = 0; i < TAM_FAT; i++) {
			if (fread(&val, sizeof(int), 1, arquivo) == 1) {
				FAT[i] = val;
			}
		}
	}
}

void regravaFATnoDisco() {
	if (arquivo) {
		int i;

		fseek(arquivo, iniFat, SEEK_SET);

		for (i = 0; i < TAM_FAT; i++)
			if (fwrite (&FAT[i], sizeof(int), 1, arquivo) != 1)
				printf("Não escreveu 1 int!\n");
	}
}

/*********************************************** MKDIR ****************************************/ 
void ls(char* path) {
	int endFinal;

	if (strcmp("/\0", path) != 0) {
		numNiveis = parserPath(path);

		percorreArvoreFS();

		int endFatPai = mapsBlockToFAT(addressBlock[numNiveis-1]);
		endFinal = buscaEntradaEmDiretorio(matrizPath[numNiveis-1], endFatPai, addressBlock[numNiveis-1], NULL);
	}
	else {
		endFinal = raiz.byteInicio;
	}	
	
	listDirectory(endFinal);
}

int mkdir(char* path) {
	char nome[20];
	Arquivo novo;
	int endEntry;

	numNiveis = parserPath(path);

	percorreArvoreFS();

	strcpy(nome, matrizPath[numNiveis-1]);

	novo = setStruct(nome, 0, data(), data(), data(), -1); 

	endEntry = insereEntradaEmDiretorio(&novo);

	if (endEntry) {	
		escreveStruct(arquivo, novo, endEntry, 1);
		alocaDiretorio(novo.byteInicio);

		return 1;
	}

	return 0;
}

int touch(char* path) {
	char nome[20];
	Arquivo novo;
	int endEntry;
	int endFinal;

	numNiveis = parserPath(path);
	
	percorreArvoreFS();

	// existe ou não o arquivo ?
	endFinal = buscaEntradaEmDiretorio(matrizPath[numNiveis-1], mapsBlockToFAT(addressBlock[numNiveis-1]), 
					addressBlock[numNiveis-1], &novo);

	if (endFinal < 0) { 							// se arquivo nao existe
		strcpy(nome, matrizPath[numNiveis-1]);

		novo = setStruct(nome, 0, data(), data(), data(), 0); 

		endEntry = insereEntradaEmDiretorio(&novo);

		if (endEntry) {	
			escreveStruct(arquivo, novo, endEntry, 1);
			alocaArquivo(novo.byteInicio);

			return endEntry;
		}
	}
	else {										
		strcpy(novo.tempoAcessado, data());
		escreveStruct(arquivo, novo, endEntry, 1);	

		return 1;
	}

	return 0;
}

// Referencia: Tanenbaum (copia de arquivo)
void cp(char* origem, char* destino) {
	int in_fd, rd_count;
	char buffer[TAM_BLOCO];
	int endEntry, i, totalBytes = 0;
	Arquivo reg;

	in_fd = open(origem, O_RDONLY);
	
	if (in_fd < 0) {
		printf("Arquivo nao encontrado");
	}

	endEntry = touch(destino);		// cria o arquivo destino e retorna a posicao 
									// onde o registro foi inserido

	reg = leStruct(arquivo, endEntry);

	while (1) { // TODO: ver o FAT e atualizar os tempos
		rd_count = read(in_fd, buffer, TAM_BLOCO);
		if (rd_count <= 0) 
			break;

		totalBytes += rd_count;
		// expandir o arquivo!

		for (i = 0; i < rd_count; i++) {
			escreveChar(arquivo, buffer[i], reg.byteInicio+i, 1);
		}
	}

	reg.tamanho = totalBytes;
	escreveStruct(arquivo, reg, endEntry, 1);

	close(in_fd);
}

void cat(char* path) {
	int endFinal, i;
	Arquivo arq;

	numNiveis = parserPath(path);
	percorreArvoreFS();

	endFinal = buscaEntradaEmDiretorio(matrizPath[numNiveis-1], mapsBlockToFAT(addressBlock[numNiveis-1]), 
					addressBlock[numNiveis-1], &arq);	

	printf("TAM: %d\n", arq.tamanho);
	for (i = 0; i < arq.tamanho; i++)
		printf("%c", leChar(arquivo, endFinal+i));
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
void percorreArvoreFS() {
	int i, endPai = 0;

	endPai = addressBlock[0] = raiz.byteInicio; 
	
	for (i = 1; i < numNiveis; i++) {
		endPai = buscaEntradaEmDiretorio(matrizPath[i-1], mapsBlockToFAT(addressBlock[i-1]), 
														  addressBlock[i-1], NULL);
		addressBlock[i] = endPai; // vai salvando o caminho
	}
}

// busca por endereco da entrada no diretorio endInicioFAT, 
// cujo endereco de bloco eh endInicioBloco 
// => pra montar a arvore :)
int buscaEntradaEmDiretorio(char* entrada, int endInicioFAT, int endInicioBloco, Arquivo* reg) {
	int endPai, endEntrada;

	// percorre a lista ligada do diretorio
	for (endPai = endInicioFAT; endPai != -1; endPai = FAT[endPai]) {
		endEntrada = buscaEnderecoDaEntradaPorBloco(entrada, endInicioBloco, reg);
		
		if (endEntrada != -1)
			return endEntrada;
	}
	return -2; // caminhos sempre serao validos
}

// retorna endereco do bloco de inicio da entrada ou -1 => pra montar a arvore :)
int buscaEnderecoDaEntradaPorBloco(char* entrada, int enderecoBlocoPai, Arquivo* reg) {
	int numEntradasLivres = leInt(arquivo, enderecoBlocoPai);
	int pos;
	int numEntradas = totalArquivo - numEntradasLivres; 

	if (numEntradas > 0) {
		// começa a busca da entrada depois do inteiro
		for (pos = enderecoBlocoPai+4; pos < enderecoBlocoPai + TAM_BLOCO; ) {
			if (leChar(arquivo, pos) == '\0') 
				pos++;
			
			else { 
				Arquivo arq = leStruct(arquivo, pos);

				if (strcmp(entrada, &arq.nome[0]) == 0) {
					if (reg != NULL) *reg = arq;
					return arq.byteInicio;
				}
				else pos++;
			}
		} 
	}
	return -1;
}

// retorna o endereco do bloco da entrada onde deve inserir
int insereEntradaPorBloco(int endInicioBloco) {
	int pos;
	int NEntradas = leInt(arquivo, endInicioBloco);

	if (NEntradas > 0) {
		for (pos = endInicioBloco+4; pos < endInicioBloco + TAM_BLOCO; ) {
			if (leChar(arquivo, pos) == '\0') {
				// só decrementa entradas livres do bloco do pai
				escreveInt(arquivo, NEntradas-1, endInicioBloco, 1); 
				return pos;	// ja retorna
			}
			else pos += sizeof(Arquivo);
		}
	}
	return 0;
}

// insere a entrada em um bloco do diretorio e retorna 1 ou 0 se conseguiu ou nao
// Tb guarda o endereco do bloco do novo diretorio
int insereEntradaEmDiretorio(Arquivo* entrada) {
	int i, lastBlock, endInsert;

	if (qtadeBlocosLivres > 1) 
	{
		// percorre os blocos do pai via FAT
		for (i = mapsBlockToFAT(addressBlock[numNiveis-1]); i != -1; i = FAT[i]) 
		{	
			// tenta escrever a entrada em algum bloco do diretorio pai
			endInsert = insereEntradaPorBloco(mapsFATtoBlock(i));

			if (endInsert) 
			{		
				// aloca bloco do diretorio
				qtadeBlocosLivres--;
				int novoBloco = posLivreBitmap(1);
				escreveChar(arquivo, 1, iniBitmap + novoBloco, 1); // seta 1 no bitmap
				FAT[novoBloco] = -1; // inicio do diretorio	

				entrada->byteInicio = mapeiaBitmapEmByte(novoBloco);

				return endInsert;
			}
			lastBlock = i;	
		}

		// não tem entrada disponivel: + 1 bloco pra entrada!
		if (qtadeBlocosLivres > 2) 
		{
			// algo a mais ...
			qtadeBlocosLivres -= 2;

			int novoBlocoPai = posLivreBitmap(0);	
			// seta 1 no bitmap (bloco do pai para alocar entrada)
			escreveChar(arquivo, 1, iniBitmap + novoBlocoPai, 1); 
			expandeFAT(lastBlock, novoBlocoPai); //atualiza o FAT da posPai 
			
			endInsert = mapeiaBitmapEmByte(novoBlocoPai);
			alocaDiretorio(endInsert); // escreve novo bloco do pai no FS

			int novoBloco = posLivreBitmap(novoBlocoPai+1); 
			// seta 1 no bitmap (bloco do filho) como antes
			escreveChar(arquivo, 1, iniBitmap + novoBloco, 1); 
			FAT[novoBloco] = -1; // primeiro bloco do diretorio

			entrada->byteInicio = mapeiaBitmapEmByte(novoBloco);

			return (endInsert+4);
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

int mapsBlockToFAT(int endBloco) {
	return (int)((endBloco - raiz.byteInicio)/TAM_BLOCO);
}

int mapsFATtoBlock(int endFAT) {
	return (int)(raiz.byteInicio + endFAT * TAM_BLOCO);
}

int mapeiaBitmapEmByte(int posBitmap) {
	return (int)(raiz.byteInicio + posBitmap * TAM_BLOCO);
}

// escreve bloco de dados do diretorio no FS
void alocaDiretorio(int byteInicio) {
	escreveInt(arquivo, totalArquivo, byteInicio, 1);
	escreveChar(arquivo, '\0', byteInicio+4, TAM_BLOCO-4);
}

void alocaArquivo(int byteInicio) {
	escreveChar(arquivo, '\0', byteInicio, TAM_BLOCO);
}

/*********************************************** TESTE ****************************************/ 
// pra 1 bloco!
void imprimeDiretorio(int inicioBytes) {
	int i;

	for (i = inicioBytes+4; i < inicioBytes + TAM_BLOCO; ) {
		if (leChar(arquivo, i) == '\0')
			i++;
		else {
			Arquivo entrada;
			entrada = leStruct(arquivo, i);
			i += sizeof(Arquivo);

			printf("Nome: %s|\n", entrada.nome);
			printf("Inicio byte: %d|\n", entrada.byteInicio);
			printf("Tamanho: %d|x|\n\n", entrada.tamanho);
		}
	}
}

void imprimeFAT(int quantidadeBlocos) {
	int i;

	for (i = 0; i < quantidadeBlocos; i++) {
		printf("%d ", FAT[i]);
	}
}