/**
 *  \file worker.c (source file)
 *
 *  \brief Worker thread definition.
 * *
 *  Definition of the operations carried out by the worker thread:
 *     \li *worker
 *     \li computeValue
 *
 *  \author Rafael Sá (104552), Luís Laranjeira (81526)
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include "def.h"
#include "sharedRegion.h"
#include "worker.h"

/** \brief worker threads return status array */
extern int *statusWorker;

/**
 *  \brief Get the next point to be computed by the thread.
 *
 *  \param threadId thread identification
 *  \param fileId pointer to the file identification
 *  \param n pointer to the signal length
 *  \param xArr pointer to the signal x array
 *  \param yArr pointer to the signal y array
 *  \param point pointer to the point to be computed
 *
 *  \return END_PROCESS, when there is no more work to be done. 0, otherwise.
 */
void *worker(void *arg){
	unsigned int threadId = *((unsigned int *) arg);
	struct PARTFILEINFO partialInfo;
	char buff[MAX_STRING_LENGTH];
    while (getDataChunk (threadId, buff, &partialInfo) != END_PROCESS)
    {
        processDataChunk (buff, &partialInfo);
        savePartialResult (threadId, &partialInfo);
    }
	statusWorker[threadId] = EXIT_SUCCESS;
	pthread_exit (&statusWorker[threadId]);
}


void processDataChunk(char *buffer, struct PARTFILEINFO *partialInfo){
    int inWord = 0;
    int nCharactersWord = 0;
    int nConsoantsWord = 0;
    char ch;

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
            partialInfo->largestWord = maxWord(partialInfo->largestWord, nCharactersWord);
            partialInfo->nCharacters += nCharactersWord;
            partialInfo->nConsoants += nConsoantsWord;
            partialInfo->nWordsBySize[nCharactersWord]++;
            partialInfo->nConsoantsByWordLength[nConsoantsWord][nCharactersWord]++;
            nCharactersWord = 0;
            nConsoantsWord = 0;
        }
        else if ((isAlphaNumeric(ch) || isUnderscore(ch)) && !inWord)
        {
            inWord = 1;
            partialInfo->wordCount++;
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
int isVowel(char character) {
    if (character == 'a' || character == 'e' || character == 'i' || character == 'o' || character == 'u' || character == 'y')
    {
        return 1;

    }
    return 0;
}

int isAlphaNumeric(char character) {
    if ((character >= 65 && character <= 90) || (character >= 97 && character <= 122) || (character >= 48 && character <= 57) || character == 95 || character == 39) {
        return 1;
    }
    return 0;
}
