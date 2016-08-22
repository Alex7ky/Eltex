#include "header_udp.h"

uint16_t create_packet_udp(char *buf) {
		
	UDP_HDR udphdr;
	uint16_t total_size, udp_size;
	char *ptr, message[SIZE_MSG];
	
	strcpy(message,"Hello! (udp_header)\n");
	
	total_size = sizeof(udphdr) + strlen(message);
				
	udp_size = sizeof(udphdr) + strlen(message);
	udphdr.source_port = htons(CLIENT_PORT);
	udphdr.dest_port   = htons(SERVER_PORT);
	udphdr.udp_len     = htons(udp_size);
	udphdr.udp_sum     = 0;
	ptr = buf;
								 
	bzero(buf, SIZE_UDP_PACKET);
	ptr = buf;
	memcpy(ptr, &udphdr,sizeof(udphdr));ptr += sizeof(udphdr);
	memcpy(ptr, message, strlen(message));
	
	return total_size;
}
