#include <stdio.h>
#include <unistd.h>

main(int argc, char *argv[])
{
	if (argc != 2)  {
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);
		exit(1);
	}

	if (chdir(argv[1]) < 0)  { /* change working directory to argv[1]. only apply to the current process. return 0 if OK, -1 on error */
		perror("chdir");
		exit(1);
	}
}
