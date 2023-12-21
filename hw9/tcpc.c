#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "tcp.h"

/* client */
main(int argc, char *argv[])
{
	int					sockfd, n;
	struct sockaddr_in	servAddr;
	MsgType				msg;

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  { /* creat a socket - protocol is IPv4 and type is stream socket */
		perror("socket");
		exit(1);
	}

	/* To get an server address for Socket binding */
	bzero((char *)&servAddr, sizeof(servAddr)); /* set the pointer servAddr to 0  */
	servAddr.sin_family = PF_INET; /* protocol is IPv4*/
	servAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR); /* In tcp.h, we defined SERVER_HOST_ADDR as dotted decimal. change to 4byte integer network byte order - no need to htonl()*/
	servAddr.sin_port = htons(SERV_TCP_PORT); /* conversion between host byte order to network byte order (short)*/

	/* connection - three hand shaking : need three packets exchange */
	if (connect(sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {/* establish a connection on a socket using the server address got above */
		perror("connect");
		exit(1);
	}

	msg.type = MSG_REQUEST;
	sprintf(msg.data, "This is a request from %d.", getpid()); /* save request message to msg.data */
	if (write(sockfd, (char *)&msg, sizeof(msg)) < 0)  { /* request to server : write a message on server */
		perror("write");
		exit(1);
	}
	printf("Sent a request.....");
	
		
	if ((n = read(sockfd, (char *)&msg, sizeof(msg))) < 0)  { /* check the response - read the message from socket */
		perror("read");
		exit(1);
	}
	printf("Received reply: %s\n", msg.data);

	close(sockfd); /* close a socket */
}