#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm.h"

main()
{
    int     shmid;
    char    *ptr, *pData;
    int     *pInt;

    // Get the shared memory segment ID
    if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
        perror("shmget");
        exit(1);
    }
    // Attach to the shared memory segment
    if ((ptr = shmat(shmid, 0, 0)) == (void *) -1)  {
        perror("shmat");
        exit(1);
    }

    pInt = (int *)ptr;
    // Wait for the request to be received
    while ((*pInt) == 0)
        ;

    pData = ptr + sizeof(int);
    printf("Received request: %s.....", pData);
    // Prepare and send a reply
    sprintf(pData, "This is a reply from %d.", getpid());
    *pInt = 0;
    printf("Replied.\n");

    sleep(1);

    // Remove the shared memory segment
    if (shmctl(shmid, IPC_RMID, 0) < 0)  {
        perror("shmctl");
        exit(1);
    }
}
