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
void escreveRaizEmDisco(); 
void alocaDiretorio(int byteInicio);
Arquivo setStruct(char* nome, int byteInicio, int tempoCriado, 
				  int tempoAcessado, int tempoModificado, int tamanho); 

int mkdir(char* nomeDir);
void ls(char* nome, int tempoModificado, int tamanho);

/*********************** ARVORE ************************/
void percorreArvoreFS(char* path);

int buscaEntradaEmDiretorio(char* entrada, int endInicioFAT, int endInicioBloco);
int buscaEnderecoDaEntradaPorBloco(char* entrada, int enderecoBlocoPai);
int insereEntradaPorBloco(Arquivo entrada, int endInicioBloco);
int insereEntradaEmDiretorio(Arquivo entrada);

int posLivreBitmap(int inicioBusca);
void expandeFAT(int posicaoInicio, int novaPos);
int mapeiaBitmapEmByte(int posBitmap);
int mapsBlockToFAT(int endBloco);
int mapsFATtoBlock(int endFAT);

#endif