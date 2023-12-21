#include <stdio.h>
#include <stdlib.h>

main()
{
	int		status;

	if ((status = system("date")) < 0)  { /* run the command "date" by fork() and exec(). and then return termination status of the shell if OK, -1 on error */
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status); /* print the termination status */

	if ((status = system("nosuchcommand")) < 0)  { /* if there are no such command, no found(stat = 32512) */
		perror("system");
		exit(1);
	}
	printf("exit stauts = %d\n", status);

	if ((status = system("who; exit 44")) < 0)  { /* can run multiple commands. separated as semicolon; */
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status); /* because the exit 44. the status set according to the exit code */
}
