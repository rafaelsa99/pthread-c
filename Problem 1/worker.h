/**
 *  \file worker.h (header file)
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
/**
 *  \brief Processa data chunk.
 *
 *  Internal thread operation.
 *
 *  \param buffer chunk to be processed
 *  \param *partialInfo struct to save the chunk information
 */
void processDataChunk(char buffer[MAX_STRING_LENGTH], struct PARTFILEINFO *partialInfo);
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
int maxWord(int size1, int size2);
/**
 *  \brief Removes accent from vowels. Transform from multibyte character to single byte.
 *
 *  Internal thread operation.
 *
 *  \param ch multibyte character
 *
 *  \return single byte character
 */
char removeAccented(unsigned char ch);
/**
 *  \brief Check if char is space,tab, newline or carriage return.
 *
 *  Internal thread operation.
 *
 *  \param ch character
 *
 *  \return 1 if it is. 0 otherwise.
 */
int isSpace(unsigned char ch);
/**
 *  \brief Check if char is an apostrophe.
 *
 *  Internal thread operation.
 *
 *  \param ch character
 *
 *  \return 1 if it is. 0 otherwise.
 */
int isApostrophe(unsigned char ch);
/**
 *  \brief Check if char is an underscore.
 *
 *  Internal thread operation.
 *
 *  \param ch character
 *
 *  \return 1 if it is. 0 otherwise.
 */
int isUnderscore(unsigned char ch);
/**
 *  \brief Check if char is a separation symbol (hyphen, double quotation mark, bracket or parentheses).
 *
 *  Internal thread operation.
 *
 *  \param ch character
 *
 *  \return 1 if it is. 0 otherwise.
 */
int isSeparationSymbol(unsigned char ch);
/**
 *  \brief Check if char is a ponctuation symbol (full point, comma, colon, semicolon, question mark or exclamation point).
 *
 *  Internal thread operation.
 *
 *  \param ch character
 *
 *  \return 1 if it is. 0 otherwise.
 */
int isPonctuationSymbol(unsigned char ch);
/**
 *  \brief Check if char is a vowel.
 *
 *  Internal thread operation.
 *
 *  \param ch character
 *
 *  \return 1 if it is. 0 otherwise.
 */
int isVowel(char character);
/**
 *  \brief Check if char is an alphanumerical character.
 *
 *  Internal thread operation.
 *
 *  \param ch character
 *
 *  \return 1 if it is. 0 otherwise.
 */
int isAlphaNumeric(char character);
/**
 *  \brief Check if char is a numeric character.
 *
 *  Internal thread operation.
 *
 *  \param ch character
 *
 *  \return 1 if it is. 0 otherwise.
 */
int isNumeric(char character);

#endif /* WORKER_H_ */
