#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char**argv)
{
	int sockfd;
	struct sockaddr_in servaddr, cliaddr;
	socklen_t len;
	char mesg[100];

	sockfd = socket(AF_INET, SOCK_DGRAM,0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(32002);
	bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	len = sizeof(cliaddr);
	recvfrom(sockfd, mesg, 100, 0, (struct sockaddr *)&cliaddr, &len);
	printf("message client: %s", mesg);

	close(sockfd);

	return 0;
}
