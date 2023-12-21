#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "tcp.h"

/* client */
main(int argc, char *argv[])
{
	int					sockfd, n;
	struct sockaddr_in	servAddr; /* sockaddr_in -> IPv4*/
	MsgType				msg;
	struct hostent		*hp;

	if (argc != 2)  {
		fprintf(stderr, "Usage: %s IPaddress\n", argv[0]);
		exit(1);
	}

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  { /* creat a socket - protocol is IPv4 and type is stream socket */
		perror("socket");
		exit(1);
	}
	/* To get an server address for Socket binding */
	bzero((char *)&servAddr, sizeof(servAddr)); /* set the pointer servAddr to 0  */
	servAddr.sin_family = PF_INET; /* protocol is IPv4*/
	servAddr.sin_port = htons(SERV_TCP_PORT); /* conversion between host byte order to network byte order (short)*/

	/* To get an IP address */
	if (isdigit(argv[1][0]))  { /* if the input argument is type of dotted decimal */
		servAddr.sin_addr.s_addr = inet_addr(argv[1]); /* change to 4byte integer network byte order - no need to htonl()*/
	}
	else  { /* if it is DNS type */
		if ((hp = gethostbyname(argv[1])) == NULL)  {/* change DNS to IP address (saved in h_addr) as network byte ordred address*/
			fprintf(stderr, "Unknown host: %s\n", argv[1]);
			exit(1);
		}
		memcpy(&servAddr.sin_addr, hp->h_addr, hp->h_length); /* add Ip address and its length to struct sockadd_in servAddr's sin_addr(save network byte ordered address) */
	}

	if (connect(sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  { /* establish a connection on a socket using the server address got above */
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