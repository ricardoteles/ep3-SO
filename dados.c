#include "fs.h"
#include "dados.h"

Arquivo raiz;

void criaRaiz() {
	Atributos atr;
	atr.tempoCriado = 0;
	atr.tempoModificado = 0;
	atr.tempoAcessado = 0;
	atr.nome = "/";
	raiz.

	escreveInt(TAM_FS/TAM_BLOCO, 0, 4);			// escreve numero de blocos
	escreveInt(TAM_BLOCO, 4, 4);				// escreve inicio bitmap
	escreveInt(2*TAM_BLOCO, 8, 4);				// escreve inicio FAT
	escreveInt(3*TAM_BLOCO, 12, 4);				// escreve inicio Raiz
}

// void criaDiretorio() {
// 	Atributos atr;
// 	atr.tempoCriado = 8;
// 	atr.tempoModificado = 8;
// 	atr.tempoAcessado = 8;
// 	atr.nome = "ricardo";
	


// 	escreveDiretorio(TAM_FS/TAM_BLOCO, 0, 4);			// escreve numero de blocos
// }

// void escreveDiretorio(int valor, int inicioBytes, int nbytes) {
// 		if (arquivoMount) {
// 		int i;
// 		int myVal = valor;

// 		fseek(arquivoMount, inicioBytes, SEEK_SET);

// 		for (i = 0; i < nbytes; i++)
// 			if (fwrite (&myVal, sizeof(Arquivo), 1, arquivoMount) != 1)
// 				printf("Não escreveu 1 int!\n");
// 	}
// }