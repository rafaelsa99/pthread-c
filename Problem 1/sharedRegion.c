/**
 *  \file sharedRegion.c (source file)
 *
 *  \brief Shared region definition.
 *
 *  Synchronization based on monitors.
 *
 *  \author Rafael Sá (104552), Luís Laranjeira (81526)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include "def.h"
#include "sharedRegion.h"
#include "worker.h"

/** \brief worker threads return status array */
extern int *statusWorker;
/** \brief array of file names to be processed */
char **filenames;
/** \brief number of files to be processed */
int numFiles;
/** \brief id of the file currently being processed */
int currentFileId;
/** \brief array of structs to save the file information */
struct PARTFILEINFO **fileInfo;
/** \brief file pointer */
FILE *inputStream;
/** \brief locking flag which warrants mutual exclusion inside the monitor */
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
/** \brief flag which warrants that the data transfer region is initialized exactly once */
static pthread_once_t init = PTHREAD_ONCE_INIT;
/** \brief Closes the current file, if any, and opens the next file. */
int processNextFile();
/** \brief Frees the allocated memory to the file names. */
void freeFilenames();

/**
 *  \brief Initialization of the shared region.
 *
 *  Internal operation.
 */
static void initialization (void){
	currentFileId = 0;
	if((fileInfo = malloc(sizeof(struct PARTFILEINFO *) * numFiles)) == NULL){
		perror ("Error on allocating memory to PARTFILEINFO pointers");
		free(statusWorker);
		freeFilenames();
		exit (EXIT_FAILURE);
	}
	for(int i = 0; i < numFiles;i++){
		if((fileInfo[i] = malloc(sizeof(struct PARTFILEINFO))) == NULL){
			perror ("Error on allocating memory to PARTFILEINFO");
			free(statusWorker);
			freeFilenames();
			exit (EXIT_FAILURE);
		}
		fileInfo[i]->nCharacters = 0;
		fileInfo[i]->nConsoants = 0;
		fileInfo[i]->largestWord = 0; 
		fileInfo[i]->wordCount = 0; 
		for (int j = 0; j < 50; j++) {
			fileInfo[i]->nWordsBySize[j] = 0;
			for (int k = 0; k < 50; k++)
				fileInfo[i]->nConsoantsByWordLength[j][k] = 0;
		}
	}
	processNextFile(); //Opens the first file
}

/**
 *  \brief Store the file names in the shared region.
 *
 *  \param nFileNames number of files to be processed
 *  \param fileNames array of file names to be processed
 */
void storeFileNames(int nFileNames, char **fileNames){
	int i;
	filenames = malloc(sizeof(char *) * nFileNames);
	numFiles = nFileNames;
	for(i = 0; i < nFileNames; i++){
		filenames[i] = malloc(sizeof(char) * MAX_STRING_LENGTH);
		strcpy(filenames[i], fileNames[i]);
	}
}

/**
 *  \brief Get the next chunk to be processed by a worker.
 *
 *  \param threadId thread identification
 *  \param *buffer pointer to the buffer for the chunk
 *  \param *partialInfo pointer to the struct to save the chunk information
 *
 *  \return END_PROCESS, when there is no more work to be done. 0, otherwise.
 */
int getDataChunk (int threadId, char (*buffer)[MAX_STRING_LENGTH], struct PARTFILEINFO *partialInfo){
	int ret = 0, i, j;
	char ch;
	if ((statusWorker[threadId] = pthread_mutex_lock (&mutex)) != 0){
		errno = statusWorker[threadId];
	    perror ("Error on entering monitor");
	    statusWorker[threadId] = EXIT_FAILURE;
	    pthread_exit (&statusWorker[threadId]);
	}
	pthread_once (&init, initialization);
	// Checks if all points in the file currently being processed have already been assigned to be computed
	if(fgets(*buffer, sizeof(*buffer), inputStream) == NULL){
		if((currentFileId + 1) == numFiles){ // If it has already finished the last file, it ends the processing
			ret = END_PROCESS;
		} else {
			// Processes the next file, as long as there is an error and there are files to read
			do{
				currentFileId++;
			}while(currentFileId < numFiles && processNextFile() == -1);
			// Checks if the processing of the last file gave an error and if the processing should be terminated
			if(currentFileId == numFiles){
				currentFileId--;
				ret = END_PROCESS;
			}
			else{
                fgets(*buffer, sizeof(*buffer), inputStream);
			}
		}
	}
	if(ret != END_PROCESS){
		for(i = strlen(*buffer)-1, j = 0; i >= 0; i--, j++){ //Verification if there is not an uncomplete word
            ch = (*buffer)[i];
        	 if (isPonctuationSymbol(ch) || isSpace(ch) || isSeparationSymbol(ch)){ //Check if is word delimiter
        		fseek(inputStream, -j, SEEK_CUR); //Moves the file pointer back to the last delimiter of the chunk.
        		(*buffer)[i+1] = '\0'; //Cuts the uncomplete words from the chunk.
        		break;
        	}
		} 
		//Reset struct
		partialInfo->fileId = currentFileId;
		partialInfo->nCharacters = 0;
		partialInfo->nConsoants = 0;
		partialInfo->largestWord = 0; 
		partialInfo->wordCount = 0; 
		for (int j = 0; j < 50; j++) {
			partialInfo->nWordsBySize[j] = 0;
			for (int k = 0; k < 50; k++)
				partialInfo->nConsoantsByWordLength[j][k] = 0;
		}
	}
	if ((statusWorker[threadId] = pthread_mutex_unlock (&mutex)) != 0){
		errno = statusWorker[threadId];
		perror ("Error on exiting monitor");
		statusWorker[threadId] = EXIT_FAILURE;
	    pthread_exit (&statusWorker[threadId]);
	}
	return ret;
}
/**
 *  \brief Save the information of a chunk.
 *
 *  \param threadId thread identification
 *  \param *partialInfo pointer to the struct where the chunk information is saved
 */
void savePartialResult (int threadId, struct PARTFILEINFO *partialInfo){
	if ((statusWorker[threadId] = pthread_mutex_lock (&mutex)) != 0){
		errno = statusWorker[threadId];
	    perror ("Error on entering monitor");
	    statusWorker[threadId] = EXIT_FAILURE;
	    pthread_exit (&statusWorker[threadId]);
	}
    if(fileInfo[partialInfo->fileId]->largestWord < partialInfo->largestWord)
	    fileInfo[partialInfo->fileId]->largestWord = partialInfo->largestWord;
	fileInfo[partialInfo->fileId]->nCharacters += partialInfo->nCharacters;
	fileInfo[partialInfo->fileId]->nConsoants += partialInfo->nConsoants;
	fileInfo[partialInfo->fileId]->wordCount += partialInfo->wordCount;
    for (int i = 0; i < 50; i++) {
        fileInfo[partialInfo->fileId]->nWordsBySize[i] += partialInfo->nWordsBySize[i];
    }
    for (int l = 0; l < 50; l++) {
        for (int c = 0; c < 50; c++) {
            fileInfo[partialInfo->fileId]->nConsoantsByWordLength[l][c] += partialInfo->nConsoantsByWordLength[l][c];
        }
    }

	if ((statusWorker[threadId] = pthread_mutex_unlock (&mutex)) != 0){
		errno = statusWorker[threadId];
	    perror ("Error on exiting monitor");
	    statusWorker[threadId] = EXIT_FAILURE;
	    pthread_exit (&statusWorker[threadId]);
	}
}

/**
 *  \brief Prints the final results.
 */
void printProcessingResults(){
    for (int i = 0; i < numFiles; i++) {
    	printf("\nFile name: %s\n", filenames[i]);
    	printf("Total number of words = %d\nWord Length\n", fileInfo[i]->wordCount);
        printWordLength(fileInfo[i]->largestWord);
        printWordBySize(fileInfo[i]->largestWord, fileInfo[i]->nWordsBySize);
        printRelativeLength(fileInfo[i]->largestWord, fileInfo[i]->wordCount, fileInfo[i]->nWordsBySize);
        printRelativeCountConsoant(fileInfo[i]->largestWord, fileInfo[i]->wordCount, fileInfo[i]->nWordsBySize, fileInfo[i]->nConsoantsByWordLength);
    }
}

/**
 *  \brief Opens the next file to be processed.
 *
 *  Internal operation.
 */
int processNextFile(){
	if(inputStream) 
		fclose(inputStream);
    inputStream = fopen(filenames[currentFileId], "rb");
    if(inputStream == NULL){
        fprintf(stderr,"Error on opening file %s: %s\n",filenames[currentFileId], strerror(errno));
        return -1;
    }
    return 0;
}

/**
 *  \brief Frees the allocated memory to the file names.
 */
void freeFilenames(){
	for(int i = 0; i < numFiles; i++)
		free(filenames[i]);
	free(filenames);
}

/**
 *  \brief Frees the allocated dynamic memory.
 */
void freeMemory(){
	free(statusWorker);
	for(int i = 0; i < numFiles; i++)
		free(fileInfo[i]);
	free(fileInfo);
	freeFilenames();
}

void printWordLength(int largestWord){
    int i;
    for (i = 1; i <= largestWord; i++)
    {
        printf("\t");
        printf("%d",i);
    }
    printf("\n");
}

void printWordBySize(int largestWord, int *arrayLenght) {
    for (int i = 1; i <= largestWord; i++)
    {
        printf("\t");
        printf("%d", arrayLenght[i]);
    }
    printf("\n");
}

void printRelativeLength(int largestWord, int wordCount,int* arrayLenght) {
    for (int i = 1; i <= largestWord; i++)
    {
        printf("\t");
        printf("%0.2f", (((double)arrayLenght[i]/ (double)wordCount)*100));
    }
    printf("\n");
}

void printRelativeCountConsoant(int largestWord, int wordCount, int* arrayLength, int arrayConsoantCount[50][50]) {
    int value;
    for (int lign = 0; lign <= largestWord; lign++){
        printf("%d", lign);
        for (int column = 1; column <= largestWord; column++)
        {
            printf("\t");
            if(lign <= column){
            	value = arrayConsoantCount[lign][column];
            	if(arrayLength[column]){
                	printf("%0.1f", ((double)value / (double)arrayLength[column]) * 100);
            	}
            	else {
                	printf("%0.1f", (double)0);
            	}
            }
        }
        printf("\n");
    }
}

