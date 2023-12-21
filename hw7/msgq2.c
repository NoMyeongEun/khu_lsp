#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "msg.h" // Include the message header file with definitions

#define	MSG_MODE	(0600) // Message queue mode

int main() {
    int msqid, n; // Message queue ID and message size
    MsgType msg; // Message structure

    // Access the existing message queue using the defined key
    if ((msqid = msgget(MSG_KEY, MSG_MODE)) < 0)  {
        perror("msgget"); // Error handling for message queue access
        exit(1);
    }

    // Compose a request message and send it to the message queue
    msg.type = MSG_REQUEST; // Set the message type for the request
    sprintf(msg.data, "This is a request from %d.", getpid()); // Create the request message
    if (msgsnd(msqid, (char *)&msg, sizeof(msg), 0) < 0)  {
        perror("msgsnd"); // Error handling for message send
        exit(1);
    }
    printf("Sent a request.....");

    // Receive a reply message from the message queue
    if ((n = msgrcv(msqid, (char *)&msg, sizeof(msg), MSG_REPLY, 0)) < 0)  {
        perror("msgrcv"); // Error handling for message receive
        exit(1);
    }
    printf("Received reply: %s\n", msg.data);
    return 0;
}

