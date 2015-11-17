#include "fs.h"
#include "dados.h"

void criaDiretorio() {
	Arquivo arq;
	
	arq.tempoCriado = 8;
	arq.tempoModificado = 8;
	arq.tempoAcessado = 8;
	arq.tamArquivo = -1;
	arq.endereceInicio = 

	arq.nome = "ricardo";
	
	escreveDiretorio(arq);			// escreve numero de blocos
}

void escreveDiretorio(Arquivo arq, int inicioBytes, int nbytes) {
		if (arquivoMount) {
		int i;
		Arquivo myVal = arq;

		fseek(arquivoMount, inicioBytes, SEEK_SET);

		for (i = 0; i < nbytes; i++)
			if (fwrite (&myVal, sizeof(Arquivo), 1, arquivoMount) != 1)
				printf("Não escreveu 1 Arquivo!\n");
	}
}