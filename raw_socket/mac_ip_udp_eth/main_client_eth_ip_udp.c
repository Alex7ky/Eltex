#include "header_ip_udp.h"

#define DS_MAC0	0x48
#define DS_MAC1	0x5b
#define DS_MAC2	0x39
#define DS_MAC3	0x98
#define DS_MAC4	0xdb
#define DS_MAC5	0x2a

#define SR_MAC0	0xdc
#define SR_MAC1	0x0e
#define SR_MAC2	0xa1
#define SR_MAC3	0x5a
#define SR_MAC4	0x8e
#define SR_MAC5	0x7c

#define ETH_HEADER_LEN 14

/* * Сетевой интерфейс(ip a) * */
#define DEFAULT_IF	"enp2s0f0"

int main()
{
	int sckt;
	struct ifreq ifIdx;
	struct sockaddr_ll socket_address;
	void* buffer = (void *)calloc(ETH_FRAME_SIZE, 1);
	char* data = (char *)buffer + ETH_HEADER_LEN;
	int16_t size_packet;
	unsigned char srcMac[6] = {SR_MAC0, SR_MAC1, SR_MAC2, 
								SR_MAC3, SR_MAC4, SR_MAC5};
	unsigned char destMac[6] = {DS_MAC0, DS_MAC1, DS_MAC2, 
								 DS_MAC3, DS_MAC4, DS_MAC5};
	
	sckt = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	
	if (sckt < 0) perror("create client socket");
	
	strncpy(ifIdx.ifr_name, DEFAULT_IF, IFNAMSIZ-1);
	if (ioctl(sckt, SIOCGIFINDEX, &ifIdx) < 0)
	    perror("ioctl SIOCGIFINDEX");

	socket_address.sll_family   = AF_INET;	
	socket_address.sll_protocol = htons(ETH_P_IP);
	socket_address.sll_ifindex  = ifIdx.ifr_ifindex;
	socket_address.sll_hatype   = ARPHRD_ETHER;
	socket_address.sll_pkttype  = PACKET_HOST;
	socket_address.sll_halen    = ETH_ALEN;

	/* * Destination MAC * */
	socket_address.sll_addr[0] = srcMac[0];
	socket_address.sll_addr[1] = srcMac[1];
	socket_address.sll_addr[2] = srcMac[2];
	socket_address.sll_addr[3] = srcMac[3];
	socket_address.sll_addr[4] = srcMac[4];
	socket_address.sll_addr[5] = srcMac[5];
	
	/* * MAC - end * */
	socket_address.sll_addr[6]  = 0x00;
	socket_address.sll_addr[7]  = 0x00;

	/* * set the frame header * */
	memcpy((void*)buffer, (void*)destMac, ETH_ALEN);
	memcpy((void*)(buffer + ETH_ALEN), (void*)srcMac, ETH_ALEN);
	
	char ether_frame[2];
	ether_frame[0] = ETH_P_IP / 256;
	ether_frame[1] = ETH_P_IP % 256;
	
	memcpy((void*)(buffer + ETH_ALEN + ETH_ALEN), (void*)ether_frame, 2);  
	
	size_packet = CreatePacketEthIpUdp(data);
	printf("data len = %d\n", size_packet + ETH_HEADER_LEN);
	size_packet += ETH_HEADER_LEN;

	if (sendto(sckt, buffer, size_packet, 0, 
		(struct sockaddr*)&socket_address, sizeof(socket_address)) < 0){
		perror("sendto client");
		exit(-1);
	} else {
		printf("sendto - OK\n");
	}	
		
	if (close(sckt) < 0) {
		perror("clietn close sock");
		exit(-1);
	} else {
		printf("close sock - OK\n");
	}		 

	return 0;
}
