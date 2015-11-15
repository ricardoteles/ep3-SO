#ifndef FS_H
#define FS_H

void imprimeFS();
void escreveInt(int valor, int inicioBytes, int nbytes);
void escreveChar(char valor, int inicioBytes, int nbytes);
int mountFS(char* fname);

#endif