#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void
CharAtaTime(char *str) /* print the string by char at a time slowly */
{
	char	*ptr;
	int		c, i;

	setbuf(stdout, NULL);
	for (ptr = str ; c = *ptr++ ; )  {
		for(i = 0 ; i < 999999 ; i++)
			;
		putc(c, stdout);
	}
}

main()
{
	pid_t	pid;


	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	else if (pid == 0)  { /* if the process is a child process */
		CharAtaTime("output from child\n");
	} 
	else  { /* if the process is a parent process */
		wait(NULL); /* wait until the child process ends */
		CharAtaTime("output from parent\n"); /* print after the child process terminated */
	}
}
