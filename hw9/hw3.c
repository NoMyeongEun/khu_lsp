#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include "select.h"

/* define fds for global variable for using signal handler*/
int	TcpSockfd; /* TCP*/
int	UdpSockfd; /* UDP */
int	UcoSockfd; /* Unix Connection-Oriented(TCP) */
int	UclSockfd; /* Unix Connection-less(UDP) */

/* Define Signal Handler - Even though the process is terminated abnormally, 
the socket descriptors must be closed and FIFO PATHs must be removed  */
void
CloseServer()
{
	close(TcpSockfd);
	close(UdpSockfd);
	close(UcoSockfd);
	close(UclSockfd);
	if (remove(UNIX_STR_PATH) < 0)  { 
		perror("remove");
	}
	if (remove(UNIX_DG_PATH) < 0)  {
		perror("remove");
	}

	printf("\nServer daemon exit.....\n");

	exit(0);
}

/* TCP - server : same as tcps.c */
void
MakeTcpSocket()
{
	struct sockaddr_in	servAddr;

	if ((TcpSockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERV_TCP_PORT);

	if (bind(TcpSockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
		perror("bind");
		exit(1);
	}

	listen(TcpSockfd, 5);
}

/* UDP - server : same as uds.c */
void
MakeUdpSocket()
{
	struct sockaddr_in	servAddr;

	if ((UdpSockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERV_UDP_PORT);

	if (bind(UdpSockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
		perror("bind");
		exit(1);
	}

}
/* UNIX TCP server : same as ucos.c */
void
MakeUcoSocket()
{
	struct sockaddr_un	servAddr;
	int					servAddrLen;

	if ((UcoSockfd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sun_family = PF_UNIX;
	strcpy(servAddr.sun_path, UNIX_STR_PATH);
	servAddrLen = strlen(servAddr.sun_path) + sizeof(servAddr.sun_family);

	if (bind(UcoSockfd, (struct sockaddr *) &servAddr, servAddrLen) < 0)  {
		perror("bind");
		exit(1);
	}

	listen(UcoSockfd, 5);

}
/* UNIX UDP server : same as ucls.c*/
void
MakeUclSocket()
{
	struct sockaddr_un	servAddr;
	int					servAddrLen;

	if ((UclSockfd = socket(PF_UNIX, SOCK_DGRAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sun_family = PF_UNIX;
	strcpy(servAddr.sun_path, UNIX_DG_PATH);
	servAddrLen = strlen(servAddr.sun_path) + sizeof(servAddr.sun_family);

	if (bind(UclSockfd, (struct sockaddr *) &servAddr, servAddrLen) < 0)  {
		perror("bind");
		exit(1);
	}

}
/* TCP server : same as tcps.c */
void
ProcessTcpRequest()
{
	int					newSockfd, cliAddrLen, n;
	struct sockaddr_in	cliAddr;
	MsgType				msg;

	cliAddrLen = sizeof(cliAddr);
	while(1) { /* make a loop */
		newSockfd = accept(TcpSockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);
		if (newSockfd < 0)  {
			perror("accept");
			exit(1);
		}
			
		if ((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0)  {
			perror("read");
			exit(1);
		}
		printf("Received TCP request: %s.....", msg.data);

		msg.type = MSG_REPLY;
		sprintf(msg.data, "This is a reply from %d.", getpid());
		if (write(newSockfd, (char *)&msg, sizeof(msg)) < 0)  {
			perror("write");
			exit(1);
		}
		printf("Replied.\n");

		close(newSockfd);
	}
}
/* UDP server : same as udps.c */
void
ProcessUdpRequest()
{
	int					cliAddrLen, n;
	struct sockaddr_in	cliAddr;
	MsgType				msg;

	cliAddrLen = sizeof(cliAddr);
	while(1) { /* make a loop */
		if ((n = recvfrom(UdpSockfd, (char *)&msg, sizeof(msg), 
					0, (struct sockaddr *)&cliAddr, &cliAddrLen)) < 0)  {
			perror("recvfrom");
			exit(1);
		}
		printf("Received UDP request: %s.....", msg.data);

		msg.type = MSG_REPLY;
		sprintf(msg.data, "This is a reply from %d.", getpid());
		if (sendto(UdpSockfd, (char *)&msg, sizeof(msg),
					0, (struct sockaddr *)&cliAddr, cliAddrLen) < 0)  {
			perror("sendto");
			exit(1);
		}
		printf("Replied.\n");
	}
}
/* UNIX TCP server : same as ucos.c */
void
ProcessUcoRequest()
{
	int					newSockfd, cliAddrLen, n;
	struct sockaddr_un	cliAddr;
	MsgType				msg;

	cliAddrLen = sizeof(cliAddr);
	while(1) { /* make a loop */
		newSockfd = accept(UcoSockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);
		if (newSockfd < 0)  {
			perror("accept");
			exit(1);
		}
			
		if ((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0)  {
			perror("read");
			exit(1);
		}
		printf("Received UNIX-domain CO request: %s.....", msg.data);

		msg.type = MSG_REPLY;
		sprintf(msg.data, "This is a reply from %d.", getpid());
		if (write(newSockfd, (char *)&msg, sizeof(msg)) < 0)  {
			perror("write");
			exit(1);
		}
		printf("Replied.\n");
	
		close(newSockfd);
	}
}
/* UNIX UDP server : same as ucls.c*/
void
ProcessUclRequest()
{
	int					cliAddrLen, n;
	struct sockaddr_un	cliAddr;
	MsgType				msg;

	cliAddrLen = sizeof(cliAddr);
	while(1) {
		if ((n = recvfrom(UclSockfd, (char *)&msg, sizeof(msg), 
					0, (struct sockaddr *)&cliAddr, &cliAddrLen)) < 0)  {
			perror("recvfrom");
			exit(1);
		}
		printf("Received UNIX-domain CL request: %s.....", msg.data);

		msg.type = MSG_REPLY;
		sprintf(msg.data, "This is a reply from %d.", getpid());
		if (sendto(UclSockfd, (char *)&msg, sizeof(msg),
					0, (struct sockaddr *)&cliAddr, cliAddrLen) < 0)  {
			perror("sendto");
			exit(1);
		}
		printf("Replied.\n");
	}
}

main(int argc, char *argv[])
{
	fd_set	fdvar;
	int		count;
	pthread_t tid1, tid2, tid3, tid4;

	signal(SIGINT, CloseServer); /* register a handler */

    /* make 4 type sockets */
	MakeTcpSocket(); 
	MakeUdpSocket();
	MakeUcoSocket();
	MakeUclSocket();

	printf("Server daemon started.....\n");

	if (pthread_create(&tid1, NULL, (void *)ProcessTcpRequest, (void *)0) < 0)  { /* create thread for TCP */
                perror("pthread_create");
                exit(1);
    }
	if (pthread_create(&tid2, NULL, (void *)ProcessUdpRequest, (void *)0) < 0)  { /* create thread for UDP */
                perror("pthread_create");
                exit(1);
    }
	if (pthread_create(&tid3, NULL, (void *)ProcessUcoRequest, (void *)0) < 0)  { /* create thread  for Uco*/
                perror("pthread_create");
                exit(1);
    }
	if (pthread_create(&tid4, NULL, (void *)ProcessUclRequest, (void *)0) < 0)  { /* create thread for Ucl */
                perror("pthread_create");
                exit(1);
    }
	if (pthread_join(tid1, NULL) < 0)  { /* wait for thread1 */
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(tid2, NULL) < 0)  { /* wait for thread2 */
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(tid3, NULL) < 0)  { /* wait for thread3 */
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(tid4, NULL) < 0)  { /* wait for thread4 */
		perror("pthread_join");
		exit(1);
	}
}