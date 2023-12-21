#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

main()
{
	pid_t	pid;
	int		status;

	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	else if (pid == 0)  { /* if the process is a child process, just print message and wait 2 seconds */
		printf("I'm a child\n");
		sleep(2);
	}
	else  { /* if the process is a parent process */
		wait(&status); /* wait the end of any child process. return immediately with the termination status of a child */ 
		printf("A child killed with %d status\n", status);
	}
}
