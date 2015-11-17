#include <stdio.h>
#include <stdlib.h>

FILE* arquivoMount;

typedef struct arquivo {
	int enderecoInicio;
	char* nome;
	int tempoCriado;
	int tempoModificado;
	int tempoAcessado;
	int tamanhoArquivo;
} Arquivo; 

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

Arquivo* leDiretorio(int inicioBytes) {
	if (arquivoMount) {
		Arquivo val;

		fseek(arquivoMount, inicioBytes, SEEK_SET);

		if (fread(&val, sizeof(Arquivo), 1, arquivoMount) == 1) {
			return &val;
		}
	}

	return NULL;
}

void criaDiretorio(Arquivo arq) {	
	arq.tempoCriado = 8;
	arq.tempoModificado = 8;
	arq.tempoAcessado = 8;
	arq.tamanhoArquivo = -1;
	arq.enderecoInicio = 5; 

	arq.nome = "ricardo";
	
	escreveDiretorio(arq, 0, 1);			// escreve numero de blocos
}

int main(int argc, char* argv[]){
	arquivoMount = fopen(argv[1], "r+b");
	Arquivo arq;
	Arquivo* arqSaida;

	criaDiretorio(arq);
	arqSaida = leDiretorio(0);

	printf("%s\n", arqSaida->nome);

	return 0;
}

