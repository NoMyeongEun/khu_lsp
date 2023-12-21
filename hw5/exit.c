#include <stdio.h>
#include <stdlib.h>

void
myexit1()
{
	printf("first exit handler\n");
}

void
myexit2()
{
	printf("second exit handler\n");
}

main()
{
	/* Add myexit2 to exit functions */
	if (atexit(myexit2) != 0)  { /* atexit : register an exit handler. return 0 if OK, nonzero on error */
		perror("atexit");
		exit(1);
	} 

	if (atexit(myexit1) != 0)  { /* register myexit1() as exit handler */
		perror("atexit");
		exit(1);
	}

	if (atexit(myexit1) != 0)  { /* register again myexit1() as exit handler. the message will be printed twice */
		perror("atexit");
		exit(1);
	}

	printf("main is done\n");
}
