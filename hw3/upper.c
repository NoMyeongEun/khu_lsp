#include <stdio.h>

main(int argc, char *argv[])
{
	FILE		*src;
	char		ch;

	if (argc != 2)  {
		fprintf(stderr, "Usage: %s source", argv[0]);
		exit(1);
	}

	if ((src = fopen(argv[1], "rt")) == NULL)  { // open a file as read access text type
		perror("fopen");
		exit(1);
	}

	while ((ch = fgetc(src)) != EOF)  { // read the file src by charcacter, and store the value at the character ch
		if (ch >= 'a' && ch <= 'z') putchar(ch - 32); // if the character is the lower case, print the character that is changed to the upper case
		else putchar(ch); // else, just print the original character
	}

	fclose(src); // close a stream
}
