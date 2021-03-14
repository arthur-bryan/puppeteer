// MIT License
/*
 * This header file belongs to https://github.com/arthur-bryan/puppeteer
 *
 * Sends data from disk over a connected socket
 *
 * Copyright (c) 2021 Arthur Bryan <arthurbryan2030@gmail.com>
 */
#include "../include/send_file.h"

#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#ifdef _WIN32
#include <winsock2.h>
#endif

#include "../include/sockets.h"


/*
 * Checks the smallest size between two operands
 *
 * Parameters:
 *      size_t first: size of the first operand
 *      size_t second: size of the second operand
 * Returns:
 *      Returns first if it's smaller or equal to second, second otherwise
 */
size_t
minimum(size_t first, size_t second) {
    if (first < second)
        return first;
    else if (second < first)
        return second;
    return first;
}

/*
 * Uploads a file with path specified by the server over an connected socket
 *
 * Parameters:
 *      int16_t socket_fd:  the socket file descriptor
 *
 * Returns:
 *      Returns 0 if the file was fully uploaded, -1 otherwise
 */
int8_t
send_file(int16_t socket_fd) {
    char        file_name[128];
    struct stat file_structure;

    memset(file_name, 0, sizeof file_name);
    recv(socket_fd, file_name, sizeof file_name, 0);  // receives filename
    if (stat(file_name, &file_structure) == -1) {
        return -1;
    }
    FILE        *file = fopen(file_name, "rb");

    if (file == NULL) {
        return -1;
    }
    uint32_t    file_size = file_structure.st_size;
    int16_t     send_result;
    // send size of file to prepare the server to receive that data
    #ifdef __unix__
    send_result = send(socket_fd, &file_size, sizeof file_size, 0);
    #else
    send_result = send(socket_fd, (char *)&file_size, sizeof file_size, 0);
    #endif
    if (send_result > 0) {
        char    *file_buffer = calloc(file_size, sizeof(char));
        while (file_size > 0) {
            uint32_t    bytes_read = fread(file_buffer,
                                           sizeof(char),
                                           minimum(DATA_BLOCK_SIZE, file_size),
                                           file);

            if (bytes_read == 0) {
                break;
            }
            if (send_all_data(socket_fd, file_buffer, bytes_read) == 0) {
                void *tmp = realloc(file_buffer,
                                      strlen(file_buffer) +
                                      DATA_BLOCK_SIZE);
                if (tmp == NULL) {
                    return -1;
                }
                file_buffer = tmp;
                file_size -= bytes_read;
            } else {
                free(file_buffer);
                fclose(file);
                return -1;
            }
        }
        free(file_buffer);
        fclose(file);
    }
    fclose(file);
    return 0;
}
