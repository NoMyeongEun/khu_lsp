#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semlib.c"

#define	MY_ID			16
#define	SHM_KEY			(0x9000 + MY_ID)
#define	SHM_MODE		(SHM_R | SHM_W | IPC_CREAT)
#define	SHM_SIZE		1024
#define	SEM_KEY			(0x5000 + MY_ID)
#define	MUTEX_SEM_KEY	(0x7000 + MY_ID)

/* sipc2.c */
main()
{
	int		shmid;
	char	*ptr, *pData;
	int		Semid, mutexSemid;

	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
		perror("shmget");
		exit(1);
	}
	if ((ptr = shmat(shmid, 0, 0)) == (void *) -1)  {
		perror("shmat");
		exit(1);
	}

    /* make a semaphore */
    if ((Semid = semInit(SEM_KEY)) < 0)  {  /* get a semaphore ID */
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}

	if ((mutexSemid = semInit(MUTEX_SEM_KEY)) < 0)  {  /* get a mutex semaphore ID - for protecting CS */
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}

    if (semWait(mutexSemid) < 0)  { /* wait if the other is in CS */
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}
	pData = ptr + sizeof(int); 

    if (semPost(mutexSemid) < 0)  {  /* get out of CS */
		fprintf(stderr, "semPost failure\n");
		exit(1);
	}
	if (semPost(Semid) < 0)  { /* increase the semaphore value -> signal to sipc1*/
		fprintf(stderr, "semPost failure\n");
		exit(1);
	}

	sprintf(pData, "This is a request from %d.", getpid()); 
	printf("Sent a request.....");

    if (semWait(Semid) < 0)  {  /* decrease semaphore value or wait until semaphore value is not zero*/
		fprintf(stderr, "semWait failure\n");
		exit(1);
	} 

	printf("Received reply: %s\n", pData);

    if (semDestroy(Semid) < 0)  { /* remove the semaphore */
		fprintf(stderr, "semDestroy failure\n");
	}
	if (semDestroy(mutexSemid) < 0)  { /* remove the mutex semaphore */
		fprintf(stderr, "semDestroy failure\n");
	}

    if (shmctl(shmid, IPC_RMID, 0) < 0)  { /* remove the shared memory */
		perror("shmctl");
		exit(1);
	}
}