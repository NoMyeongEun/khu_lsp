#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "udp.h"


main(int argc, char *argv[])
{
	int					sockfd, n, peerAddrLen;
	struct sockaddr_in	servAddr, peerAddr; /* sockaddr_in -> IPv4 */
	MsgType				msg;

	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)  { /* creat a socket - protocol is IPv4 and type is datagram socket(UDP) */
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr)); /* initialize : set the pointer servAddr to 0  */
	servAddr.sin_family = PF_INET; /* protocol family is IPv4 */
	servAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR); /* In udp.h, we defined SERVER_HOST_ADDR as dotted decimal. change to 4byte integer network byte order - no need to htonl()*/
	servAddr.sin_port = htons(SERV_UDP_PORT); /* conversion between host byte order to network byte order (short)*/

	msg.type = MSG_REQUEST;
	sprintf(msg.data, "This is a request from %d.", getpid());
	if (sendto(sockfd, (char *)&msg, sizeof(msg),  /* request to server */
			0, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)  {
		perror("sendto");
		exit(1);
	}
	printf("Sent a request.....");
		
	peerAddrLen = sizeof(peerAddr);
	if ((n = recvfrom(sockfd, (char *)&msg, sizeof(msg), /* check the response - read the message from socket */
				0, (struct sockaddr *)&peerAddr, &peerAddrLen)) < 0)  {
		perror("recvfrom");
		exit(1);
	}
	printf("Received reply: %s\n", msg.data);

	close(sockfd);
}