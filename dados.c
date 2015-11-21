#include <string.h>
#include "dados.h"
#include "binario.h"
#include "fs.h"
#include "path.h"

#define MAX_NIVEIS 100

int addressBlock[MAX_NIVEIS];
int numNiveis;
const int totalArquivo = (TAM_BLOCO - 4)/(sizeof(Arquivo)); 

void imprimeDiretorio(int inicioBytes);

void listDirectory(int iniBytes) {
 	int i, pos;
 	//int numEntradaslivres, numArquivos;
 	int iniFAT = mapsBlockToFAT(iniBytes);
 	int myIniBytes;

 	do 
 	{
 		//pos = 4;
 		myIniBytes = mapsFATtoBlock(iniFAT);
 		//numEntradaslivres = leInt(arquivo, myIniBytes);
 		//numArquivos = totalArquivo - numEntradaslivres;
	 	
	 	//for(i = 0; i < numArquivos; ) {
	 	for(pos = 4; pos < TAM_BLOCO;) {

	 		if (leChar(arquivo, myIniBytes+pos) != '\0') {  
		 		Arquivo saida = leStruct(arquivo, myIniBytes+pos);
			 	
			 	if(saida.tamanho == -1) {
			 		printf("%-20s %-10d (DIR)\n", saida.nome, saida.tempoModificado);
			 	}
			 	else {
			 		printf("%-20s %-10d %10d KB\n", saida.nome, saida.tempoModificado, saida.tamanho);
			 	}
		  		
		  		i++;
		  	}
		 	pos += sizeof(Arquivo);
	 	}
	 	
	 	iniFAT = FAT[iniFAT];

 	} while (iniFAT != -1);
}

Arquivo setStruct(char nome[20], int byteInicio, int tempoCriado,
			 	   int tempoAcessado, int tempoModificado, int tamanho) 
{
	Arquivo arq;
	strcpy(arq.nome, nome);
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

	raiz = setStruct("/\0", (int)(iniFat + TAM_FAT), 0, 0, 0, -1);
	
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

void imprimeFAT(int quantidadeBlocos) {
	int i;

	for (i = 0; i < quantidadeBlocos; i++) {
		printf("%d ", FAT[i]);
	}
}


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

/*********************************************** MKDIR ****************************************/ 
void ls(char* path) {
	int endFinal;

	if (strcmp("/\0", path) != 0) {
		numNiveis = parserPath(path);

		percorreArvoreFS(path);

		int endFatPai = mapsBlockToFAT(addressBlock[numNiveis-1]);
		endFinal = buscaEntradaEmDiretorio(matrizPath[numNiveis-1], endFatPai, addressBlock[numNiveis-1]);
	}
	else {
		endFinal = raiz.byteInicio;
	}	
	
	listDirectory(endFinal);
}

int mkdir(char* path) {
	char nome[20];
	Arquivo novo;
	//int i;
	int endEntry;

	numNiveis = parserPath(path);

	// for (i = 0; i < MAX_NIVEIS; i++)
	// 	addressBlock[i] = 0;

	// printf("Path:@");
	// imprimePath(); 
	
	percorreArvoreFS(path);

	strcpy(nome, matrizPath[numNiveis-1]);

	novo = setStruct(nome, 0, 10, 10, 10, -1); 

	endEntry = insereEntradaEmDiretorio(&novo);

	if (endEntry) {	
		escreveStruct(arquivo, novo, endEntry, 1);
		alocaDiretorio(novo.byteInicio);

		// teste
	 	if (addressBlock[numNiveis-1] > 0)
			listDirectory(addressBlock[numNiveis-1]);

		return 1;
	}

	return 0;
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
														  addressBlock[i-1]);
		addressBlock[i] = endPai; // vai salvando o caminho
	}

	// printf("Endereços encontrados:\n");
	// for (i = 0; i < numNiveis; i++) {
	// 	printf("%d ", addressBlock[i]);
	// }
	// printf("|\n\n");
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
				//escreveStruct(arquivo, *entrada, endInsert, 1); // escreve entrada no diretorio

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
			//escreveStruct(arquivo, *entrada, endInsert+4, 1);

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
