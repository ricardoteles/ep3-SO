#include <stdio.h>
#include <stdlib.h>

static FILE* arquivo = NULL;

void imprimeFS() {
	if (arquivo) {
		char val;
		//arquivo = fopen(fname, "rb");

		while (!feof(arquivo)) {
			if (fread(&val, sizeof(char), 1, arquivo) == 1) {
				printf("%d", val);
			}
		}
		//fclose(arquivo);
	}
}

int main(int argc, char* argv[]) {
	arquivo = fopen(argv[1], "r+b");

	imprimeFS();
}