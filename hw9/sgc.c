#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "sg.h"


main(int argc, char *argv[])
{
	int					sockfd, n;
	struct sockaddr_in	servAddr; /* IPv4 */
	MsgType				msg;
	HeaderType			hdr;
	struct iovec		iov[2];

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  { /* creat a socket - protocol is IPv4 and type is stream socket(TCP) */
		perror("socket");
		exit(1);
	}

	/* get a server address */
	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
	servAddr.sin_port = htons(SERV_TCP_PORT);

	if (connect(sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  { /* establish a connection on a socket using the server address got above */
		perror("connect");
		exit(1);
	}

	strcpy(hdr.info, "REQST");
	msg.type = MSG_REQUEST;
	sprintf(msg.data, "This is a request from %d.", getpid());
	iov[0].iov_base = (char *)&hdr; /* void *iov_base : starting address of buffer*/
	iov[0].iov_len = sizeof(hdr); /* size_t iov_len : size of buffer */
	iov[1].iov_base = (char *)&msg; /* message exitest in non-continuous in memory*/
	iov[1].iov_len = sizeof(msg);
	if (writev(sockfd, iov, 2) < 0)  { /* gather write - write message that exists non-continuous */
		perror("write");
		exit(1);
	}
	printf("Sent a request.....");
		
	if ((n = readv(sockfd, iov, 2)) < 0)  { /* scatter read - fd, array of iovec, # of iovec */
		perror("read");
		exit(1);
	}
	printf("Received reply: %s(%s)\n", msg.data, hdr.info);

	close(sockfd); /* close a socket */
}