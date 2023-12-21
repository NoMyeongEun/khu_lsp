#include <stdio.h>

main(int argc, char *argv[])
{
	FILE	*src, *dst;
	int		ch;

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

	while ((ch = fgetc(src)) != EOF) { // input function character at a time. from the FILE where the pointer src points. return : next Character if OK, EOF on end of file or error
		fputc(ch, dst); // output function character at a time. write the value of the character ch at the FILE where the pointer dst points. return ch if OK, EOF on error
	}

	fclose(src); // close a stream src
	fclose(dst); // close a stream dst
}
