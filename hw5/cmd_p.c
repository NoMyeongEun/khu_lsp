#include <stdio.h>

#define	MAX_CMD		256

void
DoCmd(char *cmd) /* a function that the child process will do */
{
	printf("Doing %s", cmd);
	sleep(1);
	printf("Done\n");
}

main()
{
	char	cmd[MAX_CMD];
	int		pid;

	while (1)  {
		printf("CMD> ");
		fgets(cmd, MAX_CMD, stdin); /* command input */	
		if (cmd[0] == 'q') /* exit condition */
			break;
		if ((pid = fork()) < 0) { /* create a new process */
			perror("fork");
			exit(1);
		}
		else if (pid == 0) { /* if child process */
			DoCmd(cmd); /* to the task once */
			exit(0);
		}
#if 0
		else {
			wait(NULL);
		}
#endif
	}
}
