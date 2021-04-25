/**
 *  \file sharedRegion.h (interface file)
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
/**
 *  \brief Get the next point to be computed by the thread.
 *
 *  \param threadId thread identification
 *  \param fileId file identification
 *  \param n signal length
 *  \param xArr signal x
 *  \param yArr signal y
 *  \param point point to be computed
 *
 *  \return END_PROCESS, when there is no more work to be done. 0, otherwise.
 */
int processConvPoint (int threadId, int *fileId, int *n, double **xArr, double **yArr, int *point);
/**
 *  \brief Save the value calculated by the thread.
 *
 *  \param threadId thread identification
 *  \param fileId file identification
 *  \param point point computed
 *  \param val value computed by the thread
 */
void savePartialResult (int threadId, int fileId, int point, double val);
/**
 *  \brief Comparison of the computed signals by the threads with the previous computed signals.
 */
void checkProcessingResults();
/**
 *  \brief Frees the allocated dynamic memory.
 */
void freeMemory();

#endif /* SHAREDREGION_H_ */
