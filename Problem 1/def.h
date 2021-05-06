/**
 *  \file def.h (header file)
 *
 *  Generic definitions.
 *
 *  \author Rafael Sá (104552), Luís Laranjeira (81526)
 */

#ifndef DEF_H_
#define DEF_H_

/** \brief max length of strings */
#define MAX_STRING_LENGTH 255

/** \brief struct for saving partial and total results */
struct PARTFILEINFO {
    int nCharacters; //Number of characteres 
    int fileId;	//File identifier
    int nConsoants; //Number of consoants
    int largestWord; //Number of characters of the largest word
    int wordCount; //Number of words
    int nWordsBySize[50]; //Number of words for each word size
    int nConsoantsByWordLength[50][50]; //Number of consoants for each word size
};

/** \brief flag to end the threads */
#define END_PROCESS -1

#endif /* DEF_H_ */
