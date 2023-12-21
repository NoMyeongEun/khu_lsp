#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

static int Pfd1[2], Pfd2[2]; // Pipe file descriptors for communication

void TELL_WAIT(void) {
    // Create two pipes for communication
    if (pipe(Pfd1) < 0 || pipe(Pfd2) < 0)  {
        perror("pipe"); // Error handling for pipe creation failure
        exit(1);
    }
}

void TELL_PARENT(void) {
    // Notify the parent process by writing to the pipe
    if (write(Pfd2[1], "c", 1) != 1)  {
        perror("write"); // Error handling for write failure
        exit(1);
    }
}

void WAIT_PARENT(void) {
    char c;

    // Wait for the parent's notification by reading from the pipe
    if (read(Pfd1[0], &c, 1) != 1)  {
        perror("read"); // Error handling for read failure
        exit(1);
    }
    if (c != 'p')  {
        fprintf(stderr, "WAIT_PARENT: incorrect data"); // Incorrect data received
        exit(1);
    }
}

void TELL_CHILD(void) {
    // Notify the child process by writing to the pipe
    if (write(Pfd1[1], "p", 1) != 1)  {
        perror("write"); // Error handling for write failure
        exit(1);
    }
}

void WAIT_CHILD(void) {
    char c;

    // Wait for the child's notification by reading from the pipe
    if (read(Pfd2[0], &c, 1) != 1)  {
        perror("read"); // Error handling for read failure
        exit(1);
    }
    if (c != 'c')  {
        fprintf(stderr, "WAIT_CHILD: incorrect data"); // Incorrect data received
        exit(1);
    }
}

