#include "fs.h"
#include "superbloco.h"

void criaSuperBloco() {
	escreveInt(TAM_FS/TAM_BLOCO, 0, 4);			// escreve numero de blocos
	escreveInt(TAM_BLOCO, 4, 4);				// escreve inicio bitmap
	escreveInt(2*TAM_BLOCO, 8, 4);				// escreve inicio FAT
	escreveInt(3*TAM_BLOCO, 12, 4);				// escreve inicio Raiz
}