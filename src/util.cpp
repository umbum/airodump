#include "util.h"

#include <iostream>

#include <stdio.h>
#include <stdint.h>

#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

void clearConsole() {
    std::cout << "\x1B[2J\x1B[H";
}

void printTableHeader() {
    printf("\n");
    printf(" %-18s %-4s %-10s %-3s %-4s %-3s %-4s %-4s %-6s %-4s %s\n\n", 
           "BSSID", "PWR", "Beacons", "#Data,", "#/s", "CH", "MB", "ENC", "CIPHER", "AUTH", "ESSID");
}

void printPacket(uint8_t *p, int len) {
	int i;
	for (i = 1; i < len; i++) {
		printf("%02x ", p[i - 1]);
		if (i % 16 == 0) {
			printf("\n");
		}
	}
	printf("%02x ", p[i]);
	printf("\n");
}


void parse_ip(const uint8_t* packet) {
    struct ip* ip_header = (struct ip*)packet;
    printf("    src IP   : %s\n", inet_ntoa(ip_header->ip_src));
    printf("    dst IP   : %s\n", inet_ntoa(ip_header->ip_dst));
    
    switch (ip_header->ip_p) {
        case IPPROTO_TCP:
            {
            // caculate header length : 4bit(& 0xF) and * word(4)
            uint8_t ip_header_len = (ip_header->ip_hl & 0xF) * 4;
            // total length - header length = next protocol lenth
            uint16_t tcp_len = htons(ip_header->ip_len) - ip_header_len;
            parse_tcp(packet + ip_header_len, tcp_len);
            break;
            }
        default:
            printf("Upper proto is not TCP / Protocol : %u\n", ip_header->ip_p);
            break;
    }
}

void parse_tcp(const uint8_t* packet, uint16_t tcp_len) {
    struct tcphdr* tcp_header = (struct tcphdr*)packet;
    printf("    src port : %hu\n", htons(tcp_header->th_sport));
    printf("    dst port : %hu\n", htons(tcp_header->th_dport));

    // caculate payload lenth
    uint8_t tcp_header_len = ((tcp_header->th_off & 0xF) * 4);
    const uint8_t* payload = packet + tcp_header_len;
    uint16_t payload_len = tcp_len - tcp_header_len;
    printf("    payload length : %hu\n", payload_len);

    if (payload_len > 0) {
        printf("    DATA : ");
        int print_len = (16 < payload_len) ? 16 : payload_len;
        for (int i = 0; i < print_len; i++) {
            printf("%c", payload[i]);
        }
        puts("");
    }
}
