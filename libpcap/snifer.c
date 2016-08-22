/*      
	simple snifer
	gcc -o sniffer main.c -lpcap        
*/                  
#include <pcap.h>                    
#include <arpa/inet.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 

#include <netinet/ip.h>
#include <netinet/if_ether.h>  

struct my_eth {
	u_char ether_dhost[ETHER_ADDR_LEN]; // MAC адрес получателя пакета
	u_char ether_shost[ETHER_ADDR_LEN]; // MAC адрес отправителя пакета
	u_short ether_type;                 // тип протокола
} __attribute__ ((__packed__));

/* Структура IP заголовка */
struct ip_hdr {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    unsigned int ihl:4;
    unsigned int version:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
    unsigned int version:4;
    unsigned int ihl:4;
#else
# error	"Please fix <bits/endian.h>"
#endif
    u_int8_t tos;
    u_int16_t tot_len;
    u_int16_t id;
    u_int16_t frag_off;
    u_int8_t ttl;
    u_int8_t protocol;
    u_int16_t check;
    u_int32_t saddr;
    u_int32_t daddr;
};

//Получив пакет, достаем из него информацию об Ethernet уровне. Возвращает тип протокола 
u_short handle_ethernet(u_char *args, const struct pcap_pkthdr* pkthdr, const u_char* packet)
{
	struct my_eth *eth = (struct my_eth *) packet;
	
	if (eth->ether_type == 8) {
		printf("\n");
		printf("Ethernet Header\n");
		printf("   |-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", 
			eth->ether_shost[0] , eth->ether_shost[1] , eth->ether_shost[2] , 
			eth->ether_shost[3] , eth->ether_shost[4] , eth->ether_shost[5] );
		printf("   |-Source Address      : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", 
			eth->ether_dhost[0] , eth->ether_dhost[1] , eth->ether_dhost[2] , 
			eth->ether_dhost[3] , eth->ether_dhost[4] , eth->ether_dhost[5] );
		printf("   |-Protocol            : %u \n",(unsigned short)eth->ether_type);
	}
	//sleep(1);
	
	return ntohs(eth->ether_type);
}

uint16_t calc_checksum(void *buffer, int size)
{
	uint32_t cksum = 0; 
	uint16_t *ptr = buffer;
	
    while (size > 1){
        cksum += *ptr++;
        size -= 2;  
    }
    if (size){
		cksum += *ptr;  
    }
    
    while (cksum >> 16)
		cksum = (cksum >> 16) + (cksum & 0xffff);
    
    return (uint16_t)(~cksum);
}

void print_buff(void *buff, int size) {
	struct one_byte {
		u_char c1 : 1;
		u_char c2 : 1;
		u_char c3 : 1;
		u_char c4 : 1;
		u_char c5 : 1;
		u_char c6 : 1;
		u_char c7 : 1;
		u_char c8 : 1;
	};
	
	struct one_byte *new_struct;
	
	int i;
	char *c = buff;
	char t;
	
	printf("   |-Binary: " );
	for (i = 0; i < size; i++) {
		new_struct = (struct one_byte *)c++;
		
		t = 0;
		if (i % 4 == 0 && i > 0)
			printf("\n             " );
		
		if (1 == t + new_struct->c1)
			printf("1");
		else printf("0");
		if (1 == t + new_struct->c2)
			printf("1");
		else printf("0");
		if (1 == t + new_struct->c3)
			printf("1");
		else printf("0");
		if (1 == t + new_struct->c4)
			printf("1");
		else printf("0");
		if (1 == t + new_struct->c5)
			printf("1");
		else printf("0");
		if (1 == t + new_struct->c6)
			printf("1");
		else printf("0");
		if (1 == t + new_struct->c7)
			printf("1");
		else printf("0");
		if (1 == t + new_struct->c8)
			printf("1");
		else printf("0");
		
		printf(" ");	
	}
	
	printf("\n");
	return;
}

/* Для обработки IP заголовка */
void handle_ip(char *buffer) 
{
	struct iphdr *ip = (struct iphdr*) (buffer + sizeof(struct ethhdr));
	printf("IP Header:\n");
	printf("   |-Version: %d\n", ip->version);
	printf("   |-Internet Header Length: %d DWORDS, %d bytes\n", ip->ihl, ip->ihl*4);
	printf("   |-Total Length: %d bytes\n", ntohs(ip->tot_len));
	printf("   |-Time To Life: %d\n", ip->ttl);
	printf("   |-Protocol Type: %d\n", ip->protocol);
	printf("   |-Checksum: %d\n", ip->check);

	ip->check = 0x00;
	ip->check = calc_checksum((void*) ip, ip->ihl * 4);
	printf("   |-My Checksum: %d\n", ip->check);
	print_buff(buffer, ip->ihl * 4);
}     

/* Функция обработчика полученных пакетов */
void callback(u_char *args, const struct pcap_pkthdr* pkthdr, const u_char* packet)
{
   u_int16_t etype = handle_ethernet(args, pkthdr, packet);
	
	if(etype == ETHERTYPE_IP) { 
		handle_ip(packet); 
	} else if(etype == ETHERTYPE_ARP) { 
			printf("(ARP)\n"); 
		} else if(etype == ETHERTYPE_REVARP) { 
			printf("(RARP)\n"); 
		}
}


int main(int argc, char *argv[])
{
	char *dev;                     // устройство для «прослушивания»
	char errbuf[PCAP_ERRBUF_SIZE]; // сообщение об ошибке:
	pcap_t* descr;                 // идентификатор сессии для прослушивания
	struct bpf_program fp;         // указатель на скомпилированный фильтр
	struct in_addr addr;
	bpf_u_int32 maskp;
	bpf_u_int32 netp;
   	
	if(argc == 2) {
		// выполняет поиск устройств для «прослушивания»
		if((dev=pcap_lookupdev(errbuf))==NULL) {
			printf("%s\n", errbuf); exit(1); 
		}
		
		// запрашивает сетевую карту о классе сети и маске
		pcap_lookupnet(dev, &netp, &maskp, errbuf);
		
		// создаем сессию для прослушивания и возвращает ее идентификатор
		if((descr=pcap_open_live(dev, BUFSIZ, 1, 0, errbuf))==NULL) {
			printf("pcap_open_live(): %s\n", errbuf); exit(1); 
		}
		
		// компилирует фильтр отбора пакетов из текстового вида, во внутреннее представление
		if(pcap_compile(descr, &fp, argv[2], 0, netp)==-1) {
			printf("Error pcap_compile()\n"); exit(1); 
		}
		
		if(pcap_setfilter(descr, &fp)==-1) {
			printf("Error pcap_setfilter\n"); exit(1); 
		}
		
		printf("DEV: %s\n", dev);
		addr.s_addr=netp;
		printf("NET: %s\n", inet_ntoa(addr));
		addr.s_addr=maskp;
		printf("MASK: %s\n", inet_ntoa(addr));
		
		// цикл для перехвата пакетов
		pcap_loop(descr, -1, callback, NULL);
		
		pcap_close(descr);
	} else {
		printf("Usage: %s \"filter\"\n", argv[0]);
	}
 
	return 0;
}
