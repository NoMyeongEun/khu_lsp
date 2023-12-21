#include <stdio.h>
#include <pthread.h>

#define	MAX_CMD		256

void
DoCmd(char *cmd) /* a function that the thread will do */
{
	printf("Doing %s", cmd);
	sleep(1);
	printf("Done\n");

	pthread_exit(NULL); /* thread termination */
}

main()
{
	char		cmd[MAX_CMD];
	pthread_t	tid;

	while (1)  {
		printf("CMD> ");
		fgets(cmd, MAX_CMD, stdin); /* command input */
		if (cmd[0] == 'q') /* exit condition */
			break;
		
		if (pthread_create(&tid, NULL, (void *)DoCmd, (void *)cmd) < 0) { /* create a thread that will do DoCmd function */
			perror("pthread_create");
			exit(1);
		}
# if 0
		pthread_join(tid, NULL);
#endif
	}
}
