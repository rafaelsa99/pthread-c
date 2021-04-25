/**
 *  \file worker.h (header file)
 *
 *  \brief Worker thread definition.
 * *
 *  Definition of the operations carried out by the worker thread:
 *     \li *worker
 *     \li computeValue
 *
 *  \author Rafael Sá (104552), Luís Laranjeira (81526)
 */

#ifndef WORKER_H_
#define WORKER_H_

/**
 *  \brief Function worker.
 *
 *  Its role is to simulate the life cycle of a worker.
 *
 *  \param arg pointer to application defined worker identification
 */
void *worker(void *arg);

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
double computeValue(int n, double *x, double *y, int point);

#endif /* WORKER_H_ */
