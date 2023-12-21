#include <stdio.h>

#define MAX_BUF 256

main(int argc, char *argv[])
{
	FILE	*fp;
	char	buf[MAX_BUF];
	int 	line;

	if (argc != 2) {
		printf("Usage: %s filename\n",argv[0]);
		exit(1);
	}

	if ((fp = fopen(argv[1], "rt")) == NULL) { // open a stream. filename is the value of the string argv[1]. type is "rt" that means read acces mode text file. return : a file pointer if OK, NULL on error
		perror("fopen");
		exit(1);
	}

	line = 1;
	while (fgets(buf, MAX_BUF, fp)) { // input functions line at a time. read at this buffer buf, the maximum size of the buffer is MAX_BUF, from this FILE pointer fp. return buf is OK, NULL on end of file or error
		printf("%4d: %s", line++, buf); // print the line that is read from file
	}

	fclose(fp); // close a stream
}
