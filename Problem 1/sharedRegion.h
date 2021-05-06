/**
 *  \file sharedRegion.h (header file)
 *
 *  \brief Shared region definition.
 *
 *  Synchronization based on monitors.
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
/**
 *  \brief Get the next chunk to be processed by a worker.
 *
 *  \param threadId thread identification
 *  \param *buffer pointer to the buffer for the chunk
 *  \param *partialInfo pointer to the struct to save the chunk information
 *
 *  \return END_PROCESS, when there is no more work to be done. 0, otherwise.
 */
int getDataChunk (int threadId, char (*buffer)[MAX_STRING_LENGTH], struct PARTFILEINFO *partialInfo);
/**
 *  \brief Save the information of a chunk.
 *
 *  \param threadId thread identification
 *  \param *partialInfo pointer to the struct where the chunk information is saved
 */
void savePartialResult (int threadId, struct PARTFILEINFO *partialInfo);
/**
 *  \brief Prints the final results.
 */
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
