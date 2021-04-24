#pragma once
/* alusão a funções internas */
static void lerNomesFic(char[], char[], char[]);
static void abrirCriarFic(char nF[], FILE* f, char m[]);
static void fecharFic(FILE*);
static void lerPalavra(FILE*, bool*, bool*, char[]);
static void escreverPalavra(FILE*, bool*, char[]);
char removeAccented(unsigned char ch);