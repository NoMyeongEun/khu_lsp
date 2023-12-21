#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include "tcp.h"


int	Sockfd; /* declared as global variable for using in CloseServer() Signal Handler */

/* Define Signal Handler - Even though the process is terminated abnormally, the socket descriptor must be closed */
void
CloseServer()
{
	close(Sockfd); /* A socket must be closed before process exited */
	printf("\nTCP Server exit.....\n");

	exit(0);
}

/* server */
main(int argc, char *argv[])
{
	int					newSockfd, cliAddrLen, n;
	struct sockaddr_in	cliAddr, servAddr; /* sockaddr_in -> IPv4 */
	MsgType				msg;
    pid_t               pid;

	signal(SIGINT, CloseServer); /* register a signal handler */

	if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  { /* creat a socket - protocol is IPv4 and type is stream socket(TCP) */
		perror("socket");
		exit(1);
	}

	/* To get an server address for Socket binding - conversion to host byte ordr -> network byte order */
	bzero((char *)&servAddr, sizeof(servAddr)); /* initialize : set the pointer servAddr to 0  */
	servAddr.sin_family = PF_INET; /* protocol family is IPv4 */
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* IP ADDRESS -> htonl() : conversion host byte order to network byte order (long integer) , INADDR_ANY : server -> can multiple IP addresses -> allow any of these */
	servAddr.sin_port = htons(SERV_TCP_PORT); /* PORT NUMBER -> conversion host byte order(= port number declared in tcp.h ) to network byte order (short integer - 16bit) */

	if (bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  { /* bind to socket descriptor, for address my socket address, size of address */
		perror("bind");
		exit(1);
	}

	listen(Sockfd, 5); /* 5 connection requests can be queued by while executing accept system call */

	printf("TCP Server started.....\n");

	cliAddrLen = sizeof(cliAddr);
	while (1)  {
		newSockfd = accept(Sockfd, (struct sockaddr *) &cliAddr, &cliAddrLen); /* wait a socket connection from Client -> IF connection success, save client address to cliAddr and return to new socket descriptor */
		if (newSockfd < 0)  {
			perror("accept");
			exit(1);
		}
		
        if ((pid = fork()) < 0)  { /* fork after accept */
		    perror("fork");
		    exit(1);
	    }
	    else if (pid == 0)  { /* child process -> do connection*/
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

		    usleep(10000);
		    close(newSockfd); /* close a connection*/
            return (0);
	    }
        /* parent process -> return to accept */
	}
}