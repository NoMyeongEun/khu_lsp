#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm.h" // Including the header file with shared memory definitions

int main() {
    int shmid; // Shared memory ID
    char *ptr, *pData; // Pointers for shared memory and data
    int *pInt; // Pointer to an integer in shared memory

    // Access the existing shared memory segment using a predefined key and size
    if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
        perror("shmget"); // Error handling for shared memory access
        exit(1);
    }

    // Attach the shared memory segment to the process's address space
    if ((ptr = shmat(shmid, 0, 0)) == (void *) -1)  {
        perror("shmat"); // Error handling for shared memory attachment
        exit(1);
    }

    pInt = (int *)ptr; // Set the pointer to the integer in shared memory
    pData = ptr + sizeof(int); // Move the data pointer to after the integer in shared memory

    // Create a request message with the process ID and write it to shared memory
    sprintf(pData, "This is a request from %d.", getpid());
    *pInt = 1; // Set the integer in shared memory to one (indicating a request sent)
    printf("Sent a request.....");

    // Wait until the integer in shared memory changes to a value other than one
    while ((*pInt) == 1)
        ;

    // Display the received reply from shared memory
    printf("Received reply: %s\n", pData);
    return 0;
}

