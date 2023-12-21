#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <signal.h>
#include "unix.h"


int	Sockfd; /* declared as global variable for using in CloseServer() Signal Handler */

/* Define Signal Handler - Even though the process is terminated abnormally, the socket descriptor must be closed */
void
CloseServer()
{
	close(Sockfd); /* A socket must be closed before process exited */
	if (remove(UNIX_DG_PATH) < 0)  { /* remove UNIX_DG_PATH for FIFO*/
		perror("remove");
	}

	printf("\nUNIX-domain Connection-Less Server exit.....\n");

	exit(0);
}


main(int argc, char *argv[])
{
	int					servAddrLen, cliAddrLen, n;
	struct sockaddr_un	cliAddr, servAddr; /* sockaddr_un -> unix */
	MsgType				msg;

	signal(SIGINT, CloseServer); /* register a signal handler */

	if ((Sockfd = socket(PF_UNIX, SOCK_DGRAM, 0)) < 0)  { /* creat a socket - protocol is UNIX and type is datagram socket(UDP) */
		perror("socket");
		exit(1);
	}
	/* get a server address */
	bzero((char *)&servAddr, sizeof(servAddr)); /* initialize : set the pointer servAddr to 0  */
	servAddr.sun_family = PF_UNIX; /* protocol family is UNIX */
	strcpy(servAddr.sun_path, UNIX_DG_PATH); /* save UNIX_DG_PATH to FIFO PATH NAME(sun_path)*/
	servAddrLen = strlen(servAddr.sun_path) + sizeof(servAddr.sun_family); /* serveraddress is sun_path + sun_family */

	if (bind(Sockfd, (struct sockaddr *) &servAddr, servAddrLen) < 0)  { /* bind to socket descriptor, for address my socket address, size of address */
		perror("bind");
		exit(1);
	}

	printf("UNIX-domain Connection-Less Server started.....\n");

	cliAddrLen = sizeof(cliAddr);
	while (1)  {
		/* UDP -> connection less -> no need to connection */
		if ((n = recvfrom(Sockfd, (char *)&msg, sizeof(msg), 
					0, (struct sockaddr *)&cliAddr, &cliAddrLen)) < 0)  {/* read message from client, save client address to cliAddr */
			perror("recvfrom");
			exit(1);
		}
		printf("Received request: %s.....", msg.data);

		msg.type = MSG_REPLY;
		sprintf(msg.data, "This is a reply from %d.", getpid());
		if (sendto(Sockfd, (char *)&msg, sizeof(msg),
					0, (struct sockaddr *)&cliAddr, cliAddrLen) < 0)  { /* response : write a reply message to cliAddr got above*/
			perror("sendto");
			exit(1);
		}
		printf("Replied.\n");
	}
}