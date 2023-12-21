#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int fd[2]; // File descriptors for the pipe
    pid_t child; // Process ID for the child process

    // Create a pipe
    if (pipe(fd) == -1)  {
        perror("pipe"); // Pipe creation error
        exit(1);
    }

    // Create a child process
    child = fork();
    if (child == (pid_t)(-1))  {
        perror("fork"); // Fork error
        exit(1);
    } else if (child == (pid_t)0) { // Child process
        close(1); // Close standard output
        close(fd[0]); // Close the read end of the pipe
        
        // Redirect stdout to the write end of the pipe
        if (dup(fd[1]) == -1)  {
            perror("dup"); // Duplication error
            exit(1);
        }

        // Execute the command "ls -l"
        if ((execlp("ls", "ls", "-l", NULL)) == -1)  {
            perror("execlp"); // Execution error
            exit(1);
        }
    } else { // Parent process
        close(0); // Close standard input
        close(fd[1]); // Close the write end of the pipe
        
        // Redirect stdin to the read end of the pipe
        if (dup2(fd[0], 0) == -1)  {
            perror("dup"); // Duplication error
            exit(1);
        }

        // Execute the command "more"
        if ((execlp("more", "more", NULL)) == -1)  {
            perror("execlp"); // Execution error
            exit(1);
        }
    }

    return 0;
}

