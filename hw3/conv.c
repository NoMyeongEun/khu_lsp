#include <stdio.h>
#include <string.h>
#include "record.h"

main(int argc, char *argv[])
{
	FILE	*src, *dst;
	Record	rec;

	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source dest\n", argv[0]);
		exit(1);
	}

	if ((src = fopen(argv[1], "rt")) == NULL)  { // open a file as read access text type
		perror("fopen");
		exit(1);
	}
	if ((dst = fopen(argv[2], "wb")) == NULL)  { // open a file as write access binary type
		perror("fopen");
		exit(1);
	}

	while (fgets(rec.stud, SMAX, src))  { // read line at a time from file src by SMAX at rec.stud buffer
		*strchr(rec.stud, '\n') = '\0'; // if there is '\n' in rec.stud, change to '\0'
		fgets(rec.num, NMAX, src); // read line at a time from file src by NMAX at rec.num buffer
		*strchr(rec.num, '\n') = '\0'; // if there is '\n' in rec.num, change to '\0'
		fgets(rec.dept, DMAX, src); // read line at a time from file src by DMAX at rec.dept buffer
		*strchr(rec.dept, '\n') = '\0'; // if there is '\n' in rec.dept, change to '\0'

		fwrite(&rec, sizeof(rec), 1, dst); // write the rec(rec.stud, rec.num, rec.dept) at the file dst
	}

	fclose(src); // close a stream src
	fclose(dst); // close a stream dst
}
