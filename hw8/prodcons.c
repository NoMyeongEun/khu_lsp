#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>
#include "prodcons.h"

BoundedBufferType	Buf;
sem_t				EmptySem, FullSem, MutexSem;

void
ThreadUsleep(int usecs)
{
	pthread_cond_t		cond;
	pthread_mutex_t		mutex;
	struct timespec		ts;
	struct timeval		tv;

	if (pthread_cond_init(&cond, NULL) < 0)  {
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}
	if (pthread_mutex_init(&mutex, NULL) < 0)  {
		perror("pthread_mutex_init");
		pthread_exit(NULL);
	}

	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec + usecs/1000000;
	ts.tv_nsec = (tv.tv_usec + (usecs%1000000)) * 1000;
	if (ts.tv_nsec >= 1000000000)  {
		ts.tv_nsec -= 1000000000;
		ts.tv_sec++;
	}

	if (pthread_mutex_lock(&mutex) < 0)  {
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}
	if (pthread_cond_timedwait(&cond, &mutex, &ts) < 0)  {
		perror("pthread_cond_timedwait");
		pthread_exit(NULL);
	}

	if (pthread_cond_destroy(&cond) < 0)  {
		perror("pthread_cond_destroy");
		pthread_exit(NULL);
	}
	if (pthread_mutex_destroy(&mutex) < 0)  {
		perror("pthread_mutex_destroy");
		pthread_exit(NULL);
	}
}

void
Producer(void *dummy)
{
	int		i, data;

	printf("Producer: Start.....\n");

	for (i = 0 ; i < NLOOPS ; i++)  {
        /* wait(empty) */
		if (sem_wait(&EmptySem) < 0)  { /* wait until the empty semaphore value is not zero */
			perror("sem_wait");
			pthread_exit(NULL);
		}
        /* wait(mutex) */
		if (sem_wait(&MutexSem) < 0)  {  /* wait if consumer is executing the CS */
			perror("sem_wait");
			pthread_exit(NULL);
		}

		printf("Producer: Producing an item.....\n");
		data = (rand()%100)*10000; /* make data */
		Buf.buf[Buf.in].data = data; /* save data in buffer */
		Buf.in = (Buf.in + 1) % MAX_BUF; /* circular -> set in index */
		Buf.counter++; /* increase the number of items*/

        /* signal(mutex) */
		if (sem_post(&MutexSem) < 0)  { /* get ouf of CS */
			perror("sem_post");
			pthread_exit(NULL);
		}
        /* signal(full) */
		if (sem_post(&FullSem) < 0)  { /* increase the full semaphore value since the data was made */
			perror("sem_post");
			pthread_exit(NULL);
		}

		ThreadUsleep(data); /* wait */
	}

	printf("Producer: Produced %d items.....\n", i);
	printf("Producer: %d items in buffer.....\n", Buf.counter);
	
	pthread_exit(NULL);
}

void
Consumer(void *dummy)
{
	int		i, data;

	printf("Consumer: Start.....\n");

	for (i = 0 ; i < NLOOPS ; i++)  {
        /* wait(full) */
		if (sem_wait(&FullSem) < 0)  { /* wait until the full semaphore value is not zero */
			perror("sem_wait");
			pthread_exit(NULL);
		}
        /* wait(mutex) */
		if (sem_wait(&MutexSem) < 0)  { /* wait if producer is executing the CS */
			perror("sem_wait");
			pthread_exit(NULL);
		}

		printf("Consumer: Consuming an item.....\n");
		data = Buf.buf[Buf.out].data; /* consume data */
		Buf.out = (Buf.out + 1) % MAX_BUF; /* circular queue -> set out index */
		Buf.counter--; /* decrease the number of items */

        /* signal(mutex) */
		if (sem_post(&MutexSem) < 0)  { /* get ouf of CS */
			perror("sem_post");
			pthread_exit(NULL);
		}
        /* signal(empty) */
		if (sem_post(&EmptySem) < 0)  { /* increase the empty semaphore value since the data was consumed */
			perror("sem_post");
			pthread_exit(NULL);
		}

		ThreadUsleep((rand()%100)*10000); /* wait */
	}

	printf("Consumer: Consumed %d items.....\n", i);
	printf("Consumer: %d items in buffer.....\n", Buf.counter);

	pthread_exit(NULL);
}

main()
{
	pthread_t	tid1, tid2;

	srand(0x9999);

    /* make semaphores and initialize */
	if (sem_init(&EmptySem, 0, MAX_BUF) < 0)  {
		perror("sem_init");
		exit(1);
	}
	if (sem_init(&FullSem, 0, 0) < 0)  {
		perror("sem_init");
		exit(1);
	}
	if (sem_init(&MutexSem, 0, 1) < 0)  {
		perror("sem_init");
		exit(1);
	}

    /* create a producer thread */
	if (pthread_create(&tid1, NULL, (void *)Producer, (void *)NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}

    /* create a consumer thread */
	if (pthread_create(&tid2, NULL, (void *)Consumer, (void *)NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}

    /* wait until the producer thread exit */
	if (pthread_join(tid1, NULL) < 0)  {
		perror("pthread_join");
		exit(1);
	}
    /* wait untile the consumer thread exit*/
	if (pthread_join(tid2, NULL) < 0)  {
		perror("pthread_join");
		exit(1);
	}

	printf("Main    : %d items in buffer.....\n", Buf.counter);

	if (sem_destroy(&EmptySem) < 0)  { /* remove the empty semaphore */
		perror("sem_destroy");
	}
	if (sem_destroy(&FullSem) < 0)  { /* remove the full semaphore */
		perror("sem_destroy");
	}
	if (sem_destroy(&MutexSem) < 0)  { /* remove the mutex semaphore */
		perror("sem_destroy");
	}
}