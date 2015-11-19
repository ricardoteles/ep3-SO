#include <stdio.h>
#include <stdlib.h>

FILE* arquivo;

typedef struct arquivo {
	int enderecoInicio;
	char* nome;
	int tempoCriado;
	int tempoModificado;
	int tempoAcessado;
	int tamanhoArquivo;
} Arquivo; 

void escreveDiretorio(Arquivo arq, int inicioBytes, int nbytes) {
		if (arquivo) {
		int i;
		Arquivo myVal = arq;

		fseek(arquivo, inicioBytes, SEEK_SET);

		for (i = 0; i < nbytes; i++)
			if (fwrite (&myVal, sizeof(Arquivo), 1, arquivo) != 1)
				printf("Não escreveu 1 Arquivo!\n");
	}
}

Arquivo* leDiretorio(int inicioBytes) {
	if (arquivo) {
		Arquivo val;

		fseek(arquivo, inicioBytes, SEEK_SET);

		if (fread(&val, sizeof(Arquivo), 1, arquivo) == 1) {
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
	arquivo = fopen(argv[1], "r+b");
	Arquivo arq;
	Arquivo* arqSaida;

	criaDiretorio(arq);
	arqSaida = leDiretorio(0);

	printf("%s\n", arqSaida->nome);

	return 0;
}

