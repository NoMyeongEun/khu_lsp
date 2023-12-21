#include <stdio.h>

#define	MAX_CMD		256

void
DoCmd(char *cmd) /* a fuction that will do in main */
{
	printf("Doing %s", cmd);
	sleep(1);
	printf("Done\n");
}

main()
{
	char	cmd[MAX_CMD];

	while (1)  {
		printf("CMD> ");
		fgets(cmd, MAX_CMD, stdin); /* input the command and store at cmd */
		if (cmd[0] == 'q') /* while the q is first input, quit  */
			break;

		DoCmd(cmd); /* just print the cmd message */
	}
}
