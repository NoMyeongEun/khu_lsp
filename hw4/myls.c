#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

main()
{
	DIR				*dp;
	struct dirent	*dep;

	if ((dp = opendir(".")) == NULL)  { /* read the list of files that are on the current directory and store at stuct DIR. opendir return the pointer of DIR, NULL on error */
		perror("opendir");
		exit(0);
	}

	while (dep = readdir(dp))  { /* whenever call readdir system call, return a file entry from DIR. readdir return the pointer of sturct dirent if OK, NULL at the end of directory or error. */
		printf("%s\n", dep->d_name); /* struct dirent has char d_name[NAME_MAX + 1] that means null-terminated file name */
	}

	closedir(dp);
}
