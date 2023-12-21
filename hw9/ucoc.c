#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include "unix.h"


main(int argc, char *argv[])
{
	int					sockfd, n, servAddrLen;
	struct sockaddr_un	servAddr;
	MsgType				msg;

	if ((sockfd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0)  { /* creat a socket - protocol is UNIX and type is stream socket(TCP) */
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));  /* initialize : set the pointer servAddr to 0  */
	servAddr.sun_family = PF_UNIX; /* protocol family is UNIX */
	strcpy(servAddr.sun_path, UNIX_STR_PATH); /* save UNIX_STR_PATH to FIFO PATH NAME(sun_path)*/
	servAddrLen = strlen(servAddr.sun_path) + sizeof(servAddr.sun_family); /* serveraddress is sun_path + sun_family */

	if (connect(sockfd, (struct sockaddr *) &servAddr, servAddrLen) < 0)  {  /* establish a connection on a socket using the server address got above */
		perror("connect");
		exit(1);
	}

	msg.type = MSG_REQUEST;
	sprintf(msg.data, "This is a request from %d.", getpid()); /* save request message to msg.data */
	if (write(sockfd, (char *)&msg, sizeof(msg)) < 0)  { /* request to server */
		perror("write");
		exit(1);
	}
	printf("Sent a request.....");
		
	if ((n = read(sockfd, (char *)&msg, sizeof(msg))) < 0)  { /* check the response - read the message from socket */
		perror("read");
		exit(1);
	}
	printf("Received reply: %s\n", msg.data);

	close(sockfd);  /* close a socket */
}