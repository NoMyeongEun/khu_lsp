#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void
CharAtaTime(char *str)
{
	char	*ptr;
	int		c, i;

	setbuf(stdout, NULL); /* clear the standard output buffer */
	for (ptr = str ; c = *ptr++ ; )  { /* until the string ends, read each character */
		for (i = 0 ; i < 999999 ; i++) /* for making the process long */
			;
		putc(c, stdout); /* print the character */
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
	else  { /* if the process is a parent process. there are no wait, so each process execute taking turns. then the content of the print is mixed */
		CharAtaTime("output from parent\n");
	}
}
