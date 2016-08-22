CC=gcc
CFLAGS=-g -c -Wall -pedantic

all: main_raw server_udp

main_raw: main_client_ip_udp.o create_packet_ip_udp.o checksum.o
	$(CC) main_client_ip_udp.o create_packet_ip_udp.o checksum.o -o main_raw

server_udp: 
	$(CC) server_udp.c -o server_udp

main_client_ip_udp.o: main_client_ip_udp.c
	$(CC) $(CFLAGS) main_client_ip_udp.c

create_packet_ip_udp.o: create_packet_ip_udp.c
	$(CC) $(CFLAGS) create_packet_ip_udp.c

checksum.o: checksum.c
	$(CC) $(CFLAGS) checksum.c

clean:
	rm -rf *.o main_raw server_udp