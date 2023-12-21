#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>

/* Using Semaphore Version */

char                data;
sem_t				Sem, MutexSem;

/* Usleep : system call that sleeps process, NOW linux make thread sleep at that time
	BUT Solaris could not... -> sleep by threads -> ThreadUSleep -> pthread_cond_timedwait
*/
void
ThreadUsleep(int usecs) 
{
	pthread_cond_t		cond;
	pthread_mutex_t		mutex;
	struct timespec		ts;
	struct timeval		tv;

	if (pthread_cond_init(&cond, NULL) < 0)  { /* condition variable for pthread_cond_timedwait */
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}
	if (pthread_mutex_init(&mutex, NULL) < 0)  { /* mutex for condition variable for pthread_cond_timedwait */
		perror("pthread_mutex_init");
		pthread_exit(NULL);
	}

	gettimeofday(&tv, NULL); /* get current time */
	/* add current time to the time we are goint to wait*/
	ts.tv_sec = tv.tv_sec + usecs/1000000;
	ts.tv_nsec = (tv.tv_usec + (usecs%1000000)) * 1000;
	if (ts.tv_nsec >= 1000000000)  {
		ts.tv_nsec -= 1000000000;
		ts.tv_sec++;
	}

	if (pthread_mutex_lock(&mutex) < 0)  { /* have to lock before pthread_cond_timedwait */
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}
	/* timedwait -> wait at specified time(= &ts) -> if no signal during &ts, return */
	if (pthread_cond_timedwait(&cond, &mutex, &ts) < 0)  { /* ts structure -> absolute time */
		perror("pthread_cond_timedwait");
		pthread_exit(NULL);
	}

	if (pthread_cond_destroy(&cond) < 0)  { /* remove the condition variable */
		perror("pthread_cond_destroy");
		pthread_exit(NULL);
	}
	if (pthread_mutex_destroy(&mutex) < 0)  { /* remove the mutex */
		perror("pthread_mutex_destroy");
		pthread_exit(NULL);
	}
}

/* sipc1.c */
void
sipc1(void *dummy)
{    
	char *pData;

    if (sem_wait(&Sem) < 0)  { /* decrease semaphore value or wait until Sem semaphore value is not zero*/
		perror("sem_wait");
		pthread_exit(NULL);
	}
	if (sem_wait(&MutexSem) < 0)  {  /* wait if the other is in CS */
		perror("sem_wait");
		pthread_exit(NULL);
	}

	pData = &data;
    printf("Received request: %s.....", pData);
	sprintf(pData, "This is a reply from %d.", getpid());

    if (sem_post(&MutexSem) < 0)  {  /* get out of CS */
		perror("sem_post");
		pthread_exit(NULL);
	}
	if (sem_post(&Sem) < 0)  { /* increase the semaphore value -> release the sem_wait(&Sem) for sipc2*/
		perror("sem_post");
		pthread_exit(NULL);
	}

	printf("Replied.\n");
    sleep(1);

    ThreadUsleep(data);
	pthread_exit(NULL);
}

void
sipc2(void *dummy)
{
	char *pData;

	if (sem_wait(&MutexSem) < 0)  { /* wait if the other is in CS */
		perror("sem_wait");
		pthread_exit(NULL);
	}

    pData = &data;

    if (sem_post(&MutexSem) < 0)  { /* out of CS */
		perror("sem_post");
		pthread_exit(NULL);
	}
	if (sem_post(&Sem) < 0)  { /* increase the Sem semaphore value -> release the sem_wait(&Sem) for sipc1*/
		perror("sem_post");
		pthread_exit(NULL);
	}

    sprintf(pData, "This is a request from %d.", getpid());
	printf("Sent a request.....");

    if (sem_wait(&Sem) < 0)  { /* decrease the Sem semaphore value or wait until the semaphore value is not zero(=until sipc1 call sem_post)*/
		perror("sem_wait");
		pthread_exit(NULL);
	}

    printf("Received reply: %s\n", pData);

	pthread_exit(NULL);
}

main()
{
	pthread_t	tid1, tid2;

	srand(0x8888);

	if ((sem_init(&Sem, 0, 0)) < 0)  { /* make a semaphore and initialzie to 0 */
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	if ((sem_init(&MutexSem, 0, 1)) < 0)  {  /* make a semaphore for protect critical section and initalzie to 1 - no one is using*/
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}

	if (pthread_create(&tid1, NULL, (void *)sipc1, (void *)NULL) < 0)  { /* create a thread for sipc1 */
		perror("pthread_create");
		exit(1);
	}

	if (pthread_create(&tid2, NULL, (void *)sipc2, (void *)NULL) < 0)  { /* create a thread for sipc2 */
		perror("pthread_create");
		exit(1);
	}

	if (pthread_join(tid1, NULL) < 0)  { /* wait until sipc1 thread exit */
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(tid2, NULL) < 0)  { /* wait until sipc2 thread exit */
		perror("pthread_join");
		exit(1);
	}
	
	if (sem_destroy(&Sem) < 0)  { /* remove the Sem semaphore*/
		perror("sem_destroy");
	}

	if (sem_destroy(&MutexSem) < 0)  { /* remove the mutex semaphore */
		perror("sem_destroy");
	}
}