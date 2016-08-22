#include "header_ip_udp.h"

int main()
{
	int sckt;
	uint16_t total_size; 
	char buf[SIZE_IP_UDP_PACKET];
	struct sockaddr_in remote;
	
	sckt = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
	
	if (sckt == -1) perror("Socket");

	total_size = create_packet_ip_udp(buf);
	
	bzero(&remote, sizeof(remote));
	
	remote.sin_family      = AF_INET;
	remote.sin_port        = htons(SERVER_PORT);
	remote.sin_addr.s_addr = inet_addr(D_ADDR);
	
	if (sendto(sckt, buf, total_size, 0, (struct sockaddr *)&remote,
		sizeof(remote)) < 0){
		perror("close_sock");
		exit(-1);
	} else {
		printf("sendto - OK\n");
	}	
		
	if (close(sckt) < 0) {
		perror("close_sock");
		exit(-1);
	} else {
		printf("close_sock - OK\n");
	}		 

	return 0;
}
