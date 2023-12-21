#include <stdio.h>
#include <pthread.h>
#include "semlib2.h"


int
sem_init(sem_t *sem, int pshared, int value)
{
	if (pshared)  { /* pshared is not supported, so it has to be set 0 */
		fprintf(stderr, "Function not supported\n");
		return -1;
	}

	if (pthread_mutex_init(&sem->mutex, NULL) < 0) /* make a mutex lock for protecting CS */
		return -1;

	if (pthread_cond_init(&sem->cond, NULL) < 0) /* make a condition variable */
		return -1;

	sem->sval = value; /* initialize semaphore value */

	return 0;
}

int
sem_wait(sem_t *sem)
{
	if (pthread_mutex_lock(&sem->mutex) < 0) /* mutex lock : protect CS */
		return -1;

	if (sem->sval == 0)  { /* if semaphore value is 0 - empty -> wait */
		while (sem->sval == 0)  { /* while -> check again */
			if (pthread_cond_wait(&sem->cond, &sem->mutex) < 0)  { /* wait until the signal is sent and temporarily unlock mutex*/
				if (pthread_mutex_unlock(&sem->mutex) < 0) /* exception - Although error, mutex must be unlocked before exit */
					return -1;
				return -1;
			}
		}
		sem->sval--; /* decrease the semaphore value */
	}
	else  { /* if the semaphore value is not 0, just decrease the value */
		sem->sval--;
	}

	if (pthread_mutex_unlock(&sem->mutex) < 0) /* mutex unlock : out of CS */
		return -1;

	return 0;
}

int
sem_trywait(sem_t *sem)
{
	if (pthread_mutex_lock(&sem->mutex) < 0) /* mutex lock : protect CS */
		return -1;

	if (sem->sval == 0)  { /* if the semaphore value is 0 */
		if (pthread_mutex_unlock(&sem->mutex) < 0) /* exception - Although error, mutex must be unlocked before exit */
			return -1;
		return -1; /* return error */
	}
	else  { /* if the semaphore value is not 0 */
		sem->sval--; /* decrease the semaphore value */
	}

	if (pthread_mutex_unlock(&sem->mutex) < 0) /* mutex unlock : out of CS */
		return -1;

	return 0;
}

int
sem_post(sem_t *sem)
{
	if (pthread_mutex_lock(&sem->mutex) < 0) /* mutex lock : protect CS */
		return -1;

	if (sem->sval == 0)  { /* if semaphore value is 0 - some threads may be wating for signal */
		if (pthread_cond_signal(&sem->cond) < 0)  { /* signal to pthread_cond_wait() */
			if (pthread_mutex_unlock(&sem->mutex) < 0) /* exception - Although error, mutex must be unlocked before exit */
				return -1;
			return -1;
		}
	}

	sem->sval++; /* increase the semaphore value */

	if (pthread_mutex_unlock(&sem->mutex) < 0) /* out of CS */
		return -1;

	return 0;
}

int
sem_getvalue(sem_t *sem, int *sval)
{
	*sval = sem->sval; /* just save the semaphore value */

	return 0;
}

int
sem_destroy(sem_t *sem) /* remove the semaphore */
{
	if (pthread_mutex_destroy(&sem->mutex) < 0) /* remove the mutex */
		return -1;

	if (pthread_cond_destroy(&sem->cond) < 0) /* remove the condition variable */
		return -1;

	return 0;
}
