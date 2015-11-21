#ifndef DADOS_H
#define DADOS_H

typedef struct arquivo {
	char nome[20];
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
void carregaFATnaMemoria();
void regravaFATnoDisco();
void imprimeFAT(int quantidadeBlocos);

/*************** DADOS ******************/
void alocaDiretorio(int byteInicio);
void alocaArquivo(int byteInicio);
Arquivo setStruct(char nome[20], int byteInicio, int tempoCriado, 
				  int tempoAcessado, int tempoModificado, int tamanho); 

int mkdir(char* path);
void listDirectory(int iniBytes);
void ls(char* path);
int touch(char* path);
void cp(char* origem, char* destino);
void cat(char* path);
/*********************** ARVORE ************************/
void percorreArvoreFS();

int buscaEntradaEmDiretorio(char* entrada, int endInicioFAT, int endInicioBloco, Arquivo* reg);
int buscaEnderecoDaEntradaPorBloco(char* entrada, int enderecoBlocoPai, Arquivo* reg);
int insereEntradaPorBloco(int endInicioBloco);
int insereEntradaEmDiretorio(Arquivo* entrada);

int posLivreBitmap(int inicioBusca);
void expandeFAT(int posicaoInicio, int novaPos);
int mapeiaBitmapEmByte(int posBitmap);
int mapsBlockToFAT(int endBloco);
int mapsFATtoBlock(int endFAT);

#endif