#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

main(int argc, char *argv[])
{
	struct stat	statbuf;
	char		*mode;
	int			i;

	for (i = 1 ; i < argc ; i++)  {
		printf("%s: ",argv[i]);
		/* lstat : read the status of specified file and store at the struct statbuf
		 * 	   return : 0 if OK, -1 on error
		 * 	   while the 'stat' system call stores the state of file where symbolic link file points, 'lstat' system call stores the state of the file symbolic link itself.
		 */
		if (lstat(argv[i], &statbuf) < 0)  {
			perror("lstat");
			continue;
		}
		/* macro functons for file types */
		if (S_ISREG(statbuf.st_mode)) /* if file type is regular file, then return true */
			mode = "regular";
		else if (S_ISDIR(statbuf.st_mode)) /* same for direcotry type */
			mode = "directory";
		else if (S_ISCHR(statbuf.st_mode)) /* character special : for device by byte */
			mode = "character special";
		else if (S_ISBLK(statbuf.st_mode)) /* block special : for divice by block */
			mode = "block special";
		else if (S_ISFIFO(statbuf.st_mode)) /* pipe of FIFO file */
			mode = "FIFO";
		else if (S_ISLNK(statbuf.st_mode)) /* symbolic link */
			mode = "symbolic link";
		else if (S_ISSOCK(statbuf.st_mode)) /* socket */
			mode = "socket";
		printf("%s\n", mode);
		printf("\tst_mode = %d\n", statbuf.st_mode); /* file type & mode (permissions) */
		printf("\tst_ino = %d\n", statbuf.st_ino); /* i-node number (serial number) */
		printf("\tst_dev = %d\n", statbuf.st_dev); /* device number (file system) */
		printf("\tst_rdev = %d\n", statbuf.st_rdev); /* device number for special files */
		printf("\tst_nlink = %d\n", statbuf.st_nlink); /* number of links */
		printf("\tst_uid = %d\n", statbuf.st_uid); /* user ID of owner */
		printf("\tst_gid = %d\n", statbuf.st_gid); /* group ID of owner */
		printf("\tst_size = %d\n", statbuf.st_size); /* size in bytes, for regular files */
		printf("\tst_atime = %d\n", statbuf.st_atime); /* time of last access */
		printf("\tst_mtime = %d\n", statbuf.st_mtime); /* time of last modification */
		printf("\tst_ctime = %d\n", statbuf.st_ctime); /* time of last file status change */
		printf("\tst_blksize = %d\n", statbuf.st_blksize); /* best I/O block size */
		printf("\tst_blocks = %d\n", statbuf.st_blocks); /* no. of 512-byte blocks allocated */
	}
}
