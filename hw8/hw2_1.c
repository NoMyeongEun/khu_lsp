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

/* sipc1.c */
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

	if ((mutexSemid = semInit(MUTEX_SEM_KEY)) < 0)  { /* get a mutex semaphore ID - for protecting CS */
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}

    if (semInitValue(Semid, 0) < 0)  {  /* Initialize a semaphore to 0 */
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}
	if (semInitValue(mutexSemid, 1) < 0)  { /* Initialize a mutex semaphore to 1 - no one is in CS */
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}
    
    if (semWait(Semid) < 0)  {  /* decrease semaphore value or wait for a request  - until semaphore value is not zero */
		fprintf(stderr, "semWait failure\n");
		exit(1);
	} 
	if (semWait(mutexSemid) < 0)  { /* wait if the other is in CS */
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}

	pData = ptr + sizeof(int); 
    printf("Received request: %s.....", pData);
	sprintf(pData, "This is a reply from %d.", getpid());

	if (semPost(mutexSemid) < 0)  {  /* get out of CS */
		fprintf(stderr, "semPost failure\n");
		exit(1);
	}
	if (semPost(Semid) < 0)  { /* decrease the semaphore value */
		fprintf(stderr, "semPost failure\n");
		exit(1);
	}

	printf("Replied.\n");

	sleep(1);

	if (shmctl(shmid, IPC_RMID, 0) < 0)  { /* remove the shared memory */
		perror("shmctl");
		exit(1);
	}
}