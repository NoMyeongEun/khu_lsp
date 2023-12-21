#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define	MAX_BUF	128

main()
{
    int		n, fd[2]; // fd = file descriptors for the pipe. we need to file descriptors : one for parent process, the other for child process
    pid_t	pid; 
    char	buf[MAX_BUF]; // buffer for saving data

    // Create a pipe
    if (pipe(fd) < 0)  { // return 0 if OK, -1 on error
        perror("pipe"); 
        exit(1);
    }

    // Create a child process
    if ((pid = fork()) < 0)  {
        perror("fork"); 
        exit(1);
    } else if (pid == 0)  { // Child process
        close(fd[1]); // Close write end in the child process
        printf("Child : Wait for parent to send data\n");
        if ((n = read(fd[0], buf, MAX_BUF)) < 0)  {
            perror("read"); 
            exit(1);
        }
        printf("Child : Received data from parent: ");
        fflush(stdout); // Flush the output buffer to ensure immediate printing
        write(STDOUT_FILENO, buf, n);  // Write data to stdout
    } else  { // Parent process
        close(fd[0]); // Close read end in the parent process
        strcpy(buf, "Hello, World!\n"); // save the data at the buffer
        printf("Parent: Send data to child\n");
        write(fd[1], buf, strlen(buf) + 1); // Write data to the pipe
    }

    exit(0);
}

