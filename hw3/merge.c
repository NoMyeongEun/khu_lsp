#include <stdio.h>

#define	MAX_BUF	1024

main(int argc, char *argv[])
{
	FILE	*src1, *src2, *dst;
	char	buf[MAX_BUF];
	int		count;

	if (argc != 4)  {
		fprintf(stderr, "Usage: %s source1 source2 dest\n", argv[0]);
		exit(1);
	}

	if ((src1 = fopen(argv[1], "rb")) == NULL)  { // open a file as read access binary type
		perror("fopen");
		exit(1);
	}
	if ((src2 = fopen(argv[2], "rb")) == NULL)  { // open a file as read access binary type
		perror("fopen");
		exit(1);
	}
	if ((dst = fopen(argv[3], "wb")) == NULL)  { // open a file as write access binary type
		perror("fopen");
		exit(1);
	}

	while ((count = fread(buf, 1, MAX_BUF, src1)) > 0)  { // read file by count from src1
		fwrite(buf, 1, count, dst); // write file by count to dst
	}

	while ((count = fread(buf, 1, MAX_BUF, src2)) > 0)  { // read file by count from src2
		fwrite(buf, 1, count, dst); // write file by count to dst
	}

	fclose(src1); // close a stream src1
	fclose(src2); // close a stream src2
	fclose(dst); // close a stream dst
}
