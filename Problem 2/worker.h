/*
 * worker.h
 *
 *  Created on: 24/04/2021
 *      Author: rafael
 */

#ifndef WORKER_H_
#define WORKER_H_

void *worker(void *arg);
double computeValue(int n, double *x, double *y, int point);

#endif /* WORKER_H_ */
