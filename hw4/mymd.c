#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

main(int argc, char *argv[])
{
	if (argc != 2)  {
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);
		exit(1);
	}

	if (mkdir(argv[1], 0755) < 0)  { /* make directory whose name is argv[1], and its mode(permission) is 755 that means read_write_execute for user, read_execute fo group and other. return 0 if OK, -1 on error */
		perror("mkdir");
		exit(1);
	}
}
