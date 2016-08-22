#include "header_ip_udp.h"

#define S_ADDR "192.168.2.131"
#define D_ADDR "192.168.2.48"

uint16_t CreatePacketEthIpUdp(char *buf) {
	int i;
	
	IP_HDR iphdr;
	UDP_HDR udphdr;
	uint16_t totalSize, ipSize, udpSize, udpChecksum;
	char *ptr, message[SIZE_MSG];
	
	strcpy(message,"Hello! (eth_header, udp_header, ip_header)\n");
	
	totalSize = sizeof(iphdr) + sizeof(udphdr) + strlen(message);
	ipSize    = sizeof(iphdr) / sizeof(uint32_t);
			
	iphdr.version  = 4;
	iphdr.ihl      = ipSize;
	iphdr.tos      = 0;
	iphdr.tot_len  = htons(totalSize);
	iphdr.id       = 0;
	iphdr.frag_off = 0;
	iphdr.ttl      = 128;
	iphdr.protocol = IPPROTO_UDP;
	iphdr.check    = 0x00;
	iphdr.saddr    = inet_addr(S_ADDR);
	iphdr.daddr    = inet_addr(D_ADDR);				
	
	/* 
	 * Внимание!!! Считаем чексумму самостоятельно!
	 * т.к. система подсчет чексуммы(для RAW сокетов) выполняться не будет!
	*/
	iphdr.check = Checksum((uint16_t *)&iphdr, iphdr.ihl * 4);

	udpSize = sizeof(udphdr) + strlen(message);
	udphdr.source_port = htons(CLIENT_PORT);
	udphdr.dest_port   = htons(SERVER_PORT);
	udphdr.udp_len     = htons(udpSize);
	udphdr.udp_sum     = 0;
	udpChecksum = 0;
	ptr = buf;
		
	/* * Подсчет чексуммы udp заголовка * */
	bzero(buf, SIZE_IP_UDP_PACKET);
	memcpy(ptr, &iphdr.saddr, sizeof(iphdr.saddr));  
	ptr          += sizeof(iphdr.saddr);
	udpChecksum += sizeof(iphdr.saddr);
	memcpy(ptr, &iphdr.daddr, sizeof(iphdr.daddr));
	ptr          += sizeof(iphdr.daddr);
	udpChecksum += sizeof(iphdr.daddr);
	ptr++;
	udpChecksum += 1;
	memcpy(ptr, &iphdr.protocol, sizeof(iphdr.protocol));
	ptr          += sizeof(iphdr.protocol);
	udpChecksum += sizeof(iphdr.protocol);
	memcpy(ptr, &udphdr.udp_len, sizeof(udphdr.udp_len));
	ptr          += sizeof(udphdr.udp_len);
	udpChecksum += sizeof(udphdr.udp_len);
	memcpy(ptr, &udphdr, sizeof(udphdr));
	ptr          += sizeof(udphdr);
	udpChecksum += sizeof(udphdr);
		
	for(i = 0; i < strlen(message); i++, ptr++)
		*ptr = message[i];
			
	udpChecksum += strlen(message);
	udphdr.udp_sum = Checksum((uint16_t *)buf, udpChecksum); 
	/* * * * * * * * * * * * * * * * * * * */					 
							 
	bzero(buf, SIZE_IP_UDP_PACKET);
	ptr = buf;
	memcpy(ptr, &iphdr, sizeof(iphdr)); 
	ptr += sizeof(iphdr);
	memcpy(ptr, &udphdr,sizeof(udphdr)); ptr += sizeof(udphdr);
	memcpy(ptr, message, strlen(message));
	
	return totalSize;
}
