#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_BUF 128

int main() {
    int n, fd[2];
    pid_t pid;
    char buf[MAX_BUF];

    if (pipe(fd) < 0) {
        perror("pipe");
        exit(1);
    }

    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        close(fd[1]);
        printf("Child: Wait for parent to send data\n");
        if ((n = read(fd[0], buf, MAX_BUF)) < 0) {
            perror("read");
            exit(1);
        }
        printf("Child: Received data from parent: ");
        fflush(stdout);
        write(STDOUT_FILENO, buf, n);

        // Child sends a message to the parent
        close(fd[0]); // Close the read end
        sprintf(buf, "Message from child to parent\n");
        printf("Child: Send message to parent\n");
        write(fd[1], buf, strlen(buf) + 1);
    } else {
        close(fd[0]);
        strcpy(buf, "Hello, World!\n");
        printf("Parent: Send data to child\n");
        write(fd[1], buf, strlen(buf) + 1);

        // Parent waits for the child's message
        close(fd[1]); // Close the write end
        if ((n = read(fd[0], buf, MAX_BUF)) < 0) {
            perror("read");
            exit(1);
        }
        printf("Parent: Received message from child: %s\n", buf);
    }

    exit(0);
}

