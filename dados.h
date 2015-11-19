#ifndef DADOS_H
#define DADOS_H

typedef struct arquivo {
	char* nome;
	int byteInicio;
	int tempoCriado;
	int tempoModificado;
	int tempoAcessado;
	int tamanho;
} Arquivo;


/*************** SUPERBLOCO ******************/
void criaSuperBloco();

/**************** BITMAP ******************/
void criaBitMap();

/*************** FAT ******************/
void criaFAT();
void carregaFATnaMemoria(int inicioBytes, int quantidadeBlocos);
void regravaFATnoDisco(int inicioBytes, int nbytes);
void imprimeFAT(int quantidadeBlocos);

/*************** DADOS ******************/
void criaRaiz(); 
void alocaDiretorio(int byteInicio);
int alocaEntradaArquivo(char* nomeDir);
Arquivo setStruct(char* nome, int byteInicio, int tempoCriado, 
				  int tempoAcessado, int tempoModificado, int tamanho); 
void mkdir(char* nomeDir);

void ls(char* nome, int tempoModificado, int tamanho);

#endif