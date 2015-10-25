#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <readline/readline.h>

#define LINMAX 10
#define COLMAX 50

FILE* arqEntrada;
char word[LINMAX][COLMAX];

/*================================= PROTOTIPOS ========================================*/
int interpretaComandosShell();
void limpaMatriz();
void parserCommandShell(char *line);

int main() {
	int status = 1;
	char* line;
	printf("\n--------------------- SHELL EP2 ---------------------\n\n");

	while (status) {
		line = readline("[ep3]: ");
		limpaMatriz();
		parserCommandShell(line);
		status = interpretaComandosShell();

		if (word[0][0] != '\0')
			add_history (line);
	}

	return 0;
}

int interpretaComandosShell() {
	int aux, i;
	
	if (strcmp(word[0], "mount") == 0) {
		printf("Farei o mount do %s\n", word[1]);
	}

	else if (strcmp(word[0], "cp") == 0) {
		printf("Copiarei o arquivo %s para %s\n", word[1], word[2]);
	}

	else if (strcmp(word[0], "mkdir") == 0) {
		printf("Criarei a pasta %s\n", word[1]);
	}

	else if (strcmp(word[0], "rmdir") == 0) {
		printf("Removerei a pasta %s\n", word[1]);	
	}
	else if (strcmp(word[0], "cat") == 0) {
		printf("Farei o cat do arquivo %s\n", word[1]);	
	}
	else if (strcmp(word[0], "touch") == 0) {
		printf("Farei o touch do arquivo %s\n", word[1]);	
	}
	else if (strcmp(word[0], "rm") == 0) {
		printf("Removerei o arquivo %s\n", word[1]);	
	}
	else if (strcmp(word[0], "ls") == 0) {
		printf("Farei o ls do diretorio %s\n", word[1]);	
	}
	else if (strcmp(word[0], "find") == 0) {
		printf("Farei o find do arquivo %s no diretorio %s\n", word[2], word[1]);	
	}
	else if (strcmp(word[0], "df") == 0) {
		printf("Farei o df\n");	
	}
	else if (strcmp(word[0], "umount") == 0) {
		printf("Farei o umount\n");	
	}
	else if (strcmp(word[0], "sai") == 0) {
		return 0;		
	}
	
	else if (word[0][0] != '\0'){
		fprintf(stderr, "comando %s inválido!\n", word[0]);
	}

	return 1;
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