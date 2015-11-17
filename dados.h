#ifndef DADOS_H
#define DADOS_H

typedef struct arquivo {
	int enderecoInicio;
	char nome[20];
	int tempoCriado;
	int tempoModificado;
	int tempoAcessado;
	int tamanhoArquivo;
} Arquivo; 

#endif