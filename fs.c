#include <stdlib.h>
#include "fs.h"
#include "dados.h"
#include "binario.h"

char blocoRaiz[TAM_BLOCO];

int umountFS() {
	if (arquivo != NULL) {
		// if (changeFAT)
		regravaFATnoDisco();

		escreveInt(arquivo, qtadeBlocos 		,  0, 1);
		escreveInt(arquivo, qtadeBlocosLivres	,  4, 1);
		escreveInt(arquivo, memUsada			,  8, 1);
		escreveInt(arquivo, qteDiretorios		, 12, 1);
		escreveInt(arquivo, qteArquivos			, 16, 1);
		escreveInt(arquivo, espacoLivre			, 20, 1);
		escreveInt(arquivo, espacoDesperd		, 24, 1);
		escreveInt(arquivo, iniBitmap			, 28, 1);
		escreveInt(arquivo, iniFat				, 32, 1);
		escreveStruct(arquivo, raiz 			, 36, 1);
		
		fclose(arquivo);
		arquivo = NULL;
		
		return 1;
	}
	return 0;
}

int mountFS(char* fname) {
	// se o arquivo não foi aberto ainda
	if (arquivo == NULL) {
		// se o arquivo não foi criado ainda
		if (!existeFS(fname)) {	
			criaFS(fname);
		}

		arquivo = fopen(fname, "r+b");
		if (arquivo) {
			carregaFS();
			changeFAT = 0;
			return 1;
		}
	}

	return 0;
}

void carregaFS() {
	iniSuperbloco = 0;

	qtadeBlocos 	  = leInt(arquivo, 0);			// le #blocos totais
    qtadeBlocosLivres = leInt(arquivo, 4); 			// le #blocos livres
	memUsada 		  = leInt(arquivo, 8);			// le memoria utilizada
	
	qteDiretorios	  = leInt(arquivo, 12);			// le qte diretorios 
	qteArquivos		  = leInt(arquivo, 16);			// le qte arquivos
	espacoLivre		  = leInt(arquivo, 20);			// le espaco livre
	espacoDesperd	  = leInt(arquivo, 24);			// le espaco desperdicado

	iniBitmap   	  = leInt(arquivo, 28); 		// le inicio bitmap do superbloco
	iniFat      	  = leInt(arquivo, 32);			// le inicio FAT do superbloco
	raiz        	  = leStruct(arquivo, 36);		// le Raiz do superbloco

	carregaFATnaMemoria();

	printf("Carreguei os dados:\n");
	printf(" #blocos: %d\n #blocos livre: %d\n qteDiretorios: %d\n qteArquivos: %d\n espacoLivre: %d\n espacoDesperd: %d\n Bitmap: %d\n FAT: %d\n Raiz: %d\n", 
		qtadeBlocos, qtadeBlocosLivres, qteDiretorios, qteArquivos, espacoLivre, espacoDesperd, iniBitmap, iniFat, raiz.byteInicio);
}


int existeFS(char* fname) {
	// r+b eh modo que obriga existencia do arquivo binario
	arquivo = fopen(fname, "r+b");

	if (!arquivo) { // se NULL é porque não existe
		return 0;
	}

	fclose(arquivo);
	return 1;
}

void criaFS(char* fname) {
	arquivo = fopen(fname, "wb");

	// ve se criou
	if (arquivo) {
		criaSuperBloco();
		criaBitMap();
		criaFAT();
		alocaDiretorio(iniFat + TAM_FAT);

		fclose(arquivo);
	}
}

