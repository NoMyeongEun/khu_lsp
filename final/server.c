#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define	MY_ID		16

#define	SERV_TCP_PORT	(7000 + MY_ID)

typedef struct  {
	int	num;
}
	MsgType;

int	Sockfd, newSockfd;
MsgType	msg;

void
CloseServer()
{
	close(Sockfd);
	printf("\nTCP Server exit.....\n");

	exit(0);
}

void
Task()
{
	int num, n, total,threadfd;
	
	threadfd = newSockfd;
	if ((n = read(threadfd, (char *)&msg, sizeof(msg))) < 0)  {
		perror("read");
                exit(1);
	}
	num = msg.num;
	total = 0;
	for (int i = 1; i <= num; i++)
	{
		total += i;
	}
	msg.num = total;
	if (write(threadfd, (char *)&msg, sizeof(msg)) < 0)  {
		perror("write");
		exit(1);
	}
	close(threadfd);
	pthread_exit(NULL);
}

main(int argc, char *argv[])
{
	int			cliAddrLen, n;
	FILE			*fp;
	struct sockaddr_in	cliAddr, servAddr;
	pthread_t		tid1;

	signal(SIGINT, CloseServer);

	if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERV_TCP_PORT);

	if (bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
		perror("bind");
		exit(1);
	}

	listen(Sockfd, 5);

	cliAddrLen = sizeof(cliAddr);
	while (1)  {
		newSockfd = accept(Sockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);
		if (newSockfd < 0)  {
			perror("accept");
			exit(1);
		}

		if (pthread_create(&tid1, NULL, (void *)Task, (void *)0) < 0)  {
			perror("pthread_create");
			exit(1);
		}
	}
}
