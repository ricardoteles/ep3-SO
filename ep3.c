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
			printf("mkdir %s\n", word[1]);
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
			printf("ls %s\n", word[1]);
		}
	}
	else if (strcmp(word[0], "find") == 0) {
		if (word[1][0] != '\0' && word[2][0] != '\0') {
			printf("find %s %s\n", word[1], word[2]);
		}
	}
	else if (strcmp(word[0], "df") == 0) {
		printf("Farei o df\n");	
	}
	else if (strcmp(word[0], "umount") == 0) {
		printf("Farei o umount\n");	
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