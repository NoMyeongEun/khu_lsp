#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int		Var = 6; /* global variable */
char	Buf[] = "a write to stdout\n";

main()
{
	int		var; /* local varaible */
	pid_t	pid;

	var = 88; 
	write(STDOUT_FILENO, Buf, sizeof(Buf) - 1);
	printf("Before fork\n");

	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	else if (pid == 0)  { /* if it is the child process, add 1 to each value */
		/* child */	
		Var++;
		var++;
	}
	else  { /* if it is the parent process, do not anything and wait 2 seconds */
		/* Parent */
		sleep(2);
	}

	printf("pid = %d, Var = %d, var = %d\n", getpid(), Var, var); /* print the value. only the child process's value changed. Among the processes, they do not share the variables. */ 
}
