/**
 *  \file sharedRegion.h (header file)
 *
 *  \brief Shared region definition.
 *
 *  Synchronization based on monitors.
 *
 *  Definition of the operations carried out on the shared region:
 *     \li storeFileNames
 *     \li processConvPoint
 *     \li savePartialResult
 *     \li checkProcessingResults
 *     \li freeMemory.
 *
 *  \author Rafael Sá (104552), Luís Laranjeira (81526)
 */

#ifndef SHAREDREGION_H_
#define SHAREDREGION_H_

/**
 *  \brief Store the file names in the shared region.
 *
 *  \param nFileNames number of files to be processed
 *  \param fileNames array of file names to be processed
 */
void storeFileNames(int nFileNames, char **fileNames);

int getDataChunk (int threadId, char *buffer, struct PARTFILEINFO *partialInfo);

void savePartialResult (int threadId, struct PARTFILEINFO *partialInfo);

void printProcessingResults();
/**
 *  \brief Frees the allocated dynamic memory.
 */
void freeMemory();

void printWordLength(int largestWord);

void printWordBySize(int largestWord, int *arrayLenght);

void printRelativeLength(int largestWord, int wordCount,int* arrayLenght);

void printRelativeCountConsoant(int largestWord, int wordCount, int* arrayLength, int arrayConsoantCount[50][50]);


#endif /* SHAREDREGION_H_ */
