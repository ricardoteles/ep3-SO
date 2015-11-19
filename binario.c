#include "binario.h"

void escreveInt(FILE* bin, int valor, int inicioBytes, int numInt) {
	if (bin) {
		int i;
		int myVal = valor;

		fseek(bin, inicioBytes, SEEK_SET);

		for (i = 0; i < numInt; i++)
			if (fwrite (&myVal, sizeof(int), 1, bin) != 1)
				printf("Não escreveu 1 int!\n");
	}
}

void escreveChar(FILE* bin, char valor, int inicioBytes, int nChar) {
	if (bin) {
		int i;
		char myVal = valor;

		fseek(bin, inicioBytes, SEEK_SET);

		for (i = 0; i < nChar; i++)
			if (fwrite (&myVal, sizeof(char), 1, bin) != 1)
				printf("Não escreveu 1 byte!\n");
	}
}

void escreveStruct(FILE* bin, Arquivo valor, int inicioBytes, int nStruct) {
	if (bin) {
		int i;
		Arquivo myVal = valor;

		fseek(bin, inicioBytes, SEEK_SET);

		for (i = 0; i < nStruct; i++)
			if (fwrite (&myVal, sizeof(Arquivo), 1, bin) != 1)
				printf("Não escreveu 1 struct!\n");
	}
}

int leInt(FILE* bin, int inicioBytes) {
	if (bin) {
		int val;

		fseek(bin, inicioBytes, SEEK_SET);

		if (fread(&val, sizeof(int), 1, bin) == 1) {
			return val;
		}
	}

	return 0;
}

char leChar(FILE* bin, int inicioBytes) {
	if (bin) {
		char val;
		
		fseek(bin, inicioBytes, SEEK_SET);
		
		if (fread(&val, sizeof(char), 1, bin) == 1) {
			return val;
		}
	}
	
	return 0;
}

Arquivo leStruct(FILE* bin, int inicioBytes) {
	Arquivo val;
	
	if (bin) {
		fseek(bin, inicioBytes, SEEK_SET);
		
		if (fread(&val, sizeof(Arquivo), 1, bin) == 1) {
			return val;
		}
	}
	
	return val;
}
