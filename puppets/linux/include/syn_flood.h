// MIT License
/*
 * This header file belongs to https://github.com/arthur-bryan/puppeteer
 * Thi file contains struct declarations for:
 *      struct  pseudo_header:  an pseudo TCP header
 * This file contains function declarations for:
 *      char *random_ip(void):  generates a random private IP address
 *      uint16_t random_port(void): generates a random dynamic port number
 *      csum(uint16_t, size_t): calculates the checksum of the segment
 *      syn_flood(int16_t): floods a target with forged packets
 */
#ifndef SYN_FLOOD_H
#define SYN_FLOOD_H

#include <netinet/tcp.h>

typedef struct pseudo_header {
    uint32_t        source_address;
    uint32_t        destination_address;
    uint8_t         reserved;
    uint8_t         protocol;
    uint16_t        tcp_length;
    struct tcphdr   tcp;
} pseudo_header;

void
syn_flood(int16_t);

uint16_t
csum(uint16_t *, size_t);

#endif // SYN_FLOOD_H
