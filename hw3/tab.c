#include <stdio.h>

enum { FALSE, TRUE };

void
conv(char *fname)
{
	FILE	*src, *dst;
	char	fdest[40];
	int		ch, first;

	if ((src = fopen(fname, "rt")) == NULL)  { // open a file as read access text type
		perror("fopen");
		return;
	}

	strcpy(fdest, fname); // store the filename to the value fdest
	strcat(fdest, ".t"); // attach the extension (concatenation)
	if ((dst = fopen(fdest, "wt")) == NULL)  { // open a file as write access text type
		perror("fopen");
		return;
	}

	first = TRUE; // flag whether it is the one of the tabs that locates first of the line
	while ((ch = fgetc(src)) != EOF)  { // get a character at a time from the file src until the file ends
		if (first && ch == '\t')  { // if the character is tab('\t') and(&&) the tab that is one of the tabs locates the first of the line 
			fputc(' ', dst); // put the space character(32) at the file
			fputc(' ', dst); 
			fputc(' ', dst);
			fputc(' ', dst);
		}
		else  {
			fputc(ch, dst); // write the character that you read
			if (first) // when tabs end
				first = FALSE; // change the first FALSE
			if (ch == '\n') // if the line is end
				first = TRUE; // change the first TRUE
		}
	}

	fclose(src); // close a stream src
	fclose(dst); // close a stream dst
}

main(int argc, char *argv[])
{
	while (--argc)  { // for all argument values -> argv[argc], argv[argc-1], ... , argv[1]
		conv(argv[argc]); // convert the first tabs of each line of the file to four spaces
	}
}
