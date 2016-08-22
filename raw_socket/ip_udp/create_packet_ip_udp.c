#include "header_ip_udp.h"

uint16_t create_packet_ip_udp(char *buf) {
	int i;
	
	IP_HDR iphdr;
	UDP_HDR udphdr;
	uint16_t total_size, ip_size, udp_size, udp_checksum;
	char *ptr, message[SIZE_MSG];
	
	strcpy(message, "Hello! (udp_header, ip_header)\n");
	
	total_size = sizeof(iphdr) + sizeof(udphdr) + strlen(message);
	ip_size    = sizeof(iphdr) / sizeof(uint32_t);
			
	iphdr.version  = 4;
	iphdr.ihl      = ip_size;
	iphdr.tos      = 0;
	iphdr.tot_len  = htons(total_size);
	iphdr.id       = 0;
	iphdr.frag_off = 0;
	iphdr.ttl      = 128;
	iphdr.protocol = IPPROTO_UDP;
	iphdr.check    = 0;
	iphdr.saddr    = inet_addr("S_ADDR");
	iphdr.daddr    = inet_addr("D_ADDR");
				
				
	udp_size = sizeof(udphdr) + strlen(message);
	udphdr.source_port = htons(CLIENT_PORT);
	udphdr.dest_port   = htons(SERVER_PORT);
	udphdr.udp_len     = htons(udp_size);
	udphdr.udp_sum     = 0;
	udp_checksum = 0;
	ptr = buf;
		
	/* * Подсчет чексуммы udp заголовка * */
	bzero(buf, SIZE_IP_UDP_PACKET);
	memcpy(ptr, &iphdr.saddr, sizeof(iphdr.saddr));  
	ptr          += sizeof(iphdr.saddr);
	udp_checksum += sizeof(iphdr.saddr);
	memcpy(ptr, &iphdr.daddr, sizeof(iphdr.daddr));
	ptr          += sizeof(iphdr.daddr);
	udp_checksum += sizeof(iphdr.daddr);
	ptr++;
	udp_checksum += 1;
	memcpy(ptr, &iphdr.protocol, sizeof(iphdr.protocol));
	ptr          += sizeof(iphdr.protocol);
	udp_checksum += sizeof(iphdr.protocol);
	memcpy(ptr, &udphdr.udp_len, sizeof(udphdr.udp_len));
	ptr          += sizeof(udphdr.udp_len);
	udp_checksum += sizeof(udphdr.udp_len);
	memcpy(ptr, &udphdr, sizeof(udphdr));
	ptr          += sizeof(udphdr);
	udp_checksum += sizeof(udphdr);
		
	for(i = 0; i < strlen(message); i++, ptr++)
		*ptr = message[i];
			
	udp_checksum += strlen(message);
	udphdr.udp_sum = checksum((uint16_t *)buf, udp_checksum); 
	/* * * * * * * * * * * * * * * * * * * */					 
							 
	bzero(buf, SIZE_IP_UDP_PACKET);
	ptr = buf;
	memcpy(ptr, &iphdr, sizeof(iphdr)); 
	ptr += sizeof(iphdr);
	memcpy(ptr, &udphdr,sizeof(udphdr)); ptr += sizeof(udphdr);
	memcpy(ptr, message, strlen(message));
	
	return total_size;
}
