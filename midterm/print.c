#include <stdio.h>

main(int argc, char *argv[])
{
	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s string number\n",argv[0]);
		exit(1);
	}

	for(int i = 0; i < atoi(argv[2]); i++)
		printf("%c",*(argv[1] + i));
	printf("\n");
}
