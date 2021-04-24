#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Assigment1.h"

/* Defini��o do tamanho do buufer de leitura*/
#define BUFFER_SIZE 255 /* n� m�ximo de caracteres do buffer */
#define BITS sizeof(int) * 8 // Total bits required to represent integer

int main(int argc, char* argv[])
{
	char* file_name = argv[1];
	FILE *inputStream;
	int nCharacters = 0;
	int nConsoants = 0;
	int nWords = 0;
	char buffer[BUFFER_SIZE];
	unsigned char auxArray[4];
	char a;
	int aux = 0;

	fprintf(stdout, file_name);

	//abrirCriarFic(file_name, &inputStream, "r");
	fopen_s(&inputStream, "text0.txt", "rb");
	fseek(inputStream, 0, SEEK_END);
	long int size = ftell(inputStream); // now you got size of file in bytes
	fseek(inputStream, 0, SEEK_SET);    // same as rewind(fp)

	for (int i = 0; i < size; i++)
	{
		fread(&a, sizeof(char), 1, inputStream); // you read one int (sizeof(int)!=1 byte)
		printf("\n%x", a);
		//printf("%d",(a >> 8) == -1);
		if ((a >> 8) == -1) {
			auxArray[aux] = a;
			aux++;
		}
		else {
			aux = 0;
		}
		printf("%x", auxArray);
	}
	fclose(inputStream);
	getch();

	//while (fread_s(buffer, sizeof(buffer), ELEMENTSIZE, ELEMENTCOUNT, &inputStream) != NULL)
	//{
	//	for (int i = 0; i < sizeof buffer; i++)
	//	{
	//		while (buffer[i] != '\0')
	//		{
	//			while (buffer[i] != ' ' && buffer[i] != '\0' && !isAlphaNumeric(buffer[i]))
	//			{
	//				result = tolower(buffer[i]);
	//				if (!isVowel(&result))
	//				{
	//					nConsoants++;
	//				}
	//				nCharacters++;
	//				i++;
	//			}
	//			nWords++;
	//			i++;
	//		}
	//	}
	//}
	//if (feof(inputStream))
	//{
	//	// hit end of file
	//}
	//else
	//{
	//	// some other error interrupted the read
	//}

	printf("N de Caracteres: %d\n", nCharacters);
	printf("N de Consoantes: %d\n", nConsoants);
	printf("N de Palavras: %d\n", nWords);
}

/* Verificar se � vogal
* Procedimento * Par�metros de entrada:
* character ---- character a avaliar
* Par�metro de sa�da:
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

char handleUTF8(char character) {
	if ((character & 0xC0) == 0x80)
	{
		// Handle multi-byte
	}
}

/* Abertura de um ficheiro
* Procedimento * Par�metros de entrada:
* nF ---- ponteiro para o nome do ficheiro
* m ---- modo de acesso
* Par�metro de sa�da:
* f ---- ponteiro para o stream associado ao ficheiro
* (passado por refer�ncia)
*/
static void abrirCriarFic(char nF[], FILE** f, char m[])
{
	if ((fopen_s(f, nF, m)) == NULL)
	{
		perror("erro na abertura/cria��o de um ficheiro");
		exit(EXIT_FAILURE);
	}
}
/* Fecho de um ficheiro
* Procedimento * Par�metro de entrada:
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
* Par�metro de entrada:
* f ---- ponteiro para o 'stream' associado ao ficheiro
* Par�metros de sa�da:
* fFim ---- sinaliza��o de que a leitura dos ficheiros de entrada chegou
* ao fim (passada por refer�ncia)
* nPal ---- sinaliza��o de leitura de uma nova palavra (passada por refer�ncia)
* pal ---- palavra a ler (passada por refer�ncia)
*/
void lerPalavra(FILE* f, bool* fFim, bool* nPal, char pal[])
{
	int stat; /* status da opera��o */
	do
	{
		stat = fscanf_s(f, "%19s", pal);
		fscanf_s(f, "%*[^\n]");
		fscanf_s(f, "%*c");
	} while (stat == 0);
	if (stat == 1) *nPal = true;
	else if (stat == EOF) *fFim = true;
}

/* Escrita de uma palavra
* Procedimento
* Par�metro de entrada:
* f ---- ponteiro para o stream associado ao ficheiro
* pal ---- palavra a escrever
* Par�metro de sa�da:
* nPal ---- sinaliza��o de exist�ncia de uma nova palavra
* (passada por refer�ncia)
*/
void escreverPalavra(FILE* f, bool* nPal, char pal[])
{
	if (fprintf(f, "%s\n", pal) < 0)
	{
		perror("erro na escrita no ficheiro");
		exit(EXIT_FAILURE);
	}
	*nPal = false;
}

/* Escrita no �cran do monitor v�deo do conte�do de um registo
* Procedimento
* Par�metros de entrada:
* r ---- n� de ordem do registo
* reg ---- registo (passado por refer�ncia)
*/
static void escreverStdoutReg(unsigned int r)
{
	fprintf(stdout, "\nRegisto n�%u\n", r);
	fprintf(stdout, "\nCarregue em Enter para continuar");
	fscanf_s(stdin, "%*[^\n]");
	fscanf_s(stdin, "%*c");
}

/* Leitura dos nomes dos ficheiros
* Procedimento
* Par�metros de sa�da :
*nFe1----nome do primeiro ficheiro de entrada(passado por refer�ncia)
* nFe2----nome do segundo ficheiro de entrada(passado por refer�ncia)
* nFs----nome do ficheiro de sa�da(passado por refer�ncia)
*/
static void lerNomesFic(char nFe1[], char nFe2[], char nFs[])
{
	fprintf(stdout, "\nNome dos ficheiros de entrada? ");
	fscanf_s(stdin, "%39s%39s", nFe1, nFe2);
	fscanf_s(stdin, "%*[^\n]");
	fscanf_s(stdin, "%*c");
	fprintf(stdout, "Nome do ficheiro de sa�da? ");
	fscanf_s(stdin, "%39s", nFs);
	fscanf_s(stdin, "%*[^\n]");
	fscanf_s(stdin, "%*c");
}