// MIT License
/*
 * This header file belongs to https://github.com/arthur-bryan/puppeteer
 *
 * Receives data over a connected socket and writes to disk
 *
 * Copyright (c) 2021 Arthur Bryan <arthurbryan2030@gmail.com>
 */
#include "../include/receive_file.h"

#include <stdlib.h>
#include <string.h>

#include "../include/sockets.h"
#include "../include/footprint.h"

/* Writes all data from the buffer to the specified file
 *
 * Parameters:
 *      FILE *file: the file object
 *      void *buffer: the data buffer
 *      size_t len_buffer: size (bytes) of the buffer
 *
 * Returns:
 *      Returns 0 after all buffer data has been writen, -1 otherwise
 */
int8_t
write_all(FILE *file, void *buffer, size_t len_buffer) {
    char  *buffer_pointer = buffer;

    while (len_buffer > 0) {
        uint32_t    bytes_writen = fwrite(buffer_pointer, 1, len_buffer, file);
        if (bytes_writen != len_buffer) {
            return -1;
        }
        buffer_pointer += bytes_writen;
        len_buffer -= bytes_writen;
    }
    return 0;
}

/*
 * Receives a whole file from the server and writes it to disk
 *
 * Parameters:
 *      int16_t socket_fd: the socket file descriptor
 *
 * Returns:
 *      Returns 0 if the file was fully downloaded, -1 otherwise
 */
int8_t
receive_file(int16_t socket_fd) {
    char        *file_name = calloc(256, sizeof(char));
    uint32_t    file_size;
    uint32_t    len_filename;

    if (recv(socket_fd, &file_size, sizeof file_size, 0) == -1) {
        return -1;   // receving file size
    } else if (recv(socket_fd, &len_filename, sizeof len_filename, 0) == -1) {
        return -1;  // receiving file name length
    } else if (recv(socket_fd, file_name, len_filename, 0) == -1) {
        return -1;  // receiving file name
    } else if (file_exists(file_name) == 1) {
        return -1;
    }
    char    *file_buffer;
    FILE    *file = fopen(file_name, "wb");

    if (file == NULL) {
        return -1;
    }
    file_buffer = calloc(DATA_BLOCK_SIZE, sizeof(char));
    while (file_size > 0) {
        uint32_t    bytes_read = 0;

        bytes_read = recv_all_data(socket_fd,
                                   file_buffer,
                                   min(DATA_BLOCK_SIZE, file_size));
        if (bytes_read == 0) {
            fclose(file);
            free(file_buffer);
            return -1;
        }
        if (write_all(file, file_buffer, bytes_read) == 0) {
            void    *tmp = realloc(file_buffer,
                                  strlen(file_buffer) + DATA_BLOCK_SIZE);
            if (tmp == NULL) {
                return -1;
            }
            file_buffer = tmp;
            file_size -= bytes_read;
        } else {
            fclose(file);
            free(file_buffer);
            return -1;
        }
    }
    fclose(file);
    free(file_name);
    free(file_buffer);
    return 0;
}

