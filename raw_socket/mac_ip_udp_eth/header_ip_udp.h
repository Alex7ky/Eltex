#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <sys/ioctl.h>

#define ETH_FRAME_SIZE 500 
#define SIZE_IP_UDP_PACKET 486
#define SIZE_MSG 458

#define CLIENT_PORT 6666
#define SERVER_PORT 8622

typedef struct IpHdr {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    unsigned int ihl:4;
    unsigned int version:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
    unsigned int version:4;
    unsigned int ihl:4;
#else
# error	"Please fix <bits/endian.h>"
#endif
    uint8_t tos;
    uint16_t tot_len;
    uint16_t id;
    uint16_t frag_off;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t check;
    uint32_t saddr;
    uint32_t daddr;
} IP_HDR;
 
typedef struct UdpHdr {
	uint16_t source_port;
    uint16_t dest_port;
    uint16_t udp_len;
    uint16_t udp_sum;
} UDP_HDR;

uint16_t Checksum(uint16_t *, int);
uint16_t CreatePacketEthIpUdp(char *);
