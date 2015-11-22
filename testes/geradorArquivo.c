#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	char valor = 'a';
	FILE* arq;
	int i;
	
	int tam = atoi(argv[1]);
	if (tam == 1)
		arq = fopen("lixo1.txt", "w");

	if (tam == 10)
		arq = fopen("lixo10.txt", "w");

	if (tam == 30)
		arq = fopen("lixo30.txt", "w");

	tam *= 1000000;

	for (i = 0; i < tam; i++)
		fprintf(arq, "%c", valor);

	fclose(arq);

	return 0;
}
