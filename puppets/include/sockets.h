// MIT License
/*
 * This header file belongs to https://github.com/arthur-bryan/puppeteer
 * This file contains function declarations for:
 *      create_socket(void): creates a file descriptor for socket communication
 *      connect_to_server(void): creates and keeps tries to connect to a socket
 *      send_all_data(int16_t, void *, size_t): sends all buffer over the socket
 *      recv_all_data(int16_t, void *, size_t): receives all buffer over
 *                                              the socket
 */
#ifndef SOCKETS_H
#define SOCKETS_H
#define SERVER_ADDR "172.16.100.3"
#define SERVER_PORT 1771

#ifdef __unix__
#include <netdb.h>
#else
#include <stdint.h>
#endif

int16_t
create_socket(void);

int16_t
connect_to_server(void);

int8_t
send_all_data(int16_t, void *, size_t);

uint32_t
recv_all_data(int16_t, void *, size_t);

#endif //SOCKETS_H
