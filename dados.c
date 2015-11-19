#include "dados.h"
#include "binario.h"
#include "fs.h"

static int FAT[TAM_FAT];
static int iniSuperbloco;
static int iniBitmap;		
static int iniFat;		
static int iniRaiz;
static int qtadeBlocos;
//static int iniDados;

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
	//iniDados = 4 * TAM_BLOCO;
	Arquivo raiz;
	raiz = setStruct("/\0", iniRaiz, 0, 0, 0, -1);
	
	escreveInt(arquivo, 0		  , iniSuperbloco	  ,	 TAM_BLOCO);	// preenche com zero				  
	escreveInt(arquivo, qtadeBlocos, iniSuperbloco     ,  1);			// escreve numero de blocos totais
	escreveInt(arquivo, 1    	  , iniSuperbloco + 4 ,  1);		 	// escreve # blocos usados
	escreveInt(arquivo, 0     	  , iniSuperbloco + 8 ,  1);		 	// escreve mem. utilizada
	escreveInt(arquivo, iniBitmap  , iniSuperbloco + 12,  1);			// escreve inicio bitmap
	escreveInt(arquivo, iniFat     , iniSuperbloco + 16,  1);		 	// escreve inicio FAT
	escreveStruct(arquivo, raiz 	  , iniSuperbloco + 20,  1);			// escreve entrada da raiz
}

void criaBitMap() {
	escreveChar(arquivo, 1, iniBitmap, 1); 							// bloco da raiz
	escreveChar(arquivo, 0, iniBitmap+1, iniFat-iniBitmap-1);		// inicializa mapa de bits com 0
}

void criaFAT() {
	escreveInt(arquivo, 0, iniFat, 1);				// inicializa o FAT com -2
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

// aloca raiz
void criaRaiz() {
	int numEntradas = (TAM_BLOCO-4)/(sizeof(Arquivo));

	escreveInt(arquivo, numEntradas, iniRaiz, 1);
	escreveChar(arquivo, '\0', iniRaiz+4, TAM_BLOCO-4);
}