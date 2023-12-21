#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "udp.h"


int	Sockfd; /* declared as global variable for using in CloseServer() Signal Handler */

/* Define Signal Handler - Even though the process is terminated abnormally, the socket descriptor must be closed */
void
CloseServer()
{
	close(Sockfd); /* A socket must be closed before process exited */
	printf("\nUDP Server exit.....\n");

	exit(0);
}

/* server */
main(int argc, char *argv[])
{
	int					cliAddrLen, n;
	struct sockaddr_in	cliAddr, servAddr; /* sockaddr_in -> IPv4 */
	MsgType				msg;

	signal(SIGINT, CloseServer); /* register a signal handler */

	if ((Sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)  { /* creat a socket - protocol is IPv4 and type is datagram socket(UDP) */
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr)); /* initialize : set the pointer servAddr to 0  */
	servAddr.sin_family = PF_INET; /* protocol family is IPv4 */
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* IP ADDRESS -> htonl() : conversion host byte order to network byte order (long integer) , INADDR_ANY : server -> can multiple IP addresses -> allow any of these */
	servAddr.sin_port = htons(SERV_UDP_PORT); /* PORT NUMBER -> conversion host byte order(= port number declared in udp.h ) to network byte order (short integer - 16bit) */

	if (bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  { /* bind to socket descriptor, for address my socket address, size of address */
		perror("bind");
		exit(1);
	}

	printf("UDP Server started.....\n");

	cliAddrLen = sizeof(cliAddr);
	while (1)  {
		/* UDP -> connectionless -> NO NEED TO CONNECT */
		if ((n = recvfrom(Sockfd, (char *)&msg, sizeof(msg), /* read message from client, save client address to cliAddr */
					0, (struct sockaddr *)&cliAddr, &cliAddrLen)) < 0)  {
			perror("recvfrom");
			exit(1);
		}
		printf("Received request: %s.....", msg.data);

		msg.type = MSG_REPLY;
		sprintf(msg.data, "This is a reply from %d.", getpid());
		if (sendto(Sockfd, (char *)&msg, sizeof(msg),  /* response : write a reply message to cliAddr got above*/
					0, (struct sockaddr *)&cliAddr, cliAddrLen) < 0)  {
			perror("sendto");
			exit(1);
		}
		printf("Replied.\n");
	}
}