// MIT License
/*
 * This header file belongs to https://github.com/arthur-bryan/puppeteer
 * This file contains function declarations for:
 *      start_communication(int16_t): starts interaction with a connected socket
 *      *execute_cmd(int16_t, const char *): executes a command and grabs output
 */
#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>

#include "../include/information.h"

#define COMMAND_BUFFER_SIZE 512
#define OUTPUT_BUFFER_SIZE 48000

void
start_communication(int16_t, host_t);

char
*execute_cmd(const char *);

#endif //COMMANDS_H
