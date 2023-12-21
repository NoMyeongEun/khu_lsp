#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_BUF 1024

main(int argc, char *argv[])
{
	int	fd1, fd2, count;
	char	buf[MAX_BUF];

	if (argc != 3) {
		printf("Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	if ((fd1 = open(argv[1], O_RDONLY)) < 0) { // open file whose pathname equals to the value of the string argv[1], option -> Others ReaD ONly, return value : a file descriptor if OK, -1 on error
		perror("open");
		exit(1);
	}

	if((fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) { // permission : O_WRONLY = WriteONly, O_CREAT = CREATE file if file doesn't exist, O_TRUNC = TRUNCATE the existing file when you open the file as write mode, 0644 -> Owner : Read and Write(4 + 2), Group : Read(4), Others : Read(4) 
		perror("open");
		exit(1);
	}

	while ((count = read(fd1, buf, MAX_BUF)) > 0) { // at fd1 that is opened, at this buffer buf, read maximum MAX_BUF bytes, return : number of bytes read if OK, 0 on end of file, -1 on error -> while condition = count is positive
		write(fd2, buf, count); // at fd2 that is opened, from this buffer buf, write maximum MAX_BUF bytes. return : number of bytes written if OK, -1 on error 
	}

	close(fd1); // close a file descriptor fd1
	close(fd2); // close a file descriptor fd2
}
