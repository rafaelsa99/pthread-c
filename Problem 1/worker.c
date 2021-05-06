/**
 *  \file worker.c (source file)
 *
 *  \brief Worker thread definition.
 * *
 *  Definition of the operations carried out by the worker thread:
 *     \li *worker
 *     \li processDataChunk
 *	   \li Auxiliar functions for the processing
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
 *  \brief Function worker.
 *
 *  Its role is to simulate the life cycle of a worker.
 *
 *  \param arg pointer to application defined worker identification
 */
void *worker(void *arg){
	unsigned int threadId = *((unsigned int *) arg);
	struct PARTFILEINFO partialInfo;
	char buff[MAX_STRING_LENGTH];
    while (getDataChunk (threadId, &buff, &partialInfo) != END_PROCESS)
    {
        processDataChunk (buff, &partialInfo);
        savePartialResult (threadId, &partialInfo);
    }
	statusWorker[threadId] = EXIT_SUCCESS;
	pthread_exit (&statusWorker[threadId]);
}

/**
 *  \brief Processa data chunk.
 *
 *  Internal thread operation.
 *
 *  \param buffer chunk to be processed
 *  \param *partialInfo struct to save the chunk information
 */
void processDataChunk(char buffer[MAX_STRING_LENGTH], struct PARTFILEINFO *partialInfo){
    int inWord = 0;
    int nCharactersWord = 0;
    int nConsoantsWord = 0;
    char ch;
    for (int i = 0; i < MAX_STRING_LENGTH; i++)
    {
        if ((unsigned char)buffer[i] == 0xc3) { //Second bit more significant
            i++;
            ch = removeAccented((unsigned char)buffer[i]);
        }
        else if ((unsigned char)buffer[i] == 0xc2) { //Second bit more significant
            i++;
            if ((unsigned char)buffer[i] == 0xB4) //Accent to apostrophe
            {
                ch = 0x27;
            } else if ((unsigned char)buffer[i] == 0xAB || (unsigned char)buffer[i] == 0xBB) { //Left and right angled double quotation marks to double quotation mark
                ch = 0x22;
            }
        }
        else if ((unsigned char)buffer[i] == 0xe2) //Third bit more significant
        {
            i = i + 2;
            if ((unsigned char)buffer[i] == 0x98 || (unsigned char)buffer[i] == 0x99) //Left and right single quotation marks to apostrophe
            {
                ch = 0x27;
            }
            else if ((unsigned char)buffer[i] == 0x9c || (unsigned char)buffer[i] == 0x9d) { //Left and right double quotation marks to double quotation mark
                ch = 0x22;
            }
            else if ((unsigned char)buffer[i] >= 0x90 || (unsigned char)buffer[i] <= 0x94) { //Dash to hyphen
                ch = 0x2d;
            }
            else if ((unsigned char)buffer[i] == 0xa6) { //Ellipsis to full point
                ch = 0x2e;
            }
        }
        else
        {
        	if(buffer[i] == 0x60) //Accent to apostrophe
        		ch = 0x27;
            else
            	ch = buffer[i];
        }
		ch = tolower(ch);
        if ((isPonctuationSymbol(ch) || isSpace(ch) || isSeparationSymbol(ch)) && inWord) { //Check if is delimiter
            inWord = 0;
            partialInfo->largestWord = maxWord(partialInfo->largestWord, nCharactersWord);
            partialInfo->nCharacters += nCharactersWord;
            partialInfo->nConsoants += nConsoantsWord;
            partialInfo->nWordsBySize[nCharactersWord]++;
            partialInfo->nConsoantsByWordLength[nConsoantsWord][nCharactersWord]++;
            nCharactersWord = 0;
            nConsoantsWord = 0;   
        }
        else if (((isAlphaNumeric(ch) || isUnderscore(ch))) && !inWord) //Check if is start of word
        {
            inWord = 1;
            partialInfo->wordCount++;
        }
        if (inWord && !isApostrophe(ch)) //Counters on word
        {
            if (!isVowel(ch) && !isUnderscore(ch) && !isNumeric(ch))
            {
                nConsoantsWord++;
            }
            nCharactersWord++;
        }
        
        if (buffer[i] == '\0') {
            break;
        }
    }
}

/**
 *  \brief Returns the biggest of two sizes.
 *
 *  Internal thread operation.
 *
 *  \param size1 first size
 *  \param size2 second size
 *
 *  \return biggest size between the two
 */
int maxWord(int size1, int size2) {
    if (size1 <= size2)
        return size2;
    else
        return size1;
}
/**
 *  \brief Removes accent from vowels. Transform from multibyte character to single byte.
 *
 *  Internal thread operation.
 *
 *  \param ch multibyte character
 *
 *  \return single byte character
 */
char removeAccented(unsigned char ch){
	
    if ((ch >= 0xA0 && ch <= 0xA5) || (ch >= 0x80 && ch <= 0x85))
        return 'a';

    if ((ch >= 0xA8 && ch <= 0xAB) || (ch >= 0x88 && ch <= 0x8B))
        return 'e';

    if ((ch >= 0xAC && ch <= 0xAF) || (ch >= 0x8C && ch <= 0x8F))
        return 'i';

    if ((ch >= 0xB2 && ch <= 0xB6) || (ch >= 0x92 && ch <= 0x96))
        return 'o';

    if ((ch >= 0xB9 && ch <= 0xBB) || (ch >= 0x99 && ch <= 0x9B)) 
        return 'u';

    if (ch == 0xA7 || ch == 0x87)
        return 'c';
        
    if (ch == 0xBC || ch == 0x9C)
        return 0x27;
        
    if (ch == 0xBF || ch == 0xBD || ch == 0x9D)
        return 'y';

    return ch;
}
/**
 *  \brief Check if char is space,tab, newline or carriage return.
 *
 *  Internal thread operation.
 *
 *  \param ch character
 *
 *  \return 1 if it is. 0 otherwise.
 */
int isSpace(unsigned char ch) {
    if (ch == 0x20 || ch == 0x9 || ch == 0xA)
    {
        return 1;

    }
    return 0;
}
/**
 *  \brief Check if char is an apostrophe.
 *
 *  Internal thread operation.
 *
 *  \param ch character
 *
 *  \return 1 if it is. 0 otherwise.
 */
int isApostrophe(unsigned char ch) {
    if (ch == 0x27)
    {
        return 1;

    }
    return 0;
}
/**
 *  \brief Check if char is an underscore.
 *
 *  Internal thread operation.
 *
 *  \param ch character
 *
 *  \return 1 if it is. 0 otherwise.
 */
int isUnderscore(unsigned char ch) {
    if (ch == 0x5f)
    {
        return 1;

    }
    return 0;
}
/**
 *  \brief Check if char is a separation symbol (hyphen, double quotation mark, bracket or parentheses).
 *
 *  Internal thread operation.
 *
 *  \param ch character
 *
 *  \return 1 if it is. 0 otherwise.
 */
int isSeparationSymbol(unsigned char ch) {
    if (ch == 0x22 || ch == 0x5b || ch == 0x5d || ch == 0x28 || ch == 0x29 || ch == 0x2d)
    {
        return 1;

    }
    return 0;
}
/**
 *  \brief Check if char is a ponctuation symbol (full point, comma, colon, semicolon, question mark or exclamation point).
 *
 *  Internal thread operation.
 *
 *  \param ch character
 *
 *  \return 1 if it is. 0 otherwise.
 */
int isPonctuationSymbol(unsigned char ch) {
    if (ch == 0x2e || ch == 0x2c || ch == 0x3a || ch == 0x3b || ch == 0x3f || ch == 0x21)
    {
        return 1;

    }
    return 0;
}

/**
 *  \brief Check if char is a vowel.
 *
 *  Internal thread operation.
 *
 *  \param ch character
 *
 *  \return 1 if it is. 0 otherwise.
 */
int isVowel(char character) {
    if (character == 'a' || character == 'e' || character == 'i' || character == 'o' || character == 'u' || character == 'y')
    {
        return 1;

    }
    return 0;
}
/**
 *  \brief Check if char is an alphanumerical character.
 *
 *  Internal thread operation.
 *
 *  \param ch character
 *
 *  \return 1 if it is. 0 otherwise.
 */
int isAlphaNumeric(char character) {
    if ((character >= 65 && character <= 90) || (character >= 97 && character <= 122) || (character >= 48 && character <= 57)) {
        return 1;
    }
    return 0;
}
/**
 *  \brief Check if char is a numeric character.
 *
 *  Internal thread operation.
 *
 *  \param ch character
 *
 *  \return 1 if it is. 0 otherwise.
 */
int isNumeric(char character) {
    if (character >= 48 && character <= 57) {
        return 1;
    }
    return 0;
}
