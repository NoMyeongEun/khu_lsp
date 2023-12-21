#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void SigChldHandler(int signo)
{
	if (signo == SIGCHLD)
	{
		printf("Received a SIGCHILD signal.\n");
		exit(0);
	}
	else
	{
		printf("Received unknown signal.\n");
		exit(0);
	}
}

main()
{
	pid_t	pid;
	int	status;

    	// Register SigChldHandler as the handler for SIGCHLD signal
    	if (signal(SIGCHLD, SigChldHandler) == SIG_ERR) {
        	perror("signal");
        	exit(1);
	}
	
	if ((pid = fork()) < 0) {
		perror("fork");
		exit(1);
	}
	else if (pid == 0) {
		printf("I'm a child\n");
		sleep(2);
	}
	else {
        	// Parent continues to do other tasks while waiting for child termination
		while (1)
			;  // Simulate other tasks
	// The paraent relies on the asynchronous handling of child termination via SIGCHLD signal.
	}
}

