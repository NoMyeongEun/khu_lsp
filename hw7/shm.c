#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define	ARRAY_SIZE	40000
#define	MALLOC_SIZE	100000
#define	SHM_SIZE	100000
#define	SHM_MODE	(SHM_R | SHM_W) // Shared memory mode (read/write)

char Array[ARRAY_SIZE]; // Array in the program's data segment

int main() {
    int shmid; // Shared memory ID
    char *ptr, *shmptr; // Pointers for shared memory and malloc'd memory

    // Allocate memory using malloc for demonstration purposes
    if ((ptr = (char *)malloc(MALLOC_SIZE)) == NULL)  {
        perror("malloc"); // Error handling for malloc failure
        exit(1);
    }

    // Create a shared memory segment
    if ((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE)) < 0)  {
        perror("shmget"); // Error handling for shared memory creation
        exit(1);
    }

    // Attach the shared memory segment to the process's address space
    if ((shmptr = shmat(shmid, 0, 0)) == (void *) -1)  {
        perror("shmat"); // Error handling for shared memory attachment
        exit(1);
    }

    // Print memory addresses to demonstrate memory layout
    printf("Array[] from %p to %p\n", &Array[0], &Array[ARRAY_SIZE]);
    printf("Stack around %p\n", &shmid);
    printf("Malloced from %p to %p\n", ptr, ptr + MALLOC_SIZE);
    printf("Shared memory attached from %p to %p\n", shmptr, shmptr + SHM_SIZE);

    // Remove the shared memory segment
    if (shmctl(shmid, IPC_RMID, 0) < 0)  {
        perror("shmctl"); // Error handling for shared memory removal
        exit(1);
    }
    return 0;
}

