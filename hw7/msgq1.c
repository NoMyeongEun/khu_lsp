#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "fifo.h" // Include the header file containing definitions

void SigIntHandler(int signo) {
    // Signal handler for SIGINT (Ctrl+C)
    if (remove(SERV_FIFO) < 0)  {
        perror("remove");
        exit(1);
    }
    exit(0);
}

int main() {
    int fd, cfd, n; // File descriptors and size
    MsgType msg; // Message structure

    // Signal handling for SIGINT (Ctrl+C)
    if (signal(SIGINT, SigIntHandler) == SIG_ERR)  {
        perror("signal");
        exit(1);
    }

    // Create the server FIFO if it doesn't exist
    if (mkfifo(SERV_FIFO, 0600) < 0)  {
        if (errno != EEXIST)  {
            perror("mkfifo");
            exit(1);
        }
    }

    // Open the server FIFO
    if ((fd = open(SERV_FIFO, O_RDWR)) < 0)  {
        perror("open");
        exit(1);
    }

    // Continuously read messages from the server FIFO
    while (1)  {
        // Read a message from the server FIFO
        if ((n = read(fd, (char *)&msg, sizeof(msg))) < 0)  {
            if (errno == EINTR)  {
                continue; // Continue if interrupted by a signal
            } else  {
                perror("read");
                exit(1);
            }
        }
        printf("Received request: %s.....", msg.data);

        // Open the client FIFO to send a reply
        if ((cfd = open(msg.returnFifo, O_WRONLY)) < 0)  {
            perror("open");
            exit(1);
        }
        // Construct a reply message and write it to the client FIFO
        sprintf(msg.data, "This is a reply from %d.", getpid());
        write(cfd, (char *)&msg, sizeof(msg));
        close(cfd);
        printf("Replied.\n");
    }
}

