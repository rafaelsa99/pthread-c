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

/** \brief worker threads return status array */
extern int *statusWorker;
/** \brief array of file names to be processed */
char **filenames;
/** \brief number of files to be processed */
int numFiles;
/** \brief id of the file currently being processed */
int currentFileId;
/** \brief array for the length of the signals of each file */
int *length;
/** \brief array for the number of points assigned to the threads for each file */
int *pointsAssigned;
/** \brief array for the number of points calculated by thethreads for each file */
int *pointsCalculated;
/** \brief array for the arrays of signal x of each file */
double **x;
/** \brief array for the arrays of signal y of each file */
double **y;
/** \brief array for the arrays of previously computed signal xy of each file */
double **xyPrev;
/** \brief array for the arrays of currently computed signal xy of each file */
double **xyCurr;
/** \brief locking flag which warrants mutual exclusion inside the monitor */
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
/** \brief flag which warrants that the data transfer region is initialized exactly once */
static pthread_once_t init = PTHREAD_ONCE_INIT;

/** \brief Reads the contents of the next file to be processed. */
int processNextFile();
/** \brief Frees the allocated memory to the file names. */
void freeFilenames();
/** \brief Frees the allocated memory to the signals x and y. */
void freeSignalArrays(int fileId);

/**
 *  \brief Initialization of the shared region.
 *
 *  Internal operation.
 */
static void initialization (void){
	currentFileId = 0;
	if((x = malloc(sizeof(double *) * numFiles)) == NULL){
		perror ("Error on allocating memory to array x");
		free(statusWorker);
		freeFilenames();
		exit (EXIT_FAILURE);
	}
	if((y = malloc(sizeof(double *) * numFiles)) == NULL){
		perror ("Error on allocating memory to array y");
		free(statusWorker);
		free(x);
		freeFilenames();
		exit (EXIT_FAILURE);
	}
	if((xyPrev = malloc(sizeof(double *) * numFiles)) == NULL){
		perror ("Error on allocating memory to array xy previously computed");
		free(statusWorker);
		free(x);
		free(y);
		freeFilenames();
		exit (EXIT_FAILURE);
	}
	if((xyCurr = malloc(sizeof(double *) * numFiles)) == NULL){
		perror ("Error on allocating memory to array xy current");
		free(statusWorker);
		free(x);
		free(y);
		free(xyPrev);
		freeFilenames();
		exit (EXIT_FAILURE);
	}
	if((length = malloc(sizeof(int) * numFiles)) == NULL){
		perror ("Error on allocating memory to array length");
		free(statusWorker);
		free(x);
		free(y);
		free(xyPrev);
		free(xyCurr);
		freeFilenames();
		exit (EXIT_FAILURE);
	}
	if((pointsAssigned = malloc(sizeof(int) * numFiles)) == NULL){
		perror ("Error on allocating memory to array of points assigned");
		free(statusWorker);
		free(x);
		free(y);
		free(xyPrev);
		free(xyCurr);
		free(length);
		freeFilenames();
		exit (EXIT_FAILURE);
	}
	if((pointsCalculated = malloc(sizeof(int) * numFiles)) == NULL){
		perror ("Error on allocating memory to array of points calculated");
		free(statusWorker);
		free(x);
		free(y);
		free(xyPrev);
		free(xyCurr);
		free(length);
		free(pointsAssigned);
		freeFilenames();
		exit (EXIT_FAILURE);
	}
	processNextFile();
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
int processConvPoint (int threadId, int *fileId, int *n, double **xArr, double **yArr, int *point){
	int ret = 0;
	if ((statusWorker[threadId] = pthread_mutex_lock (&mutex)) != 0){
		errno = statusWorker[threadId];
	    perror ("Error on entering monitor");
	    statusWorker[threadId] = EXIT_FAILURE;
	    pthread_exit (&statusWorker[threadId]);
	}
	pthread_once (&init, initialization);
	// Checks if all points in the file currently being processed have already been assigned to be computed
	if(pointsAssigned[currentFileId] == length[currentFileId]){
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
		}
	}
	if(ret != END_PROCESS){
		*point = pointsAssigned[currentFileId]++;
		*xArr = x[currentFileId];
		*yArr = y[currentFileId];
		*n = length[currentFileId];
		*fileId = currentFileId;
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
 *  \brief Save the value calculated by the thread.
 *
 *  \param threadId thread identification
 *  \param fileId file identification
 *  \param point point computed
 *  \param val value computed by the thread
 */
void savePartialResult (int threadId, int fileId, int point, double val){
	if ((statusWorker[threadId] = pthread_mutex_lock (&mutex)) != 0){
		errno = statusWorker[threadId];
	    perror ("Error on entering monitor");
	    statusWorker[threadId] = EXIT_FAILURE;
	    pthread_exit (&statusWorker[threadId]);
	}

	xyCurr[fileId][point] = val;
	pointsCalculated[fileId]++;
	// Checks if all the points in a file have already been calculated
	if(pointsCalculated[fileId] == length[fileId])
		freeSignalArrays(fileId); //Frees the memory allocated to the signals x and y of that file

	if ((statusWorker[threadId] = pthread_mutex_unlock (&mutex)) != 0){
		errno = statusWorker[threadId];
	    perror ("Error on exiting monitor");
	    statusWorker[threadId] = EXIT_FAILURE;
	    pthread_exit (&statusWorker[threadId]);
	}
}

/**
 *  \brief Comparison of the computed signals by the threads with the previous computed signals.
 */
void checkProcessingResults(){
	int error = 0;
	for(int i = 0; i < numFiles; i++)
		for(int j = 0; j < length[i]; j++)
			if(xyPrev[i][j] != xyCurr[i][j]){ //Check if the results were different
				printf("The point %d of the file %s obtained a wrong result. Expected: %f -> "
						"Obtained: %f\n", j, filenames[i], xyPrev[i][j], xyCurr[i][j]);
				error = 1;
			}
	if(error == 0) //Check if all results were equal
		printf("All results were equal, so there was no computational error.\n");
}

/**
 *  \brief Reads the contents of the next file to be processed.
 *
 *  Internal operation.
 */
int processNextFile(){
	FILE *file;
	pointsAssigned[currentFileId] = pointsCalculated[currentFileId] = 0;
	file = fopen(filenames[currentFileId], "rb+");
	if(file == NULL){
		fprintf(stderr,"Error on opening file %s: %s\n",filenames[currentFileId], strerror(errno));
		return -1;
	}
	if (fread(&length[currentFileId], sizeof (int), 1, file) != 1){
		fprintf(stderr,"Error reading signal length from file %s: %s\n",filenames[currentFileId], strerror(errno));
		fclose(file);
		return -1;
	}
	if((x[currentFileId] = malloc(sizeof(double) * length[currentFileId])) == NULL){
		fprintf(stderr,"Error allocating memory to array x of file %s: %s\n",filenames[currentFileId], strerror(errno));
		fclose(file);
		return -1;
	}
	if (fread(x[currentFileId], sizeof (double), length[currentFileId], file) != length[currentFileId]){
		fprintf(stderr,"Error reading array x from file %s: %s\n",filenames[currentFileId], strerror(errno));
		free(x[currentFileId]);
		fclose(file);
		return -1;
	}
	if((y[currentFileId] = malloc(sizeof(double) * length[currentFileId])) == NULL){
		fprintf(stderr,"Error allocating memory to array y of file %s: %s\n",filenames[currentFileId], strerror(errno));
		free(x[currentFileId]);
		fclose(file);
		return -1;
	}
	if (fread(y[currentFileId], sizeof (double), length[currentFileId], file) != length[currentFileId]){
		fprintf(stderr,"Error reading array y from file %s: %s\n",filenames[currentFileId], strerror(errno));
		freeSignalArrays(currentFileId);
		fclose(file);
		return -1;
	}
	if((xyPrev[currentFileId] = malloc(sizeof(double) * length[currentFileId])) == NULL){
		fprintf(stderr,"Error allocating memory to array xy previously computed of file %s: %s\n",filenames[currentFileId], strerror(errno));
		freeSignalArrays(currentFileId);
		fclose(file);
		return -1;
	}
	if (fread(xyPrev[currentFileId], sizeof (double), length[currentFileId], file) != length[currentFileId]){
		fprintf(stderr,"Error reading array xy previously computed from file %s: %s\n",filenames[currentFileId], strerror(errno));
		freeSignalArrays(currentFileId);
		free(xyPrev[currentFileId]);
		fclose(file);
		return -1;
	}
	if((xyCurr[currentFileId] = malloc(sizeof(double) * length[currentFileId])) == NULL){
		fprintf(stderr,"Error allocating memory to array xy to be computed of file %s: %s\n",filenames[currentFileId], strerror(errno));
		freeSignalArrays(currentFileId);
		free(xyPrev[currentFileId]);
		fclose(file);
		return -1;
	}
	fclose(file);
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
 *  \brief Frees the allocated memory to the signals x and y.
 */
void freeSignalArrays(int fileId){
	free(x[fileId]);
	free(y[fileId]);
}

/**
 *  \brief Frees the allocated dynamic memory.
 */
void freeMemory(){
	free(statusWorker);
	for(int i = 0; i < numFiles; i++){
		if(xyPrev[i] != NULL)
			free(xyPrev[i]);
		if(xyCurr[i] != NULL)
			free(xyCurr[i]);
	}
	free(x);
	free(y);
	free(xyPrev);
	free(xyCurr);
	free(length);
	free(pointsAssigned);
	free(pointsCalculated);
	freeFilenames();
}
