#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

main()
{
    int		pid;

    if ((pid = fork() < 0))  { /* fork() : create a new process, return  -1 on error */
	    perror("fork");
	    exit(1);
    } 
    else if (pid == 0)  { /* if the process is a child process, the pid value is 0 */
	    /* child */
	    printf("I am %d. My parent is %d. \n", getpid(), getppid()); /* getpid() - return process ID of calling process, getppid() - return parent process ID of calling process */
    }
    else  { /* if the process is a parent process, the pid value is the process ID of child process */
	    /* parent */
	    printf("I am %d. My child is %d.\n", getpid(), pid);
    }
}
