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
	if ((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *) -1)  {  /* shared memory attach - use buffer as pBuf*/
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
	if ((mutexSemid = semInit(MUTEX_SEM_KEY)) < 0)  { /* make a semaphore for protect critical section. binary semaphore*/
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}

	srand(0x8888);
	for (i = 0 ; i < NLOOPS ; i++)  {
        /* wait(empty) */
		if (semWait(emptySemid) < 0)  {  /* wait until the empty semaphore value is not zero */
			fprintf(stderr, "semWait failure\n");
			exit(1);
		} 
        /* wait(mutex) */
		if (semWait(mutexSemid) < 0)  { /* wait if consumer is executing the CS */
			fprintf(stderr, "semWait failure\n");
			exit(1);
		}

		printf("Producer: Producing an item.....\n");
		data = (rand()%100)*10000; /* make data */
		pBuf->buf[pBuf->in].data = data; /* save data in buffer */
		pBuf->in = (pBuf->in + 1) % MAX_BUF; /* circular -> set in index */
		pBuf->counter++; /* increase the number of items*/

        /* signal(mutex) */
		if (semPost(mutexSemid) < 0)  { /* get ouf of CS */
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}
        /* signal(full) */
		if (semPost(fullSemid) < 0)  { /* increase the full semaphore value since the data was made */
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}
		usleep(data); /* wait random time */
	}

	printf("Producer: Produced %d items.....\n", i);

	sleep(2);
	printf("Producer: %d items in buffer.....\n", pBuf->counter);
	
	if (semDestroy(emptySemid) < 0)  { /* remove the empty semaphore */
		fprintf(stderr, "semDestroy failure\n");
	}
	if (semDestroy(fullSemid) < 0)  { /* remove the full semaphore */
		fprintf(stderr, "semDestroy failure\n");
	}
	if (semDestroy(mutexSemid) < 0)  { /* remove the mutex semaphore */
		fprintf(stderr, "semDestroy failure\n");
	}
	if (shmctl(shmid, IPC_RMID, 0) < 0)  { /* release the shared memory */
		perror("shmctl");
		exit(1);
	}
}