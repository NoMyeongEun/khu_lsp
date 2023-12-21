#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include "unix.h"

/* client */
main(int argc, char *argv[])
{
	int					sockfd, n, servAddrLen, myAddrLen, peerAddrLen;
	struct sockaddr_un	servAddr, myAddr, peerAddr; /* sockaddr_un -> unix */
	MsgType				msg;

	if ((sockfd = socket(PF_UNIX, SOCK_DGRAM, 0)) < 0)  { /* creat a socket - protocol is UNIX and type is datagram socket(UDP) */
		perror("socket");
		exit(1);
	}
	/* get my address */
	bzero((char *)&myAddr, sizeof(myAddr)); /* initialize : set the pointer myAddr to 0  */
	myAddr.sun_family = PF_UNIX;  /* protocol family is UNIX */
	sprintf(myAddr.sun_path, ".unix-%d", getpid()); /* sun_path is .unix-[pid]*/
	myAddrLen = strlen(myAddr.sun_path) + sizeof(myAddr.sun_family); /* mmy address is sun_path + sun_family */

	if (bind(sockfd, (struct sockaddr *)&myAddr, myAddrLen) < 0)  { /* bind to socket descriptor, for address my socket address, size of address */
		perror("bind");
		exit(1);
	}
	/* get a server address */
	bzero((char *)&servAddr, sizeof(servAddr)); /* initialize : set the pointer servAddr to 0  */
	servAddr.sun_family = PF_UNIX; /* protocol family is UNIX */
	strcpy(servAddr.sun_path, UNIX_DG_PATH); /* save UNIX_DG_PATH to FIFO PATH NAME(sun_path)*/
	servAddrLen = strlen(servAddr.sun_path) + sizeof(servAddr.sun_family); /* serveraddress is sun_path + sun_family */

	msg.type = MSG_REQUEST;
	sprintf(msg.data, "This is a request from %d.", getpid());
	if (sendto(sockfd, (char *)&msg, sizeof(msg), /* request to server using address got above */
			0, (struct sockaddr *)&servAddr, servAddrLen) < 0)  { 
		perror("sendto");
		exit(1);
	}
	printf("Sent a request.....");
		
	peerAddrLen = sizeof(peerAddr);
	if ((n = recvfrom(sockfd, (char *)&msg, sizeof(msg), /* check the response - read the message from socket */
				0, (struct sockaddr *)&peerAddr, &peerAddrLen)) < 0)  { /* and save where the message from*/
		perror("recvfrom");
		exit(1);
	}
	printf("Received reply: %s\n", msg.data);

	close(sockfd); /* close a socket */
	if (remove(myAddr.sun_path) < 0)  { /* remove myAddr.sun_path(".unix-[pid]")*/
		perror("remove");
		exit(1);
	}
}