#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>


#define MAX_BUF 20

main()
{
	DIR		*dp;
	struct dirent	*dep;
	struct stat	statbuf;
	struct group	*grp;
	struct passwd	*pwd;
	struct tm	*lctm;
	char		mode[11] = "----------";
	char		time_buf[MAX_BUF];
	char		link_buf[1024];
	long		total;

	if ((dp = opendir(".")) == NULL) { /* load the file list for the current working directory */
			perror("opendir");
			exit(0);
	}

	total = 0; /* initialize the total block size */
	while(dep = readdir(dp)) /* read for each file */
	{
		if (lstat(dep->d_name, &statbuf) < 0) /* load the file status */
		{
			perror("lstat");
			continue;
		}
		if (dep ->d_name[0] != '.') /* if the file name do not start with '.' */
			total += statbuf.st_blocks; /* add the block size of file at the total */
	}
	printf("total %ld\n",total / 2); /* st_blocks : 512 kybtes -> divide into half) */
	rewinddir(dp); /* return the pointer front of the directory */
	while(dep = readdir(dp)) /* read again for each file */
	{
		if (dep->d_name[0] == '.') /* skip for the hidden file */
			continue;
		if (lstat(dep->d_name, &statbuf) < 0) /* load the file status */
		{
			perror("lstat");
			continue;
		}
		/* the first character -> the type of file */
		if (S_ISREG(statbuf.st_mode)) 
			mode[0] = '-';
		else if (S_ISDIR(statbuf.st_mode))
			mode[0] = 'd';
		else if (S_ISCHR(statbuf.st_mode))
			mode[0] = 'c';
		else if (S_ISBLK(statbuf.st_mode))
			mode[0] = 'b';
		else if (S_ISFIFO(statbuf.st_mode))
			mode[0] = 'p';
		else if (S_ISLNK(statbuf.st_mode))
			mode[0] = 'l';
		else if (S_ISSOCK(statbuf.st_mode))
			mode[0] = 's';
		else
			mode[0] = '?';
		
		/* 9 characters for the permission of the file (read, write, execute for user, group, others) */
		if (statbuf.st_mode & S_IRUSR)
			mode[1] = 'r';
		if (statbuf.st_mode & S_IWUSR)
			mode[2] = 'w';
		if (statbuf.st_mode & S_IXUSR)
			mode[3] = 'x';
		if (statbuf.st_mode & S_IRGRP)
			mode[4] = 'r';
		if (statbuf.st_mode & S_IWGRP)
			mode[5] = 'w';
		if (statbuf.st_mode & S_IXGRP)
			mode[6] = 'x';
		if (statbuf.st_mode & S_IROTH)
			mode[7] = 'r';
		if (statbuf.st_mode & S_IWOTH)
			mode[8] = 'w';
		if (statbuf.st_mode & S_IXOTH)
			mode[9] = 'x';
		printf("%s ",mode); /* print the mode(type + permission) */
		printf("%ld ",statbuf.st_nlink); /* print the number of hard links */
		uid_t u_id = statbuf.st_uid; /* store the uid */
		pwd = getpwuid(u_id); /* get more information about user by u_id */
		printf("%s ",pwd->pw_name); /* find the user name (for string) and print that */
		gid_t g_id = statbuf.st_gid; /* do for gid same to uid */
		grp = getgrgid(g_id);
		printf("%s ",grp->gr_name);
		printf("%ld ",statbuf.st_size); /* print the size of file (number of bytes) */
		lctm = localtime(&statbuf.st_mtime); /* store the last modification time as localtime */
		
		strftime(time_buf, MAX_BUF, "%b %d %H:%M", lctm); /* change the time same as the format - Mon Day HH:MM */
		printf("%s ", time_buf); /* print the time */
		printf("%s",dep->d_name); /* print the file name */
		if (S_ISLNK(statbuf.st_mode))  { /* if it is the symbolic link type */
			int length = readlink(dep->d_name, link_buf, sizeof(link_buf)); /* find the original file that the symbolic link points */
			link_buf[length] = '\0';
			printf(" -> %s",link_buf); /* print the original file name */
		}
		printf("\n");
	}

	closedir(dp);
}
