#include <stdio.h>
#include <unistd.h>
#include "record.h"

void
print_record(Record *rp) // a function that prints all member variables
{
	puts(rp->stud); // print string 
	puts(rp->num);
	puts(rp->dept);
}

main(int argc, char *argv[])
{
	FILE	*fp;
	Record	rec;

	if (argc != 2)  {
		fprintf(stderr, "Usage: %s file\n", argv[0]);
		exit(1);
	}

	if ((fp = fopen(argv[1], "r+b")) == NULL)  { // open a file as read and write(read basic) access binary type
		perror("fopen");
		exit(1);
	}

	printf("-----Ordered Record List-----\n");

	while (fread(&rec, sizeof(rec), 1, fp))  { // print record list as ordered 
		print_record(&rec); // print by record
	}
	rewind(fp); // turn back the file pointer to the front of the file
	getchar();  

	printf("-----Shuffled Record List (3, 6, 2, 4, 1, 5)-----\n"); // print as shuffled order

	fseek(fp, sizeof(rec)*2L, SEEK_SET); // move file pointer by two (recs) from the front of the file
	fread(&rec, sizeof(rec), 1, fp); // read
	print_record(&rec); // print

	fseek(fp, sizeof(rec)*5L, SEEK_SET); // move file pointer by five (recs) from the front of the file
	fread(&rec, sizeof(rec), 1, fp); // read
	print_record(&rec); // print

	fseek(fp, sizeof(rec)*1L, SEEK_SET); // move file pointer by one (recs) from the front of the file
	fread(&rec, sizeof(rec), 1, fp); // read
	print_record(&rec); // print

	fseek(fp, sizeof(rec)*3L, SEEK_SET); // move file pointer by three (recs) from the front of the file
	fread(&rec, sizeof(rec), 1, fp); //read
	print_record(&rec); //print

	fseek(fp, sizeof(rec)*0L, SEEK_SET); // do not move file pointer from the front of the file -> the first record
	fread(&rec, sizeof(rec), 1, fp); // read
	print_record(&rec); // print

	fseek(fp, sizeof(rec)*4L, SEEK_SET); // move file pointer by four (recs) from the front of the file
	fread(&rec, sizeof(rec), 1, fp); //read
	print_record(&rec); // print
	getchar();

	printf("-----Updated Record List-----\n"); 

	fseek(fp, sizeof(rec)*2L, SEEK_SET); // go to the third record of the list
	fread(&rec, sizeof(rec), 1, fp); // read
	strcpy(rec.dept, "Movie"); // strcpy the value "Movie"
	fseek(fp, sizeof(rec)*2L, SEEK_SET); // go to the third record of the list (when you called the fread() -> the file pointer changed)
	fwrite(&rec, sizeof(rec), 1, fp); // write -> change the dept of the third record to "Movie"

	fseek(fp, sizeof(rec)*5L, SEEK_SET); // go to the last record of the list
	fread(&rec, sizeof(rec), 1, fp); // read
	strcpy(rec.dept, "Baseball"); // string copy the value "Baseball"
	fseek(fp, sizeof(rec)*5L, SEEK_SET); // again go to the last record of the list (because the file pointer changed)
	fwrite(&rec, sizeof(rec), 1, fp); // write -> change the dept of the last record to "Baseball"

	rewind(fp); // turn back the file pointer to the front of the file
	while (fread(&rec, sizeof(rec), 1, fp))  { // read by rec from the file
		print_record(&rec); // print by rec from the file
	}

	fclose(fp); // close a stream fp
}
