#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

main()
{
	int	fd;
	char	ch = '\0';

	if((fd = creat("file.hole", 0400)) < 0) { // create file whose filename is "file.hole", and permission is Owner Read Only. return value is a file descriptor opened for write-only if OK, -1 on error
		perror("creat");
		exit(1);
	}

	if(lseek(fd, 1023, SEEK_SET) < 0) { // move a file pointer 1023 from the front of a file. return value : a new file offset if OK, -1 on error
		perror("lseek");
		exit(1);
	}
	write(fd, &ch, 1); // write null characater at the end of file

	close(fd); // close a file descriptor
}
