#include "header_udp.h"

int main(int argc, char**argv)
{
	int sockfd;
	struct sockaddr_in servaddr, cliaddr;

	socklen_t len;
	char mesg[SIZE_MSG];

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	if (sockfd < 0) {
		perror("create server socket");
		exit(-1);
	}

	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(SERVER_PORT);
	
	bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	for (;;) {
		len = sizeof(cliaddr);
		memset(mesg, 0, sizeof(char) * SIZE_MSG);

		if (recvfrom(sockfd, mesg, SIZE_MSG, 0, 
			        (struct sockaddr *)&cliaddr, &len) < 0) {
			close(sockfd);
			perror("server recv err");
			exit(-1);
		}

		printf("recvfrom: %s\n", mesg);
	}

	return 0;
}

