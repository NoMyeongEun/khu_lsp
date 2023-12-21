#include <stdio.h>

#define MAX_BUF 256

main(int argc, char *argv[])
{
	FILE	*src, *dst;
	char	buf[MAX_BUF];

	if (argc != 3) {
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	if ((src = fopen(argv[1], "rt")) == NULL) { // open a file whose filepath is the value of the string argv[1], as read access text file type
		perror("fopen");
		exit(1);
	}

	if ((dst = fopen(argv[2], "wt")) == NULL) { // open a file whose filepath is the value of the string argv[2], as write access text file type
		perror("fopen");
		exit(1);
	}

	while (fgets(buf, MAX_BUF, src) != NULL) { // input function line at a time. from the FILE where the pointer src points. return : buf if OK, NULL  on end of file or error
		fputs(buf, dst); // output function line at a time. write the value of the string str at the FILE where the pointer dst points. return non-negative value if OK, EOF on error
	}

	fclose(src); // close a stream src
	fclose(dst); // close a stream dst
}
