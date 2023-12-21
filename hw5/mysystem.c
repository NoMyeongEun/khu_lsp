#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int mysystem(char *cmd)
{
	pid_t	pid;
	int	i;

	if ((pid = fork()) < 0) { /* create a new process */
		perror("fork");
		exit(1);
	}
	else if (pid == 0) /* if child */
	{
		char	*command[] = {"bash","-c",cmd,NULL}; /* vector type argument. first argument is bash. and the second argument is "-c", the third argument is the pointer of commands, and the last parameter has to be NULL */ 
		if (execv("/bin/bash",command) < 0) { /* to run the command, the path is /bin/bash. then the pointer of full command is the second parameter */
			perror("execv");
			exit(1);
		}
	}
	if(waitpid(pid, NULL, 0) < 0) { /* parent wait for child termination */
		perror("waitpid");
		exit(1);
	}
}

main()
{
	mysystem("ls -al");
	mysystem("date");
	mysystem("who");
	mysystem("who; date");
}
