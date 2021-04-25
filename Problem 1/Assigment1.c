#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Assigment1.h"

/* Definição do tamanho do buufer de leitura*/
#define BUFFER_SIZE 255 /* nº máximo de caracteres do buffer */
#define BITS sizeof(int) * 8 // Total bits required to represent integer

int main(int argc, char* argv[])
{
	char* file_name;
	for (int i = 1; i < argc; i++)
	{
		file_name = argv[i];
		fileAnalysis(file_name);
	}
}

int maxWord(int size1, int size2) {
	if (size1 <= size2)
		return size2;
	else
		return size1;
}

char removeAccented(unsigned char ch){
	if ((ch >= 0xA0 && ch <= 0xA3) || (ch >= 0x80 && ch <= 0x83))
		return 'a';

	if ((ch >= 0xA8 && ch <= 0xAA) || (ch >= 0x88 && ch <= 0x8A))
		return 'e';

	if ((ch >= 0xAC && ch <= 0xAD) || (ch >= 0x8C && ch <= 0x8D))
		return 'i';

	if ((ch >= 0xB2 && ch <= 0xB5) || (ch >= 0x92 && ch <= 0x95))
		return 'o';

	if ((ch >= 0xB9 && ch <= 0xBB) || (ch >= 0x99 && ch <= 0x9B))
		return 'u';

	if (ch == 0xA7 || ch == 0x87)
		return 'c';

	return ch;
}

int isSpace(unsigned char ch) {
	if (ch == 0x20 || ch == 0x9 || ch == 0xA)
	{
		return 1;

	}
	return 0;
}

int isApostrophe(unsigned char ch) {
	if (ch == 0x27)
	{
		return 1;

	}
	return 0;
}

int isUnderscore(unsigned char ch) {
	if (ch == 0x5f)
	{
		return 1;

	}
	return 0;
}

int isSeparationSymbol(unsigned char ch) {
	if (ch == 0x22 || ch == 0x5b || ch == 0x5d || ch == 0x28 || ch == 0x29 || ch == 0x2d)
	{
		return 1;

	}
	return 0;
}

int isPonctuationSymbol(unsigned char ch) {
	if (ch == 0x2e || ch == 0x2c || ch == 0x3a || ch == 0x3b || ch == 0x3f || ch == 0x21)
	{
		return 1;

	}
	return 0;
}

/* Verificar se é vogal
* Procedimento * Parâmetros de entrada:
* character ---- character a avaliar
* Parâmetro de saída:
* bool ---- devolve verdadeiro se for vogal
*/
int isVowel(char* character) {
	if (character == 'a' || character == 'e' || character == 'i' || character == 'o' || character == 'u' || character == "y")
	{
		return 1;

	}
	return 0;
}

int isAlphaNumeric(char* character) {
	if ((character >= 65 && character <= 90) || (character >= 97 && character <= 122) || (character >= 48 && character <= 57) || character == 95 || character == 39) {
		return 1;
	}
	return 0;
}

void printWordLength(int largestWord){
	int i;
	for (i = 1; i <= largestWord; i++)
	{
		printf("\t");
		printf("%d",i);
	}
	printf("\n");
}

void printWordBySize(int largestWord, int *arrayLenght) {
	for (int i = 1; i <= largestWord; i++)
	{
		printf("\t");
		printf("%d", arrayLenght[i]);
	}
	printf("\n");
}

void printRelativeLength(int largestWord, int wordCount,int* arrayLenght) {
	for (int i = 1; i <= largestWord; i++)
	{
		printf("\t");
		//double val = ((double)arrayLenght[i] / (double)wordCount);
		printf("%0.2f", (((double)arrayLenght[i]/ (double)wordCount)*100));
	}
	printf("\n");
}

void printRelativeCountConsoant(int largestWord, int wordCount, int* arrayLength, int* arrayConsoantCount[][50]) {
	int value;
	for (int lign = 0; lign <= largestWord; lign++){
		printf("%d", lign);
		for (int column = 1; column <= largestWord; column++)
			{
			printf("\t");
			value = arrayConsoantCount[lign][column];
			if(arrayLength[column]){
				printf("%0.2f", ((double)value / (double)arrayLength[column]) * 100);
			}
			else {
				printf("%0.2f", (double)0);

			}
		}
		printf("\n");
	}

}

void fileAnalysis(char* file_name) {
	FILE* inputStream;
	int nCharacters = 0;
	int nConsoants = 0;
	int nWords = 0;
	int largestWord = 0; //numero de caracteres da maior palavra
	int wordCount = 0; //numero de palavras
	char buffer[BUFFER_SIZE];
	char ch;
	int inWord = 0;
	int wordSize = 0;
	int nCharactersWord = 0;
	int nConsoantsWord = 0;
	int nWordsBySize[50];
	int nConsoantsByWordLength[50][50];

	for (int i = 0; i < 50; i++) {
		nWordsBySize[i] = 0;
	}

	for (int j = 0; j < 50; j++)
	{
		for (int k = 0; k < 50; k++)
		{
			nConsoantsByWordLength[j][k] = 0;
		}
	}

	fopen_s(&inputStream, file_name, "rb");

	while (fgets(buffer, sizeof(buffer), inputStream) != NULL)
	{
		for (int i = 0; i < sizeof(buffer); i++)
		{
			if ((unsigned char)buffer[i] == 0xc3) {
				i++;
				ch = removeAccented((unsigned char)buffer[i]);
			}
			else if ((unsigned char)buffer[i] == 0xe2)
			{
				i = i + 2;
				if ((unsigned char)buffer[i] == 0x98 || (unsigned char)buffer[i] == 0x99)
				{
					ch = 0x27;
				}
				else if ((unsigned char)buffer[i] == 0x9c || (unsigned char)buffer[i] == 0x9d) {
					ch = 0x22;
				}
				else if ((unsigned char)buffer[i] == 0x93 || (unsigned char)buffer[i] == 0xa6) {
					ch = 0x2e;
				}
			}
			else
			{
				ch = buffer[i];
			}

			if (isPonctuationSymbol(ch) || isSpace(ch) || isSeparationSymbol(ch)) {
				inWord = 0;
				largestWord = maxWord(largestWord, nCharactersWord);
				nCharacters += nCharactersWord;
				nConsoants += nConsoantsWord;
				nWordsBySize[nCharactersWord] = nWordsBySize[nCharactersWord] + 1;
				nConsoantsByWordLength[nConsoantsWord][nCharactersWord]++;
				nCharactersWord = 0;
				nConsoantsWord = 0;
			}
			else if ((isAlphaNumeric(ch) || isUnderscore(ch)) && !inWord)
			{
				inWord = 1;
				wordCount++;
			}
			if (inWord)
			{
				ch = tolower(ch);
				if (!isVowel(ch) && !isUnderscore(ch))
				{
					nConsoantsWord++;
				}
				if (!isApostrophe(ch))
				{
					nCharactersWord++;
				}
			}
			if (buffer[i] == '\0') {
				break;
			}
		}
	}
	if (feof(inputStream))
	{
		// hit end of file
	}
	else
	{
		// some other error interrupted the read
	}

	//}
	fclose(inputStream);

	printf("N de Caracteres: %d\n", nCharacters);
	printf("N de Consoantes: %d\n", nConsoants);
	printf("N de Palavras: %d\n", wordCount);
	printf("+ Palavra: %d\n", largestWord);

	printWordLength(largestWord);
	printWordBySize(largestWord, nWordsBySize);
	printRelativeLength(largestWord, wordCount, nWordsBySize);
	printRelativeCountConsoant(largestWord, wordCount, nWordsBySize, nConsoantsByWordLength);
}

/* Abertura de um ficheiro
* Procedimento * Parâmetros de entrada:
* nF ---- ponteiro para o nome do ficheiro
* m ---- modo de acesso
* Parâmetro de saída:
* f ---- ponteiro para o stream associado ao ficheiro
* (passado por referência)
*/
static void abrirCriarFic(char nF[], FILE** f, char m[])
{
	if ((fopen_s(f, nF, m)) == NULL)
	{
		perror("erro na abertura/criação de um ficheiro");
		exit(EXIT_FAILURE);
	}
}
/* Fecho de um ficheiro
* Procedimento * Parâmetro de entrada:
* f ---- ponteiro para o stream associado ao ficheiro
*/
static void fecharFic(FILE* f)
{
	if (fclose(f) == EOF)
	{
		perror("erro no fecho de um ficheiro");
		exit(EXIT_FAILURE);
	}
}

/* Leitura de uma palavra
* Procedimento
* Parâmetro de entrada:
* f ---- ponteiro para o 'stream' associado ao ficheiro
* Parâmetros de saída:
* fFim ---- sinalização de que a leitura dos ficheiros de entrada chegou
* ao fim (passada por referência)
* nPal ---- sinalização de leitura de uma nova palavra (passada por referência)
* pal ---- palavra a ler (passada por referência)
*/
void lerPalavra(FILE* f, bool* fFim, bool* nPal, char pal[])
{
	int stat; /* status da operação */
	do
	{
		stat = fscanf_s(f, "%19s", pal);
		fscanf_s(f, "%*[^\n]");
		fscanf_s(f, "%*c");
	} while (stat == 0);
	if (stat == 1) *nPal = true;
	else if (stat == EOF) *fFim = true;
}
