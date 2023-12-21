#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

main()
{
	pid_t	pid;
	char	*argv[] = { "mycp", "exec.c", "exec2.c", NULL }; /* the list of files to execute. must null-terminated */

	// execl
	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	else if (pid == 0)  {
		// mycp exec.c exec2.c
		if (execl("/home/cjs/lsp/file-io/mycp",
				"mycp", "exec.c", "execl.c", NULL) < 0)  { /* the first argument is pathname where the process to be executed. From the second argument, list up the files to execute, the last parameter has to be NULL to specified the end of arguments. return -1 on error, no return on success  */
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
		if (execv("/home/cjs/lsp/file-io/mycp", argv) < 0)  { /* same as execl, but the list of files to execute specified as pointer char *argv[] */
			perror("execv");
			exit(1);
		}
	}
	if (waitpid(pid, NULL, 0) < 0)  {
		perror("waitpid");
		exit(1);
	}

	// execlp
	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	else if (pid == 0)  {
		if (execlp("env", "env", NULL) < 0)  { /* find the environment variable env(the first parameter). and that path. the other is same as execl. */
			perror("execlp");
			exit(1);
		}
	}
	if (waitpid(pid, NULL, 0) < 0)  {
		perror("waitpid");
		exit(1);
	}
}
