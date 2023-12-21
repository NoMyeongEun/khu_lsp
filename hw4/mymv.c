#include <stdio.h>

main(int argc, char *argv[])
{
	if (argc != 3)  {
		fprintf(stderr, "Usage: %s old new\n", argv[0]);
		exit(1);
	}

	if (rename(argv[1], argv[2]) < 0)  { /* rename the file whose old name is argv[1] to the new name argv[2]. return 0 if OK, -1 on error */
		perror("rename");
		exit(1);
	}
}
