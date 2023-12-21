#include <stdio.h>
#include "synclib.h"

#define	NLOOPS	5

main()
{
    int		i;
    pid_t	pid;

    TELL_WAIT(); // Initialize synchronization

    // Create a child process
    if ((pid = fork()) < 0)  {
        perror("fork");
        exit(1);
    }
    else if (pid > 0)  { // Parent process
        for (i = 0 ; i < NLOOPS ; i++)  {
            TELL_CHILD(); // Notify the child process
            printf("Parent: Tell to child\n");
            printf("Parent: Wait for child to tell\n");
            WAIT_CHILD(); // Wait for the child until the child process call TELL_PARENT()
        }
    }
    else  { // Child process
        for (i = 0 ; i < NLOOPS ; i++)  {
            printf("Child: Wait for parent to tell\n");
            WAIT_PARENT(); // Wait for the parent until the parent process call TELL_CHILD()
            TELL_PARENT(); // Notify the parent process
            printf("Child: Tell to parent\n");
        }
    }
}

