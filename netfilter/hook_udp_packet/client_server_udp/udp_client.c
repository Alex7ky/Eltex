#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char**argv)
{
	int sockfd;
	struct sockaddr_in servaddr, cliaddr;
	char mesg[100];

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(32001);

	while (1) {
		printf("pls input msg: ");
		fgets(mesg, 100, stdin);
		sendto(sockfd, mesg, 100, 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
	}

	close(sockfd);
	return 0;
}