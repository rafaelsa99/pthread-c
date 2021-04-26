/**
 *  \file worker.h (header file)
 *
 *  \brief Worker thread definition.
 * *
 *  Definition of the operations carried out by the worker thread:
 *     \li *worker
 *     \li computeValue
 *
 *  \author Rafael Sá (104552), Luís Laranjeira (81526)
 */

#ifndef WORKER_H_
#define WORKER_H_

/**
 *  \brief Function worker.
 *
 *  Its role is to simulate the life cycle of a worker.
 *
 *  \param arg pointer to application defined worker identification
 */
void *worker(void *arg);


void processDataChunk(char *buffer, struct PARTFILEINFO *partialInfo);

int maxWord(int size1, int size2);
char removeAccented(unsigned char ch);
int isSpace(unsigned char ch);
int isApostrophe(unsigned char ch);
int isUnderscore(unsigned char ch);
int isSeparationSymbol(unsigned char ch);
int isPonctuationSymbol(unsigned char ch);
int isVowel(char character);
int isAlphaNumeric(char character);

#endif /* WORKER_H_ */
