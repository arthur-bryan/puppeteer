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

#include <stdint.h>

#ifdef __unix__
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#else
#include <winsock2.h>

typedef struct ip_hdr
{
    uint8_t ihl;  // 4-bit header length (in 32-bit words) normally=5 (Means 20 Bytes may be 24 also)
    uint8_t version;  // 4-bit IPv4 version
    uint8_t tos;           // IP type of service
    uint16_t tot_len;  // Total length
    uint16_t id;            // Unique identifier

    uint8_t frag_off;        // Fragment offset field

    uint8_t ip_more_fragment;
    uint8_t ip_dont_fragment;
    uint8_t ip_reserved_zero;

    uint8_t ip_frag_offset1;    //fragment offset

    uint8_t ttl;           // Time to live
    uint8_t protocol;      // Protocol(TCP,UDP etc)
    uint16_t check;      // IP checksum
    uint32_t  saddr;       // Source address
    uint32_t  daddr;      // Source address
}   IPV4_HDR, *PIPV4_HDR, FAR * LPIPV4_HDR , IPHeader;

typedef struct tcp_hdr {
    uint16_t source;   // source port
    uint16_t dest;     // destination port
    uint32_t seq;        // sequence number - 32 bits
    uint32_t ack_seq;     // acknowledgement number - 32 bits

    uint8_t ns;          //Nonce Sum Flag Added in RFC 3540.
    uint8_t reserved_part1; //according to rfc
    uint8_t doff;    /*The number of 32-bit words
                                   in the TCP header.
                                   This indicates where the data begins.
                                   The length of the TCP header
                                   is always a multiple
                                   of 32 bits.*/

    uint8_t fin; //Finish Flag
    uint8_t syn; //Synchronise Flag
    uint8_t rst; //Reset Flag
    uint8_t psh; //Push Flag
    uint8_t ack; //Acknowledgement Flag
    uint8_t urg; //Urgent Flag

    uint8_t ecn; //ECN-Echo Flag
    uint8_t cwr; //Congestion Window Reduced Flag

    uint16_t window; // window
    uint16_t check; // checksum
    uint16_t urg_ptr; // urgent pointer
} TCP_HDR , *PTCP_HDR , FAR * LPTCP_HDR , TCPHeader , TCP_HEADER;
#endif

typedef struct pseudo_header {
    uint32_t        source_address;
    uint32_t        destination_address;
    uint8_t         reserved;
    uint8_t         protocol;
    uint16_t        tcp_length;
    #ifdef __unix__
    struct tcphdr   tcp;
    #else
    struct tcp_hdr  tcp;
    #endif
} pseudo_header;


void
syn_flood(int16_t);

uint16_t
csum(uint16_t *, size_t);

#endif // SYN_FLOOD_H
