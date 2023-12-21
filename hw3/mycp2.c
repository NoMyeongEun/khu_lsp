#include <stdio.h>

#define	MAX_BUF	1024

main(int argc, char *argv[])
{
	FILE	*src, *dst;
	char	buf[MAX_BUF];
	int		count;

	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	if ((src = fopen(argv[1], "rb")) == NULL)  { // open a file whose filepath is the value of the string argv[1], as read access binary file type
		perror("fopen");
		exit(1);
	}

	if ((dst = fopen(argv[2], "wb")) == NULL)  { // open a file whose filepath is the value of the string argv[2], as write access binary file type
		perror("fopen");
		exit(1);
	}

	while ((count = fread(buf, 1, MAX_BUF, src)) > 0)  { // direct I/O(Binary I/O) input function. at this buffer buf, the object size is 1, the total size is MAX_BUF, from file src. return : number of objects read if OK, 0 on end of file or error 
		fwrite(buf, 1, count, dst); // direct I/O(Binary I/o) output function. from this buffer, the object size is 1, the total size is count, at file dst
	}

	fclose(src); // close a stream src
	fclose(dst); // close a stream dst
}
