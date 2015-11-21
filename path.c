#include "path.h"
#include <stdio.h>

static void limpaMatrizPath();

int parserPath(char *path) {
	int i, lin = 0, col = 0;
	limpaMatrizPath();

	for (i = 0; path[i] != '\0'; i++) {
		if (path[i] != '/') {
			matrizPath[lin][col++] = path[i];
		}
		else if (col != 0) {
			lin++;
			col = 0;
		}
	}

	//printf("return : %d\n", lin+1);
	return (lin+1);
}

static void limpaMatrizPath() {
	int i, j;

	for (i = 0; i < LINPATH; i++) {
		for (j = 0; j < COLPATH; j++) {
			matrizPath[i][j] = 0;
		}
	}
}

void imprimePath() {
	int i;

	for (i = 0; matrizPath[i][0] != 0; i++) {
		printf("%s\n", matrizPath[i]);
	}	
}
