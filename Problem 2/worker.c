/**
 *  \file worker.c (source file)
 *
 *  \brief Worker thread definition.
 * *
 *  Definition of the operations carried out by the worker thread:
 *     \li *worker
 *     \li computeValue
 *
 *  \author Rafael Sá (104552), Luís Laranjeira (81526)
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "def.h"
#include "sharedRegion.h"
#include "worker.h"

/** \brief worker threads return status array */
extern int *statusWorker;

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

/**
 *  \brief Compute the value of one point.
 *
 *  Internal thread operation.
 *
 *  \param n signal length
 *  \param x signal x
 *  \param y signal y
 *  \param point point to be computed
 *
 *  \return value computed
 */
double computeValue(int n, double *x, double *y, int point){
	double res = 0;
	for(int i = 0; i < n; i++)
		res += x[i] * y[(point + i) % n];
	return res;
}
