#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "prodcons.h"

main()
{
	BoundedBufferType	*pBuf;
	int					shmid, i, data;

    /* different point with thread version - use shared memory */
	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  { /* get shared memory id - for sharing bouned buffer */
		perror("shmget");
		exit(1);
	}
	if ((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *) -1)  { /* shared memory attach - use buffer as pBuf*/
		perror("shmat");
		exit(1);
	}

	srand(0x8888);
	for (i = 0 ; i < NLOOPS ; i++)  {
		if (pBuf->counter == MAX_BUF)  {
			printf("Producer: Buffer full. Waiting.....\n");
			while (pBuf->counter == MAX_BUF) /* busy wating : when the queue if full */
				;
		}

		printf("Producer: Producing an item.....\n");
		data = (rand()%100)*10000; /* make data */
		pBuf->buf[pBuf->in].data = data; /* save data in buffer */
		pBuf->in = (pBuf->in + 1) % MAX_BUF; /* circular -> set in index */
		pBuf->counter++; /* increase the number of items*/

		usleep(data); /* wait random time */
	}

	printf("Producer: Produced %d items.....\n", i);

	sleep(2);
	printf("Producer: %d items in buffer.....\n", pBuf->counter);
	
	if (shmctl(shmid, IPC_RMID, 0) < 0)  { /* detach the shared memory */
		perror("shmctl");
		exit(1);
	}
}