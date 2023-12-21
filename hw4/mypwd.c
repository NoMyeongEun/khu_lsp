#include <stdio.h>
#include <unistd.h>

#define	MAX_BUF	256

main(int argc, char *argv[])
{
	char	buf[MAX_BUF];

	if (getcwd(buf, MAX_BUF) == NULL)  { /* get current working directory at the buf, and the max size of buffer is MAX_BUF. buf if OK, NULL on error */
		perror("getcwd");
		exit(1);
	}

	printf("%s\n", buf);
}
