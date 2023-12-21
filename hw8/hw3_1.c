#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

/* Using Mutex and Condition Variable Version */

char                data;
pthread_cond_t		Cond;
pthread_mutex_t		Mutex;

/* 
	Usleep : system call that sleeps process, NOW linux make thread sleep at that time
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

    if (pthread_mutex_lock(&Mutex) < 0)  { /* At fist, you have to mutex lock : protect CS */
			perror("pthread_mutex_lock");
			pthread_exit(NULL);
	}

	if (pthread_cond_wait(&Cond, &Mutex) < 0)  { /* wait until get signal from pthread_cond_signal(&Cond), and temporarily unlock mutex */
				if (pthread_mutex_unlock(&Mutex) < 0) /* exception - Although error, mutex must be unlocked before exit */
					pthread_exit(NULL);
				perror("pthread_cond_wait");
				pthread_exit(NULL);
	}
	pData = &data;
    printf("Received request: %s.....", pData);
	sprintf(pData, "This is a reply from %d.", getpid());

	if (pthread_mutex_unlock(&Mutex) < 0)  { /* mutex unlock : out of CS */
			perror("pthread_mutex_unlock");
			pthread_exit(NULL);
	}
	if (pthread_cond_signal(&Cond) < 0)  { /* signal to pthred_cond_wait(&Cond) -> release the wating for sipc2*/
			perror("pthread_cond_signal");
			pthread_exit(NULL);
	}
	printf("Replied.\n");
    sleep(1);

    ThreadUsleep(data);
	pthread_exit(NULL);
}

/* sipc2 */
void
sipc2(void *dummy)
{
	char *pData;

    if (pthread_mutex_lock(&Mutex) < 0)  { /* mutex lock : protect CS */
			perror("pthread_mutex_lock");
			pthread_exit(NULL);
	}

    pData = &data;

	if (pthread_mutex_unlock(&Mutex) < 0)  {  /* mutex unlock : out of CS */
			perror("pthread_mutex_unlock");
			pthread_exit(NULL);
		}
	if (pthread_cond_signal(&Cond) < 0)  { /* signal to pthred_cond_wait(&Cond) -> release the sipc1 that waiting:  pthread_cond_wait*/
			perror("pthread_cond_signal");
			pthread_exit(NULL);
		}

    sprintf(pData, "This is a request from %d.", getpid());
	printf("Sent a request.....");

	if (pthread_cond_wait(&Cond, &Mutex) < 0)  { /* wait until get signal from pthread_cond_signal(&Cond), and temporarily unlock mutex */
				if (pthread_mutex_unlock(&Mutex) < 0) /* exception - Although error, mutex must be unlocked before exit */
					pthread_exit(NULL);
				perror("pthread_cond_wait");
				pthread_exit(NULL);
	}

    printf("Received reply: %s\n", pData);

	pthread_exit(NULL);
}

main()
{
	pthread_t	tid1, tid2;

	srand(0x8888);

	if (pthread_cond_init(&Cond, NULL) < 0)  { /* make a Cond condition variable */
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}
	if (pthread_mutex_init(&Mutex, NULL) < 0)  { /* make a mutex to prootect CS */
		perror("pthread_mutex_init");
		pthread_exit(NULL);
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
	
	if (pthread_cond_destroy(&Cond) < 0)  { /* remove the Cond condition variable */
		perror("pthread_cond_destroy");
	}
	if (pthread_mutex_destroy(&Mutex) < 0)  { /* remove the mutex */
		perror("pthread_mutex_destroy");
	}
}