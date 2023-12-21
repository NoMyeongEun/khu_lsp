#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "fifo.h"

main()
{
    char    fname[MAX_FIFO_NAME];
    int     fd, sfd, n;
    MsgType msg;

    // Create a unique FIFO filename
    sprintf(fname, ".fifo%d", getpid());
    // Create a FIFO with read and write permissions for the user
    if (mkfifo(fname, 0600) < 0)  {
        perror("mkfifo");
        exit(1);
    }

    // Open the FIFO for reading and writing
    if ((fd = open(fname, O_RDWR)) < 0)  {
        perror("open");
        exit(1);
    }
    // Open the server FIFO for reading and writing
    if ((sfd = open(SERV_FIFO, O_RDWR)) < 0)  {
        perror("open");
        exit(1);
    }

    // Prepare message to be sent
    strcpy(msg.returnFifo, fname);
    sprintf(msg.data, "This is a request from %d.", getpid());
    // Write the message to the server FIFO
    write(sfd, (char *)&msg, sizeof(msg));
    printf("Sent a request.....");

    // Read the reply from the FIFO
    if ((n = read(fd, (char *)&msg, sizeof(msg))) < 0)  {
        perror("read");
        exit(1);
    }

    printf("Received reply: %s\n", msg.data);

    // Close file descriptors and remove the FIFO file
    close(fd);
    close(sfd);

    if (remove(fname) < 0)  {
        perror("remove");
        exit(1);
    }
}
