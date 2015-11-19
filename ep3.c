#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "fs.h"

#define LINMAX 10
#define COLMAX 50

char word[LINMAX][COLMAX];

int continua = 1;

int qtdadeDiretorios = 0;
int qtdadeArquivos = 0;
int espacoLivre = 1600;
int espacoDesperdicado = 0;

/*================================= PROTOTIPOS ========================================*/
void interpretaComandosShell();
void limpaMatriz();
void parserCommandShell(char *line);

int main() {
	char* line;
	printf("\n--------------------- SHELL EP3 ---------------------\n\n");

	while (continua) {
		line = readline("[ep3]: ");
		limpaMatriz();

		parserCommandShell(line);
		interpretaComandosShell();

		if (word[0][0] != '\0')
			add_history (line);
	}

	return 0;
}

void interpretaComandosShell() {
	if (strcmp(word[0], "mount") == 0) {
		if (word[1][0] != '\0') {
			if (mountFS(word[1]))
				printf("Unidade %s montada com sucesso!\n", word[1]);

			else printf("Falha na montagem de %s\n", word[1]);
		}
	}

	else if (strcmp(word[0], "cp") == 0) {
		if (word[1][0] != '\0' && word[2][0] != '\0') {
			printf("cp %s %s\n", word[1], word[2]);
		}
	}

	else if (strcmp(word[0], "mkdir") == 0) {
		if (word[1][0] != '\0') {
			mkdir(word[1]);
			printf("mkdir %s foi realizado com sucesso\n", word[1]);
		}
	}

	else if (strcmp(word[0], "rmdir") == 0) {
		if (word[1][0] != '\0') {
			printf("rmdir %s\n", word[1]);
		}
	}
	else if (strcmp(word[0], "cat") == 0) {
		if (word[1][0] != '\0') {
			printf("cat %s\n", word[1]);
		}		
	}
	else if (strcmp(word[0], "touch") == 0) {
		if (word[1][0] != '\0') {
			printf("touch %s\n", word[1]);
		}
	}
	else if (strcmp(word[0], "rm") == 0) {
		if (word[1][0] != '\0') {
			printf("rm %s\n", word[1]);
		}
	}
	else if (strcmp(word[0], "ls") == 0) {
		if (word[1][0] != '\0') {
			printf("\n---------------------------------------------------------------\n");
			printf("nome			  tempoModificacao	      tamanho\n");
			printf("---------------------------------------------------------------\n");
			// ls(word[1], 23213, 4096);
			printf("\n");
		}
	}
	else if (strcmp(word[0], "find") == 0) {
		if (word[1][0] != '\0' && word[2][0] != '\0') {
			printf("find %s %s\n", word[1], word[2]);
		}
	}
	else if (strcmp(word[0], "df") == 0) {
		printf("\nQuantidade de diretorios: %d\n", qtdadeDiretorios);	
		printf("Quantidade de arquivos:   %d\n", qtdadeArquivos);	
		printf("Espaco livre:             %d\n", espacoLivre);	
		printf("Espaco desperdicado:      %d\n\n", espacoDesperdicado);	
	}
	else if (strcmp(word[0], "umount") == 0) {
		if (umountFS())	
			printf("Unidade desmontada com sucesso!\n");

		else printf("Não existe unidade montada!\n");
	}
	else if (strcmp(word[0], "sai") == 0) {
		continua = 0;		
	}
	else if (word[0][0] != '\0'){
		fprintf(stderr, "comando %s inválido!\n", word[0]);
	}
}

void limpaMatriz() {
	int i, j;

	for (i = 0; i < LINMAX; i++) {
		for (j = 0; j < COLMAX; j++) {
			word[i][j] = 0;
		}
	}
}

void parserCommandShell(char *line) {
	int i, lin = 0, col = 0;

	for (i = 0; line[i] != '\0'; i++) {
		if (line[i] != ' ') {
			word[lin][col++] = line[i];
		}
		else if (col != 0) {
			lin++;
			col = 0;
		}
	}
}