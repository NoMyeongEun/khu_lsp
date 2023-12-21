#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "semlib2.h"
#include "prodcons.h"

BoundedBufferType	Buf;
sem_t				EmptySem, FullSem, MutexSem;


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

void
Producer(void *dummy)
{
	int		i, data;

	printf("Producer: Start.....\n");

	for (i = 0 ; i < NLOOPS ; i++)  {
		if (sem_wait(&EmptySem) < 0)  {  /* wait until the empty semaphore is not zero */
			perror("sem_wait");
			pthread_exit(NULL);
		}
		if (sem_wait(&MutexSem) < 0)  { /* protect CS */
			perror("sem_wait");
			pthread_exit(NULL);
		}

		printf("Producer: Producing an item.....\n");
		data = (rand()%100)*10000; /* make a data */
		Buf.buf[Buf.in].data = data; /* save data in buffer */
		Buf.in = (Buf.in + 1) % MAX_BUF; /* circular -> set in index */
		Buf.counter++; /* data makde -> increase the number of items*/

		if (sem_post(&MutexSem) < 0)  { /* out of CS */
			perror("sem_post");
			pthread_exit(NULL);
		}
		if (sem_post(&FullSem) < 0)  { /* increase the full semaphore value */
			perror("sem_post");
			pthread_exit(NULL);
		}

		ThreadUsleep(data);
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
		if (sem_wait(&FullSem) < 0)  {  /* wait until full semaphore is not zero */
			perror("sem_wait");
			pthread_exit(NULL);
		}
		if (sem_wait(&MutexSem) < 0)  { /* protect CS */
			perror("sem_wait");
			pthread_exit(NULL);
		}

		printf("Consumer: Consuming an item.....\n");
		data = Buf.buf[Buf.out].data; /* consume a data */
		Buf.out = (Buf.out + 1) % MAX_BUF; /* circular -> set out index */
		Buf.counter--; /* decrease the number of itmes */

		if (sem_post(&MutexSem) < 0)  { /* out of CS */
			perror("sem_post");
			pthread_exit(NULL);
		}
		if (sem_post(&EmptySem) < 0)  { /* increase the empty semaphore value */
			perror("sem_post");
			pthread_exit(NULL);
		}

		ThreadUsleep((rand()%100)*10000);
	}

	printf("Consumer: Consumed %d items.....\n", i);
	printf("Consumer: %d items in buffer.....\n", Buf.counter);

	pthread_exit(NULL);
}

main()
{
	pthread_t	tid1, tid2;

	srand(0x8888);

	if (sem_init(&EmptySem, 0, MAX_BUF) < 0)  { /* make a Empty semaphore and initialize to MAX_BUF */
		perror("sem_init");
		exit(1);
	}
	if (sem_init(&FullSem, 0, 0) < 0)  { /* make a Full semaphore and initialize to 0 */
		perror("sem_init");
		exit(1);
	}
	if (sem_init(&MutexSem, 0, 1) < 0)  { /* make a Mutex Semaphore and initialize to 1 */
		perror("sem_init");
		exit(1);
	}

	if (pthread_create(&tid1, NULL, (void *)Producer, (void *)NULL) < 0)  { /* create a thread for producer */
		perror("pthread_create");
		exit(1);
	}

	if (pthread_create(&tid2, NULL, (void *)Consumer, (void *)NULL) < 0)  { /* create a thread for consumer */
		perror("pthread_create");
		exit(1);
	}

	if (pthread_join(tid1, NULL) < 0)  { /* wait until producer thread exit */
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(tid2, NULL) < 0)  { /* wait until consumer thread exit */
		perror("pthread_join");
		exit(1);
	}
	
	printf("Main    : %d items in buffer.....\n", Buf.counter);

	if (sem_destroy(&EmptySem) < 0)  { /* remove the empty semaphore*/
		perror("sem_destroy");
	}
	if (sem_destroy(&FullSem) < 0)  { /* remove the full semaphore */
		perror("sem_destroy");
	}
	if (sem_destroy(&MutexSem) < 0)  { /* remove the mutex semaphore */
		perror("sem_destroy");
	}
}