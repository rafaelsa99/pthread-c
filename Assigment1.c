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
	char* file_name = argv[1];
	FILE *inputStream;
	int nCharacters = 0;
	int nConsoants = 0;
	int nWords = 0;
	char buffer[BUFFER_SIZE];
	unsigned char auxArray[4];
	unsigned char a;
	int aux = 0;
	int flag = 0;
	char ch;

	fprintf(stdout, file_name);

	//abrirCriarFic(file_name, &inputStream, "r");
	fopen_s(&inputStream, "text0.txt", "rb");
	fseek(inputStream, 0, SEEK_END);
	long int size = ftell(inputStream); // now you got size of file in bytes
	fseek(inputStream, 0, SEEK_SET);

	for (int i = 0; i < size; i++)
	{
		fread(&a, sizeof(unsigned char), 1, inputStream); // you read one int (sizeof(int)!=1 byte)
		ch = removeAccented((unsigned char)a);
		printf("\n%x", ch);
		if(ch != ' ' && ch != '\0' && isAlphaNumeric(ch))
		{
			flag = 1;
			ch = tolower(ch);
			if (!isVowel(&ch))
			{
				nConsoants++;
			}
			nCharacters++;
		}else if ((unsigned char)ch == 0xC3)
		{
			//this is a special char needs to be converted
			//flag = 1;
			continue;
		}
		else if ((unsigned char)ch == 0x20 || (unsigned char)ch == 0x29 || (unsigned char)ch == 0xA)
		{
			flag = 0;
			nCharacters++;
			continue;
		}
		else if(flag)
		{
			nWords++;
		}
	}
	fclose(inputStream);

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

/* Escrita de uma palavra
* Procedimento
* Parâmetro de entrada:
* f ---- ponteiro para o stream associado ao ficheiro
* pal ---- palavra a escrever
* Parâmetro de saída:
* nPal ---- sinalização de existência de uma nova palavra
* (passada por referência)
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

/* Escrita no écran do monitor vídeo do conteúdo de um registo
* Procedimento
* Parâmetros de entrada:
* r ---- nº de ordem do registo
* reg ---- registo (passado por referência)
*/
static void escreverStdoutReg(unsigned int r)
{
	fprintf(stdout, "\nRegisto nº%u\n", r);
	fprintf(stdout, "\nCarregue em Enter para continuar");
	fscanf_s(stdin, "%*[^\n]");
	fscanf_s(stdin, "%*c");
}

/* Leitura dos nomes dos ficheiros
* Procedimento
* Parâmetros de saída :
*nFe1----nome do primeiro ficheiro de entrada(passado por referência)
* nFe2----nome do segundo ficheiro de entrada(passado por referência)
* nFs----nome do ficheiro de saída(passado por referência)
*/
static void lerNomesFic(char nFe1[], char nFe2[], char nFs[])
{
	fprintf(stdout, "\nNome dos ficheiros de entrada? ");
	fscanf_s(stdin, "%39s%39s", nFe1, nFe2);
	fscanf_s(stdin, "%*[^\n]");
	fscanf_s(stdin, "%*c");
	fprintf(stdout, "Nome do ficheiro de saída? ");
	fscanf_s(stdin, "%39s", nFs);
	fscanf_s(stdin, "%*[^\n]");
	fscanf_s(stdin, "%*c");
}