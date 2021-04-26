/**
 *  \file main.c (source file)
 *
 *  \brief General Problems - Problem 1.
 *
 *  Multi-threaded version.
 *
 *  \author Rafael Sá (104552), Luís Laranjeira (81526)
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "def.h"
#include "sharedRegion.h"
#include "worker.h"

/** \brief worker threads return status array */
int *statusWorker;

/** \brief Process the arguments of the command line. */
void processCommandLine(int argc, char **argv, int *nThreads, char **fileNames, int *nFileNames);

/**
 *  \brief Main thread.
 *
 *  It starts the simulation by generating the worker threads and waits for their termination.
 */
int main(int argc, char** argv) {
	int nThreads, nFileNames, t, *status_p;
	char *fileNames[MAX_STRING_LENGTH];
	double t0, t1;

	processCommandLine (argc, argv, &nThreads, fileNames, &nFileNames);
	printf("Number of Threads: %d\n", nThreads);
	printf("Number of Files: %d\n", nFileNames);
	for(int i = 2, j = 0; i < argc; i++, j++)
		printf("\tFilename %d: %s\n", j, fileNames[j]);
	if((statusWorker = (int *)malloc(sizeof(int)*nThreads)) == NULL){
		perror ("Error on allocating memory to array of status");
		exit (EXIT_FAILURE);
	}
	pthread_t tIdWorkers[nThreads];   // Workers internal thread id array
	unsigned int IdWorkers[nThreads]; // Workers application defined thread id array

	t0 = ((double) clock())  / CLOCKS_PER_SEC;
	storeFileNames (nFileNames, fileNames);
	for (t = 0; t < nThreads; t++){
		IdWorkers[t] = t;
		if (pthread_create (&tIdWorkers[t], NULL, worker, &IdWorkers[t]) != 0){
			perror ("Error on creating thread worker");
			freeMemory();
		    exit (EXIT_FAILURE);
		}
	}
	for (t = 0; t < nThreads; t++){
		if (pthread_join (tIdWorkers[t], (void *) &status_p) != 0){
			perror ("Error on waiting for thread worker");
			freeMemory();
			exit (EXIT_FAILURE);
		}
		printf ("Thread worker, with id %u, has terminated: its status was %d\n", t, *status_p);
	}
    printProcessingResults ();
	t1 = ((double) clock())  / CLOCKS_PER_SEC;
	printf ("\nElapsed time = %.6f s\n", t1 - t0);
	freeMemory(); // Frees the memory allocated in the shared area
	exit(EXIT_SUCCESS);
}

/**
 *  \brief Process the arguments of the command line.
 *
 *  \param argc number of arguments
 *  \param argv arguments array
 *  \param nThreads pointer to the variable for the number of threads
 *  \param fileNames array for the fileNames
 *  \param nFileNames pointer to the variable for the number of files
 *
 */
void processCommandLine(int argc, char **argv, int *nThreads, char **fileNames, int *nFileNames){
	if(argc < 3){
		printf("Invalid number of arguments.\nParameters: <NumberThreads> <FileName1> [<FileName2> ...]\n");
		exit(EXIT_FAILURE);
	}
	*nThreads = atoi(argv[1]);
	if(*nThreads <= 0){
		printf("Invalid number of Threads.\nParameters: <NumberThreads> <FileName1> [<FileName2> ...]\n");
		exit(EXIT_FAILURE);
	}
	for(int i = 2, j = 0; i < argc; i++, j++)
		fileNames[j] = argv[i];
	*nFileNames = argc - 2;
}
