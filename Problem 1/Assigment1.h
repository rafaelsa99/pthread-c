#pragma once
/* alusão a funções internas */
static void lerNomesFic(char[], char[], char[]);
static void abrirCriarFic(char nF[], FILE* f, char m[]);
static void fecharFic(FILE*);
static void lerPalavra(FILE*, bool*, bool*, char[]);
static void escreverPalavra(FILE*, bool*, char[]);
char removeAccented(unsigned char ch);
int maxWord(int size1, int size2);
int isSpace(unsigned char ch);
int isApostrophe(unsigned char ch);
int isUnderscore(unsigned char ch);
int isSeparationSymbol(unsigned char ch);
int isPonctuationSymbol(unsigned char ch);
int isVowel(char* character);
int isAlphaNumeric(char* character);
void printWordLength(int largestWord);
void printWordBySize(int largestWord, int* arrayLenght);
void printRelativeLength(int largestWord, int wordCount, int* arrayLenght);
void printRelativeCountConsoant(int largestWord, int wordCount, int* arrayLength, int* arrayConsoantCount[][50]);
void fileAnalysis(char* file_name);