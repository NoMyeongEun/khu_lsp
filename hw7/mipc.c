#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "synclib.h"

#define	NLOOPS	10
#define	SIZE	sizeof(long)

// Function to increment the value pointed by ptr and return its previous value
int update(long *ptr) {
    return ((*ptr)++);
}

int main() {
    int fd, i, counter; // File descriptor, loop variables, and counter
    pid_t pid; // Process ID
    caddr_t area; // Shared memory area

    // Open /dev/zero to provide memory for mapping
    if ((fd = open("/dev/zero", O_RDWR)) < 0)  {
        perror("open");
        exit(1);
    }

    // Map the shared memory area using /dev/zero
    if ((area = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == (caddr_t) -1)  {
        perror("mmap");
        exit(1);
    }
    close(fd); // Close the file descriptor

    TELL_WAIT(); // Synchronization initialization

    // Create a child process
    if ((pid = fork()) < 0)  {
        perror("fork");
        exit(1);
    }
    else if (pid > 0)  { // Parent process
        for (i = 0 ; i < NLOOPS ; i += 2)  {
            // Update the value in the shared memory and check for consistency
            if ((counter = update((long *) area)) != i)  {
                fprintf(stderr, "Counter mismatch\n");
                exit(1);
            }
            printf("Parent: counter=%d\n", counter);
            TELL_CHILD(); // Notify child process
            WAIT_CHILD(); // Wait for child's response
        }
    }
    else  { // Child process
        for (i = 1 ; i < NLOOPS ; i += 2)  {
            WAIT_PARENT(); // Wait for parent's signal
            // Update the value in the shared memory and check for consistency
            if ((counter = update((long *) area)) != i)  {
                fprintf(stderr, "Counter mismatch\n");
                exit(1);
            }
            printf("Child : counter=%d\n", counter);
            TELL_PARENT(); // Notify parent process
        }
    }
    return 0;
}

