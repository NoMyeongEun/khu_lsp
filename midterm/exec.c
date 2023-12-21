#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

main()
{
	pid_t	pid;
	char	*argv[] = { "print", "HelloWorld!", "6", NULL };

	// execl
	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	else if (pid == 0)  {
		if (execl("/home/lsp16/midterm/print", "print", "HelloWorld!","6", NULL) < 0)
		{
			perror("execl");
			exit(1);
		}
	}
	if (waitpid(pid, NULL, 0) < 0)  { /* wait until the child process terminated */
		perror("waitpid");
		exit(1);
	}

	// execv
	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	else if (pid == 0)  {
		if (execv("/home/lsp16/midterm/print", argv) < 0)  {
			perror("execv");
			exit(1);
		}
	}
	if (waitpid(pid, NULL, 0) < 0)  {
		perror("waitpid");
		exit(1);
	}
}
