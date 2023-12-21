#include <stdio.h>
#include <sys/types.h>

main()
{
	pid_t	pid;

	for (int i = 0; i < 4; i++)
	{
		if (i == 0) printf("parent process exists\n");
		if ((pid = fork()) < 0)
		{
			perror("fork");
			exit(1);
		}
		if (pid != 0)
			printf("%d newly forked\n",pid);
		
	}
}
