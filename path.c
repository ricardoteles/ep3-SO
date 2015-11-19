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
		else {
			lin++;
			col = 0;
		}
	}

	return lin;
}

static void limpaMatrizPath() {
	int i, j;

	for (i = 0; i < LINPATH; i++) {
		for (j = 0; j < COLPATH; j++) {
			matrizPath[i][j] = 0;
		}
	}
}
