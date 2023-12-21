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
	if (remove(UNIX_STR_PATH) < 0)  { /* remove UNIX_STR_PATH for FIFO*/
		perror("remove");
	}

	printf("\nUNIX-domain Connection-Oriented Server exit.....\n");

	exit(0);
}

/* server */
main(int argc, char *argv[])
{
	int					newSockfd, servAddrLen, cliAddrLen, n;
	struct sockaddr_un	cliAddr, servAddr; /* sockaddr_un -> unix */
	MsgType				msg;

	signal(SIGINT, CloseServer); /* register a signal handler */

	if ((Sockfd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0)  { /* creat a socket - protocol is UNIX and type is stream socket(TCP) */
		perror("socket");
		exit(1);
	}

	/* get a server address */
	bzero((char *)&servAddr, sizeof(servAddr)); /* initialize : set the pointer servAddr to 0  */
	servAddr.sun_family = PF_UNIX; /* protocol family is UNIX */
	strcpy(servAddr.sun_path, UNIX_STR_PATH); /* save UNIX_STR_PATH to FIFO PATH NAME(sun_path)*/
	servAddrLen = strlen(servAddr.sun_path) + sizeof(servAddr.sun_family); /* serveraddress is sun_path + sun_family */

	if (bind(Sockfd, (struct sockaddr *) &servAddr, servAddrLen) < 0)  {  /* bind to socket descriptor, for address my socket address, size of address */
		perror("bind");
		exit(1);
	}

	listen(Sockfd, 5); /* 5 connection requests can be queued by while executing accept system call */

	printf("UNIX-domain Connection-Oriented Server started.....\n");

	cliAddrLen = sizeof(cliAddr);
	while (1)  {
		newSockfd = accept(Sockfd, (struct sockaddr *) &cliAddr, &cliAddrLen); /* wait a socket connection from Client -> IF connection success, save client address to cliAddr and return to new socket descriptor */
		if (newSockfd < 0)  {
			perror("accept");
			exit(1);
		}
		
		if ((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0)  { /* read message from client */
			perror("read");
			exit(1);
		}
		printf("Received request: %s.....", msg.data); 

		msg.type = MSG_REPLY;
		sprintf(msg.data, "This is a reply from %d.", getpid());
		if (write(newSockfd, (char *)&msg, sizeof(msg)) < 0)  { /* response : write a reply message */
			perror("write");
			exit(1);
		}
		printf("Replied.\n");

		close(newSockfd); /* close a connection*/
	}
}