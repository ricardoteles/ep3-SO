#ifndef FAT_H
#define FAT_H

void criaFAT();
void carregaFATnaMemoria(int inicioBytes, int quantidadeBlocos);
void regravaFATnoDisco(int inicioBytes, int nbytes);

#endif