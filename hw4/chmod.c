#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

main()
{
	struct stat	statbuf;

	if (stat("bar", &statbuf) < 0)  { /* load the status of file to know the current mode */
		perror("stat");
		exit(1);
	}

	if (chmod("bar", (statbuf.st_mode & ~S_IWGRP) | S_ISUID) < 0)  { /*  remove the permisson on write for group from the current mode(permission) and set user-id on execution(euid to be file owner. if not, then ruid will be.) */
		perror("chmod");
		exit(1);
	}

	if (chmod("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0)  { /* change mode to read_user, write_user, read_group, read_other */
		perror("chmod");
		exit(1);
	}
}
