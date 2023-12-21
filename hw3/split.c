#include <stdio.h>
#include <unistd.h>

#define	MAX_BUF	1024

long
filesize(FILE *fp) // a function that finds the total file size
{
	long	cur, size;

	cur = ftell(fp); // find where the current file pointer is and save it in a variable cur = remember the original file pointer position
	fseek(fp, 0L, SEEK_END); // (cf. OL = long type 0) move file pointer to the end of file
	size = ftell(fp); // find where the current file pointer is and save it in a variable size = at the end of file -> becomes the size of file
	fseek(fp, cur, SEEK_SET); // move the file pointer by cur from the front of the file = return the file pointer to its original poisition 
	return(size); 
}

main(int argc, char *argv[])
{
	FILE	*src, *dst1, *dst2;
	char	buf[MAX_BUF];
	int		count, size;

	if (argc != 4)  {
		fprintf(stderr, "Usage: %s source dest1 dest2\n", argv[0]);
		exit(1);
	}

	if ((src = fopen(argv[1], "rb")) == NULL)  { // open file as read access binary type
		perror("fopen");
		exit(1);
	}
	if ((dst1 = fopen(argv[2], "wb")) == NULL)  { // open file as write access binary type
		perror("fopen");
		exit(1);
	}
	if ((dst2 = fopen(argv[3], "wb")) == NULL)  { // open file as write access binary type
		perror("fopen");
		exit(1);
	}

	size = filesize(src) / 2; // To find the each size of split file, divide the total file size by two

	while (size > 0)  { // for the first half of the src file -> dst1
		count = (size > MAX_BUF) ?  MAX_BUF : size; // count means the maximum size that can be currently held in the buffer  
		fread(buf, 1, count, src); // binary I/O input function. at buffer buf, the size of object is 1, 'count' objects, from the file src
		fwrite(buf, 1, count, dst1); // write at the file dst1 by count
		size -= count; // write at file by count, so exclude as much as you write from the file size
	}

	while ((count = fread(buf, 1, MAX_BUF, src)) > 0)  { // for the second half of the src file -> dst2. save the actual read value to count, when trying to read by MAX_BUF in src
		fwrite(buf, 1, count, dst2); // write at the file dst2 by count
	}

	fclose(src); // close a stream src
	fclose(dst1); // close a stream dst1
	fclose(dst2); // close a stream dst2
}
