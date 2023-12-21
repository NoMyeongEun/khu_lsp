#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

main()
{
	umask(0); /* do not mask anything */
	if (creat("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | 
				S_IROTH | S_IWOTH) < 0)  { /* create a file named "bar", as permission user = read, write and group = read, write, other = read, write . return : a file descripter opened for write-only if OK, -1 on error*/
		perror("creat");
		exit(1);
	}

	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH); /* mask on permission read_group, write_group, read_other, write_other */
        if (creat("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | 
                                S_IROTH | S_IWOTH) < 0)  { /* because of the mask, although specified on option in creat system call, the permissons that masked are not on */
                perror("creat");
                exit(1);
        }
}
