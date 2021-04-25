/*
 * worker.c
 *
 *  Created on: 24/04/2021
 *      Author: rafael
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "def.h"
#include "sharedRegion.h"
#include "worker.h"

/** \brief worker threads return status array */
extern int *statusWorker;

void *worker(void *arg){
	unsigned int threadId = *((unsigned int *) arg);
	int fileId, n, point;
	double *x = NULL, *y = NULL, val;
	while(processConvPoint (threadId, &fileId, &n, &x, &y, &point) != END_PROCESS){
		val = computeValue (n, x, y, point);
		savePartialResult (threadId, fileId, point, val);
	}
	statusWorker[threadId] = EXIT_SUCCESS;
	pthread_exit (&statusWorker[threadId]);
}

double computeValue(int n, double *x, double *y, int point){
	double res = 0;
	for(int i = 0; i < n; i++)
		res += x[i] * y[(point + i) % n];
	return res;
}
