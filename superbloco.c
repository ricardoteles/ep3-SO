#include "fs.h"
#include "superbloco.h"

void criaSuperBloco() {
	qtadeBlocos = TAM_FS/TAM_BLOCO;
	iniSuperbloco = 0;
	iniBitmap = TAM_BLOCO;		
	iniFat = 2*TAM_BLOCO;		
	iniRaiz = 3*TAM_BLOCO;
	iniDados = 4*TAM_BLOCO;

	escreveInt(qtadeBlocos, iniSuperbloco     , 4);				// escreve numero de blocos
	escreveInt(iniBitmap  , iniSuperbloco + 4 , 4);				// escreve inicio bitmap
	escreveInt(iniFat     , iniSuperbloco + 8 , 4);			 	// escreve inicio FAT
	escreveInt(iniRaiz    , iniSuperbloco + 12, 4);				// escreve inicio Raiz
	escreveInt(iniDados   , iniSuperbloco + 16, 4);				// escreve inicio Dados
}