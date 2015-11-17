#include "fs.h"
#include "superbloco.h"
#include "bitmap.h"
#include "fat.h"
#include "dados.h"

static void criaFS(char* fname); 
static int existeFS(char* fname);
static void carregaFS();

void umountFS() {
	regravaFATnoDisco(iniFat, iniRaiz-iniFat);
	fclose(arquivoMount);	
	arquivoMount = NULL;
}

int mountFS(char* fname) {
	// se o arquivoMount não foi aberto ainda
	if (arquivoMount == NULL) {
		// se o arquivoMount não foi criado ainda
		if (!existeFS(fname)) {	
			criaFS(fname);
		}

		arquivoMount = fopen(fname, "r+b");
		if (arquivoMount) {
			carregaFS();
			return 1;
		}
	}

	return 0;
}

static void carregaFS() {
	qtadeBlocos = leInt(iniSuperbloco);				// le numero de blocos
	iniBitmap   = leInt(iniSuperbloco + 4); 		// le inicio bitmap
	iniFat      = leInt(iniSuperbloco + 8);		 	// le inicio FAT
	iniRaiz     = leInt(iniSuperbloco + 12);		// le inicio Raiz
	iniDados    = leInt(iniSuperbloco + 16);		// le inicio Dados

	carregaFATnaMemoria(iniFat, TAM_FAT);

	printf("Carreguei os dados:\n");
	printf("%d %d %d %d %d\n", qtadeBlocos, iniBitmap, iniFat, iniRaiz, iniDados);
}

static int existeFS(char* fname) {
	// r+b eh modo que obriga existencia do arquivoMount binario
	arquivoMount = fopen(fname, "r+b");

	if (!arquivoMount) { // se NULL é porque não existe
		return 0;
	}

	fclose(arquivoMount);
	return 1;
}

static void criaFS(char* fname) {
	arquivoMount = fopen(fname, "wb");

	// ve se criou
	if (arquivoMount) {
		criaSuperBloco();
		criaBitMap();
		criaFAT();
		// criaRaiz();

		fclose(arquivoMount);
	}
}

void escreveInt(int valor, int inicioBytes, int nbytes) {
	if (arquivoMount) {
		int i;
		int myVal = valor;

		fseek(arquivoMount, inicioBytes, SEEK_SET);

		for (i = 0; i < nbytes; i++)
			if (fwrite (&myVal, sizeof(int), 1, arquivoMount) != 1)
				printf("Não escreveu 1 int!\n");
	}
}

void escreveChar(char valor, int inicioBytes, int nbytes) {
	if (arquivoMount) {
		int i;
		char myVal = valor;

		fseek(arquivoMount, inicioBytes, SEEK_SET);

		for (i = 0; i < nbytes; i++)
			if (fwrite (&myVal, sizeof(char), 1, arquivoMount) != 1)
				printf("Não escreveu 1 byte!\n");
	}
}

int leInt(int inicioBytes) {
	if (arquivoMount) {
		int val;

		fseek(arquivoMount, inicioBytes, SEEK_SET);

		if (fread(&val, sizeof(int), 1, arquivoMount) == 1) {
			return val;
		}
	}

	return 0;
}