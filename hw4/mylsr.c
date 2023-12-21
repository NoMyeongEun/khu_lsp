#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_PATH	256

void
JustDoIt(char *path) /* function that will be called recursively */
{
	DIR			*dp;
	struct dirent	*dep;
	struct stat		statbuf;
	char			fullpath[MAX_PATH];

	if ((dp = opendir(path)) == NULL) { /* store the list of file on the direcotry 'path' to the sturct dp */
		perror("opendir");
		exit(0);
	}

	printf("\n%s:\n",path); /* print the current working path */
	while (dep = readdir(dp)) { /* read all the file by a file */ 
		if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0) /* except . (current directory) or .. (parent directory) */
				continue;
		printf("%s\n",dep->d_name); /* print the file name */
	}

	rewinddir(dp); /* set the struct dirent pointer the front of the DIR(eqaul the first value when you call the opendir) */
	while (dep = readdir(dp)) /* again read each file */
	{
		if(strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0) /* except same as before */
			continue;
		strcpy(fullpath, path); /* copy the path to fullpath */
		strcat(fullpath, "/"); /* add "/" to the fullpath */
		strcat(fullpath, dep->d_name); /* add file name to the fullpath. then the fullpath is completed */
		if (lstat(fullpath, &statbuf) < 0)  { /* load the status information from the file */
			perror("lstat");
			exit(1);
		}
		if (S_ISDIR(statbuf.st_mode)) /* if the file is directory, recursive call */
			JustDoIt(fullpath);
	}

	closedir(dp);
}

main()
{
	JustDoIt("."); /* start from current directory */
}
