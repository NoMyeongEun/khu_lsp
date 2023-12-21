#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semlib.h"
#include "prodcons.h"

main()
{
	BoundedBufferType	*pBuf;
	int					shmid, i, data;
	int					emptySemid, fullSemid, mutexSemid;

	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  { /* get shared memory id - for sharing bouned buffer */
		perror("shmget");
		exit(1);
	}
	if ((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *) -1)  { /* shared memory attach - use buffer as pBuf*/
		perror("shmat");
		exit(1);
	}

	if ((emptySemid = semInit(EMPTY_SEM_KEY)) < 0)  { /* make a semaphore for representing the number of empty items */
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	if ((fullSemid = semInit(FULL_SEM_KEY)) < 0)  { /* make a semaphore for representing the number of full itmes */
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	if ((mutexSemid = semInit(MUTEX_SEM_KEY)) < 0)  {  /* make a semaphore for protect critical section. binary semaphore*/
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}

    /* Because we assume that consumer process started first, initalize the semaphores value in consumer process */
	if (semInitValue(emptySemid, MAX_BUF) < 0)  { /* At first, there is no item, empty semaphore is initalized to MAX_BUF*/
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}
	if (semInitValue(fullSemid, 0) < 0)  { /* At first, there is no item, full semaphore is initialized to 0 */
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}
	if (semInitValue(mutexSemid, 1) < 0)  { /* Binary semaphored. At first, there is no process in CS, initialize to 1 */
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}

	srand(0x9999);
	for (i = 0 ; i < NLOOPS ; i++)  {
        /* wait(full) */
		if (semWait(fullSemid) < 0)  { /* wait until the full semaphore value is not zero */
			fprintf(stderr, "semWait failure\n");
			exit(1);
		}
		if (semWait(mutexSemid) < 0)  { /* wait if producer is executing the CS */
			fprintf(stderr, "semWait failure\n");
			exit(1);
		}
		printf("Consumer: Consuming an item.....\n");
		data = pBuf->buf[pBuf->out].data; /* consume data */
		pBuf->out = (pBuf->out + 1) % MAX_BUF; /* circular queue -> set out index */
		pBuf->counter--; /* decrease the number of items */

        /* signal(mutex) */
		if (semPost(mutexSemid) < 0)  { /* get ouf of CS */
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}
        /* signal(empty) */
		if (semPost(emptySemid) < 0)  { /* increase the empty semaphore value since the data was consumed */
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}

		usleep((rand()%100)*10000); /* wait random time */
	}

	printf("Consumer: Consumed %d items.....\n", i);
	printf("Consumer: %d items in buffer.....\n", pBuf->counter);
}