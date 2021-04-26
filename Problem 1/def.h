/**
 *  \file def.h (header file)
 *
 *  Generic definitions.
 *
 *  \author Rafael Sá (104552), Luís Laranjeira (81526)
 */

#ifndef DEF_H_
#define DEF_H_

/** \brief max length of file names */
#define MAX_STRING_LENGTH 255

struct PARTFILEINFO {
    int nCharacters;
    int fileId;
    int nConsoants;
    int largestWord; //numero de caracteres da maior palavra
    int wordCount; //numero de palavras
    int nWordsBySize[50];
    int nConsoantsByWordLength[50][50];
};

/** \brief flag to end the threads */
#define END_PROCESS -1

#endif /* DEF_H_ */
