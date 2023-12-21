#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "sg.h"


int	Sockfd; /* declared as global variable for using in CloseServer() Signal Handler */

/* Define Signal Handler - Even though the process is terminated abnormally, the socket descriptor must be closed */
void
CloseServer()
{
	close(Sockfd); /* A socket must be closed before process exited */
	printf("\nScatter/Gather TCP Server exit.....\n");

	exit(0);
}


main(int argc, char *argv[])
{
	int					newSockfd, cliAddrLen, n;
	struct sockaddr_in	cliAddr, servAddr; /* sockaddr_in -> IPv4*/
	MsgType				msg;
	HeaderType			hdr;
	struct iovec		iov[2]; 

	signal(SIGINT, CloseServer); /* register a handler */

	if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  { /* creat a socket - protocol is IPv4 and type is stream socket(TCP) */
		perror("socket");
		exit(1);
	}

	/* get a server address */
	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET; /* IPv4*/
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* accept all server address to network byte order type*/
	servAddr.sin_port = htons(SERV_TCP_PORT);

	if (bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  { /* bind to socket descriptor */
		perror("bind");
		exit(1);
	}

	listen(Sockfd, 5); /* 5 connection requests can be queued by while executing accept system call */

	printf("Scatter/Gather TCP Server started.....\n");

	cliAddrLen = sizeof(cliAddr);
	while (1)  {
		newSockfd = accept(Sockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);  /* wait a socket connection from Client -> IF connection success, save client address to cliAddr and return to new socket descriptor */
		if (newSockfd < 0)  {
			perror("accept");
			exit(1);
		}
	
		iov[0].iov_base = (char *)&hdr; /* void *iov_base : starting address of buffer*/
		iov[0].iov_len = sizeof(hdr); /* size_t iov_len : size of buffer */
		iov[1].iov_base = (char *)&msg; /* message exitest in non-continuous in memory*/
		iov[1].iov_len = sizeof(msg);
		if ((n = readv(newSockfd, iov, 2)) < 0)  { /* scatter read - fd, array of iovec, # of iovec */
			perror("read");
			exit(1);
		}
		printf("Received request: %s(%s).....", msg.data, hdr.info);

		strcpy(hdr.info, "REPLY");
		msg.type = MSG_REPLY;
		sprintf(msg.data, "This is a reply from %d.", getpid());
		if (writev(newSockfd, iov, 2) < 0)  { /* gather write - write message that exists non-continuous */
			perror("write");
			exit(1);
		}
		printf("Replied.\n");

		close(newSockfd); /* close a socket */
	}
}